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

clear

exit 1