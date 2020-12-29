#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <atomic>
#include <algorithm> /*std::find*/
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
int protected_main(void);
std::vector<std::string> split_str(std::string str_to_split);
void send_file(std::exception_ptr &eptr, std::string &filename);
void send_operations(const std::string &filename);
void receive_files(std::exception_ptr &eptr, std::string &dir);
void receive_operations(const std::string &dir);
void cli_menu(std::exception_ptr &main_eptr);
bool not_a_task(const std::string &action);
// void receive_function(/* --- */);

int main(int argc, char *argv[])
{
    char *program_name = basename(argv[0]);

    try
    {
        return protected_main();
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
int protected_main(void)
{
    std::exception_ptr main_eptr{};

    std::thread pmain_th(&cli_menu, std::ref(main_eptr));

    if (main_eptr)
    {
        std::rethrow_exception(main_eptr);
    }
    pmain_th.join();
    return 0;
}

// Main tasks

void cli_menu(std::exception_ptr &main_eptr)
{
    try
    {
        /*Tasks threads*/
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
            // Command parse
            action = "";
            arg = "";
            std::cout << "command: ";
            char buff[100];
            std::cin.getline(buff, 100);
            std::string command = buff;
            if (!command.empty())
            {
                action = split_str(command)[0];
                arg = split_str(command)[1];
            }

            // Tasks launch
            //// Unknown action check
            if (not_a_task(action) && !command.empty())
            {
                std::cout << "\n Unknown command for Netcp, try Help for more info\n\n";
            }
            //// Help message
            if (action == "Help")
            {
                std::cout << helpmsg;
            }
            //// Errored launch of tasks
            if (action == "Receive" && arg.empty())
            {
                std::cout << "\nReceive needs a directory to store the files, try Help for more info\n";
            }
            if (action == "Send" && arg.empty())
            {
                std::cout << "\nA file must be specified as argument to send, try Help for more info\n";
            }
            //// Receive task launch
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
            //// Send task launch
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
            //// Abort commands
            if (action == "Abort" && arg.empty())
            {
                if (arg.empty())
                {
                    quit_send = true;
                }
                if (arg == "receive")
                {
                    quit_receive = true;
                    //pthread_kill(receive_th.native_handle(), SIGUSR1);
                    std::cout << "Aborted file receiving\n";
                }
            }
            //// Return to CLI message
            if (action != "Quit" && action != "Clear" && !command.empty())
            {
                sleep(1);
                std::cout << "\n\n\tPress enter to continue ...\n";
                std::getchar();
            }
            //// Clear console
            if (action == "Clear")
            {
                system("clear");
            }
            //// Quit main task
            if (action == "Quit")
            {

                system("clear");
                quit_receive = true;
                quit_send = true;
            }

        } while (action != "Quit");

        receive_th.join();
        send_th.join();
    }
    catch (...)
    {
        main_eptr = std::current_exception();
    }
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

    while (true)
    {
        if (quit_receive == true)
        {
            break;
        }
        /* 
        std::cout << "Waiting for response ...\n"
                  << std::flush;

        Message file_msg;

        std::string complete_message;
        std::string name;
        int sz = 0;

        my_socket.receive_from(file_msg, exter_sock_addr);

        if (file_msg.msg_id == 0)
        {
            name = file_msg.name.data();
            name.append(".received");
            sz = file_msg.file_size;
        }
        File received_file(dir + "/" + name,
                           O_RDWR | O_CREAT | O_TRUNC, sz);

        while (file_msg.msg_id != -2)
        {

            if (quit_receive == true)
            {
                system("clear");
                std::cout << "\nAborted file sending\n";
                return;
            }
            my_socket.receive_from(file_msg, exter_sock_addr);

            if (file_msg.msg_id != -2 && file_msg.msg_id != 0)
            {
                complete_message.append(file_msg.text.data());

                received_file.write_file(complete_message);
            }
        }
        std::cout << "Received " << name << ", stored in " << dir << "\n";
         */
    }
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
        // sleep(2);
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

// Menu tools
bool not_a_task(const std::string &action)
{
    std::vector<std::string> acc_actions =
        {"Help",
         "Receive",
         "Send",
         "Quit",
         "Abort",
         "Pause",
         "Resume"};

    return !(std::find(acc_actions.begin(), acc_actions.end(), action) != acc_actions.end());
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
