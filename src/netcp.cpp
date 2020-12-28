#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <atomic>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <exception>
#include <thread>

#include "../include/socket.hpp"
#include "../include/file.hpp"

// Atomic bools

std::atomic<bool> quit_send{false};
std::atomic<bool> quit_receive{false};

// Function declarations
int protected_main(int argc, char *argv[]);
std::vector<std::string> split_str(std::string str_to_split);
void send_file(std::exception_ptr &eptr, std::string &filename);
void send_operations(const std::string &filename);
void receive_files(std::exception_ptr &eptr, std::string &dir);
void receive_operations(const std::string &dir);
void cli_menu(std::exception_ptr &main_eptr);
// void receive_function(/* --- */);

int main(int argc, char *argv[])
{
    char *program_name = basename(argv[0]);

    try
    {
        return protected_main(argc, argv);
    }
    catch (std::bad_alloc &e)
    {
        std::cerr << program_name
                  << ": Not enough memory\n";
        return 1;
    }
    catch (std::system_error &e)
    {
        std::cerr << program_name
                  << ": " << e.what() << "\n";
        return 2;
    }
    catch (...)
    {
        std::cout << program_name
                  << ": Unknown error\n";
        return 99;
    }
}
int protected_main(int argc, char *argv[])
{
    std::exception_ptr main_eptr{};
    std::thread pmain_th(&cli_menu, std::ref(main_eptr));
    pmain_th.join();
    if (main_eptr)
    {
        std::rethrow_exception(main_eptr);
    }

    return 0;
}

std::vector<std::string> split_str(std::string str_to_split)
{
    std::vector<std::string> result;
    std::stringstream ss(str_to_split);
    for (std::string substr; ss >> substr;)
    {
        result.push_back(substr);
    }
    if (result.size() < 2)
    {
        result.push_back("");
    }
    if (result.size() > 2)
    {
        std::vector<std::string> temp = {result[0], result[1]};
        result = temp;
    }
    return result;
}

void send_file(std::exception_ptr &eptr, std::string &filename)
{
    try
    {
        send_operations(filename);
    }
    catch (...)
    {
        eptr = std::current_exception();
    }
}

void send_operations(const std::string &filename)
{
    sockaddr_in local_sock_addr = make_ip_address(8081, "127.0.0.1");
    sockaddr_in exter_sock_addr = make_ip_address(8080, "127.0.0.1");

    std::string buffer_str;
    //char *buffer_str[_1KB_];

    Message file_msg;

    File local_file(filename, O_RDWR);

    file_msg.file_size = local_file.length();

    Socket my_socket(local_sock_addr);

    size_t i = 0;
    while (static_cast<char *>(local_file.region())[i] != '\0')
    {
        file_msg.msg_id = i;
        buffer_str.push_back(static_cast<char *>(local_file.region())[i]);

        if (i == 0)
        {
            strcpy(file_msg.name.data(), filename.c_str());
            my_socket.send_to(file_msg,
                              file_msg.text.size() - 1,
                              exter_sock_addr);
        }

        if ((buffer_str.size() >= _1KB_) ||
            (static_cast<char *>(local_file.region())[i + 1] == '\0'))
        {
            buffer_str.copy(
                file_msg.text.data(),
                file_msg.text.size() - 1,
                0);
            my_socket.send_to(
                file_msg,
                buffer_str.size(),
                exter_sock_addr);
            buffer_str.resize(0);
        }

        i++;
        sleep(5);
        if (quit_send == true)
        {
            system("clear");
            std::cout << "\nAborted file sending\n";
            return;
        }
    }
    file_msg.msg_id = -2;
    my_socket.send_to(file_msg, sizeof(file_msg), exter_sock_addr);

    std::cout << "\nFile " << filename << " has been sent\n";
}

