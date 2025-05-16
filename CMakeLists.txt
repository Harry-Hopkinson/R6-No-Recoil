cmake_minimum_required(VERSION 3.5)
project(r6-no-recoil)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1 -static -s -Wall -mwindows")

set(APPNAME r6-no-recoil)
set(OUT build/r6-no-recoil.exe)

file(GLOB_RECURSE files src/*.cpp src/core/*.cpp src/recoil/*.cpp)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(${APPNAME} ${files})
