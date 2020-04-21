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

# Configuration Profile
set (CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS} -pg")
set (CMAKE_EXE_LINKER_FLAGS_PROFILE "${CMAKE_EXE_LINKER_FLAGS} -pg")
set (OpenMP_CXX_FLAGS "-fopenmp")

# Enable OpenMP if available
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

include_directories(include)
include_directories(lib)

add_library(CFHE ${SOURCE_FILES})
target_link_libraries(CFHE HElib NTL -lgmp -lgf2x -pthread -lm -O2)

add_executable(test_MulTree test/mulTreeTest.cpp)
target_link_libraries(test_MulTree CFHE -pthread)
add_executable(test_IntegerEquality test/integerEqualTest.cpp)
target_link_libraries(test_IntegerEquality CFHE -pthread)
add_executable(test_RealEquality test/realEqualTest.cpp)
target_link_libraries(test_RealEquality CFHE -pthread)

add_executable(test_IntegerComparison test/integerCompTest.cpp)
target_link_libraries(test_IntegerComparison CFHE -pthread)
add_executable(test_RealComparison test/realCompTest.cpp)
target_link_libraries(test_RealComparison CFHE -pthread)

add_executable(ref_EqualTest test/ref_EqualTest.cpp)
target_link_libraries(ref_EqualTest CFHE -pthread)
add_executable(ref_CompTest test/ref_CompTest.cpp)
target_link_libraries(ref_CompTest CFHE -pthread)

target_link_libraries(BGV_general_example helib)
