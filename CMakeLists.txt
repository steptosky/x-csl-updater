cmake_minimum_required(VERSION 3.1.0)

set (SOLUTION X-CSL-Updater)
set (INFO_H_PATH ${CMAKE_SOURCE_DIR}/src/Info.h)

if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE Release CACHE STRING "Default configuration is Release. Set key -DCMAKE_BUILD_TYPE=Debug for debug.")
endif()

project( ${SOLUTION} )

include(cmake/GenInfoFile.cmake)
genInfoFile("${INFO_H_PATH}" "${CMAKE_SOURCE_DIR}/Description.cmake")

add_subdirectory(src)
