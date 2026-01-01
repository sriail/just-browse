# Just-Browse

A lightweight browser engine designed for WebAssembly, built from scratch with modular components.

## Overview

Just-Browse is a basic browser engine that integrates multiple systems to provide core web browsing functionality in a WASM-compatible format. It's designed to be lightweight and modular, with clear separation between DOM, JavaScript execution, and rendering.

## Tech Stack

- **QuickJS** (~500KB): JavaScript engine for script execution
- **RmlUi** (~2-5MB): UI/rendering library for CSS and layout
- **wasm-image-decoder** (~500KB): Image decoding support
- **freetype-wasm** (~1-2MB): Font rendering
- **harfbuzzjs** (~500KB-1MB): Text shaping and layout

**Total estimated size: 5-10MB compressed**

## Features

### Current Implementation
- ✅ Basic DOM API (Document, Element, Node)
- ✅ DOM tree manipulation
- ✅ Element attributes management
- ✅ JavaScript engine integration layer
- ✅ Rendering system foundation
- ✅ WASM build support via Emscripten
- ✅ Test infrastructure

### Architecture
- **Core Engine**: Coordinates all subsystems (DOM, JS, rendering)
- **DOM System**: Basic Document Object Model implementation
- **JavaScript Engine**: QuickJS integration for script execution
- **Rendering System**: RmlUi-based rendering with font and image support

See [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) for detailed architecture documentation.

## Quick Start

### Prerequisites
- CMake 3.15+
- C/C++ compiler (GCC, Clang, or MSVC)
- Emscripten SDK (for WASM builds)
- Node.js (optional, for npm scripts)

### Building

```bash
# Native build
mkdir build && cd build
cmake ..
make

# Or use npm
npm run build
```

### WebAssembly Build

```bash
# Make sure Emscripten is installed and sourced
source /path/to/emsdk/emsdk_env.sh

# Build for WASM
npm run build:wasm
```

See [docs/BUILD.md](docs/BUILD.md) for detailed build instructions.

### Running Tests

```bash
# Run all tests
npm test

# Or manually
cd build
ctest --output-on-failure
```

### Running the Demo

```bash
# After building
./build/src/just-browse-test
```

## Project Structure

```
just-browse/
├── include/        # Public API headers
├── src/            # Implementation
│   ├── core/       # Browser engine core
│   ├── dom/        # DOM implementation
│   ├── js/         # JavaScript engine
│   └── rendering/  # Rendering system
├── tests/          # Unit tests
├── examples/       # Example HTML/JS files
└── docs/           # Documentation
```

## API Example

```c
#include "core/engine.h"

// Initialize the engine
BrowserEngine* engine = browser_engine_init();

// Load HTML content
browser_engine_load_html(engine, 
    "<html><body><h1>Hello World</h1></body></html>");

// Execute JavaScript
browser_engine_execute_script(engine, 
    "console.log('Hello from JS!');");

// Render the page
browser_engine_render(engine);

// Clean up
browser_engine_destroy(engine);
```

## Integration Status

| Component | Status | Description |
|-----------|--------|-------------|
| Core Engine | ✅ Complete | Main browser engine coordination |
| DOM API | ✅ Basic | Element creation, attributes, tree manipulation |
| QuickJS | 🔄 Stub | JavaScript engine (needs integration) |
| RmlUi | 🔄 Stub | Rendering system (needs integration) |
| FreeType | ⏳ Planned | Font rendering |
| HarfBuzz | ⏳ Planned | Text shaping |
| Image Decoder | ⏳ Planned | Image support |
| WASM Build | ✅ Ready | Build system configured |

## Next Steps

1. **Integrate QuickJS**: Replace stub with actual QuickJS library
2. **Integrate RmlUi**: Replace stub with actual RmlUi rendering
3. **Add Font Support**: Integrate FreeType and HarfBuzz
4. **Add Image Support**: Integrate wasm-image-decoder
5. **Improve DOM**: Extend DOM API for better compatibility
6. **CSS Support**: Full CSS parsing and styling
7. **Event System**: DOM event handling
8. **Optimize**: Profile and optimize for size/performance

## Development

### Adding Features
1. Design the API in the appropriate header file (`include/`)
2. Implement in the corresponding source file (`src/`)
3. Add tests in `tests/`
4. Update documentation

### Testing
All features should have corresponding tests. Run tests frequently during development:

```bash
npm test
```

## Documentation

- [Architecture](docs/ARCHITECTURE.md) - System design and component overview
- [Build Guide](docs/BUILD.md) - Detailed build instructions

## License

MIT

## Contributing

Contributions are welcome! This is a learning project focused on understanding browser engine internals and WebAssembly integration.

## Goals

This project aims to:
- Create a minimal but functional browser engine
- Demonstrate WASM portability of complex C/C++ systems
- Provide a foundation for understanding browser internals
- Achieve reasonable bundle size (5-10MB) for web deployment
