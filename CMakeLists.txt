cmake_minimum_required(VERSION 2.8)
project (Chip8)


include_directories(.)

# Get all the source files
file(GLOB SOURCES "*.cpp")

add_executable(chip8 ${SOURCES})


