cmake_minimum_required(VERSION 3.15)
add_subdirectory(utils)
set(CMAKE_BUILD_TYPE Debug)

add_executable(httpserver_test HttpServer_test.cpp )
target_link_libraries(httpserver_test nekohttpd)
