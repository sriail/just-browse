#include "core/engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void test_engine_init() {
    printf("Testing engine initialization...\n");
    BrowserEngine* engine = browser_engine_init();
    assert(engine != NULL);
    browser_engine_destroy(engine);
    printf("  PASSED\n");
}

void test_load_html() {
    printf("Testing HTML loading...\n");
    BrowserEngine* engine = browser_engine_init();
    assert(engine != NULL);

    const char* html = "<html><body><h1>Test</h1></body></html>";
    assert(browser_engine_load_html(engine, html) == 0);

    browser_engine_destroy(engine);
    printf("  PASSED\n");
}

void test_execute_script() {
    printf("Testing JavaScript execution...\n");
    BrowserEngine* engine = browser_engine_init();
    assert(engine != NULL);

    const char* script = "var x = 5 + 3;";
    assert(browser_engine_execute_script(engine, script) == 0);

    browser_engine_destroy(engine);
    printf("  PASSED\n");
}

void test_render() {
    printf("Testing rendering...\n");
    BrowserEngine* engine = browser_engine_init();
    assert(engine != NULL);

    const char* html = "<html><body><p>Test</p></body></html>";
    assert(browser_engine_load_html(engine, html) == 0);
    assert(browser_engine_render(engine) == 0);

    browser_engine_destroy(engine);
    printf("  PASSED\n");
}

int main() {
    printf("Running Engine tests...\n\n");

    test_engine_init();
    test_load_html();
    test_execute_script();
    test_render();

    printf("\nAll Engine tests passed!\n");
    return 0;
}
