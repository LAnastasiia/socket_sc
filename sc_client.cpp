#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <zconf.h>
#include <arpa/inet.h>
#include <vector>
#include <bitset>

#include "simple_socket_communication/simple_socket_communication.h"


int main(int argc, char** argv)
{
    if ( ! (argc >= 3  && strlen(argv[2]) == 1 && ((*argv[2] == 'm') ? argc == 4 : argc == 3)) )
        return EXIT_FAILURE;

    int sd = socket (AF_INET, SOCK_STREAM, 0);
    if (sd == -1)
        return EXIT_FAILURE;

    struct sockaddr_in sock_addr_server;
    memset(&sock_addr_server, 0, sizeof(sock_addr_server));
    sock_addr_server.sin_family = AF_INET;
    sock_addr_server.sin_port = htons(PORT);

    if (inet_aton(argv[1], &sock_addr_server.sin_addr) == 0){
        std::cerr << "Couldn't convert the server-address [xx.xx.xx.xx] from host to network(binary) form." << std::endl;
        return EXIT_FAILURE;
    }

    // request connection
    int res = connect(sd, reinterpret_cast<const sockaddr *>(&sock_addr_server), sizeof( sock_addr_server ));
    if (res == -1) {
        std::cerr << "Couldn't connect to server by " << inet_ntoa(sock_addr_server.sin_addr) << " address." << std::endl;
        return EXIT_FAILURE;
    }

    int num_sent = 0;
    if ( (num_sent = send(sd, argv[2], strlen(argv[2]), 0)) && num_sent > 0) {
        if (argv[2][0] == 'm'){
            std::string msg = argv[3];
            if (send_message(sd, msg) == EXIT_FAILURE) {
                std::cerr << "Couldn't send full message. | errno = " << errno << std::endl;
            }
        }

        // read response
        char* response_buff = new char[BUFFER_SIZE]();

        try_recv(sd, response_buff);
        close(sd);

    } else {
        std::cerr << "Couldn't send the option message. | errno = " << errno << std::endl;
        return EXIT_FAILURE;
    }
}