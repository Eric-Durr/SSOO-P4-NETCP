#ifndef IP_ADDRESS_H_
#define IP_ADDRESS_H_
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

sockaddr_in make_ip_address(const int &port, const std::string &ip_address = "");

#endif /* IP_ADDRESS_H_ */