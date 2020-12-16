#ifndef MESSAGE_H_
#define MESSAGE_H_
#include <array>

struct Message
{
    // char name[1024];
    std::array<char, 1024> text;
    //size_t size;
};

#endif /* MESSAGE_H_ */