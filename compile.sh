#!/bin/sh

# Delete build directory
if [ -d ./build ]; then
    rm -r ./build
fi

# Recreate build directory
mkdir build

# CMake
cmake -B ./build .

# Copy compile_commands.json from the build folder to the working directory
cp ./build/compile_commands.json ./compile_commands.json

cd build
make
