# Source files
set(CORE_SOURCES
    core/engine.c
)

set(DOM_SOURCES
    dom/dom.c
)

set(JS_SOURCES
    js/js_engine.c
)

set(RENDERING_SOURCES
    rendering/renderer.c
)

set(HTML_SOURCES
    html/parser.c
)

# Combine all sources
set(ALL_SOURCES
    ${CORE_SOURCES}
    ${DOM_SOURCES}
    ${JS_SOURCES}
    ${RENDERING_SOURCES}
    ${HTML_SOURCES}
)

# Create the main library
add_library(just-browse-core ${ALL_SOURCES})

target_include_directories(just-browse-core PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# Link QuickJS
target_link_libraries(just-browse-core PUBLIC quickjs)

# Platform-specific linking
if(BUILD_WASM)
    # WASM-specific settings
    target_compile_definitions(just-browse-core PUBLIC BUILD_WASM=1)
else()
    # Native build settings
    target_link_libraries(just-browse-core PUBLIC m pthread dl)
endif()

# Create main executable for testing
add_executable(just-browse-test
    main.c
)

target_link_libraries(just-browse-test
    just-browse-core
)
