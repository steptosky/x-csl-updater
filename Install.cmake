
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
    FILES_MATCHING 
    PATTERN "*.exe"
    PATTERN "*.dll"
)

install(FILES ${QT_LIBS} DESTINATION ${PROJECT})

# -----------------------------------------------------------------------#
# additional parts

install (
	DIRECTORY 
	${CMAKE_SOURCE_DIR}/docs/
	DESTINATION ${PROJECT}/docs/
	FILES_MATCHING PATTERN "*.txt"
)

# -----------------------------------------------------------------------#

install (CODE "message(\"[INSTALL] Install process done.\")")