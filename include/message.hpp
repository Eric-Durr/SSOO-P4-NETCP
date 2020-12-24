#ifndef MESSAGE_H_
#define MESSAGE_H_
#include <array>
#include <string>

struct Message
{
    int msg_id = -1;
    std::array<char, 1024> text;

    size_t file_size;
    std::array<char, 1024> name;
};

#endif /* MESSAGE_H_ */