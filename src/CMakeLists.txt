cmake_minimum_required(VERSION 3.24)

add_executable(30_4_1
        ../main.cpp
        ../headers/cpr_requests.h
        ../headers/cpr_requests.h
        ../headers/constants.h
        ../headers/utilities.h
        ../headers/utilities.h
        cpr_requests.cpp
        utilities.cpp
)

target_include_directories(30_4_1 PUBLIC ${CMAKE_SOURCE_DIR}/headers)