cmake_minimum_required(VERSION 3.1.0)
project(seggz VERSION 0.1.0)

add_executable(seggz ./src/main.cpp
                     ./src/uci.cpp
                     ./src/uci.h
                     ./src/search.cpp
                     ./src/search.h)
add_library(thc 
            ./lib/thc/thc.cpp 
            ./lib/thc/thc.h)

target_link_libraries(seggz thc)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++2a")
