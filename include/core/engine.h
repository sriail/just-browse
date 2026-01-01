#ifndef JUST_BROWSE_ENGINE_H
#define JUST_BROWSE_ENGINE_H

#ifdef __cplusplus
extern "C" {
#endif

// Browser engine initialization and lifecycle
typedef struct BrowserEngine BrowserEngine;

/**
 * Initialize the browser engine
 * @return Pointer to the engine instance, or NULL on failure
 */
BrowserEngine* browser_engine_init(void);

/**
 * Destroy the browser engine and free resources
 * @param engine The engine instance to destroy
 */
void browser_engine_destroy(BrowserEngine* engine);

/**
 * Load HTML content into the engine
 * @param engine The engine instance
 * @param html HTML content to load
 * @return 0 on success, -1 on failure
 */
int browser_engine_load_html(BrowserEngine* engine, const char* html);

/**
 * Execute JavaScript code in the engine context
 * @param engine The engine instance
 * @param script JavaScript code to execute
 * @return 0 on success, -1 on failure
 */
int browser_engine_execute_script(BrowserEngine* engine, const char* script);

/**
 * Render the current page
 * @param engine The engine instance
 * @return 0 on success, -1 on failure
 */
int browser_engine_render(BrowserEngine* engine);

#ifdef __cplusplus
}
#endif

#endif // JUST_BROWSE_ENGINE_H
