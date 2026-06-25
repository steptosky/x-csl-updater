message(STATUS "[INSTALL] WIN Deploying...")

message(STATUS "QT_PATH: ${QT_PATH}")
set(STRIPPED_QT_PATH ${QT_PATH})
string(REPLACE "\"" "" STRIPPED_QT_PATH ${STRIPPED_QT_PATH})
message(STATUS "STRIPPED_QT_PATH: ${STRIPPED_QT_PATH}")

set(DEPLOYER_BIN ${STRIPPED_QT_PATH}/bin/windeployqt)
message(STATUS "windeployqt path: ${DEPLOYER_BIN}")

execute_process(
    COMMAND ${DEPLOYER_BIN}
    ${TARGET_FILE_NAME}
    --no-translations
    --no-system-d3d-compiler
    --no-compiler-runtime
    --no-webkit2
    --no-angle
    --no-opengl-sw
    --no-patchqt
    --no-quick-import
    --release
    WORKING_DIRECTORY ${DEPLOY_DIR}
    RESULT_VARIABLE result
    OUTPUT_VARIABLE stdout
    ERROR_VARIABLE stderr
)

message(STATUS "windeployqt stdout: ${stdout}")
message(STATUS "windeployqt stderr: ${stderr}")
message(STATUS "windeployqt result: ${result}")

if (NOT ${result} EQUAL 0)
    message(FATAL_ERROR "windeployqt is failed!")
else()
    message(STATUS "[INSTALL] WIN Deploying done.")
endif()
