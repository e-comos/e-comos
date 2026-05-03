#!/bin/bash
# Build script for RUI project
# This ensures we use rustup's cargo instead of Homebrew's

export PATH="$HOME/.cargo/bin:$PATH"
cargo "$@"
