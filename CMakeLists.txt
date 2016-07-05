cmake_minimum_required (VERSION 2.6)

project (GwaBall)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")
SET(CMAKE_CXX_FLAGS "-g -Wall -pedantic -std=c++0x")
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

# Configure CCache if available
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CCACHE_PROGRAM}")
endif()

add_custom_target(mrproper
   COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/CMakeModules/mrproper.cmake
)

find_package(Bullet REQUIRED)
include_directories(${BULLET_INCLUDE_DIRS})
list(APPEND LIBS ${BULLET_LIBRARIES})

find_package(AngelScript REQUIRED)
include_directories(${ANGELSCRIPT_INCLUDE_DIRS})
list(APPEND LIBS ${ANGELSCRIPT_LIBRARY})

find_package(Boost COMPONENTS system filesystem REQUIRED)
list(APPEND LIBS ${Boost_SYSTEM_LIBRARY})

foreach(lib OGRE OIS)
    find_package(${lib} REQUIRED)
    include_directories(${${lib}_INCLUDE_DIRS})
    list(APPEND LIBS ${${lib}_LIBRARIES})
endforeach()

file(GLOB_RECURSE sources_files src/* thirdparty/*)
include_directories(thirdparty)

add_executable(${PROJECT_NAME} ${sources_files})

target_link_libraries(${PROJECT_NAME} ${LIBS})

