cmake_minimum_required(VERSION 3.20)
project(R6NoRecoil LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# --- Sources ---
file(GLOB_RECURSE SRC_FILES src/*.cpp)
set(RESOURCE_FILES src/resource/resource.rc)

# Include directories
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src/json)

# --- cJSON Static Library ---
add_library(cjson STATIC src/json/cJSON.c)
target_include_directories(cjson PUBLIC src/json)

# --- Main executable ---
add_executable(R6NoRecoil WIN32 ${SRC_FILES} ${RESOURCE_FILES})

# Link cJSON
target_link_libraries(R6NoRecoil PRIVATE xinput cjson)

# Enable Link-Time Optimization (LTO)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)

# --- Copy assets and data files ---
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

foreach(FILE Config.toml WeaponData.json)
    add_custom_command(TARGET R6NoRecoil POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/data/${FILE}
                ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${FILE}
    )
endforeach()

# --- Compiler and linker options ---
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
    target_compile_definitions(R6NoRecoil PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_options(R6NoRecoil PRIVATE
        $<$<CONFIG:Release>:/W4 /Os /GL /Gy /Zc:inline /Zc:preprocessor /utf-8 /GS /MD>
        $<$<CONFIG:Debug>:/W3 /Od /Zi /utf-8 /MDd>
    )
    target_link_options(R6NoRecoil PRIVATE
        $<$<CONFIG:Release>:/LTCG /INCREMENTAL:NO /OPT:REF /OPT:ICF /DYNAMICBASE /NXCOMPAT /SUBSYSTEM:WINDOWS>
        $<$<CONFIG:Debug>:/INCREMENTAL /SUBSYSTEM:WINDOWS>
    )
endif()
