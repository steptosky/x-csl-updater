set(WORKDIR "${CMAKE_SOURCE_DIR}/../bin")
set(CQTDEPLOYER_QMAKE "$ENV{QT_PATH}/bin/qmake")

message(STATUS "workdir: ${WORKDIR}")
message(STATUS "cqtdeployer qmake: ${CQTDEPLOYER_QMAKE}")

if(NOT EXISTS "${CQTDEPLOYER_QMAKE}")
    message(FATAL_ERROR "qmake not found: ${CQTDEPLOYER_QMAKE}")
endif()

# cqtdeployer -bin Guitar
execute_process(
    COMMAND cqtdeployer
    -bin X-CSL-Updater
    -qmake ${CQTDEPLOYER_QMAKE}
    noTranslations
    WORKING_DIRECTORY ${WORKDIR}
    RESULT_VARIABLE deployqt_result
    OUTPUT_VARIABLE deployqt_output
    ERROR_VARIABLE  deployqt_error
)

message(STATUS "cqtdeployer stdout: ${deployqt_output}")
message(STATUS "cqtdeployer stderr: ${deployqt_error}")
message(STATUS "cqtdeployer result: ${deployqt_result}")

if (NOT ${deployqt_result} EQUAL 0)
    message(FATAL_ERROR "cqtdeployer is failed!")
endif()