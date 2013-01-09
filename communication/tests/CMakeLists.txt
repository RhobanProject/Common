cmake_minimum_required (VERSION 2.6)
project (RhobanCommunicationTest)

option (MMNET
    "Compile for the Mmnet" OFF)

if (MMNET)
    set (CMAKE_C_COMPILER "arm-linux-uclibc-gcc"
        CACHE STRING "")
    set (CMAKE_CXX_COMPILER "arm-linux-uclibc-g++"
        CACHE STRING "")
    set (CMAKE_C_FLAGS "-DMMNET"
        CACHE STRING "")
    set (CMAKE_CXX_FLAGS "-DMMNET"
        CACHE STRING "")
endif (MMNET)

set (RHOBAN_UTILS_DIRECTORY "../../../Utils")
set (RHOBAN_COMMON_DIRECTORY "../../")

add_subdirectory ("${RHOBAN_UTILS_DIRECTORY}" utils)
add_subdirectory ("${RHOBAN_COMMON_DIRECTORY}" common)

include_directories ("${RHOBAN_UTILS_DIRECTORY}")
include_directories ("${RHOBAN_COMMON_DIRECTORY}")

set (EXTRA_LIBS RhobanUtils RhobanCommon)

# Test logging
add_executable (Tests allTests.cpp)
target_link_libraries (Tests ${EXTRA_LIBS})
