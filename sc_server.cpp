#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <zconf.h>

#include "simple_socket_communication/simple_socket_communication.h"
#include "services.h"

int main() {

    int sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd == -1) return EXIT_FAILURE;

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // converted to network byte order
    server_addr.sin_port = htons(2233);

    int reuseaddr = 1;
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int))) {
        std::cerr << "Warning! failed to set SO_REUSEADDR option." << std::endl;
        _exit(EXIT_FAILURE);
    }

    int bind_status = bind(sd, reinterpret_cast<const sockaddr *>(&server_addr), sizeof(server_addr));
    if (bind_status == -1) {
        std::cout << "Couldn't bind to address." << std::endl;
        return EXIT_FAILURE;
    }

    listen(sd, 1);
    struct sockaddr *handler;
    socklen_t handler_addr_len;

    while (true) {
        handler = static_cast<sockaddr *>(malloc(sizeof(struct sockaddr)));
        handler_addr_len = sizeof(handler);
        int handler_sd = accept(sd, handler, &handler_addr_len);

        char *opt_buff = new char[BUFFER_SIZE]();
        bzero(opt_buff, sizeof(char));

        ssize_t num_recv = recv(handler_sd, opt_buff, sizeof(char) * 1, 0);
        if (num_recv <= 0) {
            std::cerr << "Error receiving input from client. | errno = " << errno << std::endl;
            continue;
        }

        // process request options
        std::string response;
        if (num_recv == 1 && *opt_buff == 'm') {
            int word_count = 0;
            receive_and_process(handler_sd, word_count, _get_space_count);
            response = std::to_string(word_count + 1);

        } else if (*opt_buff == 't') {
            response = get_time_str();

        } else if (*opt_buff == 'd') {
            response = get_date_str();

        } else if (*opt_buff == 'g') {
            response = get_greeting();
        }

        send(handler_sd, response.c_str(), response.length(), 0);

        close(handler_sd);
        free(handler);

    }
}


// ToDO
// fork  https://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
