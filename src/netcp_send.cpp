#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <exception>
#include <thread>

//#include "../include/command_line_tools.hpp"
#include "../include/socket.hpp"
#include "../include/file.hpp"

int protected_main(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    try
    {
        return protected_main(argc, argv);
    }
    catch (std::bad_alloc &e)
    {
        std::cerr << "netcp_send: "
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
    char *program_name = basename(argv[0]);

    if (argc != 2)
    {
        std::cerr << "Usage: " << program_name << " [file]\n";
        return 1;
    }

    /* Set addresses */
    sockaddr_in local_sock_addr = make_ip_address(8081, "127.0.0.1");
    sockaddr_in exter_sock_addr = make_ip_address(8080, "127.0.0.1");

    /* Message buffer */
    std::string buffer_str;
    //char *buffer_str[_1KB_];

    /* Message structure */

    Message file_msg;

    /* File oppenning */
    File local_file(argv[1], O_RDWR);

    file_msg.file_size = local_file.length();

    /* create socket */
    Socket my_socket(local_sock_addr);

    /* Sending memory mapped file content*/
    size_t i = 0;
    while (static_cast<char *>(local_file.region())[i] != '\0')
    {
        file_msg.msg_id = i;
        buffer_str.push_back(static_cast<char *>(local_file.region())[i]);

        /* Sending file metadata for creation*/
        if (i == 0)
        {
            strcpy(file_msg.name.data(), argv[1]);
            my_socket.send_to(file_msg,
                              file_msg.text.size() - 1,
                              exter_sock_addr);
        }

        if ((buffer_str.size() >= _1KB_) ||
            (static_cast<char *>(local_file.region())[i + 1] == '\0'))
        {
            buffer_str.copy(
                file_msg.text.data(),
                file_msg.text.size() - 1,
                0);
            my_socket.send_to(
                file_msg,
                buffer_str.size(),
                exter_sock_addr);
            buffer_str.resize(0);
        }

        i++;
    }
    /* terminate message */
    file_msg.msg_id = -2;
    my_socket.send_to(file_msg, sizeof(file_msg), exter_sock_addr);

    return 0;
}