#set(WORKDIR ${CMAKE_SOURCE_DIR}/../bin)
#string(REPLACE "\"" "" STRIPPED_QT_PATH "$ENV{QT_PATH}")
#message(STATUS "deployQT path: ${STRIPPED_QT_PATH}/bin/macdeployqt")
#message(STATUS "workdir: ${WORKDIR}")
#
#execute_process(
#    COMMAND ${STRIPPED_QT_PATH}/bin/macdeployqt
#    X-CSL-Updater.app
#    WORKING_DIRECTORY ${WORKDIR}
#    RESULT_VARIABLE deployqt_result
#    OUTPUT_VARIABLE deployqt_output
#)
#
#message(STATUS "deployQT results: [${deployqt_result}]: ${deployqt_output}")
#
#if (NOT ${deployqt_result} EQUAL 0)
#    message(FATAL_ERROR "deployQT is failed!")
#endif()