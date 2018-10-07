# -----------------------------------------------------------------------#
# project

set (PROJECT X-CSL-Updater)
project( ${PROJECT} )
set (TARGET_NAME ${PROJECT})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# -----------------------------------------------------------------------#
# project files

file(GLOB_RECURSE CM_FILES 
    "*.h"
    "*.inl"
    "*.hpp"
    "*.cpp"
    "*.ui"
#    "*.qm"
#    "*.ts"
    "*.qrc"
    "*.rc"
)

foreach(FILE ${CM_FILES}) 	
    get_filename_component(PARENT_DIR "${FILE}" PATH)	
    string(REPLACE  "${CMAKE_SOURCE_DIR}/" "" GROUP "${PARENT_DIR}")	  
    string(REPLACE "/" "\\" GROUP "${GROUP}")
    source_group("${GROUP}" FILES "${FILE}")	
endforeach()

# -----------------------------------------------------------------------#
# compiler and linker settings

if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)

	message ("Building <${PROJECT}> using settings for <${CMAKE_CXX_COMPILER_ID}> compiler")
	add_definitions (/W4  -wd4996 -wd4244 -wd4099 -wd4800 -wd4127)
	
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL AppleClang)

	message ("Building <${PROJECT}> using settings for <${CMAKE_CXX_COMPILER_ID}> compiler")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas -std=c++11")
	
else ()

	message ("Building <${PROJECT}> using settings for <${CMAKE_CXX_COMPILER_ID}> compiler")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas -std=c++11")
	set (CMAKE_SHARED_LINKER_FLAGS
    		"${CMAKE_SHARED_LINKER_FLAGS} -Wl,-rpath,'$ORIGIN/'"
	)	
endif ()

# -----------------------------------------------------------------------#
# global definitions 

#add_definitions (-DXPLM200 -DXPLM210)


# Find the Qt libraries
find_package(Qt5Widgets)
find_package(Qt5Network)
#find_package(Qt5LinguistTools)

# Tell CMake to create the ${PROJECT} executable
add_executable(${TARGET_NAME} WIN32 MACOSX_BUNDLE ${CM_FILES})

# -----------------------------------------------------------------------#
# Global Libraries linkage 

target_link_libraries (${TARGET_NAME} Qt5::Widgets)
target_link_libraries (${TARGET_NAME} Qt5::Network)


# Setup output folders for specified project
set_target_properties(${TARGET_NAME}
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)

# Copy Qt libs
if (NOT APPLE)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy_if_different 
        $<TARGET_FILE:Qt5::Core> $<TARGET_FILE_DIR:${PROJECT}>
        $<TARGET_FILE:Qt5::Network> $<TARGET_FILE_DIR:${PROJECT}>
        $<TARGET_FILE:Qt5::Gui> $<TARGET_FILE_DIR:${PROJECT}>
        $<TARGET_FILE:Qt5::Widgets> $<TARGET_FILE_DIR:${PROJECT}>
    )
endif()

# Install script
include(${CMAKE_SOURCE_DIR}/Install.cmake)

