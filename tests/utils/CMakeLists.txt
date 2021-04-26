cmake_minimum_required(VERSION 3.15)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/tests)
set(CMAKE_BUILD_TYPE Debug)
add_executable(httpresponse_test HttpResponse_test.cpp ${PROJECT_SOURCE_DIR}/utils/HttpResponse.cpp)
add_executable(httprequest_test HttpRequest_test.cpp ${PROJECT_SOURCE_DIR}/utils/HttpRequest.cpp ${PROJECT_SOURCE_DIR}/utils/webstring.cpp)

target_link_libraries(httprequest_test ssl crypto)


