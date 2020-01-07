#include <cstring>
#include "simple_socket_communication.h"


char* encode_num_to_bits(uint16_t len_num)
{
    char* len_buff = new char[2];
    len_buff[1] = len_num & 0xFF;
    len_buff[0] = len_num >> 8;
    return len_buff;
}


uint16_t decode_bits_into_num(char* len_buff)
{
    uint16_t msg_len = len_buff[1] | uint16_t(len_buff[0]) << 8;
    return msg_len;
}


int send_message(int socket_descriptor, const std::string& msg_str)
{
    if (! msg_str.empty()) {
        char* len_bits = encode_num_to_bits(msg_str.length());
        send(socket_descriptor, len_bits, sizeof(char) * 2, 0);

        char* msg = new char[BUFFER_SIZE];
        strcpy(msg, msg_str.c_str());

        ssize_t bytes_sent = send(socket_descriptor, msg, sizeof(char) * msg_str.length(), 0);
        std::cout << "bytes_sent = " << bytes_sent << std::endl;
        return (bytes_sent == sizeof(char) * strlen(msg)) ? EXIT_SUCCESS : EXIT_FAILURE;

    } else {
        std::cout << "Warning. Attempt to send an empty message." << std::endl;
        return EXIT_SUCCESS;
    }
}


int try_recv(int socket_descriptor, char* buf, int num){
    int num_tries = 0, num_received = 0, total_num_received = 0;
    while ( num_tries < MAX_CONNECTION_ATTEMPTS ) {
        if ((num_received = recv(socket_descriptor, buf, sizeof(char) * (num ? num : BUFFER_SIZE), 0))
        && num_received > 0 && (num ? (total_num_received <= num) : true)){
            return num_received;
        }
        num_tries++;
        total_num_received += num_received;
    }
    return -1;
}