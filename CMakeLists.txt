cmake_minimum_required (VERSION 2.6)

project (GwaBall)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")
SET(CMAKE_CXX_FLAGS "-g -Wall -pedantic -std=c++0x")
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

add_custom_target(mrproper
   COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/CMakeModules/mrproper.cmake
)

find_package(Bullet REQUIRED)
include_directories(${BULLET_INCLUDE_DIRS})
list(APPEND LIBS ${BULLET_LIBRARIES})

foreach(lib OGRE OIS)
    find_package(${lib} REQUIRED)
    include_directories(${${lib}_INCLUDE_DIRS})
    list(APPEND LIBS ${${lib}_LIBRARIES})
endforeach()

file(GLOB_RECURSE sources_files src/*)

add_executable(${PROJECT_NAME} ${sources_files})

target_link_libraries(${PROJECT_NAME} ${LIBS})

