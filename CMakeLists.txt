cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)

project(socket_sc)

set(common_sources "simple_socket_communication/simple_socket_communication.cpp" "services.cpp")
add_executable(socket_s ${common_sources} sc_server.cpp)
add_executable(socket_c ${common_sources} sc_client.cpp)