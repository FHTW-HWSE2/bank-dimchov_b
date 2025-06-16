#!/bin/bash

# Exit on error
set -e

# Create build directory if not exists
mkdir -p build
cd build

# Run cmake to configure the project
cmake ..

# Compile the project
make

echo "Build completed successfully :D"

# Run the application
./bank

