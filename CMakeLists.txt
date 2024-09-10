set(CMAKE_C_COMPILER "C:/msys64/ucrt64/bin/gcc.exe" CACHE STRING "" FORCE)
set(CMAKE_CXX_COMPILER "C:/msys64/ucrt64/bin/g++.exe" CACHE STRING "" FORCE)

cmake_minimum_required(VERSION 3.27.1)
project(bendra_atmintis)
set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(single_include)

set(SOURCES
    src/main.cpp
    src/DataMonitor.cpp
    src/SortedResultMonitor.cpp
    src/Logger.cpp
)

add_executable(bendra_atmintis ${SOURCES})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)