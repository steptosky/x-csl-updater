message(STATUS "[INSTALL] WIN Packaging...")

set(SRC ${DEPLOY_DIR})
message(STATUS "SRC: ${SRC}")

set(DST ${PACKAGE_DIR})
message(STATUS "DST: ${DST}")
file(MAKE_DIRECTORY "${DST}")

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