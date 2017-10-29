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
    "*.qm"
    "*.ts"
    "*.qrc"
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
	add_definitions (/W4  -wd4996 -wd4244 -wd4099 -wd4800)
	
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL AppleClang)

	message ("Building <${PROJECT}> using settings for <${CMAKE_CXX_COMPILER_ID}> compiler")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas")
	
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)

	message ("Building <${PROJECT}> using settings for <${CMAKE_CXX_COMPILER_ID}> compiler")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas")
	set (CMAKE_SHARED_LINKER_FLAGS
    		"${CMAKE_SHARED_LINKER_FLAGS} -Wl,-rpath,'$ORIGIN/liblinux/'"
	)	
	
else ()
	message (FATAL_ERROR "<${PROJECT}> Unknown compiler")
endif ()

# -----------------------------------------------------------------------#
# global definitions 

#add_definitions (-DXPLM200 -DXPLM210)


# Find the Qt libraries
find_package(Qt5Widgets)
find_package(Qt5Network)

# Tell CMake to create the ${PROJECT} executable
add_executable(${TARGET_NAME} WIN32 ${CM_FILES})

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

# Install script
include(${CMAKE_SOURCE_DIR}/Install.cmake)

