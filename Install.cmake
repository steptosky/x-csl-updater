
# options
# -----------------------------------------------------------------------#
option(CLEANUP_INSTALL_FIRST "Will cleanup the install folder before install." OFF)

# -----------------------------------------------------------------------#
install (CODE "message(\"[INSTALL] Start install process...\")")

# -----------------------------------------------------------------------#
# cleanup
if (CLEANUP_INSTALL_FIRST)
	install (CODE "FILE(REMOVE_RECURSE ${CMAKE_INSTALL_PREFIX}/${PROJECT})")
endif()

# -----------------------------------------------------------------------#
# binary parts

install (
	DIRECTORY 
	${CMAKE_SOURCE_DIR}/bin/release/
	DESTINATION ${PROJECT}
	#FILES_MATCHING PATTERN "*.dll"
	#FILES_MATCHING PATTERN "*.xpl"
	PATTERN "liblinux" EXCLUDE
	#FILES_MATCHING PATTERN "*.xpl"
)

install (
	DIRECTORY 
	${CMAKE_SOURCE_DIR}/liblinux/
	DESTINATION ${PROJECT}/64/liblinux
)

# -----------------------------------------------------------------------#
# resources parts

install (
	DIRECTORY 
	${CMAKE_SOURCE_DIR}/resources/cache/
	DESTINATION ${PROJECT}/cache/
	FILES_MATCHING PATTERN "*.txt"
)

install (
	DIRECTORY 
	${CMAKE_SOURCE_DIR}/resources/routes/
	DESTINATION ${PROJECT}/routes/
	FILES_MATCHING PATTERN "*.txt"
)

install (
	FILES 
	${CMAKE_SOURCE_DIR}/resources/version.txt
	DESTINATION ${PROJECT}/
)

#version.txt

# -----------------------------------------------------------------------#

install (CODE "message(\"[INSTALL] Install process done.\")")