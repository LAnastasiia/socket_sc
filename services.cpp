
#include "services.h"


int _get_space_count(char* buf, int& curr_count)
{
    int count = 0; bool next_space = false;
    while(*buf != '\0') {
        if(*buf == ' ' && ! next_space){
            count++;
            next_space = true;
        } else next_space = false;
        buf++;
    }
    curr_count += count;
    return EXIT_SUCCESS;
}


const char* get_time_str() {
    time_t now = time(nullptr);
    tm *ltm = localtime(&now);
    std::stringstream time_str;
    time_str << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec;
    return time_str.str().c_str();
}


const char* get_date_str(){
    time_t now = time(nullptr);
    tm *ltm = localtime(&now);
    std::stringstream time_str;
    time_str << ltm->tm_mday << "." << ltm->tm_mon << "." << ltm->tm_year;
    return time_str.str().c_str();
}

const char* get_greeting(){
    std::string greeting {"Grüezi"};
    return greeting.c_str();
}