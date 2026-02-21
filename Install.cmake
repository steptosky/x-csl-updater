
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
# grab variables

install(CODE "set(QT_PATH ${QT_PATH})")
install(CODE "set(PROJECT ${PROJECT})")
install(CODE "set(VERSION ${VersionMajorString}.${VersionMinorString}.${VersionPatchString})")
install(CODE "set(vcs_revision ${vcs_revision})")

# -----------------------------------------------------------------------#
# deploying

if (MSVC)
    install(
        DIRECTORY 
        ${CMAKE_SOURCE_DIR}/bin/release/
        DESTINATION ${PROJECT}
        FILES_MATCHING
        PATTERN "${PROJECT}.exe"
    )
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
# Packaging
if (MSVC)
    install(SCRIPT "${CMAKE_SOURCE_DIR}/Install-winpackage.cmake")

elseif (APPLE)
    install(CODE "message(\"[INSTALL] MAC Packaging...\")")
    
else ()
    install(CODE "message(\"[INSTALL] Unix Packaging...\")")
    
endif()

# -----------------------------------------------------------------------#

install (CODE "message(\"[INSTALL] Install process done.\")")