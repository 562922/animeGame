cmake_minimum_required(VERSION 3.10)
project(AnimeGameEnv)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(EnvRunner main.cpp)
target_link_libraries(EnvRunner PRIVATE GameLogic ${SDL2_LIBRARIES})
