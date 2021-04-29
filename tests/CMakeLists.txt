cmake_minimum_required(VERSION 3.15)
add_subdirectory(utils)

add_executable(httpserver_test HttpServer_test.cpp )
target_link_libraries(httpserver_test nekohttpd boost_regex)
