cmake_minimum_required(VERSION 3.6)
project (continuedfractionHE)

set (CMAKE_CXX_STANDARD 11)
set (SOURCE_FILES src/arithmetic.cpp src/comparison.cpp src/generalTools.cpp src/utilities.cpp src/sorting.cpp src/gosper.cpp)
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

set (HELIB_SOURCE_DIRECTORY "/Users/heewon/Documents/_SourceCode/Library/HElib-master/src/")
 
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

add_executable(test_Sorting test/sortingTest.cpp)
target_link_libraries(test_Sorting CFHE -pthread)

#add_executable(cmp_Dec_1_GT compareDecimal/decimal_1_GT.cpp)
#target_link_libraries(cmp_Dec_1_GT CFHE -pthread)
#add_executable(cmp_CF_1_GT compareDecimal/CF_1_GT.cpp)
#target_link_libraries(cmp_CF_1_GT CFHE -pthread)
#add_executable(cmp_CF_1_EQ compareDecimal/CF_1_EQ.cpp)
#target_link_libraries(cmp_CF_1_EQ CFHE -pthread)


#add_executable(cmp_Dec_2_GT compareDecimal/decimal_2_GT.cpp)
#target_link_libraries(cmp_Dec_2_GT CFHE -pthread)
#add_executable(cmp_CF_2_GT compareDecimal/CF_2_GT.cpp)
#target_link_libraries(cmp_CF_2_GT CFHE -pthread)
#add_executable(cmp_CF_2_EQ compareDecimal/CF_2_EQ.cpp)
#target_link_libraries(cmp_CF_2_EQ CFHE -pthread)

#add_executable(cmp_Dec_3_GT compareDecimal/decimal_3_GT.cpp)
#target_link_libraries(cmp_Dec_3_GT CFHE -pthread)
#add_executable(cmp_CF_3_GT compareDecimal/CF_3_GT.cpp)
#target_link_libraries(cmp_CF_3_GT CFHE -pthread)
#add_executable(cmp_CF_3_EQ compareDecimal/CF_3_EQ.cpp)
#target_link_libraries(cmp_CF_3_EQ CFHE -pthread)