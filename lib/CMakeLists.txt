add_library(NTL STATIC IMPORTED GLOBAL)
set_target_properties(NTL PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../../../Library/ntl-10.3.0/src/ntl.a)

add_library(HElib STATIC IMPORTED GLOBAL)
set_target_properties(HElib PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../../../Library/HElib-master/src/fhe.a)