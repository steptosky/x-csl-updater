#!/bin/bash

if [ -n "${QT_PATH:-}" ]; then
    return 0 2>/dev/null || exit 0
fi

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

if ! resolved_qt_path="$(cmake -P "$script_dir/resolve-qt-path.cmake")"; then
    return 1 2>/dev/null || exit 1
fi

if [ -z "$resolved_qt_path" ]; then
    return 1 2>/dev/null || exit 1
fi

export QT_PATH="$resolved_qt_path"
