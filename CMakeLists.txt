cmake_minimum_required(VERSION 3.6)
project (continuedfractionHE)

set (CMAKE_CXX_STANDARD 11)
set (SOURCE_FILES src/arithmetic.cpp src/comparison.cpp src/generalTools.cpp src/utilities.cpp src/sorting.cpp)
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

set (HELIB_SOURCE_DIRECTORY "/home/caesar/Desktop/development/cpp/libs/HElib-master/src/")

include_directories(${HELIB_SOURCE_DIRECTORY})
include_directories(include)
include_directories(lib)
include_directories(lib/ntl-10.3.0/include)

add_subdirectory(lib)

add_library(CFHE ${SOURCE_FILES})
target_link_libraries(CFHE HElib NTL -lgmp -pthread -lgf2x -lm -O2)

add_executable(test_Addition test/addTest.cpp)
target_link_libraries(test_Addition CFHE -pthread)
add_executable(test_Subtraction test/subTest.cpp)
target_link_libraries(test_Subtraction CFHE -pthread)
add_executable(test_Division test/divTest.cpp)
target_link_libraries(test_Division CFHE -pthread)

add_executable(test_IntegerEquality test/integerEqualTest.cpp)
target_link_libraries(test_IntegerEquality CFHE -pthread)
add_executable(test_RealEquality test/realEqualTest.cpp)
target_link_libraries(test_RealEquality CFHE -pthread)

add_executable(test_IntegerComparison test/integerCompTest.cpp)
target_link_libraries(test_IntegerComparison CFHE -pthread)
add_executable(test_RealComparison test/realCompTest.cpp)
target_link_libraries(test_RealComparison CFHE -pthread)

add_executable(test_PDQ test/pdqTest.cpp)
target_link_libraries(test_PDQ CFHE -pthread)