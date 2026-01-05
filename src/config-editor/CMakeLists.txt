cmake_minimum_required(VERSION 3.20)
project(ConfigEditor LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(WEBVIEW_WEBKITGTK_API 6.0)

include(FetchContent)

FetchContent_Declare(
    webview
    GIT_REPOSITORY https://github.com/webview/webview
    GIT_TAG 0.12.0
)
FetchContent_MakeAvailable(webview)

FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json
  GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(json)

add_executable(ConfigEditor WIN32)

target_sources(ConfigEditor PRIVATE src/main.cpp ui/dist/index_html.h)
target_include_directories(ConfigEditor PRIVATE ui/dist)
target_link_libraries(ConfigEditor PRIVATE webview::core nlohmann_json::nlohmann_json)
