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
    char c_buffer[1024];
    int result = read(fd_, &c_buffer, sizeof(c_buffer));
    if (result < 0)
    {
        throw std::system_error(errno, std::system_category(), "file can't be read: ");
    }

    buffer = c_buffer;
    return result;
}