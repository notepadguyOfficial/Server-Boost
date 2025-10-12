#!/bin/bash

LOGS_FOLDER="Logs"
BUILD_FOLDER="build"
SETTINGS_FILE="Settings.json"

if [ -d "$LOGS_FOLDER" ]; then
    rm -rf Logs
fi

if [ -d "$BUILD_FOLDER" ]; then
    rm -rf build
fi

if [ -f "$SETTINGS_FILE" ]; then
    rm -f "$SETTINGS_FILE"
fi

exit 1