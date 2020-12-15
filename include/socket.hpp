#ifndef SOCKET_HPP_
#define SOCKET_HPP_

#include <iostream>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <stdexcept>

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

    void send_to(Message &message, const size_t &msg_sz, const sockaddr_in &address);
    void receive_from(Message &message, sockaddr_in &address);

private:
    int sock_fd_;
};

#endif // SOCKET_HPP_