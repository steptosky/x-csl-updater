cmake_minimum_required(VERSION 3.1.0)

set (SOLUTION X-CSL-Updater)
set (INFO_H_PATH ${CMAKE_SOURCE_DIR}/src/Info.h)

if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE Release CACHE STRING "Default configuration is Release. Set key -DCMAKE_BUILD_TYPE=Debug for debug.")
endif()

set(PLATFORM_BITS "x64" CACHE STRING "user specified flag: x86 or x64")

project( ${SOLUTION} )

add_subdirectory(src)
