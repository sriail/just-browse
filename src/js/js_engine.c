#include "js/js_engine.h"
#include "dom/dom.h"
#include "quickjs.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct JSEngine {
    JSRuntime* runtime;
    JSContext* context;
    DOMDocument* bound_document;
    char* last_error;
};

// Forward declarations for DOM bindings
static JSValue js_document_create_element(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
static JSValue js_document_get_element_by_id(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
static JSValue js_element_set_attribute(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
static JSValue js_element_get_attribute(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
static JSValue js_element_set_inner_html(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
static JSValue js_console_log(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);

JSEngine* js_engine_init(void) {
    JSEngine* engine = (JSEngine*)calloc(1, sizeof(JSEngine));
    if (!engine) {
        return NULL;
    }

    // Create QuickJS runtime
    engine->runtime = JS_NewRuntime();
    if (!engine->runtime) {
        free(engine);
        return NULL;
    }

    // Create QuickJS context
    engine->context = JS_NewContext(engine->runtime);
    if (!engine->context) {
        JS_FreeRuntime(engine->runtime);
        free(engine);
        return NULL;
    }

    engine->bound_document = NULL;
    engine->last_error = NULL;

    // Add console.log support
    JSValue global = JS_GetGlobalObject(engine->context);
    JSValue console = JS_NewObject(engine->context);
    JS_SetPropertyStr(engine->context, console, "log",
                     JS_NewCFunction(engine->context, js_console_log, "log", 1));
    JS_SetPropertyStr(engine->context, global, "console", console);
    JS_FreeValue(engine->context, global);

    return engine;
}

void js_engine_destroy(JSEngine* engine) {
    if (!engine) {
        return;
    }

    if (engine->context) {
        JS_FreeContext(engine->context);
    }
    if (engine->runtime) {
        JS_FreeRuntime(engine->runtime);
    }
    free(engine->last_error);
    free(engine);
}

int js_engine_bind_dom(JSEngine* engine, DOMDocument* document) {
    if (!engine || !document) {
        return -1;
    }

    engine->bound_document = document;

    // Create document object
    JSValue global = JS_GetGlobalObject(engine->context);
    JSValue doc_obj = JS_NewObject(engine->context);

    // Store document pointer directly as a property (simpler approach)
    JS_SetPropertyStr(engine->context, doc_obj, "_internal_doc_ptr",
                     JS_NewBigInt64(engine->context, (int64_t)(uintptr_t)document));

    // Bind document methods
    JS_SetPropertyStr(engine->context, doc_obj, "createElement",
                     JS_NewCFunction(engine->context, js_document_create_element, "createElement", 1));
    JS_SetPropertyStr(engine->context, doc_obj, "getElementById",
                     JS_NewCFunction(engine->context, js_document_get_element_by_id, "getElementById", 1));

    // Set as global document
    JS_SetPropertyStr(engine->context, global, "document", doc_obj);
    JS_FreeValue(engine->context, global);

    return 0;
}

int js_engine_eval(JSEngine* engine, const char* script) {
    if (!engine || !script) {
        return -1;
    }

    free(engine->last_error);
    engine->last_error = NULL;

    JSValue result = JS_Eval(engine->context, script, strlen(script), 
                            "<eval>", JS_EVAL_TYPE_GLOBAL);

    if (JS_IsException(result)) {
        JSValue exception = JS_GetException(engine->context);
        const char* error_str = JS_ToCString(engine->context, exception);
        if (error_str) {
            engine->last_error = strdup(error_str);
            JS_FreeCString(engine->context, error_str);
        }
        JS_FreeValue(engine->context, exception);
        JS_FreeValue(engine->context, result);
        return -1;
    }

    JS_FreeValue(engine->context, result);
    return 0;
}

const char* js_engine_get_error(JSEngine* engine) {
    if (!engine) {
        return NULL;
    }
    return engine->last_error;
}

// DOM binding implementations

static JSValue js_console_log(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    for (int i = 0; i < argc; i++) {
        const char* str = JS_ToCString(ctx, argv[i]);
        if (str) {
            printf("%s%s", i > 0 ? " " : "", str);
            JS_FreeCString(ctx, str);
        }
    }
    printf("\n");
    return JS_UNDEFINED;
}

static JSValue js_document_create_element(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if (argc < 1) {
        return JS_EXCEPTION;
    }

    // Get document pointer from _internal_doc_ptr property
    JSValue ptr_val = JS_GetPropertyStr(ctx, this_val, "_internal_doc_ptr");
    int64_t ptr_int;
    JS_ToBigInt64(ctx, &ptr_int, ptr_val);
    JS_FreeValue(ctx, ptr_val);
    
    DOMDocument* doc = (DOMDocument*)(uintptr_t)ptr_int;
    if (!doc) {
        return JS_EXCEPTION;
    }

    const char* tag_name = JS_ToCString(ctx, argv[0]);
    if (!tag_name) {
        return JS_EXCEPTION;
    }

    DOMElement* elem = dom_document_create_element(doc, tag_name);
    JS_FreeCString(ctx, tag_name);

    if (!elem) {
        return JS_NULL;
    }

    // Create JavaScript wrapper for element
    JSValue elem_obj = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, elem_obj, "_internal_elem_ptr",
                     JS_NewBigInt64(ctx, (int64_t)(uintptr_t)elem));

    // Bind element methods
    JS_SetPropertyStr(ctx, elem_obj, "setAttribute",
                     JS_NewCFunction(ctx, js_element_set_attribute, "setAttribute", 2));
    JS_SetPropertyStr(ctx, elem_obj, "getAttribute",
                     JS_NewCFunction(ctx, js_element_get_attribute, "getAttribute", 1));
    
    // Add innerHTML property setter/getter
    JSValue set_inner_html = JS_NewCFunction(ctx, js_element_set_inner_html, "set innerHTML", 1);
    JS_DefinePropertyGetSet(ctx, elem_obj, JS_NewAtom(ctx, "innerHTML"),
                           JS_UNDEFINED, set_inner_html, JS_PROP_CONFIGURABLE);

    return elem_obj;
}

static JSValue js_document_get_element_by_id(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if (argc < 1) {
        return JS_EXCEPTION;
    }

    JSValue ptr_val = JS_GetPropertyStr(ctx, this_val, "_internal_doc_ptr");
    int64_t ptr_int;
    JS_ToBigInt64(ctx, &ptr_int, ptr_val);
    JS_FreeValue(ctx, ptr_val);
    
    DOMDocument* doc = (DOMDocument*)(uintptr_t)ptr_int;
    if (!doc) {
        return JS_EXCEPTION;
    }

    const char* id = JS_ToCString(ctx, argv[0]);
    if (!id) {
        return JS_EXCEPTION;
    }

    DOMElement* elem = dom_document_get_element_by_id(doc, id);
    JS_FreeCString(ctx, id);

    if (!elem) {
        return JS_NULL;
    }

    // Create JavaScript wrapper
    JSValue elem_obj = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, elem_obj, "_internal_elem_ptr",
                     JS_NewBigInt64(ctx, (int64_t)(uintptr_t)elem));

    JS_SetPropertyStr(ctx, elem_obj, "setAttribute",
                     JS_NewCFunction(ctx, js_element_set_attribute, "setAttribute", 2));
    JS_SetPropertyStr(ctx, elem_obj, "getAttribute",
                     JS_NewCFunction(ctx, js_element_get_attribute, "getAttribute", 1));

    return elem_obj;
}

static JSValue js_element_set_attribute(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if (argc < 2) {
        return JS_EXCEPTION;
    }

    JSValue ptr_val = JS_GetPropertyStr(ctx, this_val, "_internal_elem_ptr");
    int64_t ptr_int;
    JS_ToBigInt64(ctx, &ptr_int, ptr_val);
    JS_FreeValue(ctx, ptr_val);
    
    DOMElement* elem = (DOMElement*)(uintptr_t)ptr_int;
    if (!elem) {
        return JS_EXCEPTION;
    }

    const char* name = JS_ToCString(ctx, argv[0]);
    const char* value = JS_ToCString(ctx, argv[1]);

    if (!name || !value) {
        JS_FreeCString(ctx, name);
        JS_FreeCString(ctx, value);
        return JS_EXCEPTION;
    }

    int result = dom_element_set_attribute(elem, name, value);

    JS_FreeCString(ctx, name);
    JS_FreeCString(ctx, value);

    return result == 0 ? JS_UNDEFINED : JS_EXCEPTION;
}

static JSValue js_element_get_attribute(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if (argc < 1) {
        return JS_EXCEPTION;
    }

    JSValue ptr_val = JS_GetPropertyStr(ctx, this_val, "_internal_elem_ptr");
    int64_t ptr_int;
    JS_ToBigInt64(ctx, &ptr_int, ptr_val);
    JS_FreeValue(ctx, ptr_val);
    
    DOMElement* elem = (DOMElement*)(uintptr_t)ptr_int;
    if (!elem) {
        return JS_EXCEPTION;
    }

    const char* name = JS_ToCString(ctx, argv[0]);
    if (!name) {
        return JS_EXCEPTION;
    }

    const char* value = dom_element_get_attribute(elem, name);
    JS_FreeCString(ctx, name);

    if (!value) {
        return JS_NULL;
    }

    return JS_NewString(ctx, value);
}

static JSValue js_element_set_inner_html(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if (argc < 1) {
        return JS_EXCEPTION;
    }

    JSValue ptr_val = JS_GetPropertyStr(ctx, this_val, "_internal_elem_ptr");
    int64_t ptr_int;
    JS_ToBigInt64(ctx, &ptr_int, ptr_val);
    JS_FreeValue(ctx, ptr_val);
    
    DOMElement* elem = (DOMElement*)(uintptr_t)ptr_int;
    if (!elem) {
        return JS_EXCEPTION;
    }

    const char* html = JS_ToCString(ctx, argv[0]);
    if (!html) {
        return JS_EXCEPTION;
    }

    int result = dom_element_set_inner_html(elem, html);
    JS_FreeCString(ctx, html);

    return result == 0 ? JS_UNDEFINED : JS_EXCEPTION;
}
