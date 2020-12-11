#include <iostream>
#include "../include/command_line_tools.hpp"

void usage(void)
{
    std::cout << "usage: ./netcp_send [port] [ip address] [file name]\n";
    std::cout << "try ./netcp_send -h for more information\n";
}

void help(void)
{
    /* help mesage here */
}

int handle_entrace(int argc, char *argv[])
{

    if (argc == 2 && argv[1] == "-h")
    {
        help();
        return 0;
    }
    if (argc < 3)
    {
        std::cerr << "There are less arguments than expected.\n";
        usage();
        return 1;
    }
    if (argc > 4)
    {
        std::cerr << "There are more arguments than expected.\n";
        usage();
        return 2;
    }

    if (!isdigit(*argv[1]))
    {
        std::cerr << "The port must be indicated as a number.\n";
        usage();
        return 3;
    }
    if (argc == 4 && !match_ip_addr(argv[2]))
    {
        std::cerr << "The IP adress" << argv[2] << " is not a valid IP adress.\n";
        usage();
        return 4;
    }

    return -1;
}

bool match_ip_addr(const std::string &str_ip_addr)
{

    return std::regex_match(str_ip_addr,
                            std::regex("(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.)"
                                       "{3}([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])"));
}