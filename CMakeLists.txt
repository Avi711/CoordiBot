cmake_minimum_required(VERSION 3.25)
project(CoordiBot)

set(CMAKE_CXX_STANDARD 14)


# Set the CXXFLAGS and LDLIBS variables
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} `pkg-config --cflags playerc++` -g")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} `pkg-config --cflags playerc` -g")

add_executable(CoordiBot
        src/main.cpp src/graph.cpp)

target_link_libraries(CoordiBot PUBLIC `pkg-config --libs playerc++`)


include_directories( /usr/local/include/player-3.0 )
