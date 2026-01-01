#ifndef JUST_BROWSE_JS_ENGINE_H
#define JUST_BROWSE_JS_ENGINE_H

#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct JSEngine JSEngine;
typedef struct DOMDocument DOMDocument;

/**
 * Initialize the JavaScript engine (QuickJS)
 * @return Pointer to the JS engine instance, or NULL on failure
 */
JSEngine* js_engine_init(void);

/**
 * Destroy the JavaScript engine
 * @param engine The JS engine instance to destroy
 */
void js_engine_destroy(JSEngine* engine);

/**
 * Bind DOM to the JavaScript engine
 * This creates the document object and DOM APIs in JavaScript context
 * @param engine The JS engine instance
 * @param document The DOM document to bind
 * @return 0 on success, -1 on failure
 */
int js_engine_bind_dom(JSEngine* engine, DOMDocument* document);

/**
 * Evaluate JavaScript code
 * @param engine The JS engine instance
 * @param script The JavaScript code to evaluate
 * @return 0 on success, -1 on failure
 */
int js_engine_eval(JSEngine* engine, const char* script);

/**
 * Get the last error message
 * @param engine The JS engine instance
 * @return The error message, or NULL if no error
 */
const char* js_engine_get_error(JSEngine* engine);

#ifdef __cplusplus
}
#endif

#endif // JUST_BROWSE_JS_ENGINE_H
