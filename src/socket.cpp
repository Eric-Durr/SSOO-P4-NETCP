#include "../include/socket.hpp"

Socket::Socket(const sockaddr_in &adress)
{
    sock_fd_ = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock_fd_ >= 0)
    {
        int result = bind(sock_fd_,
                          reinterpret_cast<const sockaddr *>(&adress),
                          sizeof(adress));
        if (result < 0)
        {
            throw std::system_error(errno, std::system_category(), "unable to create socket: ");
        }
    }
    else
    {
        throw std::system_error(errno, std::system_category(), "failed bind : ");
    }
}
Socket::~Socket()
{
    close(sock_fd_);
}

void Socket::send_to(Message &message, const size_t &msg_sz, const sockaddr_in &address)
{
    message.text[msg_sz] = '\0';

    int result = sendto(sock_fd_, &message, msg_sz, 0,
                        reinterpret_cast<const sockaddr *>(&address),
                        sizeof(address));
    if (result < 0)
    {
        throw std::system_error(errno, std::system_category(), "failed sendto: ");
    }
}
void Socket::receive_from(Message &message, sockaddr_in &address)
{
    socklen_t src_len = (sizeof(address));

    int result = recvfrom(sock_fd_, &message, sizeof(message), 0,
                          reinterpret_cast<sockaddr *>(&address),
                          &src_len);
    message.text[result] = '\0';
    if (result < 0)
    {
        throw std::system_error(errno, std::system_category(), "failed recvfrom: ");
    }
}