#ifndef JUST_BROWSE_DOM_H
#define JUST_BROWSE_DOM_H

#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct DOMDocument DOMDocument;
typedef struct DOMNode DOMNode;
typedef struct DOMElement DOMElement;

// Node types
typedef enum {
    NODE_ELEMENT = 1,
    NODE_TEXT = 3,
    NODE_COMMENT = 8,
    NODE_DOCUMENT = 9
} DOMNodeType;

/**
 * Create a new DOM document
 * @return Pointer to the document, or NULL on failure
 */
DOMDocument* dom_document_create(void);

/**
 * Destroy a DOM document and all its nodes
 * @param doc The document to destroy
 */
void dom_document_destroy(DOMDocument* doc);

/**
 * Create an element in the document
 * @param doc The document
 * @param tag_name The tag name for the element
 * @return Pointer to the element, or NULL on failure
 */
DOMElement* dom_document_create_element(DOMDocument* doc, const char* tag_name);

/**
 * Get the document element (root element)
 * @param doc The document
 * @return Pointer to the root element, or NULL if not set
 */
DOMElement* dom_document_get_element(DOMDocument* doc);

/**
 * Get element by ID
 * @param doc The document
 * @param id The element ID to search for
 * @return Pointer to the element, or NULL if not found
 */
DOMElement* dom_document_get_element_by_id(DOMDocument* doc, const char* id);

/**
 * Append a child node to a parent node
 * @param parent The parent node
 * @param child The child node to append
 * @return 0 on success, -1 on failure
 */
int dom_node_append_child(DOMNode* parent, DOMNode* child);

/**
 * Get the node type
 * @param node The node
 * @return The node type
 */
DOMNodeType dom_node_get_type(DOMNode* node);

/**
 * Set an attribute on an element
 * @param element The element
 * @param name The attribute name
 * @param value The attribute value
 * @return 0 on success, -1 on failure
 */
int dom_element_set_attribute(DOMElement* element, const char* name, const char* value);

/**
 * Get an attribute from an element
 * @param element The element
 * @param name The attribute name
 * @return The attribute value, or NULL if not found
 */
const char* dom_element_get_attribute(DOMElement* element, const char* name);

/**
 * Get the tag name of an element
 * @param element The element
 * @return The tag name
 */
const char* dom_element_get_tag_name(DOMElement* element);

/**
 * Set the inner HTML of an element
 * @param element The element
 * @param html The HTML content
 * @return 0 on success, -1 on failure
 */
int dom_element_set_inner_html(DOMElement* element, const char* html);

/**
 * Event callback type
 */
typedef void (*DOMEventCallback)(DOMElement* element, void* user_data);

/**
 * Add an event listener to an element
 * @param element The element
 * @param event_type The event type (e.g., "click", "change")
 * @param callback The callback function
 * @param user_data User data to pass to callback
 * @return 0 on success, -1 on failure
 */
int dom_element_add_event_listener(DOMElement* element, const char* event_type, 
                                    DOMEventCallback callback, void* user_data);

/**
 * Trigger an event on an element
 * @param element The element
 * @param event_type The event type
 * @return 0 on success, -1 on failure
 */
int dom_element_trigger_event(DOMElement* element, const char* event_type);

/**
 * Query selector - find first element matching CSS selector
 * @param element The element to search from (or document)
 * @param selector The CSS selector (simplified: tag, #id, .class)
 * @return The first matching element, or NULL if not found
 */
DOMElement* dom_element_query_selector(DOMElement* element, const char* selector);

#ifdef __cplusplus
}
#endif

#endif // JUST_BROWSE_DOM_H
