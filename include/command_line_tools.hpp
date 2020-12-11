#ifndef CL_TOOLS_HPP_
#define CL_TOOLS_HPP_

#include <regex>

void usage(void);
void help(void);
int handle_entrace(int argc, char *argv[]);
bool match_ip_addr(const std::string &str_ip_addr);

#endif /* CL_TOOLS_HPP_ */