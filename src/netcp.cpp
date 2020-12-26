#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <exception>
#include <thread>

#include "../include/socket.hpp"
#include "../include/file.hpp"

int protected_main(int argc, char *argv[]);
std::vector<std::string> split_str(std::string str_to_split);
// void send_function(/* --- */);
// void receive_function(/* --- */);

int main(int argc, char *argv[])
{
    char *program_name = basename(argv[0]);

    try
    {
        return protected_main(argc, argv);
    }
    catch (std::bad_alloc &e)
    {
        std::cerr << *program_name
                  << ": Not enough memory\n";
        return 1;
    }
    catch (std::system_error &e)
    {
        std::cerr << *program_name
                  << e.what() << "\n";
        return 2;
    }
    catch (...)
    {
        std::cout << *program_name
                  << ": Unknown error\n";
        return 99;
    }
}
int protected_main(int argc, char *argv[])
{
    /* CLI menu */
    std::string arg;
    std::string action;
    std::string helpmsg;
    std::cout << "-- NETCP CLI INTERFACE --\n";
    do
    {
        std::cout << "command: ";
        char buff[100];
        std::cin.getline(buff, 100);
        std::string command = buff;
        action = split_str(command)[0];
        arg = split_str(command)[1];

        if (action == "Help")
        {
            std::cout << helpmsg;
        }
        if (action == "Receive" && arg.empty())
        {
            std::cout << "\nReceive needs a directory to store the files, try again\n";
        }
        if (action == "Send" && arg.empty())
        {
            std::cout << "\nA file must be specified as argument to send, try again\n";
        }
        if (action == "Receive" && !arg.empty())
        {
            std::cout << "Receiving into " << arg << " directory";
            // Thread here
        }
        if (action == "Send" && !arg.empty())
        {
            std::cout << "Sending " << arg << " file";
            // Thread here
        }

        if (action != "Quit")
        {
            std::cout << "\n\n\tPress enter to continue ...\n";
            std::getchar();
            system("clear");
        }

    } while (action != "Quit");

    return 0;
}

std::vector<std::string> split_str(std::string str_to_split)
{
    std::vector<std::string> result;
    std::stringstream ss(str_to_split);
    for (std::string substr; ss >> substr;)
    {
        result.push_back(substr);
    }
    if (result.size() < 2)
    {
        result.push_back("");
    }
    if (result.size() > 2)
    {
        std::vector<std::string> temp = {result[0], result[1]};
        result = temp;
    }
    return result;
}