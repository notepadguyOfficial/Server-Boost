#!/bin/bash

LOGS_FOLDER="Logs"
BUILD_FOLDER="build"
SETTINGS_FILE="Settings.json"
BUILD_LOG="build.log"

python3 tools/update/cmake.py

[[ -d "$LOGS_FOLDER" ]] && rm -rf "$LOGS_FOLDER"
[[ -d "$BUILD_FOLDER" ]] && rm -rf "$BUILD_FOLDER"
[[ -f "$SETTINGS_FILE" ]] && rm -f "$SETTINGS_FILE"

mkdir "$BUILD_FOLDER"
cd "$BUILD_FOLDER" || exit 1

if cmake .. 2>&1 | tee "$BUILD_LOG"; then
    if make 2>&1 | tee -a "$BUILD_LOG"; then
        clear
        mv -f compile_commands.json ..
    else
        echo "Make failed. See $BUILD_FOLDER/$BUILD_LOG for details."
        exit 1
    fi
else
    echo "CMake failed. See $BUILD_FOLDER/$BUILD_LOG for details."
    exit 1
fi
