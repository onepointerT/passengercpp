cmake_minimum_required( VERSION 3.28 )
project( passengercpp )

set( CMAKE_CXX_STANDART 23 )

if( UNIX )
    add_definitions( -fpermissive )
endif()

file( GLOB SRCFILES src/*.cpp )
add_library( passengercpp STATIC ${SRCFILES} )
