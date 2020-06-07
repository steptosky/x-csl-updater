set(WORKDIR ${CMAKE_SOURCE_DIR}/../bin)
string(REPLACE "\"" "" STRIPPED_QT_PATH "$ENV{QT_PATH}")
message(STATUS "workdir: ${WORKDIR}")

# cqtdeployer -bin Guitar
execute_process(
    COMMAND cqtdeployer
    -bin X-CSL-Updater
    noTranslations
    WORKING_DIRECTORY ${WORKDIR}
    RESULT_VARIABLE deployqt_result
    OUTPUT_VARIABLE deployqt_output
)

message(STATUS "cqtdeployer results: [${deployqt_result}]: ${deployqt_output}")

if (NOT ${deployqt_result} EQUAL 0)
    message(FATAL_ERROR "cqtdeployer is failed!")
endif()