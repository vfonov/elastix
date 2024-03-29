project( ANN )


IF( APPLE )
    SET(MACOSX_RPATH             ON)
    SET(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX})
    SET(CMAKE_SKIP_RPATH         OFF)
    SET(CMAKE_SKIP_INSTALL_RPATH OFF)
    SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)
ENDIF( APPLE )

# Compile time option to select wheter or not to accept self matches
# during nearest neighbour search. The default is false for backward
# compatibility.
mark_as_advanced( ELASTIX_KNN_ALLOW_SELF_MATCH )
set( ELASTIX_KNN_ALLOW_SELF_MATCH false
  CACHE BOOL "Allow self matches during kNN search" )
set( ANN_ALLOW_SELF_MATCH "ANNfalse" )
if( ${ELASTIX_KNN_ALLOW_SELF_MATCH} )
  set( ANN_ALLOW_SELF_MATCH "ANNtrue" )
endif()
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/include/ANN/ANN.h.in
  ${elastix_BINARY_DIR}/ANN/ANN.h
  @ONLY )

# Include these directories to look for header files.
include_directories(
  ${ANN_SOURCE_DIR}/include
  ${ANN_SOURCE_DIR}/src
)

# Sources of non-templated classes.
set( ANN_SRCS
  src/ANN.cpp
  src/bd_fix_rad_search.cpp
  src/bd_pr_search.cpp
  src/bd_search.cpp
  src/bd_tree.cpp
  src/brute.cpp
  src/kd_dump.cpp
  src/kd_fix_rad_search.cpp
  src/kd_pr_search.cpp
  src/kd_search.cpp
  src/kd_split.cpp
  src/kd_tree.cpp
  src/kd_util.cpp
  src/perf.cpp
)

# Create the ANNlib library
add_library( ANNlib  ${ANN_SRCS} )

# Necessary, because in windows dll_export is needed for creating the
# dll, but dll_import for using it in a program.

if( WIN32 )
    set_target_properties( ANNlib PROPERTIES DEFINE_SYMBOL DLL_EXPORTS )
endif( WIN32 )

# Group in IDE's like Visual Studio
set_property( TARGET ANNlib PROPERTY FOLDER "libraries" )


# Install
if( WIN32 )
  install( TARGETS ANNlib
    RUNTIME DESTINATION .
    LIBRARY DESTINATION . 
    ARCHIVE DESTINATION . 
  )
    #COMPONENT libraries )
else()
  install( TARGETS ANNlib
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib 
    ARCHIVE DESTINATION lib
   )
    #COMPONENT libraries )
endif()

