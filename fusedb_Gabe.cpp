int simple_read(const char *path, char *buf, size_t size, off_t offset,
                struct fuse_file_info *fi) {
        // Tell compiler we are intentionally not using 1 parameter
        (void) fi;
        //(void) path;
        // Get the file information for this path
        // Get the information for the file
        // Copy the necessary information into the buffer
        std::string movieBuf = std::string(buf);
        // Get the file information for this path
        getMovieInfo(path, movieBuf);

        const int count = fmin(4095 - offset, size);
        if (count > 0) {
                unsigned int i;
                // changed to go until it reaches the size of the buffer
                for(i = 0; (i < movieBuf.size()); i++) {
                        buf[i] = movieBuf[i];
                }
        }
        for (unsigned int i=count;i<size;i++)
                buf[i] = ' ';
        return count;
}

