#include <iostream>
#include <fstream>

#include "../include/command_line_tools.hpp"
#include "socket.hpp"

int main(int argc, char *argv[])
{
    int status = handle_entrace(argc, argv);

    if (status == -1)
    {
        /* Set an address */
        sockaddr_in sock_addr;

        if (argc == 3) // default IP adress option
        {
            sock_addr = make_ip_address(atoi(argv[1]), "");
        }

        if (argc == 4) // specified IP adress option
        {
            sock_addr = make_ip_address(atoi(argv[1]), argv[2]);
        }

        /* open file */

        /* read file and create message */

        /* create socket */
        Socket my_socket(sock_addr);

        /* send message */

        return 0;
    }

    return status;
}
