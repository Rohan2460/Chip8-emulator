cmake_minimum_required(VERSION 3.16)
project(chip8)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

add_executable(chip8 src/main.cpp src/cpu.cpp src/gui.cpp)

target_link_libraries(chip8 PRIVATE SDL3::SDL3)