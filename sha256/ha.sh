#!/bin/bash

INPUT_DIRECTORY="_files"
OUTPUT_FILE="output.txt"
CPP_EXECUTABLE="cpp/main"
PYTHON_SCRIPT="python/main.py"

# Compile C++ files with C++17 support
g++ -std=c++17 -o $CPP_EXECUTABLE cpp/sha256.cpp cpp/main.cpp

# Create tabular format header
echo "| Filename              | C++ Hash     | Python Hash  |" > "$OUTPUT_FILE"
echo "| --------------------- | ------------ | ------------- |" >> "$OUTPUT_FILE"

# Iterate through each file in the directory
for filepath in "$INPUT_DIRECTORY"/*; do
    echo "--> FilePath: $filepath"
    filename=$(basename "$filepath")
    echo "--> FileName: $filename"

    # Get the hash values from the C++ executable and Python script
    cpp_hash=$($CPP_EXECUTABLE "$filepath")
    python_hash=$(python3 $PYTHON_SCRIPT "$filepath")

    # Append the hash values to the output file
    printf "| %-20s | %-12s | %-13s |\n" "$filename" "$cpp_hash" "$python_hash" >> "$OUTPUT_FILE"
done

# Display the output file content
cat "$OUTPUT_FILE"
