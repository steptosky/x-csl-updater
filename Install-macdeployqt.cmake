message(STATUS "[INSTALL] MAC Deploying...")

set(WORKDIR ${TARGET_FILE_DIR})
string(REPLACE "\"" "" STRIPPED_QT_PATH "${QT_PATH}")
set(DEPLOYER_BIN ${STRIPPED_QT_PATH}/bin/macdeployqt)

message(STATUS "QT_PATH: ${QT_PATH}")
message(STATUS "MacDeployQT path: ${DEPLOYER_BIN}")
message(STATUS "workdir: ${WORKDIR}")

execute_process(
    COMMAND ${DEPLOYER_BIN}
    ${TARGET_FILE_NAME}
    WORKING_DIRECTORY ${WORKDIR}
    RESULT_VARIABLE result
    OUTPUT_VARIABLE stdout
    ERROR_VARIABLE stderr
)

message(STATUS "macdeployqt stdout: ${stdout}")
message(STATUS "macdeployqt stderr: ${stderr}")
message(STATUS "macdeployqt result: ${result}")

if (NOT ${result} EQUAL 0)
    message(FATAL_ERROR "macdeployqt is failed!")
else()
    message(STATUS "[INSTALL] MAC Deploying done.")
endif()