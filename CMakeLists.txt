cmake_minimum_required(VERSION 3.20)
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
    target_compile_options(r6-no-recoil PRIVATE
        -O2 -Wall
        -fno-exceptions -fno-rtti
        -ffunction-sections -fdata-sections
    )
    target_link_options(r6-no-recoil PRIVATE
        -static -static-libgcc -static-libstdc++
        -Wl,--gc-sections
        -s
        -mwindows
    )
elseif(MSVC)
    target_compile_definitions(r6-no-recoil PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_options(r6-no-recoil PRIVATE
        /W4                     # High warning level
        /WX-                    # Do not treat warnings as errors (optional)
        /O2                     # Optimize for speed
        /GL                     # Whole Program Optimization
        /Gy                     # Enable function-level linking
        /Zc:inline              # Remove unreferenced COMDAT functions/data
        /Zc:preprocessor        # Use conforming preprocessor (C++20)
        /EHsc                   # Enable standard C++ exception handling
        /GS                     # Enable stack security checks
        /utf-8                  # Source files are in UTF-8
    )
    target_link_options(r6-no-recoil PRIVATE
        /LTCG                   # Link-time code generation (with /GL)
        /INCREMENTAL:NO         # Required for /LTCG
        /OPT:REF                # Eliminate unreferenced code/data
        /OPT:ICF                # Fold identical COMDATs
        /DYNAMICBASE            # ASLR
        /NXCOMPAT               # DEP compatibility
        /SUBSYSTEM:WINDOWS      # Windows GUI app
    )
endif()
