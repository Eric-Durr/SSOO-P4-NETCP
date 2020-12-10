#include "../include/ip_address.hpp"

sockaddr_in make_ip_address(const int &port, const std::string &ip_address)
{
    sockaddr_in local_addr{};
    in_addr_t translated_addr = inet_addr(ip_address.c_str());

    local_addr.sin_family = AF_INET;
    ip_address == "" ? local_addr.sin_addr.s_addr = htonl(INADDR_ANY) : local_addr.sin_addr.s_addr = translated_addr;
    local_addr.sin_port = htons(port);

    return local_addr;
}