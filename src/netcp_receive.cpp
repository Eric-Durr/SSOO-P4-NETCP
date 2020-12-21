#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//#include "../include/command_line_tools.hpp"
#include "../include/socket.hpp"

int protected_main(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    try
    {
        return protected_main(argc, argv);
    }
    catch (std::bad_alloc &e)
    {
        std::cerr << "netcp_receive: "
                  << "Not enough memory\n";
        return 1;
    }
    catch (std::system_error &e)
    {
        std::cerr << "netcp_send: " << e.what() << "\n";
        return 2;
    }
    catch (...)
    {
        std::cout << "Unknown error\n";
        return 99;
    }
}

int protected_main(int argc, char *argv[])
{
    sockaddr_in local_sock_addr = make_ip_address(8080, "127.0.0.1");
    sockaddr_in exter_sock_addr = make_ip_address(8081, "127.0.0.1");

    Message file_msg;

    Socket my_socket(local_sock_addr);

    while (1)
    {
        std::cout << "Waiting for response ..." << std::flush;
        my_socket.receive_from(file_msg, exter_sock_addr);
        //std::cout << "\n File received: \n"
        //          << file_msg.file_name << "\n";
        //std::cout << "\n File size: \n"
        //          << file_msg.file_size << "\n";
        std::cout << "\n Data received: \n"
                  << std::string{file_msg.text.data()} << "\n";
    }

    return 0;
}