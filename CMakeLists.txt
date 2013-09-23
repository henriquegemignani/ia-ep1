CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(ia-ep1 CXX)

IF(MSVC AND (CMAKE_SIZEOF_VOID_P EQUAL 8))
    set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS ON)
ENDIF()
if(NOT ${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
    # Don't polute the git repository with the generated files for out-of-source builds
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/.gitignore "*")
endif()

add_executable(ia-ep1 
    src/agent.h src/agent.cc 
    src/environment.h src/environment.cc
    src/main.cc)