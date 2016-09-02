cmake_minimum_required(VERSION 3.6)
project(QQCore)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp http_client.cc http_client.h)

link_directories(/usr/lib/x86_64-linux-gnu)

add_executable(QQCore ${SOURCE_FILES})
target_link_libraries(QQCore libcurl.so)