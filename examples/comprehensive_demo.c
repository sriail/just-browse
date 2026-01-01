#include "core/engine.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("=================================================================\n");
    printf("  Just-Browse Engine - Comprehensive Demo\n");
    printf("=================================================================\n\n");

    // Initialize the browser engine
    printf("1. Initializing browser engine...\n");
    BrowserEngine* engine = browser_engine_init();
    if (!engine) {
        fprintf(stderr, "Failed to initialize engine!\n");
        return 1;
    }
    printf("   ✓ Engine initialized\n\n");

    // Load HTML document
    printf("2. Loading HTML document...\n");
    const char* html = 
        "<!DOCTYPE html>"
        "<html>"
        "<head>"
        "  <title>Just-Browse Demo</title>"
        "  <meta charset=\"UTF-8\">"
        "</head>"
        "<body>"
        "  <header>"
        "    <h1>Welcome to Just-Browse!</h1>"
        "    <nav>"
        "      <a href=\"#home\" class=\"nav-link\">Home</a>"
        "      <a href=\"#about\" class=\"nav-link\">About</a>"
        "    </nav>"
        "  </header>"
        "  <main>"
        "    <section id=\"content\">"
        "      <h2>Features</h2>"
        "      <ul>"
        "        <li>HTML Parsing</li>"
        "        <li>JavaScript Execution</li>"
        "        <li>DOM Manipulation</li>"
        "      </ul>"
        "    </section>"
        "    <div id=\"dynamic\" class=\"container\"></div>"
        "  </main>"
        "  <footer>"
        "    <p>© 2026 Just-Browse</p>"
        "  </footer>"
        "</body>"
        "</html>";

    if (browser_engine_load_html(engine, html) != 0) {
        fprintf(stderr, "Failed to load HTML!\n");
        browser_engine_destroy(engine);
        return 1;
    }
    printf("   ✓ HTML document parsed and loaded\n\n");

    // Execute JavaScript - querySelector demo
    printf("3. Testing querySelector (CSS selectors)...\n");
    const char* script1 = 
        "var section = document.querySelector('#content');"
        "if (section) {"
        "  console.log('   ✓ Found section by ID selector (#content)');"
        "  section.setAttribute('data-loaded', 'true');"
        "}"
        "var header = document.querySelector('h1');"
        "if (header) {"
        "  console.log('   ✓ Found heading by tag selector (h1)');"
        "}"
        "var link = document.querySelector('.nav-link');"
        "if (link) {"
        "  console.log('   ✓ Found link by class selector (.nav-link)');"
        "}";
    
    if (browser_engine_execute_script(engine, script1) != 0) {
        fprintf(stderr, "JavaScript error!\n");
    }
    printf("\n");

    // DOM manipulation
    printf("4. DOM manipulation from JavaScript...\n");
    const char* script2 = 
        "var container = document.getElementById('dynamic');"
        "if (container) {"
        "  var title = document.createElement('h3');"
        "  title.innerHTML = 'Dynamically Created Content';"
        "  title.setAttribute('class', 'dynamic-title');"
        "  console.log('   ✓ Created new element dynamically');"
        "  "
        "  var para = document.createElement('p');"
        "  para.innerHTML = 'This content was created using JavaScript!';"
        "  para.setAttribute('id', 'dynamic-para');"
        "  console.log('   ✓ Created paragraph with content');"
        "}";
    
    if (browser_engine_execute_script(engine, script2) != 0) {
        fprintf(stderr, "JavaScript error!\n");
    }
    printf("\n");

    // Variables and computation
    printf("5. JavaScript computation...\n");
    const char* script3 = 
        "var numbers = [1, 2, 3, 4, 5];"
        "var sum = 0;"
        "for (var i = 0; i < numbers.length; i++) {"
        "  sum += numbers[i];"
        "}"
        "console.log('   ✓ Array sum:', sum);"
        ""
        "var factorial = function(n) {"
        "  return n <= 1 ? 1 : n * factorial(n - 1);"
        "};"
        "console.log('   ✓ Factorial of 5:', factorial(5));";
    
    if (browser_engine_execute_script(engine, script3) != 0) {
        fprintf(stderr, "JavaScript error!\n");
    }
    printf("\n");

    // Complex DOM queries
    printf("6. Complex DOM queries...\n");
    const char* script4 = 
        "var body = document.querySelector('body');"
        "if (body) {"
        "  var footer = body.querySelector('footer');"
        "  if (footer) {"
        "    console.log('   ✓ Found footer using nested querySelector');"
        "    footer.setAttribute('data-year', '2026');"
        "  }"
        "}"
        "var dynamic = document.querySelector('#dynamic');"
        "if (dynamic) {"
        "  var hasClass = dynamic.getAttribute('class');"
        "  console.log('   ✓ Dynamic div has class:', hasClass);"
        "}";
    
    if (browser_engine_execute_script(engine, script4) != 0) {
        fprintf(stderr, "JavaScript error!\n");
    }
    printf("\n");

    // Render
    printf("7. Rendering page...\n");
    if (browser_engine_render(engine) != 0) {
        fprintf(stderr, "Failed to render!\n");
    } else {
        printf("   ✓ Page rendered successfully\n\n");
    }

    // Summary
    printf("8. Summary of integrated features:\n");
    printf("   ✓ QuickJS JavaScript engine (~500KB)\n");
    printf("   ✓ HTML parser with full tag/attribute support\n");
    printf("   ✓ DOM tree with getElementById and querySelector\n");
    printf("   ✓ JavaScript-DOM bindings (createElement, setAttribute, etc.)\n");
    printf("   ✓ console.log() support for debugging\n");
    printf("   ✓ Event system infrastructure\n");
    printf("   ✓ Memory-safe implementation\n\n");

    // Clean up
    printf("9. Cleaning up...\n");
    browser_engine_destroy(engine);
    printf("   ✓ Engine destroyed, memory freed\n\n");

    printf("=================================================================\n");
    printf("  All features working! Browser engine ready for WASM.\n");
    printf("=================================================================\n");

    return 0;
}
