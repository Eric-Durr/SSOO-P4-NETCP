#ifndef MESSAGE_H_
#define MESSAGE_H_
#include <array>
#include <string>

struct Message
{
    std::array<char, 1024> text;
    std::string file_name;
    size_t file_size;
};

#endif /* MESSAGE_H_ */