cmake_minimum_required(VERSION 3.19)

if(DEFINED QT_PATH AND NOT QT_PATH STREQUAL "")
    set(resolved_qt_path "${QT_PATH}")
elseif(DEFINED ENV{QT_PATH} AND NOT "$ENV{QT_PATH}" STREQUAL "")
    set(resolved_qt_path "$ENV{QT_PATH}")
else()
    if(CMAKE_HOST_WIN32)
        set(platform_key "windows")
    elseif(CMAKE_HOST_APPLE)
        set(platform_key "macos")
    elseif(CMAKE_HOST_UNIX)
        set(platform_key "linux")
    else()
        message(FATAL_ERROR "Unsupported host platform: ${CMAKE_HOST_SYSTEM_NAME}")
    endif()

    get_filename_component(script_dir "${CMAKE_CURRENT_LIST_FILE}" DIRECTORY)
    set(qt_paths_file "${script_dir}/qt-paths.json")

    if(NOT EXISTS "${qt_paths_file}")
        message(FATAL_ERROR "Qt paths file not found: ${qt_paths_file}")
    endif()

    file(READ "${qt_paths_file}" qt_paths_json)
    string(JSON resolved_qt_path
        ERROR_VARIABLE json_error
        GET "${qt_paths_json}" "${platform_key}"
    )

    if(NOT json_error STREQUAL "NOTFOUND")
        message(FATAL_ERROR
            "Failed to read key '${platform_key}' from ${qt_paths_file}: ${json_error}"
        )
    endif()

    string(STRIP "${resolved_qt_path}" resolved_qt_path)

    if(resolved_qt_path STREQUAL "" OR resolved_qt_path STREQUAL "null")
        message(FATAL_ERROR
            "Qt path for '${platform_key}' is empty in ${qt_paths_file}"
        )
    endif()
endif()

execute_process(COMMAND "${CMAKE_COMMAND}" -E echo "${resolved_qt_path}")
