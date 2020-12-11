#include "../include/socket.hpp"

Socket::Socket(const sockaddr_in &adress) : bind_status_(-1)
{
    sock_fd_ = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock_fd_ >= 0)
    {
        bind_status_ = bind(sock_fd_,
                            reinterpret_cast<const sockaddr *>(&adress),
                            sizeof(adress));
        if (bind_status_ < 0)
        {
            std::cerr << "bind failed: " << std::strerror(errno) << '\n';
        }
    }
    else
    {
        std::cerr << "unable to create socket: " << std::strerror(errno) << '\n';
    }
}
Socket::~Socket()
{
    close(sock_fd_);
}

void Socket::send_to(const Message &message, const sockaddr_in &address)
{
    int result = sendto(sock_fd_, &message, sizeof(message), 0,
                        reinterpret_cast<const sockaddr *>(&address),
                        sizeof(address));
    if (result < 0)
    {
        std::cerr << "failed sendto: " << std::strerror(errno) << '\n';
    }
}
void Socket::receive_from(Message &message, sockaddr_in &address)
{
    socklen_t src_len = (sizeof(address));

    int result = recvfrom(sock_fd_, &message, sizeof(message), 0,
                          reinterpret_cast<sockaddr *>(&address),
                          &src_len);
    if (result < 0)
    {
        std::cerr << "failed recvfrom: " << std::strerror(errno) << '\n';
    }
}