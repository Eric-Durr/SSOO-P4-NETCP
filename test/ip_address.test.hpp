

#include "../include/Catch2/catch.h"
#include "../include/ip_address.hpp"

SCENARIO("IP ADDRESS MAKER FUNCTION TESTS")
{
    GIVEN("Any ip adress as string ans any port as integer")
    {
        std::string ip_str = "127.0.0.1";
        int port = 5;
        WHEN("Function called with given values")
        {
            sockaddr_in test_address = make_ip_address(port, ip_str);
            THEN("Returns a perfectly initialized sockaddr_in structure")
            {
                REQUIRE(test_address.sin_family == AF_INET);
                REQUIRE(test_address.sin_addr.s_addr == inet_addr("127.0.0.1"));
                REQUIRE(test_address.sin_port == htons(5));
            }
        }
        AND_WHEN("Function is called with an empty ip address")
        {
            sockaddr_in test_address = make_ip_address(port, "");
            THEN("IP address is storaged as INADDR_ANY")
            {
                REQUIRE(test_address.sin_family == AF_INET);
                REQUIRE(test_address.sin_addr.s_addr == INADDR_ANY);
                REQUIRE(test_address.sin_port == htons(5));
            }
        }
    }
}