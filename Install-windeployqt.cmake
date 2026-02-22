message(STATUS "[INSTALL] WIN Deploying...")

message(STATUS "QT_PATH: ${QT_PATH}")
set(STRIPPED_QT_PATH ${QT_PATH})
string(REPLACE "\"" "" STRIPPED_QT_PATH ${STRIPPED_QT_PATH})
message(STATUS "STRIPPED_QT_PATH: ${STRIPPED_QT_PATH}")

set(DEPLOYER_BIN ${STRIPPED_QT_PATH}/bin/windeployqt)
message(STATUS "windeployqt path: ${DEPLOYER_BIN}")

execute_process(
    COMMAND ${DEPLOYER_BIN}
    ${PROJECT}.exe
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
    RESULT_VARIABLE dmg_result
    OUTPUT_VARIABLE dmg_stdout
    ERROR_VARIABLE dmg_stderr
)

message(STATUS "windeployqt stdout: ${deployqt_output}")
message(STATUS "windeployqt stderr: ${deployqt_error}")
message(STATUS "windeployqt result: ${deployqt_result}")

if (NOT ${windeployqt_result} EQUAL 0)
    message(FATAL_ERROR "windeployqt is failed!")
else()
    message(STATUS "[INSTALL] WIN Deploying done.")
endif()
