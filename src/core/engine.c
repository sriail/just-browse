#include "core/engine.h"
#include "dom/dom.h"
#include "js/js_engine.h"
#include "rendering/renderer.h"
#include "html/parser.h"
#include <stdlib.h>
#include <string.h>

struct BrowserEngine {
    DOMDocument* document;
    JSEngine* js_engine;
    Renderer* renderer;
    int viewport_width;
    int viewport_height;
};

BrowserEngine* browser_engine_init(void) {
    BrowserEngine* engine = (BrowserEngine*)malloc(sizeof(BrowserEngine));
    if (!engine) {
        return NULL;
    }

    // Initialize with default viewport size
    engine->viewport_width = 1024;
    engine->viewport_height = 768;

    // Create DOM document
    engine->document = dom_document_create();
    if (!engine->document) {
        free(engine);
        return NULL;
    }

    // Initialize JavaScript engine
    engine->js_engine = js_engine_init();
    if (!engine->js_engine) {
        dom_document_destroy(engine->document);
        free(engine);
        return NULL;
    }

    // Bind DOM to JavaScript
    if (js_engine_bind_dom(engine->js_engine, engine->document) != 0) {
        js_engine_destroy(engine->js_engine);
        dom_document_destroy(engine->document);
        free(engine);
        return NULL;
    }

    // Initialize renderer
    engine->renderer = renderer_init(engine->viewport_width, engine->viewport_height);
    if (!engine->renderer) {
        js_engine_destroy(engine->js_engine);
        dom_document_destroy(engine->document);
        free(engine);
        return NULL;
    }

    return engine;
}

void browser_engine_destroy(BrowserEngine* engine) {
    if (!engine) {
        return;
    }

    if (engine->renderer) {
        renderer_destroy(engine->renderer);
    }
    if (engine->js_engine) {
        js_engine_destroy(engine->js_engine);
    }
    if (engine->document) {
        dom_document_destroy(engine->document);
    }
    free(engine);
}

int browser_engine_load_html(BrowserEngine* engine, const char* html) {
    if (!engine || !html) {
        return -1;
    }

    // Use the HTML parser to create the DOM tree
    return html_parser_parse(engine->document, html);
}

int browser_engine_execute_script(BrowserEngine* engine, const char* script) {
    if (!engine || !script) {
        return -1;
    }

    return js_engine_eval(engine->js_engine, script);
}

int browser_engine_render(BrowserEngine* engine) {
    if (!engine) {
        return -1;
    }

    return renderer_render(engine->renderer, engine->document);
}
