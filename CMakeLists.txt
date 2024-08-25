cmake_minimum_required( VERSION 3.28 )


project( passengercpp )

set( PROJECT_NAME passengercpp )
set( PROJECT_BRIEF "A library that is havening a passenger arriving and departing node graph with and without actions on edges" )
set( PROJECT_VERSION "0.1.0" )

set( CMAKE_CXX_STANDARD 23 )

if( UNIX )
    add_definitions( -fpermissive )
endif()

file( GLOB SRCFILES src/*.cpp )
add_library( passengercpp STATIC ${SRCFILES} )

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