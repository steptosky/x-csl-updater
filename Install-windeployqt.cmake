
set(STRIPPED_QT_PATH $ENV{QT_PATH})
set(WORKDIR ${CMAKE_INSTALL_PREFIX}/X-CSL-Updater)
string(REPLACE "\"" "" STRIPPED_QT_PATH ${STRIPPED_QT_PATH})
message(STATUS "windeployqt path: ${STRIPPED_QT_PATH}/bin/windeployqt")
message(STATUS "workdir: ${WORKDIR}")

execute_process(
    COMMAND ${STRIPPED_QT_PATH}/bin/windeployqt
    X-CSL-Updater.exe
    --no-translations
    --no-system-d3d-compiler
    --no-compiler-runtime
    --no-webkit2
    --no-angle
    --no-opengl-sw
    --no-patchqt
    --no-quick-import
    --release
    WORKING_DIRECTORY ${WORKDIR}
    RESULT_VARIABLE windeployqt_result
    OUTPUT_VARIABLE windeployqt_output
)

message(STATUS "windeployqt results: [${windeployqt_result}]: ${windeployqt_output}")

if (NOT ${windeployqt_result} EQUAL 0)
    message(FATAL_ERROR "windeployqt is failed!")
endif()

#file(REMOVE_RECURSE
#    ${WORKDIR}/Qt5Svg.dll
#    ${WORKDIR}/bearer
#    ${WORKDIR}/iconengines
#    ${WORKDIR}/imageformats
#    ${WORKDIR}/styles
#)