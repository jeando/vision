cmake_minimum_required (VERSION 2.6)

SET(PROJECT_NAME vision)
PROJECT(${PROJECT_NAME})


SET(CMAKE_INCLUDE_CURRENT_DIR TRUE)


# Path to the cmake module path
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)

set(SRC src)



set(OPENCV_NEW_LIBRARY_NAMES yes)
find_package(OpenCV COMPONENTS core)
find_package(OpenGL)
find_package(GLEW)
#find_package(SFML 2 COMPONENTS system window graphics network audio main)
find_package(SFML 2 COMPONENTS system window graphics)

set(SYSTEM_LIBS
	${OPENCV_LIBRARIES}
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARIES}
	${SFML_LIBRARIES}
)

set(EXTRA_LIBS
)
#message(SEND_ERROR ${SYSTEM_LIBS})
set(SYSTEM_INCLUDES
	${OPENCV_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIRS}
	${GLEW_INCLUDE_DIRS}
	${SFML_INCLUDE_DIRS}
)

set( EXTRA_INCLUDES
	include
)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

set (CMAKE_CXX_FLAGS "-std=c++11 -D_LINUX -D_REENTRANT -Wall  -O3 -march=nocona -msse3 -D__builtin_isnan=::isnan")

#set(CMAKE_CXX_FLAGS "-Wall -W -Werror -ansi -pedantic -g")

### DEFINE NEEDED SOURCES
SET(SOURCE_VISION
	${SRC}/main_2.cpp
        ${SRC}/shader.cpp
        ${SRC}/texture.cpp
		${SRC}/RessourcesManager.cpp
)
SET(SOURCES_GUI
	${SRC}/main.cpp
        ${SRC}/shader.cpp
        ${SRC}/texture.cpp
		${SRC}/RessourcesManager.cpp
)
SET(SOURCES_TEST ${SRC}/test.cpp)

#### BUILD INSTRUCTIONS
include_directories (${SRC} ${SYSTEM_INCLUDES}  ${EXTRA_INCLUDES})

### CREATE EXECUTABLE
add_executable("recollement" ${SOURCE_VISION})
add_executable("vision_gui" ${SOURCES_GUI})
add_executable("test" ${SOURCES_TEST})

target_link_libraries("recollement" ${SYSTEM_LIBS} ${EXTRA_LIBS} )
target_link_libraries("vision_gui" ${SYSTEM_LIBS} ${EXTRA_LIBS} )
target_link_libraries("test" ${SYSTEM_LIBS} ${EXTRA_LIBS} )

