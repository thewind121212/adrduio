#!/bin/bash

# Configuration
PROJECT_DIR=$(dirname "$0")
DEFAULT_FILE="main.ino"
ARDUINO_CLI="arduino-cli"

# Parse arguments
while getopts "f:" opt; do
    case $opt in
        f) INO_FILE="$PROJECT_DIR/$OPTARG" ;;
        *) echo "Usage: $0 [-f filename]"; exit 1 ;;
    esac
done

# Default to main.ino if no file is specified



# Check if arduino-cli is installed
if ! command -v $ARDUINO_CLI &> /dev/null; then
    echo "Error: arduino-cli is not installed. Please install it first."
    echo "Visit: https://arduino.github.io/arduino-cli"
    exit 1
fi

# Check if the specified .ino file exists
if [ ! -f "$INO_FILE" ]; then
    echo "Error: File $INO_FILE not found."
    exit 1
fi

# Remove the file extension
BASE_FILE=$(basename "$INO_FILE" ".ino")
DIR_NAME=$(dirname "$INO_FILE")

echo "Building ${DIR_NAME}/${BASE_FILE}"


# Compile the .ino file using arduino-cli
$ARDUINO_CLI compile --fqbn arduino:avr:uno "$INO_FILE" --build-path ./build/"$BASE_FILE"
if [ $? -ne 0 ]; then
    echo "Error: Compilation failed."
    exit 1
fi

echo "Build successful!"
echo "Generated files are located in the build directory."

echo "Done."