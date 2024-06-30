#!/bin/bash

# Compile the C++ program
make

run_graph() {
    local commands=$1
    ./graph << EOL
$commands
EOL
}

# Run
echo "Running graph..."
run_graph "5 5
1 2
2 3
3 1
4 3
4 5"

