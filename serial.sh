#!/bin/bash
set -e

# Build Rust library first
cd rusty
cargo build --release
cd ..

# Compile C++ sources and link Rust library
g++ serial.cpp -fopenmp src/*.cpp -Iinclude -Lrusty/target/release -lrusty -ldl -o a.out

# Run the program
LD_LIBRARY_PATH=rusty/target/release:$LD_LIBRARY_PATH ./a.out
