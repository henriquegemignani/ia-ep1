CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(ia-ep1 CXX)

IF(MSVC AND (CMAKE_SIZEOF_VOID_P EQUAL 8))
    set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS ON)
ENDIF()
if(NOT ${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
    # Don't polute the git repository with the generated files for out-of-source builds
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/.gitignore "*")
endif()

if (UNIX OR MINGW)
    add_definitions(
        -std=c++11 # Introduced in gcc-4.7, this option was called -std=c++0x before.
        
        -U_FORTIFY_SOURCE # buffer overflow checks
    
        # More information for the flags, check http://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
        -Wall
        -Wextra
        
        -Winit-self
        -Wzero-as-null-pointer-constant
    )
endif()

add_executable(ia-ep1 
    src/agent.h src/agent.cc 
    src/environment.h src/environment.cc
    src/mapmatrix.h src/mapmatrix.cc
    src/errors.h src/forward_declarations.h src/actions.h
    src/main.cc)