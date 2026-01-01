#include "core/engine.h"
#include "dom/dom.h"
#include "html/parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void test_html_parsing() {
    printf("Testing HTML parsing...\n");
    
    BrowserEngine* engine = browser_engine_init();
    assert(engine != NULL);

    const char* html = 
        "<!DOCTYPE html>"
        "<html>"
        "<head><title>Test Page</title></head>"
        "<body>"
        "  <div id=\"container\" class=\"main\">"
        "    <h1>Hello World</h1>"
        "    <p>This is a paragraph</p>"
        "    <div id=\"nested\">Nested content</div>"
        "  </div>"
        "</body>"
        "</html>";

    assert(browser_engine_load_html(engine, html) == 0);
    
    // Test that we can find elements by ID using JavaScript
    const char* script = "var div = document.getElementById('container'); console.log('Found element:', div);";
    assert(browser_engine_execute_script(engine, script) == 0);

    browser_engine_destroy(engine);
    printf("  PASSED\n");
}

void test_dom_manipulation_from_js() {
    printf("Testing DOM manipulation from JavaScript...\n");
    
    BrowserEngine* engine = browser_engine_init();
    assert(engine != NULL);

    const char* html = "<html><body><div id=\"app\"></div></body></html>";
    assert(browser_engine_load_html(engine, html) == 0);

    // Create elements and modify DOM from JavaScript
    const char* script = 
        "var app = document.getElementById('app');"
        "app.setAttribute('data-loaded', 'true');"
        "var heading = document.createElement('h1');"
        "heading.innerHTML = 'Dynamic Content';"
        "heading.setAttribute('class', 'title');"
        "console.log('Created and configured element');";
        
    assert(browser_engine_execute_script(engine, script) == 0);

    browser_engine_destroy(engine);
    printf("  PASSED\n");
}

void test_complex_html() {
    printf("Testing complex HTML structure...\n");
    
    BrowserEngine* engine = browser_engine_init();
    assert(engine != NULL);

    const char* html = 
        "<html>"
        "<body>"
        "  <nav>"
        "    <ul>"
        "      <li><a href=\"#\">Home</a></li>"
        "      <li><a href=\"#\">About</a></li>"
        "    </ul>"
        "  </nav>"
        "  <main>"
        "    <article id=\"post-1\">"
        "      <h2>Article Title</h2>"
        "      <p>Article content goes here.</p>"
        "    </article>"
        "  </main>"
        "</body>"
        "</html>";

    assert(browser_engine_load_html(engine, html) == 0);
    
    const char* script = 
        "var article = document.getElementById('post-1');"
        "if (article) {"
        "  console.log('Found article element');"
        "  article.setAttribute('data-views', '100');"
        "}";
        
    assert(browser_engine_execute_script(engine, script) == 0);

    browser_engine_destroy(engine);
    printf("  PASSED\n");
}

int main() {
    printf("Running HTML Parser tests...\n\n");

    test_html_parsing();
    test_dom_manipulation_from_js();
    test_complex_html();

    printf("\nAll HTML Parser tests passed!\n");
    return 0;
}
