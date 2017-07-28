set (LIB_DIRECTORY "/usr/local/lib/")

add_library(NTL STATIC IMPORTED GLOBAL)		  
add_library(GMP STATIC IMPORTED GLOBAL)
add_library(GF2X STATIC IMPORTED GLOBAL)		  

set_target_properties(NTL PROPERTIES IMPORTED_LOCATION ${LIB_DIRECTORY}libntl.a)
set_target_properties(GMP PROPERTIES IMPORTED_LOCATION ${LIB_DIRECTORY}libgmp.a)
set_target_properties(GF2X PROPERTIES IMPORTED_LOCATION ${LIB_DIRECTORY}libgf2x.a)

set (HELIB_SOURCE_DIRECTORY "/Users/heewon/Documents/_SourceCode/Library/HElib-master/src/")
add_library(HElib STATIC IMPORTED GLOBAL)		  

set_target_properties(HElib PROPERTIES IMPORTED_LOCATION ${HELIB_SOURCE_DIRECTORY}fhe.a)