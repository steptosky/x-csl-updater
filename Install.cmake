
# options
# -----------------------------------------------------------------------#
option(CLEANUP_INSTALL_FIRST "Will cleanup the install folder before install." ON)

install(CODE "message(\"[INSTALL] Start install process...\")")

# -----------------------------------------------------------------------#
# check for Release configuration for Installing

install(CODE [=[
  if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
    message(FATAL_ERROR "Install configuration is unknown.")
  endif()

  if(NOT CMAKE_INSTALL_CONFIG_NAME STREQUAL "Release")
    message(FATAL_ERROR "Only Release install is allowed. Current install config: ${CMAKE_INSTALL_CONFIG_NAME}")
  endif()
]=])

# -----------------------------------------------------------------------#
# setup variables

set(VERSION ${VersionMajorString}.${VersionMinorString}.${VersionPatchString})
set(vcs_revision ${vcs_revision})

set(BASE_DIR ${CMAKE_INSTALL_PREFIX})
set(DEPLOY_DIR_REL Deployed)
set(PACKAGE_DIR_REL Packaged)
set(DEPLOY_DIR ${BASE_DIR}/${DEPLOY_DIR_REL})
set(PACKAGE_DIR ${BASE_DIR}/${PACKAGE_DIR_REL})

# -----------------------------------------------------------------------#
# some printing, variables are expanding on configure time!

install(CODE "message(\"PROJECT: ${PROJECT}\")")
install(CODE "message(\"VERSION: ${VERSION}\")")
install(CODE "message(\"vcs_revision: ${vcs_revision}\")")

#install(CODE "message(\"QT_PATH: ${QT_PATH}\")")
install(CODE "message(\"BASE_DIR: ${BASE_DIR}\")")
install(CODE "message(\"DEPLOY_DIR: ${DEPLOY_DIR}\")")
install(CODE "message(\"PACKAGE_DIR: ${PACKAGE_DIR}\")")

install(CODE "message(\"TARGET_FILE: $<TARGET_FILE:${PROJECT}>\")")
install(CODE "message(\"TARGET_FILE_DIR: $<TARGET_FILE_DIR:${PROJECT}>\")")
install(CODE "message(\"TARGET_FILE_NAME: $<TARGET_FILE_NAME:${PROJECT}>\")")

# -----------------------------------------------------------------------#
# grab variables to Install context

install(CODE "set(QT_PATH ${QT_PATH})")
install(CODE "set(PROJECT ${PROJECT})")
install(CODE "set(VERSION ${VERSION})")
install(CODE "set(vcs_revision ${vcs_revision})")

install(CODE "set(BASE_DIR ${BASE_DIR})")
install(CODE "set(DEPLOY_DIR_REL ${DEPLOY_DIR_REL})")
install(CODE "set(PACKAGE_DIR_REL ${PACKAGE_DIR_REL})")
install(CODE "set(DEPLOY_DIR ${DEPLOY_DIR})")
install(CODE "set(PACKAGE_DIR ${PACKAGE_DIR})")

install(CODE "set(TARGET_FILE $<TARGET_FILE:${PROJECT}>)")
install(CODE "set(TARGET_FILE_DIR $<TARGET_FILE_DIR:${PROJECT}>)")
install(CODE "set(TARGET_FILE_NAME $<TARGET_FILE_NAME:${PROJECT}>)")

# -----------------------------------------------------------------------#
# cleanup

if (CLEANUP_INSTALL_FIRST)
    install(CODE "file(REMOVE_RECURSE ${DEPLOY_DIR})")
    install(CODE "file(REMOVE_RECURSE ${PACKAGE_DIR})")
endif()

# -----------------------------------------------------------------------#
# create dirs

install(CODE "file(MAKE_DIRECTORY ${DEPLOY_DIR})")
install(CODE "file(MAKE_DIRECTORY ${PACKAGE_DIR})")

# -----------------------------------------------------------------------#
# deploying

if (MSVC)
    install(TARGETS ${PROJECT}
        CONFIGURATIONS Release
        RUNTIME DESTINATION ${DEPLOY_DIR}
    )
    install(SCRIPT "${CMAKE_SOURCE_DIR}/Install-windeployqt.cmake")

elseif (APPLE)
    install(SCRIPT "${CMAKE_SOURCE_DIR}/Install-macdeployqt.cmake")
    install(
        DIRECTORY
        ${CMAKE_SOURCE_DIR}/bin/${PROJECT}.app
        DESTINATION ${DEPLOY_DIR}
        USE_SOURCE_PERMISSIONS
    )

else ()
    # install(TARGETS ${PROJECT}
    #     CONFIGURATIONS Release
    #     RUNTIME DESTINATION ${DEPLOY_DIR}
    # )
    install(SCRIPT "${CMAKE_SOURCE_DIR}/Install-lindeployqt.cmake")
    
endif()

# -----------------------------------------------------------------------#
# additional parts

install(
    DIRECTORY 
    ${CMAKE_SOURCE_DIR}/docs/
    DESTINATION ${DEPLOY_DIR}
    FILES_MATCHING PATTERN "*.txt"
)

# -----------------------------------------------------------------------#
# Packaging
if (MSVC)
    install(SCRIPT "${CMAKE_SOURCE_DIR}/Install-winpackage.cmake")
elseif (APPLE)
    install(CODE "message(\"[INSTALL] MAC Packaging...\")")
else ()
    install(SCRIPT "${CMAKE_SOURCE_DIR}/Install-linpackage.cmake")
endif()

# -----------------------------------------------------------------------#

install (CODE "message(\"[INSTALL] Install process done.\")")