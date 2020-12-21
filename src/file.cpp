#include "../include/file.hpp"

File::File(const std::string &filename, int flag, int sz)
{
    fd_ = open(filename.c_str(), flag);

    if (fd_ < 0)
    {
        throw std::system_error(errno, std::system_category(), "file " + filename + " wasnt created");
    }

    if (sz == -1)
    {
        length_ = lseek(fd_, 0, SEEK_END);
    }
    else
    {
        resize(sz);
    }

    mmapped_file_ = mmap(
        nullptr,
        length_,
        PROT_READ,
        MAP_SHARED,
        fd_,
        0);
    if (mmapped_file_ == MAP_FAILED)
    {
        throw std::system_error(errno, std::system_category(), "memory map failed");
    }
}

File::~File()
{
    munmap(mmapped_file_, length_);

    close(fd_);
}

void File::resize(int length)
{
    length_ = length;
    int result = ftruncate(fd_, length_);
    if (result < 0)
    {
        throw std::system_error(errno, std::system_category(), "failed file resize");
    }
}

int File::read_file(std::string &buffer)
{
    int result = read(fd_, buffer_, sizeof(buffer_));
    buffer_[result] = '\0';
    if (result < 0)
    {
        throw std::system_error(errno, std::system_category(), "file can't be read");
    }
    buffer.resize(sizeof(buffer_));
    buffer = std::string{buffer_};
    return result;
}

int File::write_file(const std::string &buffer)
{
    int result = write(fd_, buffer.c_str(), strlen(buffer.c_str()));

    if (result < 0)
    {
        throw std::system_error(errno, std::system_category(), "file can't be written");
    }
    return result;
}

void File::mmap_file(void)
{
    mmapped_file_ = mmap(
        nullptr,
        length_,
        PROT_READ,
        MAP_SHARED,
        fd_,
        0);
    if (mmapped_file_ == MAP_FAILED)
    {
        throw std::system_error(errno, std::system_category(), "memory map failed");
    }
}
void File::munmap_file(void)
{
    int result = munmap(mmapped_file_, length_);
    if (result < 0)
    {
        throw std::system_error(errno, std::system_category(), "memory unmap failed");
    }
}