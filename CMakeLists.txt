cmake_minimum_required(VERSION 3.6)
project(continuedfractionHE)

set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES src/arithmetic.cpp src/comparison.cpp src/generalTools.cpp src/utilities.cpp)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

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
include_directories(lib/ntl-10.3.0/include)

add_subdirectory(lib)

add_library(CFHE ${SOURCE_FILES})
target_link_libraries(CFHE HElib NTL -lgmp -pthread -lgf2x -lm -O2)

add_executable(test_Add test/addTest.cpp)
target_link_libraries(test_Add CFHE -pthread)
add_executable(test_Subtraction test/subTest.cpp)
target_link_libraries(test_Subtraction CFHE -pthread)
add_executable(test_Equality test/equalTest.cpp)
target_link_libraries(test_Equality CFHE -pthread)
add_executable(test_Comparison test/compTest.cpp)
target_link_libraries(test_Comparison CFHE -pthread)
add_executable(test_Order test/orderTest.cpp)
target_link_libraries(test_Order CFHE -pthread)