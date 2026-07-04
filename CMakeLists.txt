cmake_minimum_required(VERSION 3.25)

if (NOT DEFINED QT_PATH OR QT_PATH STREQUAL "")
    set (QT_PATH $ENV{QT_PATH})
endif()

if (QT_PATH)
    list(PREPEND CMAKE_PREFIX_PATH "${QT_PATH}")
endif()

message(STATUS "QT_PATH: ${QT_PATH}")

set (SOLUTION X-CSL-Updater)
set (INFO_H_PATH ${CMAKE_SOURCE_DIR}/src/Info.h)

if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE Release CACHE STRING "Default configuration is Release. Set key -DCMAKE_BUILD_TYPE=Debug for debug.")
endif()

project( ${SOLUTION} )

include(${CMAKE_SOURCE_DIR}/Description.cmake)
loadDescription()

include(cmake/GenInfoFile.cmake)
genInfoFile("${INFO_H_PATH}" "${CMAKE_SOURCE_DIR}/Description.cmake")

add_subdirectory(src)
