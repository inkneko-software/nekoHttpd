cmake_minimum_required(VERSION 3.15)

project(nekoHttpd CXX)
if(NOT ${CMAKE_BUILD_TYPE})
  set(${CMAKE_BUILD_TYPE} "Debug")
endif()
MESSAGE("build type: " ${CMAKE_BUILD_TYPE})

if(NOT MUDUO_PATH)
  set(MUDUO_PATH "/home/$ENV{USER}/.muduo_install")
endif()

set(CXX_FLAGS
 #-g
 # -DVALGRIND
 # -DMUDUO_STD_STRING
 -Wall
 -Wextra
 # -m32
 -Werror 
 -Wconversion
 -Wno-unused-parameter
 -Wold-style-cast
 -Woverloaded-virtual
 -Wpointer-arith
 -Wshadow
 -Wwrite-strings
 -march=native
 # -MMD
 # -std=c++0x
 -rdynamic
 )
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -finline-limit=1000 -DNDEBUG")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

find_path(Muduo_INCLUDE_DIR muduo "${MUDUO_PATH}/include")
find_path(Muduo_LIBRARY_DIR libmuduo_net.a "${MUDUO_PATH}/lib")
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${Muduo_LIBRARY_DIR})
message(STATUS ${Muduo_INCLUDE_DIR})
message(STATUS ${Muduo_LIBRARY_DIR})

include_directories(${Muduo_INCLUDE_DIR})
find_library(muduo_base muduo_base)
find_library(muduo_net muduo_net)
message(STATUS ${muduo_base})
message(STATUS ${muduo_net})

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR})

#generate
add_library(
    nekohttpd STATIC 
    HttpServer.cpp
    ${CMAKE_SOURCE_DIR}/utils/HttpResponse.cpp
    ${CMAKE_SOURCE_DIR}/utils/HttpRequest.cpp
    ${CMAKE_SOURCE_DIR}/utils/webstring.cpp
)
target_link_libraries(nekohttpd ${muduo_base} ${muduo_net})
target_link_libraries(nekohttpd pthread rt ssl crypto boost_regex)


add_subdirectory(tests)
add_subdirectory(utils)


# installation
## header files
install(FILES HttpServer.h 
DESTINATION include/nekoHttpd/
)

install(FILES ${CMAKE_SOURCE_DIR}/utils/webstring.h ${CMAKE_SOURCE_DIR}/utils/HttpRequest.h ${CMAKE_SOURCE_DIR}/utils/HttpResponse.h
DESTINATION include/nekoHttpd/utils/
)

install(TARGETS nekohttpd
LIBRARY DESTINATION lib
)