# cmake -S . -B build && cmake --build build
# ./build/lirs
# cmake --build build --target clean

cmake_minimum_required(VERSION 3.10)
project(LIRS_Cache CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/inc)

set(SOURCE_FILES
    src/main.cpp
    src/cacheRendering.cpp
    src/tests.cpp
)

add_executable(lirs ${SOURCE_FILES})
