# WebAssembly Integration Guide

This guide explains how to build Just-Browse for WebAssembly and use it in a web application.

## Prerequisites

1. **Emscripten SDK**: Install and activate
   ```bash
   git clone https://github.com/emscripten-core/emsdk.git
   cd emsdk
   ./emsdk install latest
   ./emsdk activate latest
   source ./emsdk_env.sh
   ```

2. **CMake**: Version 3.15 or higher

3. **Node.js**: For running npm scripts and testing

## Building for WebAssembly

### Basic Build

```bash
# From the project root
npm run build:wasm

# Or manually
mkdir build-wasm
cd build-wasm
emcmake cmake .. -DBUILD_WASM=ON
emmake make
```

### Build Output

The build produces:
- `just-browse-test.js` - JavaScript loader
- `just-browse-test.wasm` - WebAssembly binary
- `just-browse-core.a` - Static library (for linking)

## Using in a Web Page

### Basic Integration

```html
<!DOCTYPE html>
<html>
<head>
    <title>Just-Browse WASM Demo</title>
</head>
<body>
    <canvas id="browser-canvas"></canvas>
    <script src="just-browse-test.js"></script>
    <script>
        JustBrowse().then(module => {
            console.log('Just-Browse WASM loaded!');
            
            // Initialize the engine
            const engine = module.ccall(
                'browser_engine_init',
                'number',
                [],
                []
            );
            
            // Load HTML
            const html = '<html><body><h1>Hello WASM!</h1></body></html>';
            module.ccall(
                'browser_engine_load_html',
                'number',
                ['number', 'string'],
                [engine, html]
            );
            
            // Render
            module.ccall(
                'browser_engine_render',
                'number',
                ['number'],
                [engine]
            );
            
            // Clean up
            module.ccall(
                'browser_engine_destroy',
                'void',
                ['number'],
                [engine]
            );
        });
    </script>
</body>
</html>
```

## Emscripten Build Options

### Optimization Levels

```bash
# Debug build (larger, with symbols)
emcmake cmake .. -DBUILD_WASM=ON -DCMAKE_BUILD_TYPE=Debug

# Release build (optimized for size)
emcmake cmake .. -DBUILD_WASM=ON -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_C_FLAGS="-Os" -DCMAKE_CXX_FLAGS="-Os"

# Release build (optimized for speed)
emcmake cmake .. -DBUILD_WASM=ON -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_C_FLAGS="-O3" -DCMAKE_CXX_FLAGS="-O3"
```

### Memory Options

```cmake
# In CMakeLists.txt, add these link options:
-sINITIAL_MEMORY=16MB      # Starting memory
-sMAXIMUM_MEMORY=512MB     # Max memory (if not using ALLOW_MEMORY_GROWTH)
-sALLOW_MEMORY_GROWTH=1    # Allow dynamic growth
-sSTACK_SIZE=5MB           # Stack size
```

### File System Support

If you need to load external resources:

```cmake
add_link_options(
    -sNO_FILESYSTEM=0      # Enable filesystem
    --preload-file resources@/
)
```

## Exported Functions

To expose functions to JavaScript, update the Emscripten link options:

```cmake
-sEXPORTED_FUNCTIONS=[
    '_browser_engine_init',
    '_browser_engine_destroy',
    '_browser_engine_load_html',
    '_browser_engine_execute_script',
    '_browser_engine_render',
    '_malloc',
    '_free'
]
```

## Rendering to Canvas

### Option 1: Get Pixel Buffer

```javascript
JustBrowse().then(module => {
    const engine = module._browser_engine_init();
    
    // Render
    module._browser_engine_render(engine);
    
    // Get the pixel buffer
    const widthPtr = module._malloc(4);
    const heightPtr = module._malloc(4);
    const bufferPtr = module.ccall(
        'renderer_get_buffer',
        'number',
        ['number', 'number', 'number'],
        [engine, widthPtr, heightPtr]
    );
    
    const width = module.getValue(widthPtr, 'i32');
    const height = module.getValue(heightPtr, 'i32');
    
    // Copy to canvas
    const canvas = document.getElementById('browser-canvas');
    canvas.width = width;
    canvas.height = height;
    const ctx = canvas.getContext('2d');
    const imageData = ctx.createImageData(width, height);
    
    const pixels = new Uint8Array(
        module.HEAPU8.buffer,
        bufferPtr,
        width * height * 4
    );
    imageData.data.set(pixels);
    ctx.putImageData(imageData, 0, 0);
    
    // Clean up
    module._free(widthPtr);
    module._free(heightPtr);
    module._browser_engine_destroy(engine);
});
```

