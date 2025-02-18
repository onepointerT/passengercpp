cmake_minimum_required( VERSION 3.28 )


project( passengercpp CXX C )

set( PROJECT_NAME passengercpp )
set( PROJECT_BRIEF "A library that is havening a passenger arriving and departing node graph with and without actions on edges" )
set( PROJECT_VERSION "0.2.1" )

set( CMAKE_CXX_STANDARD 23 )
set( CMAKE_C_STANDARD 23 )

if( UNIX )
    add_definitions( -fpermissive )
endif()

#include_directories( lib/plutovg/include )
#include_directories( lib/lunasvg/include )

#add_subdirectory( lib/plutovg )
#add_subdirectory( lib/lunasvg )

include_directories( src )
file( GLOB SRCFILES src/*.cpp )
add_library( passengercpp STATIC ${SRCFILES} )

#include_directories( src/svg )
#file( GLOB SRCFILES_SVG src/svg/*.cpp )
#add_library( passengersvg STATIC ${SRCFILES_SVG} )
#target_link_libraries( passengersvg lunasvg passengercpp )

set( ENABLE_DOC_PASSENGERCPP TRUE )
if( ENABLE_DOC_PASSENGERCPP )
	find_package(Doxygen)
	if(DOXYGEN_FOUND)
		message("-- Enabled doc_passengercpp target for make")
        # CMAKE_CURRENT_SOURCE_DIR is where the last call to CMake's project was,
        # CMAKE_CURRENT_BINARY_DIR is where the build of the binaries is performed
		configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
		add_custom_target(doc_passengercpp ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMENT "Generating documentation with Doxygen")
	endif()
endif()