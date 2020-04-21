cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

## Use -std=c++14 as default.
set(CMAKE_CXX_STANDARD 14)
## Disable C++ extensions
set(CMAKE_CXX_EXTENSIONS OFF)
## Require full C++ standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(CFHE
       	LANGUAGES CXX)

find_package(helib 1.0.0 EXACT REQUIRED)

set (SOURCE_FILES src/arithmetic.cpp src/comparison.cpp src/generalTools.cpp src/utilities.cpp src/generateMsg.cpp)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(include)
include_directories(lib)

add_library(CFHE ${SOURCE_FILES})
target_link_libraries(CFHE helib)