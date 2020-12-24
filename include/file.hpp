#ifndef FILE_HPP_
#define FILE_HPP_

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <iostream>
#include <cerrno>
#include <cstring>
#include <system_error>
#include <stdexcept>
#include <string>
#include <unistd.h>

#define _1KB_ 1024
#define _2KB_ 2048
#define _4KB_ 4096
#define _8KB_ 8192
#define _16KB_ 16384
#define _32KB_ 32768

class File
{
public:
    File(const std::string &filename, int flag, int sz = -1);

    ~File();

    const int &fd(void) const { return fd_; }
    int &fd(void) { return fd_; }

    const char *buffer(void) const { return buffer_; }
    const char *buffer(void) { return buffer_; }

    const void *region(void) const { return mmapped_file_; }
    void *region(void) { return mmapped_file_; }

    const int length(void) const { return lseek(fd_, 0, SEEK_END); }
    int length(void) { return lseek(fd_, 0, SEEK_END); }

    void resize(int length);

    int read_file(std::string &buffer);
    int write_file(const std::string &buffer);

    void mmap_file(void);
    void munmap_file(void);

    bool fd_error(void) { return ((fd_ < 0) ? true : false); }

private:
    int fd_;
    char buffer_[_1KB_];
    void *mmapped_file_;
    int length_;
};

#endif // FILE_HPP_