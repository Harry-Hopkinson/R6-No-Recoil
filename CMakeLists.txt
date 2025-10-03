cmake_minimum_required(VERSION 3.20)
project(R6NoRecoil)

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
add_executable(R6NoRecoil WIN32 ${files} ${RESOURCE_FILES})

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

target_link_libraries(R6NoRecoil PRIVATE xinput)

# Platform-specific compile and link options
if(MINGW)
    target_compile_options(R6NoRecoil PRIVATE
        -O2
        -Wall
        -Wextra
        -Wno-missing-field-initializers
        -fno-exceptions
        -fno-rtti
        -ffunction-sections
        -fdata-sections
        -static
        -fstack-protector-strong
    )

    target_link_options(R6NoRecoil PRIVATE
        -static
        -static-libgcc
        -static-libstdc++
        -Wl,--gc-sections
        -s
        -mwindows
        -Wl,--nxcompat
        -Wl,--dynamicbase
    )
elseif(MSVC)
    target_compile_definitions(R6NoRecoil PRIVATE _CRT_SECURE_NO_WARNINGS _AMD64_=1 _M_X64=100)
    target_compile_options(R6NoRecoil PRIVATE
        /W4
        /WX-
        /O2
        /GL
        /Gy
        /Zc:inline
        /Zc:preprocessor
        /EHsc
        /GS
        /utf-8
    )
    target_link_options(R6NoRecoil PRIVATE
        /LTCG
        /INCREMENTAL:NO
        /OPT:REF
        /OPT:ICF
        /DYNAMICBASE
        /NXCOMPAT
        /SUBSYSTEM:WINDOWS
    )
endif()

# Keep the post-build copy as well for runtime
add_custom_command(TARGET R6NoRecoil POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
)

add_custom_command(TARGET R6NoRecoil POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/data/WeaponData.json
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/WeaponData.json
)
