#ifndef MESSAGE_H_
#define MESSAGE_H_
#include <array>

struct Message
{
    std::array<char, 1024> text;
    /* more data */
};

#endif /* MESSAGE_H_ */