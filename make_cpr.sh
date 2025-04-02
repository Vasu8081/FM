#!/bin/bash

# Define the installation directory inside the project root
INSTALL_DIR="$ROOT/libraries/cpr"

# Create a temporary build directory
mkdir -p cpr_build
cd cpr_build

# Clone CPR repository (shallow clone for faster download)
git clone --depth=1 https://github.com/libcpr/cpr.git
cd cpr

# Create build directory
mkdir build
cd build

# Run CMake with installation prefix
cmake .. -DCPR_BUILD_TESTS=OFF -DCPR_USE_SYSTEM_CURL=ON -DCMAKE_INSTALL_PREFIX="$INSTALL_DIR"
cmake --build . --config Release
cmake --install .

# Go back and clean up
cd ../..
rm -rf cpr_build

echo "CPR installed in $INSTALL_DIR"
