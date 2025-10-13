#!/bin/bash

LOGS_FOLDER="Logs"
BUILD_FOLDER="build"
SETTINGS_FILE="Settings.json"

if [ -d "$LOGS_FOLDER" ]; then
    rm -rf "$LOGS_FOLDER"
fi

if [ -d "$BUILD_FOLDER" ]; then
    rm -rf "$BUILD_FOLDER"
fi

if [ -f "$SETTINGS_FILE" ]; then
    rm -f "$SETTINGS_FILE"
fi

mkdir "$BUILD_FOLDER"
cd "$BUILD_FOLDER" || exit 1

if cmake .. && make; then
    clear
else # if error occured does not clear the screen
    exit 1
fi
