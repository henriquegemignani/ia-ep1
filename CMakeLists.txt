CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(ia-ep1 CXX)

IF(MSVC AND (CMAKE_SIZEOF_VOID_P EQUAL 8))
    set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS ON)
ENDIF()

add_executable(ia-ep1 
    src/agent.h src/agent.cc 
    src/environment.h src/environment.cc
    src/main.cc)