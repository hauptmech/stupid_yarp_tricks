# Authors: Traveler Hauptman, 23 August 2013

    SET(EXECUTABLE_NAME "yarp_write")  #<-------Change!


    PROJECT(${EXECUTABLE_NAME})
    cmake_minimum_required(VERSION 2.8)

    set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake "/usr/local/share/yarp/cmake")
	find_package(ACE)
    find_package(YARP)

    include_directories(${YARP_INCLUDE_DIRS} ) 
   #Add source files here
    set( LIBRARY1_SOURCES
        main.cpp      #<---- Change me!
    )

    add_executable(${EXECUTABLE_NAME} ${LIBRARY1_SOURCES} )

    set( LIBRARIES
		${ACE_LIBRARIES}
    )

    target_link_libraries(${EXECUTABLE_NAME} ${LIBRARIES} ${YARP_LIBRARIES} readline) 
# Install 

    install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)


