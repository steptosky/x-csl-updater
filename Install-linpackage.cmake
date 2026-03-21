message(STATUS "[INSTALL] LIN Packaging...")

set(SRC ${DEPLOY_DIR}/)
message(STATUS "SRC: ${SRC}")

set(DST ${PACKAGE_DIR})
message(STATUS "DST: ${DST}")

set(ARCHIVE ${DST}/${PROJECT}_lin_${VERSION}-${vcs_revision}.tar.gz)

file(ARCHIVE_CREATE
    OUTPUT "${ARCHIVE}"
    PATHS "."
    WORKING_DIRECTORY "${SRC}"
    FORMAT gnutar
    COMPRESSION GZip
    VERBOSE
)

if(EXISTS "${ARCHIVE}")
    message(STATUS "Archive created: ${ARCHIVE}")
else()
    message(FATAL_ERROR "Archive was not created: ${ARCHIVE}")
endif()