cmake_minimum_required(VERSION 3.1.0)
project(seggz VERSION 0.1.0)

add_executable(seggz ./src/main.cpp
                     ./src/uci.cpp
                     ./src/uci.h
                     ./src/search.cpp
                     ./src/search.h
                     ./src/evaluate.cpp 
                     ./src/evaluate.h
                     ./src/timeman.cpp
                     ./src/timeman.h)
add_library(thc 
            ./lib/thc/thc.cpp 
            ./lib/thc/thc.h)

target_link_libraries(seggz thc)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
