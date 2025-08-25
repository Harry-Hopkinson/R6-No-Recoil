cmake_minimum_required(VERSION 3.20)
project(r6-no-recoil)

set(CMAKE_CXX_STANDARD 20)

# Set output path for the final binary
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Gather all source files
file(GLOB_RECURSE files src/*.cpp src/core/*.cpp src/recoil/*.cpp)

# Add the resource file
set(RESOURCE_FILES src/resource/resource.rc)

# Include headers from the root directory
include_directories(${CMAKE_SOURCE_DIR})

# Define the executable target for Windows GUI app
add_executable(r6-no-recoil WIN32 ${files} ${RESOURCE_FILES})

# Copy assets folder to build dir
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

# MSVC-specific compile and link options
target_compile_definitions(r6-no-recoil PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(r6-no-recoil PRIVATE
    /W4
    /WX-
    /O3
    /GL
    /Gy
    /Zc:inline
    /Zc:preprocessor
    /EHsc
    /GS
    /utf-8
)
target_link_options(r6-no-recoil PRIVATE
    /LTCG
    /INCREMENTAL:NO
    /OPT:REF
    /OPT:ICF
    /DYNAMICBASE
    /NXCOMPAT
    /SUBSYSTEM:WINDOWS
)

# Keep the post-build copy as well for runtime
add_custom_command(TARGET r6-no-recoil POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
)
