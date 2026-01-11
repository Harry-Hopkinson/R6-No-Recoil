cmake_minimum_required(VERSION 3.20)
project(ConfigEditor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(WEBVIEW_WEBKITGTK_API 6.0)

include(FetchContent)
FetchContent_Declare(
    webview
    GIT_REPOSITORY https://github.com/webview/webview
    GIT_TAG 0.12.0
)
FetchContent_MakeAvailable(webview)

add_executable(ConfigEditor WIN32)

target_sources(ConfigEditor PRIVATE
    src/main.cpp
    ui/dist/index_html.h
    resource/resource.rc
    resource/resource.h
)

target_include_directories(ConfigEditor PRIVATE
    ui/dist
    resource
)

target_link_libraries(ConfigEditor PRIVATE webview::core)

set_target_properties(ConfigEditor PROPERTIES WIN32_EXECUTABLE TRUE)

if(MSVC)
    target_compile_options(ConfigEditor PRIVATE
        /O2
        /Ob2
        /Oi
        /Ot
        /GL
        /GS-
        /Gy
        /arch:AVX2
        /fp:fast
        /W3
        /DNDEBUG
    )

    target_link_options(ConfigEditor PRIVATE
        /LTCG
        /OPT:REF
        /OPT:ICF
        /SUBSYSTEM:WINDOWS
    )

elseif(MINGW)
    target_compile_options(ConfigEditor PRIVATE
        -O3
        -march=x86-64-v3
        -mtune=generic
        -flto
        -ffunction-sections
        -fdata-sections
        -ffast-math
        -funroll-loops
        -fomit-frame-pointer
        -fno-exceptions
        -fno-rtti
        -DNDEBUG
    )

    target_link_options(ConfigEditor PRIVATE
        -flto
        -Wl,--gc-sections
        -Wl,--strip-all
        -mwindows
        -static
        -static-libgcc
        -static-libstdc++
    )
endif()

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT ipo_error)

if(ipo_supported)
    set_property(TARGET ConfigEditor PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

if(MINGW OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    find_program(STRIP_PROGRAM strip)
    if(STRIP_PROGRAM)
        add_custom_command(TARGET ConfigEditor POST_BUILD
            COMMAND ${STRIP_PROGRAM} --strip-all $<TARGET_FILE:ConfigEditor>
            COMMENT "Stripping symbols"
        )
    endif()
endif()

message(STATUS "========================================")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "========================================")
