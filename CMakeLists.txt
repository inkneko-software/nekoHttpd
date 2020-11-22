cmake_minimum_required (VERSION 3.5)

set (CMAKE_CXX_STANDARD 11)
aux_source_directory (. SOURCE_TINYHTTPD)
add_library (tinyHttpd SHARED ${SOURCE_TINYHTTPD})

# for openssl dependences
set(OPENSSL_LIB_DIR /usr/local/lib/)
link_directories(${OPENSSL_LIB_DIR})
target_link_libraries (tinyHttpd crypto)

# installation
## header files
install(FILES HTTPPacket.h webstring.h tinyHttpd.h
DESTINATION include/tinyHttpd/
)
install(TARGETS tinyHttpd
LIBRARY DESTINATION lib
)

#light weight webstring library output
add_library(webstring SHARED webstring.cpp)
install(TARGETS webstring
LIBRARY DESTINATION lib
)
