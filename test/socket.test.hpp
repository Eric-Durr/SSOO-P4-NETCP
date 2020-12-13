
#include "../include/Catch2/catch.h"
#include "../include/socket.hpp"
#include "../include/ip_address.hpp"

SCENARIO("CREATING A SOCKET")
{
    GIVEN("A socket address")
    {
        sockaddr_in generic_addr = make_ip_address(8082, "");
        WHEN("The socket object is created succesfully")
        {
            Socket my_socket(generic_addr);
            THEN("A file descriptor must be accessible")
            {
                REQUIRE(my_socket.fd() >= 0);
            }
        }
    }
}

SCENARIO("COMUNICATION BETWEEN SOCKETS")
{
    GIVEN("Two sockets and a message")
    {
        std::string msg_text = "Hello world!";
        Message hello_world;
        Message received_msg;
        msg_text.copy(hello_world.text.data(), hello_world.text.size() - 1, 0);
        sockaddr_in s_addr = make_ip_address(8080, "127.0.1.1");
        sockaddr_in r_addr = make_ip_address(8081, "127.0.0.1");
        Socket socket_sender(s_addr);
        Socket socket_receiver(r_addr);
        WHEN("The sender sends a message")
        {
            socket_sender.send_to(hello_world, r_addr);
            socket_receiver.receive_from(received_msg, s_addr);
            THEN("The receiver have the message sended")
            {
                CHECK(std::string{received_msg.text.data()} ==
                      std::string{hello_world.text.data()});
            }
        }
    }
}
