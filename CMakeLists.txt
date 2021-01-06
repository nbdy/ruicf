cmake_minimum_required(VERSION 3.8)
project(ruicf C CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(tests main.cpp ruicf.h)
set_target_properties(tests PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(tests raylib dl pthread)

add_library(ruicf STATIC ruicf.h)
set_target_properties(ruicf PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(ruicf raylib dl pthread)