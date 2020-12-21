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
        std::cerr << "netcp_receive: "
                  << "Not enough memory\n";
        return 1;
    }
    catch (std::system_error &e)
    {
        std::cerr << "netcp_receive: " << e.what() << "\n";
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

    std::cout << "Waiting for response ...\n"
              << std::flush;

    std::string complete_message;
    std::string name;
    off_t sz = 0;
    while (file_msg.msg_id != -2)
    {
        my_socket.receive_from(file_msg, exter_sock_addr);

        if (file_msg.msg_id == 0)
        {
            //name.append("_received.txt");
            sz = file_msg.file_size;
        }
        else if (file_msg.msg_id != -2)
        {
            complete_message.append(file_msg.text.data());
        }
    }
    std::cout << complete_message << std::endl;
    File received_file("received.txt", O_RDWR | O_CREAT | O_TRUNC, sz);
    received_file.write_file(complete_message);

    return 0;
}