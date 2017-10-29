# Makes folder name by compiler data.
# MSVC example: msvc_18_x64
# GNU example: gcc_4_x64
# Clang example: clang_3_x64
# AppleClang example: apple_clang_6_x64
# Digital after name is major version of the compiler

macro(makeDeployFolderString)
	
	#---------------------------------------------------------------
	
	if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
		set (DEPLOY_FOLDER  "msvc")
	elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
		set (DEPLOY_FOLDER  "gcc")
	elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
		set (DEPLOY_FOLDER  "clang")
	elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL AppleClang)
		set (DEPLOY_FOLDER  "apple_clang")
	else ()
		message (FATAL_ERROR "<Deploy folder name generator> Unknown compiler")
	endif ()

	set(__VER_TEMP__)
	string(REPLACE "." ";" VERSION_LIST ${CMAKE_CXX_COMPILER_VERSION})
	list(GET VERSION_LIST 0 __VER_TEMP__)
	set (DEPLOY_FOLDER  "${DEPLOY_FOLDER}_${__VER_TEMP__}")
	
	if (${PLATFORM_BITS} STREQUAL x64)
		set (DEPLOY_FOLDER  "${DEPLOY_FOLDER}_x64")
	endif ()
	
	#---------------------------------------------------------------
	
endmacro()
