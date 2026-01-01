#include "core/engine.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    printf("Just-Browse Engine Test\n");
    printf("========================\n\n");

    // Initialize the browser engine
    printf("Initializing browser engine...\n");
    BrowserEngine* engine = browser_engine_init();
    if (!engine) {
        fprintf(stderr, "Failed to initialize browser engine\n");
        return 1;
    }
    printf("Engine initialized successfully\n\n");

    // Load some HTML
    const char* html = "<html><body><h1>Hello, World!</h1><p>This is a test page.</p></body></html>";
    printf("Loading HTML...\n");
    if (browser_engine_load_html(engine, html) != 0) {
        fprintf(stderr, "Failed to load HTML\n");
        browser_engine_destroy(engine);
        return 1;
    }
    printf("HTML loaded successfully\n\n");

    // Execute some JavaScript
    const char* script = "console.log('Hello from JavaScript!');";
    printf("Executing JavaScript...\n");
    if (browser_engine_execute_script(engine, script) != 0) {
        fprintf(stderr, "Failed to execute JavaScript\n");
        browser_engine_destroy(engine);
        return 1;
    }
    printf("JavaScript executed successfully\n\n");

    // Render the page
    printf("Rendering page...\n");
    if (browser_engine_render(engine) != 0) {
        fprintf(stderr, "Failed to render page\n");
        browser_engine_destroy(engine);
        return 1;
    }
    printf("Page rendered successfully\n\n");

    // Clean up
    printf("Destroying engine...\n");
    browser_engine_destroy(engine);
    printf("Engine destroyed successfully\n\n");

    printf("All tests passed!\n");
    return 0;
}
