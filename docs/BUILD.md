# Building Just-Browse

## Prerequisites

### For Native Build
- CMake 3.15 or higher
- C/C++ compiler (GCC, Clang, or MSVC)
- Make or Ninja

### For WebAssembly Build
- Emscripten SDK
- Node.js (for npm scripts)

## Installing Emscripten

```bash
# Clone the emsdk repository
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk

# Install and activate the latest SDK
./emsdk install latest
./emsdk activate latest

# Set up environment variables
source ./emsdk_env.sh
```

## Building

### Native Build

```bash
# Build the project
npm run build

# Or manually:
mkdir build
cd build
cmake ..
make
```

### WebAssembly Build

```bash
# Build for WASM
npm run build:wasm

# Or manually:
mkdir build-wasm
cd build-wasm
emcmake cmake .. -DBUILD_WASM=ON
emmake make
```

## Running Tests

```bash
# After native build
npm test

# Or manually:
cd build
ctest --output-on-failure
```

## Running the Test Application

After building, you can run the test application:

```bash
# Native
./build/src/just-browse-test

# WASM (requires a web server)
# The output will be in build-wasm/src/just-browse-test.js
```

## Project Structure

```
just-browse/
├── CMakeLists.txt          # Main build configuration
├── package.json            # NPM scripts and metadata
├── include/                # Public headers
│   ├── core/              # Core engine headers
│   ├── dom/               # DOM headers
│   ├── js/                # JavaScript engine headers
│   └── rendering/         # Rendering headers
├── src/                    # Implementation files
│   ├── core/              # Core engine implementation
│   ├── dom/               # DOM implementation
│   ├── js/                # JavaScript engine implementation
│   ├── rendering/         # Rendering implementation
│   ├── main.c             # Test application
│   └── CMakeLists.txt     # Source build configuration
├── tests/                  # Unit tests
│   ├── test_dom.c
│   ├── test_engine.c
│   └── CMakeLists.txt
├── examples/              # Example HTML/JS files
├── docs/                  # Documentation
└── README.md              # This file
```

## Troubleshooting

### CMake can't find compiler
Make sure you have a C/C++ compiler installed:
- Linux: `sudo apt-get install build-essential`
- macOS: `xcode-select --install`
- Windows: Install Visual Studio or MinGW

### Emscripten not found
Make sure you've sourced the Emscripten environment:
```bash
source /path/to/emsdk/emsdk_env.sh
```

### Tests failing
Make sure you've built the project first:
```bash
npm run build
npm test
```
