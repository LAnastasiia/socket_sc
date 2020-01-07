
#ifndef SOCKET_SC_SIMPLE_SOCKET_COMMUNICATION_H
#define SOCKET_SC_SIMPLE_SOCKET_COMMUNICATION_H

#define BUFFER_SIZE (1024)
#define MAX_CONNECTION_ATTEMPTS (15)
#define PORT (2233)


#include <iostream>
#include <stdint-gcc.h>
#include <sys/socket.h>


char* encode_num_to_bits(uint16_t num);

uint16_t decode_bits_into_num(char* len_buff);

int send_message(int socket_descriptor, const std::string& msg_str);

int try_recv(int socket_descriptor, char* buf, int num=0);


template <typename T, typename F>
int receive_and_process(int socket_descriptor, T& data_accumulator, F& process_function)
{
    char* len_buff = (char* )malloc(sizeof(char) * 2);
    if (try_recv(socket_descriptor, len_buff, 2) == -1) {
        std::cerr << "Error receiving length message." << std::endl;
        return EXIT_FAILURE;
    }
    uint16_t msg_len = decode_bits_into_num(len_buff);

    char* msg = new char[BUFFER_SIZE];
    if (try_recv(socket_descriptor, msg, msg_len) == -1) {
        std::cerr << "Error receiving content message." << std::endl;
        return EXIT_FAILURE;
    }
    process_function(msg, data_accumulator);

    free(msg);
    return EXIT_SUCCESS;
}

#endif //SOCKET_SC_SIMPLE_SOCKET_COMMUNICATION_H


// ToDo
// apply function in receive_and_process in chunks