void receive_files(std::exception_ptr &eptr, std::string &folder)
{
    try
    {
        receive_operations(folder);
    }
    catch (...)
    {
        eptr = std::current_exception();
    }
}
void receive_operations(const std::string &dir)
{
    // Open dir
    int result = mkdir(dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if ((result < 0) && (errno != EEXIST))
    {
        throw std::system_error(errno, std::system_category(), "unable to create directory");
    }
    else if (errno == EEXIST)
    {
    }

    // Search files
    sockaddr_in local_sock_addr = make_ip_address(8080, "127.0.0.1");
    sockaddr_in exter_sock_addr = make_ip_address(8081, "127.0.0.1");

    Socket my_socket(local_sock_addr);

    while (1)
    {
        std::cout << "Waiting for response ...\n"
                  << std::flush;

        Message file_msg;

        std::string complete_message;
        std::string name;
        off_t sz = 0;
        while (file_msg.msg_id != -2)
        {
            if (quit_receive == true)
            {
                system("clear");
                std::cout << "\nAborted file sending\n";
                return;
            }
            my_socket.receive_from(file_msg, exter_sock_addr);

            if (file_msg.msg_id == 0)
            {
                name = file_msg.name.data();
                name.append(".received");
                sz = file_msg.file_size;
            }
            else if (file_msg.msg_id != -2)
            {
                complete_message.append(file_msg.text.data());
            }
        }
        File received_file(dir + "/" + name, O_RDWR | O_CREAT | O_TRUNC, sz);
        received_file.write_file(complete_message);
        std::cout << "Received " << name << ", stored in " << dir << "\n";
    }
}

void cli_menu(std::exception_ptr &main_eptr)
{
    try
    {
        /*Useful  threads*/
        std::thread send_th;
        std::thread receive_th;

        /* CLI menu */
        std::string arg;
        std::string action;
        const std::string helpmsg = "\n\n\t\tALLOWED COMMANDS:\n"
                                    "Send [filename] | send the given file to another socket\n"
                                    "Receive [folder] | preare the given folder to receive files\n"
                                    "           (if the folder doesn't exsist a new folder is created)\n"
                                    "Clear | Clear console.\n"
                                    "Quit | End the process.\n";

        std::cout << "\n\n-- NETCP CLI INTERFACE --\n";
        do
        {
            std::cout << "command: ";
            char buff[100];
            std::cin.getline(buff, 100);

            std::string command = buff;
            if (!split_str(command).empty())
            {
                action = split_str(command)[0];
                arg = split_str(command)[1];
            }

            if (action == "Help")
            {
                std::cout << helpmsg;
            }

            if (action == "Receive" && arg.empty())
            {
                std::cout << "\nReceive needs a directory to store the files, try Help for more info\n";
            }
            if (action == "Send" && arg.empty())
            {
                std::cout << "\nA file must be specified as argument to send, try Help for more info\n";
            }

            if (action == "Receive" && !arg.empty())
            {
                quit_receive = false;
                std::cout << "\nReceiving into " << arg << " directory\n";

                std::exception_ptr receive_eptr{};
                receive_th = std::thread{&receive_files, std::ref(receive_eptr),
                                         std::ref(arg)};

                if (receive_eptr)
                {
                    std::rethrow_exception(receive_eptr);
                }
            }

            if (action == "Send" && !arg.empty())
            {
                quit_send = false;
                std::exception_ptr send_eptr{};
                send_th = std::thread{&send_file, std::ref(send_eptr),
                                      std::ref(arg)};

                if (send_eptr)
                {
                    std::rethrow_exception(send_eptr);
                }
            }

            if (action == "Abort" && arg.empty())
            {
                quit_send = true;

                send_th.join();
            }
            if (action == "Abort" && arg == "receive")
            {
                quit_receive = true;
            }

            if (action != "Quit" && action != "Clear")
            {
                std::cout << "\n\n\tPress enter to continue ...\n";
                std::getchar();
            }
            if (action == "Clear")
            {
                system("clear");
            }
            if (action == "Quit")
            {
                quit_receive = true;
                quit_send = true;

                system("clear");
            }

        } while (action != "Quit");

        send_th.join();
    }
    catch (...)
    {
        main_eptr = std::current_exception();
    }
}