### Option 2: Direct Canvas Rendering

When RmlUi is integrated, you can render directly to a WebGL canvas:

```cmake
# Add WebGL support
add_link_options(
    -sUSE_WEBGL2=1
    -sFULL_ES3=1
)
```

## Performance Considerations

### 1. Minimize JavaScript/WASM Calls
- Batch operations when possible
- Use shared memory for large data transfers

### 2. Memory Management
- Reuse allocated buffers
- Clean up resources properly
- Use memory pools for frequent allocations

### 3. Threading
```cmake
# Enable threading support
add_link_options(
    -pthread
    -sPTHREAD_POOL_SIZE=4
)
```

### 4. SIMD Support
```cmake
# Enable SIMD for better performance
add_compile_options(-msimd128)
```

## Debugging WASM

### Source Maps

```cmake
# Enable source maps for debugging
add_link_options(-gsource-map)
```

### Browser DevTools

1. Chrome/Edge: DevTools → Sources → WASM modules
2. Firefox: Debugger → WASM sources
3. Set breakpoints in C code
4. Inspect variables

### Logging

```c
#ifdef BUILD_WASM
#include <emscripten.h>
#define LOG(...) emscripten_log(EM_LOG_CONSOLE, __VA_ARGS__)
#else
#define LOG(...) printf(__VA_ARGS__)
#endif
```

## Testing WASM Build

### Local Server

```bash
# Python 3
python3 -m http.server 8000

# Node.js
npx http-server

# Then open http://localhost:8000
```

### Automated Testing

```javascript
// test.js
const Module = require('./just-browse-test.js');

Module().then(module => {
    const engine = module._browser_engine_init();
    if (!engine) {
        console.error('Failed to initialize engine');
        process.exit(1);
    }
    
    console.log('Engine initialized successfully');
    module._browser_engine_destroy(engine);
    console.log('All tests passed!');
});
```

## Bundle Size Optimization

### 1. Link Time Optimization (LTO)

```cmake
add_compile_options(-flto)
add_link_options(-flto)
```

### 2. Strip Unused Code

```cmake
add_link_options(
    -sELIMINATE_DUPLICATE_FUNCTIONS=1
    -sDEAD_FUNCTIONS=['unused_func1', 'unused_func2']
)
```

### 3. Compression

```bash
# After build, compress with Brotli
brotli -9 just-browse-test.wasm

# Serve with appropriate headers:
# Content-Encoding: br
```

### 4. Code Splitting

For large applications, consider splitting into modules:

```cmake
add_link_options(
    -sSIDE_MODULE=1  # For sub-modules
)
```

## Common Issues

### Memory Access Errors
- Check pointer validity before use
- Ensure allocated memory is not freed prematurely
- Use AddressSanitizer in debug builds

### Async Loading
- WASM modules load asynchronously
- Always use `.then()` or `async/await`

### Cross-Origin Issues
- Serve WASM files from same origin
- Or configure CORS headers properly

## Production Deployment

### CDN Serving

```html
<!-- Preload WASM for faster loading -->
<link rel="preload" href="just-browse-test.wasm" as="fetch" crossorigin>

<!-- Load the JavaScript module -->
<script src="just-browse-test.js" async></script>
```

### Caching

Configure appropriate cache headers:
```
Cache-Control: public, max-age=31536000, immutable
```

### Loading States

```javascript
const loadingDiv = document.getElementById('loading');
const contentDiv = document.getElementById('content');

JustBrowse({
    onRuntimeInitialized: () => {
        loadingDiv.style.display = 'none';
        contentDiv.style.display = 'block';
        console.log('WASM runtime ready');
    }
}).then(module => {
    // Use module
});
```

## Next Steps

1. Build for WASM: `npm run build:wasm`
2. Create a test HTML page
3. Serve locally and test
4. Optimize for production
5. Deploy to CDN

For more information, see:
- [Emscripten Documentation](https://emscripten.org/docs/)
- [WebAssembly.org](https://webassembly.org/)
- [docs/ARCHITECTURE.md](ARCHITECTURE.md)
