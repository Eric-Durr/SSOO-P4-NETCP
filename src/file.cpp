#include "../include/file.hpp"

File::File(const std::string &filename, int flag)
{
    fd_ = open(filename.c_str(), flag);
    if (fd_ < 0)
    {
        throw std::system_error(errno, std::system_category(), "file " + filename + " didn't open: ");
    }
}
File::~File()
{
    int result = close(fd_);
}

int File::read_file(std::string &buffer)
{
    int result = read(fd_, buffer_, sizeof(buffer_));
    if (result < 0)
    {
        throw std::system_error(errno, std::system_category(), "file can't be read: ");
    }
    buffer = std::string{buffer_};

    return result;
}