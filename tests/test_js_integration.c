#include "core/engine.h"
#include "dom/dom.h"
#include "js/js_engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void test_js_dom_integration() {
    printf("Testing JavaScript-DOM integration...\n");
    
    BrowserEngine* engine = browser_engine_init();
    assert(engine != NULL);

    // Test createElement from JavaScript
    const char* script1 = "var div = document.createElement('div'); div.setAttribute('id', 'test-div');";
    assert(browser_engine_execute_script(engine, script1) == 0);
    
    // Test setting innerHTML
    const char* script2 = "div.innerHTML = 'Hello from JS!';";
    assert(browser_engine_execute_script(engine, script2) == 0);
    
    // Test variables and console.log
    const char* script3 = "var x = 5 + 3; console.log('Result:', x);";
    assert(browser_engine_execute_script(engine, script3) == 0);

    browser_engine_destroy(engine);
    printf("  PASSED\n");
}

void test_js_errors() {
    printf("Testing JavaScript error handling...\n");
    
    BrowserEngine* engine = browser_engine_init();
    assert(engine != NULL);

    // Intentionally cause an error
    const char* bad_script = "this is not valid javascript!!!";
    int result = browser_engine_execute_script(engine, bad_script);
    assert(result != 0);  // Should fail
    
    printf("  Error (expected): JavaScript error detected\n");

    browser_engine_destroy(engine);
    printf("  PASSED\n");
}

int main() {
    printf("Running JavaScript Integration tests...\n\n");

    test_js_dom_integration();
    test_js_errors();

    printf("\nAll JavaScript Integration tests passed!\n");
    return 0;
}
