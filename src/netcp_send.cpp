#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

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

    /* Message structure */

    Message file_msg;

    /* File oppenning */
    File local_file(argv[1], O_RDONLY);

    /* create socket */
    Socket my_socket(local_sock_addr);

    /* read file and create message */
    std::size_t bytes_read = 0;
    for (size_t i = 0; i < local_file.length(); i += file_msg.text.size())
    {
        buffer_str = static_cast<char *>(local_file.region() + i);

        buffer_str.copy(
            file_msg.text.data(),
            file_msg.text.size() - 1,
            0);
        std::cout << file_msg.text.data() << "\n";
        file_msg.text[0] = '\0';
        /* send message */
        //my_socket.send_to(file_msg, bytes_read, exter_sock_addr);
    }

    return 0;
}