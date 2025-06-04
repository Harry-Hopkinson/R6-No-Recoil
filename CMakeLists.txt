cmake_minimum_required(VERSION 3.20)
project(r6-no-recoil)

set(CMAKE_CXX_STANDARD 20)

# Set output path for the final binary
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Gather all source files
file(GLOB_RECURSE files
    src/*.cpp
    src/core/*.cpp
    src/recoil/*.cpp
)

# Include headers from the root directory
include_directories(${CMAKE_SOURCE_DIR})

# Add ImGui
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)

file(GLOB IMGUI_SOURCES
    ${IMGUI_DIR}/*.cpp
)

set(IMGUI_BACKENDS
    ${IMGUI_DIR}/backends/imgui_impl_win32.cpp
    ${IMGUI_DIR}/backends/imgui_impl_dx11.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES} ${IMGUI_BACKENDS})
target_include_directories(imgui PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

# Define the executable target for Windows GUI app
add_executable(r6-no-recoil WIN32 ${files})

# Link ImGui to your executable
target_link_libraries(r6-no-recoil PRIVATE imgui)

# Platform-specific compile and link options
if(MINGW)
    target_compile_options(r6-no-recoil PRIVATE
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

    target_link_options(r6-no-recoil PRIVATE
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
    target_compile_definitions(r6-no-recoil PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_options(r6-no-recoil PRIVATE
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
    target_link_options(r6-no-recoil PRIVATE
        /LTCG
        /INCREMENTAL:NO
        /OPT:REF
        /OPT:ICF
        /DYNAMICBASE
        /NXCOMPAT
        /SUBSYSTEM:WINDOWS
    )
endif()
