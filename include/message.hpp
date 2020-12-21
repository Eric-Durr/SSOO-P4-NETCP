#ifndef MESSAGE_H_
#define MESSAGE_H_
#include <array>
#include <string>

struct Message
{
    int msg_id = -1;
    std::array<char, 1024> text;
    char *file_name[1024];
    size_t file_size;
};

#endif /* MESSAGE_H_ */