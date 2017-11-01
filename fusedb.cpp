
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
#include <vector>

#include "moviesdb.h"

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
			stbuf->st_nlink = 1;
			stbuf->st_size  = 4096;
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
	std::vector<std::string> movies;
	getMovieListing(movies);

	for (unsigned int i=0;i<movies.size();i++)
		filler(buf, movies[i].c_str(), NULL, 0);
	return 0;
}

int simple_open(const char *path, struct fuse_file_info *fi) {
	// Tell compiler we are intentionally not using 1 parameter
	(void) fi;
	(void)path;
	// Ensure that the file is valid and is not a directory etc.
	// We just logically assume this file to be now open. We don't really
	// use inode information etc.

	//should validate name of movie that it exists
	return 0;
}

int simple_read(const char *path, char *buf, size_t size, off_t offset,
		struct fuse_file_info *fi) {
	// Tell compiler we are intentionally not using 1 parameter
	(void) fi;
	(void) path;
	// Get the file information for this path
	// Get the information for the file
	// Copy the necessary information into the buffer

	const int count = fmin(4096 - offset, size);
	if (count > 0) {
		int i;
		for(i = 0; (i < count); i++) {
			buf[i] = 'a';
		}
	}
	for (unsigned int i=count;i<size;i++)
		buf[i] = ' ';
	
	return count;
}

int simple_write(const char *path, const char *buf, size_t size, off_t offset,
		struct fuse_file_info *fi) {
	
	// Tell compiler we are intentionally not using 1 parameter
	(void) fi;
	(void) buf;
	(void) size;
	(void) offset;
	(void) path;
	// Get the file information for this path
	return 0 ;
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
	// Startup the FUSE driver system
	int retVal = fuse_main(argc, argv, &my_oper, NULL);
	// Return status back
	return retVal;
}

// End of source code.
