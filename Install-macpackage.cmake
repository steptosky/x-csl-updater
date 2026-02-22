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

set(DMG_BACKGROUND "${SOURCE_DIR}/src/dmg_background.jpg")
set(DMG_ICON "${SOURCE_DIR}/src/xcsl.icns")

# IMPORTANT:
# These names must match exactly what is inside ${DEPLOY_DIR}
# (including spaces and extensions).
set(APP_BUNDLE_NAME "${PROJECT}.app")
set(CHANGELOG_NAME "changeLog.txt")
set(README_ENG_NAME "Readme-eng.txt")
set(README_RU_NAME "Readme-ru.txt")

execute_process(
    COMMAND "${CREATE_DMG_EXE}"
        --volname ${PROJECT}
        --volicon ${DMG_ICON}
        # Disables the automatic mount and copy functionality
        --no-internet-enable

        --background ${DMG_BACKGROUND}
        --window-size 600 460
        --window-pos 200 120
        --icon-size 96
        --text-size 12        

        # Position the app bundle icon
        --icon "${APP_BUNDLE_NAME}" 0 100
        --app-drop-link 400 100

        # Position text files
        --icon "${CHANGELOG_NAME}" 0 300
        --icon "${README_ENG_NAME}" 200 300
        --icon "${README_RU_NAME}" 400 300 

        "${DMG_OUT}"
        "${SRC}"
    RESULT_VARIABLE result
    OUTPUT_VARIABLE stdout
    ERROR_VARIABLE stderr
)

message(STATUS "create-dmg  stdout: ${stdout}")
message(STATUS "create-dmg  stderr: ${stderr}")
message(STATUS "create-dmg  result: ${result}")

if(NOT result EQUAL 0)
    message(FATAL_ERROR "create-dmg failed with code: ${result}")
endif()

if(EXISTS "${DMG_OUT}")
    message(STATUS "DMG created: ${DMG_OUT}")
else()
    message(FATAL_ERROR "DMG was not created: ${DMG_OUT}")
endif()