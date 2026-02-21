message(STATUS "[INSTALL] WIN Packaging...")

set(SRC ${CMAKE_INSTALL_PREFIX}/${PROJECT})
message(STATUS "SRC: ${SRC}")

set(DST ${CMAKE_INSTALL_PREFIX})
message(STATUS "DST: ${DST}")

set(ARCHIVE ${DST}/${PROJECT}_win_${VERSION}-${vcs_revision}.zip)

file(ARCHIVE_CREATE
    OUTPUT "${ARCHIVE}"
    PATHS "."
    WORKING_DIRECTORY "${SRC}"
    FORMAT zip
    VERBOSE
)

if(EXISTS "${ARCHIVE}")
    message(STATUS "Archive created: ${ARCHIVE}")
else()
    message(FATAL_ERROR "Archive was not created: ${ARCHIVE}")
endif()