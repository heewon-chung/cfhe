set (NTL_DIRECTORY "/home/caesar/Desktop/development/cpp/libs/ntl-10.3.0/src/")
add_library(NTL STATIC IMPORTED GLOBAL)
set_target_properties(NTL PROPERTIES IMPORTED_LOCATION ${NTL_DIRECTORY}ntl.a)

set (HELIB_SOURCE_DIRECTORY "/home/caesar/Desktop/development/cpp/libs/HElib-master/src/")
add_library(HElib STATIC IMPORTED GLOBAL)
set_target_properties(HElib PROPERTIES IMPORTED_LOCATION ${HELIB_SOURCE_DIRECTORY}fhe.a)