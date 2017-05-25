set (NTL_DIRECTORY "/Users/heewon/Documents/_SourceCode/Library/ntl-10.3.0/src/")
add_library(NTL STATIC IMPORTED GLOBAL)		  

set_target_properties(NTL PROPERTIES IMPORTED_LOCATION ${NTL_DIRECTORY}ntl.a)
  		  
set (HELIB_SOURCE_DIRECTORY "/Users/heewon/Documents/_SourceCode/Library/HElib-master/src/")
add_library(HElib STATIC IMPORTED GLOBAL)		  

set_target_properties(HElib PROPERTIES IMPORTED_LOCATION ${HELIB_SOURCE_DIRECTORY}fhe.a)