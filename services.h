
#ifndef SOCKET_SC_SERVICES_H
#define SOCKET_SC_SERVICES_H

#include <chrono>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>

int _get_space_count(char* buf, int& curr_count);

const char* get_time_str();

const char* get_date_str();

const char* get_greeting();

#endif //SOCKET_SC_SERVICES_H
