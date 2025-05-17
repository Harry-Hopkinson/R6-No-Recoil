cmake_minimum_required(VERSION 3.5)
project(r6-no-recoil)

set(CMAKE_CXX_STANDARD 20)

# Set output path for the final binary
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Gather all source files
file(GLOB_RECURSE files src/*.cpp src/core/*.cpp src/recoil/*.cpp)

# Include headers from the root directory
include_directories(${CMAKE_SOURCE_DIR})

# Define the executable target for Windows GUI app
add_executable(r6-no-recoil WIN32 ${files})

# Platform-specific compile and link options
if(MINGW)
    target_compile_options(r6-no-recoil PRIVATE -O2 -Wall)
    target_link_options(r6-no-recoil PRIVATE -static -s -mwindows)
elseif(MSVC)
    target_compile_definitions(r6-no-recoil PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_options(r6-no-recoil PRIVATE /W4)
endif()
