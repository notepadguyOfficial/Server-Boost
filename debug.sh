#!/bin/bash

PROGRAM_PATH="build/Server"

if [ -x "$PROGRAM_PATH" ]; then
    "$PROGRAM_PATH" --debug
else
    echo "Error: Server executable not found at $PROGRAM_PATH"
    exit 1
fi