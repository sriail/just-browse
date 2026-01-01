# Integration Roadmap

This document outlines the steps to integrate actual components into the Just-Browse engine.

## Phase 1: QuickJS Integration (PRIORITY)

### Current Status
- ✅ Stub implementation in `src/js/js_engine.c`
- ✅ API design complete
- ⏳ Needs actual QuickJS library integration

### Integration Steps

1. **Add QuickJS as dependency**
   ```bash
   # Option A: Git submodule
   git submodule add https://github.com/bellard/quickjs.git third_party/quickjs
   
   # Option B: Download and extract
   wget https://bellard.org/quickjs/quickjs-2021-03-27.tar.xz
   tar xf quickjs-2021-03-27.tar.xz -C third_party/
   ```

2. **Update CMakeLists.txt**
   - Add QuickJS source directory
   - Link QuickJS library
   - Set include paths

3. **Implement QuickJS bindings in js_engine.c**
   - Replace stubs with actual QuickJS calls
   - Initialize runtime: `JS_NewRuntime()`
   - Create context: `JS_NewContext()`
   - Implement eval: `JS_Eval()`

4. **Create DOM bindings**
   - Define JavaScript classes for DOM objects
   - Bind Document methods (createElement, getElementById, etc.)
   - Bind Element methods (setAttribute, getAttribute, etc.)
   - Create global `document` object

5. **Test integration**
   - Execute real JavaScript code
   - Verify DOM manipulation from JavaScript
   - Add integration tests

### Example Code Structure
```c
#include "quickjs.h"

JSEngine* js_engine_init(void) {
    JSEngine* engine = calloc(1, sizeof(JSEngine));
    engine->runtime = JS_NewRuntime();
    engine->context = JS_NewContext(engine->runtime);
    return engine;
}

int js_engine_eval(JSEngine* engine, const char* script) {
    JSValue result = JS_Eval(engine->context, script, 
                             strlen(script), "<input>", 
                             JS_EVAL_TYPE_GLOBAL);
    if (JS_IsException(result)) {
        // Handle error
        return -1;
    }
    JS_FreeValue(engine->context, result);
    return 0;
}
```

## Phase 2: RmlUi Integration

### Current Status
- ✅ Stub implementation in `src/rendering/renderer.c`
- ✅ API design complete
- ⏳ Needs actual RmlUi library integration

### Integration Steps

1. **Add RmlUi as dependency**
   ```bash
   git submodule add https://github.com/mikke89/RmlUi.git third_party/RmlUi
   ```

2. **Update build system**
   - Add RmlUi to CMakeLists.txt
   - Configure rendering backend (Software rasterizer for WASM)
   - Link RmlUi libraries

3. **Implement RmlUi integration**
   - Initialize RmlUi context
   - Create render interface
   - Implement DOM-to-RmlUi conversion
   - Handle CSS styling
   - Implement layout engine

4. **Test rendering**
   - Render simple HTML elements
   - Test CSS styling
   - Verify layout calculations

## Phase 3: Font Support (FreeType + HarfBuzz)

### Integration Steps

1. **Add FreeType**
   - For native: use system FreeType
   - For WASM: integrate freetype-wasm
   ```bash
   git submodule add https://github.com/DouglasUrner/freetype-wasm.git third_party/freetype-wasm
   ```

2. **Add HarfBuzz**
   - For native: use system HarfBuzz
   - For WASM: integrate harfbuzzjs
   ```bash
   npm install harfbuzz-wasm
   ```

3. **Integrate with RmlUi**
   - Implement font loading
   - Create font interface for RmlUi
   - Handle text shaping

## Phase 4: Image Support

### Integration Steps

1. **Add wasm-image-decoder**
   ```bash
   npm install wasm-image-decoder
   ```

2. **Implement image loading**
   - Create image decode interface
   - Support common formats (PNG, JPEG, GIF)
   - Integrate with rendering system

3. **Test image rendering**
   - Load and display images
   - Handle image sizing and scaling

## Phase 5: Improve DOM Compatibility

### Enhancements Needed

1. **HTML Parsing**
   - Integrate HTML parser (e.g., gumbo-parser)
   - Convert parsed HTML to DOM tree
   - Handle malformed HTML

2. **DOM API Extensions**
   - querySelector/querySelectorAll
   - classList manipulation
   - Event listeners (addEventListener)
   - More node manipulation methods

3. **Event System**
   - Event propagation (bubbling/capturing)
   - Event object implementation
   - Mouse/keyboard events
   - Custom events

## Phase 6: CSS Support

### Integration Steps

1. **CSS Parser**
   - Integrate CSS parser library
   - Parse inline styles
   - Parse style elements
   - Parse external stylesheets (future)

2. **Style Computation**
   - Cascade and specificity
   - Inheritance
   - Style resolution

3. **Layout Integration**
   - Connect CSS to RmlUi layout engine
   - Handle box model
   - Positioning (static, relative, absolute, fixed)
   - Flexbox/Grid (if supported by RmlUi)

## Phase 7: WASM Optimization

### Optimization Tasks

1. **Size Optimization**
   - Enable compiler optimizations
   - Strip debug symbols
   - Use code splitting if possible
   - Compress output with Brotli

2. **Performance Optimization**
   - Profile WASM execution
   - Optimize hot paths
   - Minimize memory allocations
   - Use SIMD where applicable

3. **Memory Management**
   - Implement efficient memory pooling
   - Handle garbage collection for JS objects
   - Optimize DOM tree memory usage

## Testing Strategy

### Unit Tests
- Test each component in isolation
- Mock dependencies for focused testing
- Aim for high code coverage

### Integration Tests
- Test component interactions
- End-to-end scenarios
- Real HTML/CSS/JS examples

### Performance Tests
- Measure rendering performance
- Track memory usage
- Profile JavaScript execution

### WASM Tests
- Test in browser environment
- Verify all APIs work in WASM
- Check bundle size

## Dependencies Summary

| Component | Repository | License | Size |
|-----------|-----------|---------|------|
| QuickJS | https://github.com/bellard/quickjs | MIT | ~500KB |
| RmlUi | https://github.com/mikke89/RmlUi | MIT | 2-5MB |
| FreeType | https://github.com/freetype/freetype | FreeType License | 1-2MB |
| HarfBuzz | https://github.com/harfbuzz/harfbuzz | MIT | 500KB-1MB |
| wasm-image-decoder | npm package | - | ~500KB |

## Timeline Estimate

- Phase 1 (QuickJS): 1-2 weeks
- Phase 2 (RmlUi): 2-3 weeks
- Phase 3 (Fonts): 1-2 weeks
- Phase 4 (Images): 1 week
- Phase 5 (DOM): 2-3 weeks
- Phase 6 (CSS): 2-3 weeks
- Phase 7 (Optimization): 1-2 weeks

**Total: 10-16 weeks for full integration**

## Next Immediate Steps

1. ✅ Set up basic project structure
2. ✅ Implement stub APIs
3. ✅ Create build system
4. ✅ Add basic tests
5. ⏭️ **Integrate QuickJS** (START HERE)
6. ⏭️ Integrate RmlUi
7. ⏭️ Add font and image support
8. ⏭️ Improve DOM compatibility
9. ⏭️ Test and optimize for WASM
