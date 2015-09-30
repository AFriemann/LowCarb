# <Copyright Holder>. Copyright (C) <Copyright Year(s)>. <License>.

#
# Project Properties
#
CMAKE_MINIMUM_REQUIRED (VERSION 2.6.2)
PROJECT (low-carb)
SET (APPLICATION_NAME "LowCarb")
SET (APPLICATION_CODENAME "${PROJECT_NAME}")
SET (APPLICATION_COPYRIGHT_YEARS "2015")
SET (APPLICATION_VERSION_MAJOR 1)
SET (APPLICATION_VERSION_MINOR 0)
SET (APPLICATION_VERSION_PATCH 0)
SET (APPLICATION_VERSION_TYPE SNAPSHOT)
SET (APPLICATION_VERSION_STRING "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_PATCH}-${APPLICATION_VERSION_TYPE}")

#
# Debugging Options
#
SET (CMAKE_VERBOSE_MAKEFILE 0) # Use 1 for debugging, 0 for release

#
# Project Output Paths
#
SET (MAINFOLDER ${PROJECT_SOURCE_DIR})
SET (EXECUTABLE_OUTPUT_PATH "${MAINFOLDER}/bin")
SET (LIBRARY_OUTPUT_PATH "${MAINFOLDER}/lib")

option(OPTIMIZE "build with optimization flags" ON)
option(OPENMP "build with coverage flags" ON)
option(SSE2 "build with sse2 support" ON)
option(DEBUG "build with debug stuff" OFF)
option(PROFILE "build with profiling flags" OFF)
option(COVERAGE "build with coverage flags" OFF)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if (SSE2)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse2")
endif()

if (DEBUG)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -pedantic")
endif()

if (OPENMP)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
endif()

if (COVERAGE)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

if (PROFILE)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-pie -pg")
endif()

if (OPTIMIZE)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fmerge-all-constants")
endif()

################################################################################
# Project Search Paths
################################################################################

LIST (APPEND CMAKE_PREFIX_PATH "${MAINFOLDER}")
LIST (APPEND CMAKE_PREFIX_PATH "${MAINFOLDER}/tools")
LIST (APPEND CMAKE_PREFIX_PATH "${MAINFOLDER}/thirdparty")
SET (CMAKE_MODULE_PATH "${MAINFOLDER}/tools/share/cmake")

INCLUDE_DIRECTORIES(SYSTEM "${MAINFOLDER}/include")

################################################################################
# Thirdparty libraries
################################################################################

# xdrfile
SET(xdrfile_LIBRARY_DIR "${MAINFOLDER}/thirdparty/xdrfile-1.1.4/src")
SET(xdrfile_SRCS "${xdrfile_LIBRARY_DIR}/xdrfile.c" "${xdrfile_LIBRARY_DIR}/xdrfile_xtc.c" "${xdrfile_LIBRARY_DIR}/xdrfile_trr.c")

ADD_LIBRARY(xdrfile ${xdrfile_SRCS})

# Boost
SET(Boost_USE_MULTITHREADED ON)
SET(Boost_LIBRARY_DIR "${MAINFOLDER}/thirdparty/boost_1_59_0/libs")
FILE(GLOB_RECURSE Boost_SYSTEM_SRCS "${Boost_LIBRARY_DIR}/system/src/*.cpp")
FILE(GLOB_RECURSE Boost_FILESYSTEM_SRCS "${Boost_LIBRARY_DIR}/filesystem/src/*.cpp")
FILE(GLOB_RECURSE Boost_PROGRAM_OPTIONS_SRCS "${Boost_LIBRARY_DIR}/program_options/src/*.cpp")
FILE(GLOB_RECURSE Boost_TEST_SRCS "${Boost_LIBRARY_DIR}/test/src/*.cpp")

ADD_LIBRARY(boost_system ${Boost_SYSTEM_SRCS})

ADD_LIBRARY(boost_filesystem ${Boost_FILESYSTEM_SRCS})
TARGET_LINK_LIBRARIES(boost_filesystem boost_system)

ADD_LIBRARY(boost_program_options ${Boost_PROGRAM_OPTIONS_SRCS})

ADD_LIBRARY(boost_test ${Boost_TEST_SRCS})

SET(Boost_LIBRARIES boost_filesystem boost_program_options boost_system)

# collect libraries

SET(thirdparty_LIBRARIES ${Boost_LIBRARIES} xdrfile)
SET(thirdparty_TEST_LIBRARIES ${thirdparty_LIBRARIES} boost_test)

################################################################################
# Configure Files
################################################################################

FILE (GLOB_RECURSE CONFIGINPUTS1 include/*.in.h.cmake)
FILE (GLOB_RECURSE CONFIGINPUTS2 include/*.h.in.cmake)
FILE (GLOB_RECURSE CONFIGINPUTS3 src/*.in.h.cmake)
FILE (GLOB_RECURSE CONFIGINPUTS4 src/*.h.in.cmake)
FILE (GLOB_RECURSE CONFIGINPUTS5 test/*.in.h.cmake)
FILE (GLOB_RECURSE CONFIGINPUTS6 test/*.h.in.cmake)
LIST (APPEND CONFIGINPUTS "${CONFIGINPUTS1}")
LIST (APPEND CONFIGINPUTS "${CONFIGINPUTS2}")
LIST (APPEND CONFIGINPUTS "${CONFIGINPUTS3}")
LIST (APPEND CONFIGINPUTS "${CONFIGINPUTS4}")
LIST (APPEND CONFIGINPUTS "${CONFIGINPUTS5}")
LIST (APPEND CONFIGINPUTS "${CONFIGINPUTS6}")
FOREACH (CONFIGINPUT ${CONFIGINPUTS})
    STRING (REPLACE ".in.h.cmake" ".h" CONFIGOUTPUT1 "${CONFIGINPUT}")
    STRING (REPLACE ".h.in.cmake" ".h" CONFIGOUTPUT2 "${CONFIGOUTPUT1}")
    CONFIGURE_FILE ("${CONFIGINPUT}" "${CONFIGOUTPUT2}")
ENDFOREACH (CONFIGINPUT ${CONFIGINPUTS})

################################################################################
# Add Build Targets
################################################################################

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(test)

################################################################################
# Add Install Targets
################################################################################

IF (EXISTS "${MAINFOLDER}/include/${PROJECT_NAME}" AND IS_DIRECTORY "${MAINFOLDER}/include/${PROJECT_NAME}")
    INSTALL(DIRECTORY "${MAINFOLDER}/include/${PROJECT_NAME}" DESTINATION "include")
ENDIF (EXISTS "${MAINFOLDER}/include/${PROJECT_NAME}" AND IS_DIRECTORY "${MAINFOLDER}/include/${PROJECT_NAME}")

################################################################################
# Add Documentation Targets
################################################################################

INCLUDE (DocumentationTargets)

