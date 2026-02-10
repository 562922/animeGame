cmake_minimum_required(VERSION 3.10)
project(AnimeGameBeta)

set(CMAKE_CXX_STANDARD 17)

# Build C++ logic library
add_subdirectory(game/src/logic/cpp GameLogic)

# Build environment runner (requires SDL2)
find_package(SDL2)
if(SDL2_FOUND)
  add_subdirectory(game/src/environment EnvRunner)
  message(STATUS "SDL2 found: Building environment runner")
else()
  message(WARNING "SDL2 not found: Skipping environment runner. Install SDL2 to build the visualizer.")
endif()
