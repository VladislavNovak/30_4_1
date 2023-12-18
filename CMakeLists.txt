cmake_minimum_required(VERSION 3.24)
project(30_4_1 VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(BUILD_CPR_TESTS OFF)
set(CMAKE_USE_OPENSSL OFF)
# Синхронизирует библиотеки
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git GIT_TAG 0817715923c9705e68994eb52ef9df3f6845beba)
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)


add_executable(30_4_1 main.cpp
        cpr_requests.cpp
        cpr_requests.h
        cpr_requests.h
        constants.h
        utilities.cpp
        utilities.h
        utilities.h)

#target_include_directories(30_4_1 PUBLIC ${CMAKE_SOURCE_DIR}/headers)

target_link_libraries(30_4_1 PRIVATE cpr::cpr)
target_link_libraries(30_4_1 PRIVATE nlohmann_json::nlohmann_json)