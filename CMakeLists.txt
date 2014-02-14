cmake_minimum_required(VERSION 2.6)
project(myproject_name)
 
### MSCDS_LIBRARY path, change according to your location
set(MSCDS_PATH "$ENV{HOME}/local/mscds")
set(MSCDS_LIB ${MSCDS_PATH}/build/lib/libmscdsa.a)
include_directories(${MSCDS_PATH}/)
include_directories(${MSCDS_PATH}/framework)
 
###compiler flag for your program
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g -std=c++0x")
 
###
set(SRCS
bwt.cpp
main.cpp
)
 
set(HEADERS
bwt.h
)
 
add_executable(bwt ${SRCS} ${HEADERS})
target_link_libraries(bwt ${MSCDS_LIB})
