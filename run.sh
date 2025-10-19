#!/bin/bash
set -e

# Build Rust library first
cd rusty
cargo build --release
cd ..

# Get the Rust library name from Cargo.toml
LIB_NAME="rusty"

# Compile C++ sources and link Rust library
g++ init.cpp src/*.cpp -Iinclude -Lrusty/target/release -l${LIB_NAME} -fopenmp -ldl -o a.out

# Run the program
LD_LIBRARY_PATH=rusty/target/release:$LD_LIBRARY_PATH ./a.out
