#ifndef MESSAGE_H_
#define MESSAGE_H_
#include <array>

struct Message
{
    char name[1024];
    size_t size;
    std::array<char, 1024> text;
};

#endif /* MESSAGE_H_ */