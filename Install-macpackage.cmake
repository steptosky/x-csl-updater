message(STATUS "[INSTALL] MAC Packaging...")

# Only try to create DMG if create-dmg tool exists.
find_program(CREATE_DMG_EXE create-dmg)
if(NOT CREATE_DMG_EXE)
    message(FATAL_ERROR "create-dmg not found in PATH.")
endif()

set(SRC ${DEPLOY_DIR}/)
message(STATUS "SRC: ${SRC}")

set(DST ${PACKAGE_DIR})
message(STATUS "DST: ${DST}")

set(DMG_OUT ${DST}/${PROJECT}_mac_${VERSION}-${vcs_revision}.dmg)

execute_process(
    COMMAND "${CREATE_DMG_EXE}"
        --volname ${PROJECT}
        --window-size 700 450
        --icon-size 128
        --app-drop-link 510 225
        "${DMG_OUT}"
        "${SRC}"
    RESULT_VARIABLE result
    OUTPUT_VARIABLE stdout
    ERROR_VARIABLE stderr
)

message(STATUS "create-dmg  stdout: ${stdout}")
message(STATUS "create-dmg  stderr: ${stderr}")
message(STATUS "create-dmg  result: ${result}")

if(EXISTS "${DMG_OUT}")
    message(STATUS "DMG created: ${DMG_OUT}")
else()
    message(FATAL_ERROR "DMG was not created: ${DMG_OUT}")
endif()