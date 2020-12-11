#ifndef SOCKET_HPP_
#define SOCKET_HPP_

#include <iostream>
#include <unistd.h>
#include <cerrno>
#include <cstring>

#include <system_error>

#include "ip_address.hpp"
#include "message.hpp"

class Socket
{
public:
    Socket(const sockaddr_in &adress);
    ~Socket();

    const int &fd(void) const { return sock_fd_; }
    int &fd(void) { return sock_fd_; }

    bool fd_error(void) { return ((sock_fd_ < 0) ? true : false); }
    bool bind_error(void) { return ((bind_status_ < 0) ? true : false); }

    void send_to(const Message &message, const sockaddr_in &address);
    void receive_from(Message &message, sockaddr_in &address);

private:
    int sock_fd_;
    int bind_status_;
};

#endif // SOCKET_HPP_