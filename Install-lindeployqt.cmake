message(STATUS "[INSTALL] LIN Deploying...")

set(WORKDIR "${TARGET_FILE_DIR}")
set(CQTDEPLOYER_QMAKE "${QT_PATH}/bin/qmake")

message(STATUS "workdir: ${WORKDIR}")
message(STATUS "QT_PATH: ${QT_PATH}")
message(STATUS "cqtdeployer qmake: ${CQTDEPLOYER_QMAKE}")

if(NOT EXISTS "${CQTDEPLOYER_QMAKE}")
    message(FATAL_ERROR "qmake not found: ${CQTDEPLOYER_QMAKE}")
endif()

# cqtdeployer -bin Guitar
execute_process(
    COMMAND cqtdeployer
    -bin ${TARGET_FILE_NAME}
    -qmake ${CQTDEPLOYER_QMAKE}
    -targetDir ${DEPLOY_DIR}
    noTranslations
    WORKING_DIRECTORY ${WORKDIR}
    RESULT_VARIABLE result
    OUTPUT_VARIABLE stdout
    ERROR_VARIABLE stderr
)

message(STATUS "cqtdeployer stdout: ${stdout}")
message(STATUS "cqtdeployer stderr: ${stderr}")
message(STATUS "cqtdeployer result: ${result}")

if (NOT ${result} EQUAL 0)
    message(FATAL_ERROR "cqtdeployer is failed!")
else()
    message(STATUS "[INSTALL] WIN Deploying done.")
endif()