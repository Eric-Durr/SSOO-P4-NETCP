#include "../include/file.hpp"

File::File(const std::string &filename, int flag)
{
    fd_ = open(filename.c_str(), flag);
    if (fd_ < 0)
    {
        throw std::system_error(errno, std::system_category(), "file " + filename + " didn't open: ");
    }

    length_ = lseek(fd_, 0, SEEK_END);
    mmapped_file_ = mmap(
        nullptr,
        length_,
        PROT_READ,
        MAP_SHARED,
        fd_,
        0);
    if (mmapped_file_ == MAP_FAILED)
    {
        throw std::system_error(errno, std::system_category(), "memroy map failed: ");
    }
}
File::~File()
{
    munmap(mmapped_file_, length_);

    close(fd_);
}

int File::read_file(std::string &buffer)
{
    int result = read(fd_, buffer_, sizeof(buffer_));
    buffer_[result] = '\0';
    if (result < 0)
    {
        throw std::system_error(errno, std::system_category(), "file can't be read: ");
    }
    buffer.resize(sizeof(buffer_));
    buffer = std::string{buffer_};
    return result;
}