#ifndef JUST_BROWSE_RENDERER_H
#define JUST_BROWSE_RENDERER_H

#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct Renderer Renderer;
typedef struct DOMDocument DOMDocument;

/**
 * Initialize the renderer (RmlUi-based)
 * @param width Initial viewport width
 * @param height Initial viewport height
 * @return Pointer to the renderer instance, or NULL on failure
 */
Renderer* renderer_init(int width, int height);

/**
 * Destroy the renderer
 * @param renderer The renderer instance to destroy
 */
void renderer_destroy(Renderer* renderer);

/**
 * Render a DOM document
 * @param renderer The renderer instance
 * @param document The DOM document to render
 * @return 0 on success, -1 on failure
 */
int renderer_render(Renderer* renderer, DOMDocument* document);

/**
 * Resize the viewport
 * @param renderer The renderer instance
 * @param width New viewport width
 * @param height New viewport height
 * @return 0 on success, -1 on failure
 */
int renderer_resize(Renderer* renderer, int width, int height);

/**
 * Get the rendered output buffer (for WASM)
 * @param renderer The renderer instance
 * @param width Output parameter for buffer width
 * @param height Output parameter for buffer height
 * @return Pointer to the RGBA pixel buffer, or NULL on failure
 */
const unsigned char* renderer_get_buffer(Renderer* renderer, int* width, int* height);

#ifdef __cplusplus
}
#endif

#endif // JUST_BROWSE_RENDERER_H
