# Authors: Traveler Hauptman, 23 August 2013

    SET(EXECUTABLE_NAME "yarp_write")  #<-------Change!


    PROJECT(${EXECUTABLE_NAME})
    cmake_minimum_required(VERSION 2.8)

    set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
	#include(teh-build)
	#teh_find_local_libraries()
    find_package(YARP)

    include_directories(${YARP_INCLUDE_DIRS})

   #Add source files here
    set( LIBRARY1_SOURCES
		RunReadWrite.cpp
#        main.cpp      #<---- Change me!
    )

    add_executable(${EXECUTABLE_NAME} ${LIBRARY1_SOURCES} )

    set( LIBRARIES
		${comanMath_LIBRARIES} #<---- this is our demo library found by find_package()
    )

    target_link_libraries(${EXECUTABLE_NAME} ${LIBRARIES} ${YARP_LIBRARIES}) 
# Install 

    install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)

# Packaging

	set(CPACK_PACKAGE_VERSION_MAJOR "0")
	set(CPACK_PACKAGE_VERSION_MINOR "0")
	set(CPACK_PACKAGE_VERSION_PATCH "1")

	set(CPACK_DEBIAN_PACKAGE_DEPENDS "comanMath")

	set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "DRC demo application")
	set(CPACK_PACKAGE_DESCRIPTION "
	The drc demo program is a good place to start if you are developing your own."
	)
	set(CPACK_PACKAGE_CONTACT "traveler.hauptman@iit.it")


	########### Leave the variables below as-is #############
	set(CPACK_GENERATOR "DEB")
	set(CPACK_PACKAGE_NAME ${EXECUTABLE_NAME})

	include(CPack)


