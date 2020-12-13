#ifndef FILE_HPP_
#define FILE_HPP_

#include <iostream>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <system_error>
#include <stdexcept>
#include <string>
#include <sys/stat.h>
#include <fcntl.h>

#define _1KB_ 1024
#define _2KB_ 2048
#define _4KB_ 4096
#define _8KB_ 8192
#define _16KB_ 16384
#define _32KB_ 32768

class File
{
public:
    File(const std::string &filename, int flag);
    ~File();

    const int &fd(void) const { return fd_; }
    int &fd(void) { return fd_; }

    int read_file(std::string &buffer);
    // void write(const std::string &buffer);

    bool fd_error(void) { return ((fd_ < 0) ? true : false); }

private:
    int fd_;
    char buffer_[_32KB_];
};

#endif // FILE_HPP_