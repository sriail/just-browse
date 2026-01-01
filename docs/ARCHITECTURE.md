# Just-Browse Architecture

## Overview

Just-Browse is a lightweight browser engine designed to run in WebAssembly. It integrates several key components to provide basic web browsing capabilities.

## Components

### 1. Core Engine (`src/core/engine.c`)
The main browser engine that coordinates all subsystems:
- Initializes and manages DOM, JavaScript, and rendering components
- Provides the main API for loading HTML and executing scripts
- Handles the rendering pipeline

### 2. DOM System (`src/dom/dom.c`)
Basic Document Object Model implementation:
- Supports element creation and manipulation
- Attribute management
- Tree structure for nodes
- Basic HTML parsing (simplified)

**Note:** This is not a full DOM implementation. It provides essential functionality for:
- Document creation
- Element creation and attributes
- Basic tree manipulation
- innerHTML support

### 3. JavaScript Engine (`src/js/js_engine.c`)
QuickJS integration layer:
- Currently a stub implementation
- Will integrate actual QuickJS for JavaScript execution
- Provides bindings between JavaScript and DOM
- Event handling infrastructure

**TODO:** Replace stub with actual QuickJS integration.

### 4. Rendering System (`src/rendering/renderer.c`)
RmlUi-based rendering:
- Currently a stub implementation
- Will integrate RmlUi for CSS and layout
- Uses FreeType for font rendering
- Uses HarfBuzz for text shaping
- Uses wasm-image-decoder for image support

**TODO:** Replace stub with actual RmlUi integration.

## Component Integration Status

### Completed
- ✅ Basic project structure
- ✅ Core engine API design
- ✅ DOM tree implementation
- ✅ Build system (CMake)
- ✅ Test infrastructure

### In Progress
- 🔄 QuickJS integration (stub)
- 🔄 RmlUi integration (stub)
- 🔄 Font rendering (FreeType/HarfBuzz)
- 🔄 Image decoding

### Planned
- ⏳ Full DOM API compatibility
- ⏳ CSS parser and styling
- ⏳ Layout engine
- ⏳ Event system
- ⏳ Network layer for resource loading
- ⏳ WebAssembly optimization

## WebAssembly Build

The engine is designed to compile to WebAssembly using Emscripten:

```bash
npm run build:wasm
```

This will:
1. Use Emscripten toolchain
2. Compile all C/C++ sources to WASM
3. Generate JavaScript bindings
4. Export the browser engine API

## Size Estimates

- QuickJS: ~500KB
- RmlUi: ~2-5MB
- wasm-image-decoder: ~500KB
- freetype-wasm: ~1-2MB
- harfbuzzjs: ~500KB-1MB
- **Total: 5-10MB compressed**

## API Usage

```c
// Initialize engine
BrowserEngine* engine = browser_engine_init();

// Load HTML
browser_engine_load_html(engine, "<html>...</html>");

// Execute JavaScript
browser_engine_execute_script(engine, "console.log('Hello');");

// Render
browser_engine_render(engine);

// Clean up
browser_engine_destroy(engine);
```

## Next Steps

1. **Integrate QuickJS**: Replace js_engine.c stub with actual QuickJS
2. **Integrate RmlUi**: Replace renderer.c stub with actual RmlUi
3. **Add Font Support**: Integrate FreeType and HarfBuzz
4. **Add Image Support**: Integrate wasm-image-decoder
5. **Improve DOM**: Extend DOM API for better compatibility
6. **Testing**: Comprehensive testing with real web content
7. **Optimization**: Profile and optimize for size and performance
