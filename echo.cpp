/*
Scott Campbell
g++ example using write
*/

// Use latest version of FUSE API
#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <string>

//shared storage area for al writes
char dataBuffer[4096];
int dataBufferSize = 4096;

char  fileNames[2][20] = {"/echo"};

// Define a constant for number of files to be processed
const int MaxFiles = 1;

// Helper function to search for a valid entry in RootDirEntries list
// See implementation towards end of this file.
int getIndex(const char *path);

/** Get's the attribute for a given file or the root directory.

  This is a FUSE API function that is used to obtain the file
  attributes for a given file or directory.  This method only
  handles entries for the root directory.

  \param[in] path The path to the file whose attributes are to be
  retrieved.

  \param[out] stbuf The Linux stat buffer to be populated.

  \return This method returns 0 on success or -ENOENT on errors.
 */
int simple_getattr(const char *path, struct stat *stbuf) {
	// Clear out the stat buffer first.
	memset(stbuf, 0, sizeof(struct stat));
	// Handle root directory suitably.
	if (strcmp(path, "/") == 0) {
		stbuf->st_mode  = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
		stbuf->st_size  = 4096;
	} else {
		stbuf->st_mode  = S_IFREG | 0444;
		// Ensure that this file name is valid!
		const int index = getIndex(path);
		if (index == -1) {
			// Invalid entry.
			return -ENOENT;
		}
		stbuf->st_size = dataBufferSize;
		stbuf->st_nlink=1;
		stbuf->st_uid   = getuid();
		stbuf->st_gid   = getuid();
		stbuf->st_atime = 1;
		stbuf->st_mtime = 1;
		stbuf->st_ctime = 1;
	}

	return 0; // success
}

/** FUSE API method to read all file entries in a given directory.

  This is a convenience method that is used to read all the root
  directory entries into the given buffer. This function currently
  handles only root directory entries.  Consequently, the path must
  be "/".
 */
int simple_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		off_t offset, struct fuse_file_info *fi) {
	// Tell compiler we are intentionally not using 2 parameters
	(void) offset;
	(void) fi;
	// Only handle root directory on FAT-12.
	if (strcmp(path, "/") != 0) {
		return -ENOENT;
	}
	// Setup default entries for current and parent dir
	filler(buf, ".", NULL, 0);
	filler(buf, "..", NULL, 0);
	// Populate buffer with necessary entries.
	int i;
	for(i = 0; (i < MaxFiles); i++) {
		filler(buf, fileNames[i] + 1, NULL, 0);
	}
	return 0;
}

int simple_open(const char *path, struct fuse_file_info *fi) {
	// Tell compiler we are intentionally not using 1 parameter
	(void) fi;
	// Ensure that the file is valid and is not a directory etc.
	if (getIndex(path) == -1) {
		return -ENOENT;
	}
	// We just logically assume this file to be now open. We don't really
	// use inode information etc.
	return 0;
}

int simple_read(const char *path, char *buf, size_t size, off_t offset,
		struct fuse_file_info *fi) {
	// Tell compiler we are intentionally not using 1 parameter
	(void) fi;
	(void) offset;
	(void) path;
	// Get the file information for this path
	int count = fmin(size,dataBufferSize);
	
	if (count > 0) {
		int i;
		for(i = 0; (i < count); i++) {
			buf[i] = dataBuffer[i];
		}
	}
	return count;
}

//simple write that copies data to the shared buffer
int simple_write(const char *path, const char *buf, size_t size, off_t offset,
		struct fuse_file_info *fi) {
	// Tell compiler we are intentionally not using 1 parameter
	(void) fi;
	(void) offset;
	(void) path;
	// Get the file information for this path
	dataBufferSize = fmin(size,4096);
	memcpy(dataBuffer,buf,dataBufferSize);
	return dataBufferSize;
}

//need to create dummy truncate to make writing happy
int simple_truncate(const char * path, off_t offset) {
	(void) offset;
	(void) path;
	return 0;
}


// --------------------------------------------------------------
//    DO  NOT  MODIFY  CODE  BELOW  THIS  LINE
// --------------------------------------------------------------

/** Convenience method to convert date and time to a calendar time
  value.  Calendar time value represents the number of seconds
  elapsed since the Epoch, 1970-01-01 00:00:00 +0000 (UTC).

  \param[in]  day The day of the month to be used (1 to 31)

  \param[in] month The month of the year associated with the dat (1 to 12)

  \param[in] hour The hour of the day (0 to 23)

  \param[in] minute The minute of the day (0 to 59)

  \return The absolute time in milliseconds since Epoch.
 */
time_t getAbsTime(int day, int month, int hour, int minute) {
	struct tm dateTime;
	dateTime.tm_mon  = month - 1;
	dateTime.tm_mday = day;
	dateTime.tm_year = 2014 - 1900;  // Offset since 1900
	dateTime.tm_hour = hour;
	dateTime.tm_min  = minute;
	dateTime.tm_sec  = 0; // Set seconds to zero for now
	return mktime(&dateTime);
}

/** Convenience method to search for a given file

  \param[path] The name of the file to search for.

  \return This method returns the index where the file was found or
  -1 if the file was not found.
 */
int getIndex(const char* path) {
	int index = 0;
	for(index = 0; (index < MaxFiles); index++) {
		if (strcmp(fileNames[index], path) == 0) {
			return index;
		}
	}
	return -1;
}


static struct my_fuse_operations: fuse_operations {
	my_fuse_operations() {
		getattr = simple_getattr;
		readdir = simple_readdir;
		open    = simple_open;
		read    = simple_read;
		write = simple_write;
		truncate = simple_truncate;
		
	}
} my_oper;

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Usage: %s <MountPoint>\n", argv[0]);
		return 1;
	}

	for (int i=0;i<4096;i++ )
		dataBuffer[i] = '\0';
	strcpy(dataBuffer,"Test");

	// Startup the FUSE driver system
	int retVal = fuse_main(argc, argv, &my_oper, NULL);
	// Return status back
	return retVal;
}

// End of source code.
