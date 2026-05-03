# RUI Build Instructions

## Fixed Issues

### 1. FFI Safety Warnings
- Added `#[repr(C)]` to `Rgb` and `VesaInfo` structs
- Updated `ipc_recv` function signature to use pointers instead of tuples

### 2. x86_64 Instructions Feature
- Added `features = ["instructions"]` to x86_64 dependencies in all crates
- This enables the `hlt` instruction on x86_64 targets

### 3. Missing Dependencies
- Added `linked_list_allocator` dependency to wm crate
- Fixed workspace dependency configuration

### 4. Toolchain Issues
- Set up nightly toolchain with `rust-toolchain.toml`
- Configured `.cargo/config.toml` for no_std builds with build-std

### 5. Code Quality Issues
- Fixed unused variable warnings by prefixing with `_`
- Fixed mutable static reference warnings using raw pointers
- Removed unnecessary `main.rs` from common crate

## Building the Project

### Option 1: Use the build script (Recommended)
```bash
./build.sh build
./build.sh check
./build.sh clean
```

### Option 2: Manual build with correct PATH
```bash
export PATH="$HOME/.cargo/bin:$PATH"
cargo build
```

### Option 3: If you want to permanently fix your PATH
Add this to your `~/.zshrc` or `~/.bashrc`:
```bash
export PATH="$HOME/.cargo/bin:$PATH"
```

## Project Structure
- `common/` - Shared types and kernel API
- `render/` - Rendering service
- `wm/` - Window manager
- `gpu-driver/` - GPU driver
- `rui-lib/` - UI widget library
- `apps/test-window/` - Test application

## Target Architecture
The project is configured to build for `x86_64-unknown-none` (bare metal x86_64) using nightly Rust with `build-std`.