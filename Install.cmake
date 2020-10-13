
# options
# -----------------------------------------------------------------------#
option(CLEANUP_INSTALL_FIRST "Will cleanup the install folder before install." OFF)

# -----------------------------------------------------------------------#
install(CODE "message(\"[INSTALL] Start install process...\")")

# -----------------------------------------------------------------------#
# cleanup
if (CLEANUP_INSTALL_FIRST)
    install(CODE "FILE(REMOVE_RECURSE ${CMAKE_INSTALL_PREFIX}/${PROJECT})")
endif()

# -----------------------------------------------------------------------#
# binary parts

if (MSVC)

    install(
        DIRECTORY 
        ${CMAKE_SOURCE_DIR}/bin/release/
        DESTINATION ${PROJECT}
        FILES_MATCHING
        PATTERN "${PROJECT}.exe"
    )
    #install(FILES ${QT_LIBS} DESTINATION ${PROJECT})
    install(SCRIPT "${CMAKE_SOURCE_DIR}/Install-windeployqt.cmake")

elseif (APPLE)

    install(SCRIPT "${CMAKE_SOURCE_DIR}/Install-macdeployqt.cmake")
    install(
        DIRECTORY
        ${CMAKE_SOURCE_DIR}/bin/${PROJECT}.app
        DESTINATION ${PROJECT}
        USE_SOURCE_PERMISSIONS
    )  

else ()
    install(SCRIPT "${CMAKE_SOURCE_DIR}/Install-lindeployqt.cmake")
    install(
        DIRECTORY 
        ${CMAKE_SOURCE_DIR}/bin/DistributionKit/
        DESTINATION ${PROJECT}
        USE_SOURCE_PERMISSIONS
    )
    
endif()



# -----------------------------------------------------------------------#
# additional parts

install(
	DIRECTORY 
	${CMAKE_SOURCE_DIR}/docs/
	DESTINATION ${PROJECT}
	FILES_MATCHING PATTERN "*.txt"
)

# -----------------------------------------------------------------------#

install (CODE "message(\"[INSTALL] Install process done.\")")