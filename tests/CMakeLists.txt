cmake_minimum_required(VERSION 2.8)

include_directories(../src)

add_executable(test test.c)
target_link_libraries(test
    filemon
)
