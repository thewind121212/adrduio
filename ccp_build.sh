#!/bin/bash

# Configuration
PROJECT_DIR=$(dirname "$0")
DEFAULT_FILE="main.c"
AVR_GCC="avr-gcc"

# Parse arguments
while getopts "f:" opt; do
    case $opt in
        f) CPP_FILE="$PROJECT_DIR/$OPTARG" ;;
        *) echo "Usage: $0 [-f filename]"; exit 1 ;;
    esac
done





# Check if avr-gcc is installed
if ! command -v $AVR_GCC &> /dev/null; then
    echo "Error: avr-gcc is not installed. Please install it first."
    echo "Visit: https://github.com/osx-cross/homebrew-avr"
    exit 1
fi

# Check if the specified .cpp file exists
if [ ! -f "$CPP_FILE" ]; then
    echo "Error: File $CPP_FILE not found."
    exit 1
fi





# remove the file extension
BASE_FILE=$(basename "$CPP_FILE" ".cpp")
DIR_NAME=$(dirname "$CPP_FILE")


echo "Building $DIR_NAME"

HEX_FILE="${DIR_NAME}/${BASE_FILE%}.hex"
ELF_FILE="${DIR_NAME}/${BASE_FILE%}.elf"



$AVR_GCC -mmcu=atmega328p -Os -DF_CPU=16000000UL -o "$ELF_FILE" "$CPP_FILE"
if [ $? -ne 0 ]; then
    echo "Error: Compilation failed to generate .elf file."
    exit 1
fi

avr-objcopy -O ihex -R .eeprom "$ELF_FILE" "$HEX_FILE"
if [ $? -ne 0 ]; then
    echo "Error: Failed to generate .hex file."
    exit 1
fi

echo "Build successful!"
echo "Generated files:"
echo "  ELF: $ELF_FILE"
echo "  HEX: $HEX_FILE"

echo "Done."
