#!/bin/bash

# Compile the C++ program
g++ -std=c++17 -pthread client.cpp -o client

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. Running the program..."
else
    echo "Compilation failed. Exiting."
    exit 1
fi

# Run the program
./client localhost

