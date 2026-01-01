#include "dom/dom.h"
#include <stdlib.h>
#include <string.h>

// Event listener structure
typedef struct EventListener {
    char* event_type;
    DOMEventCallback callback;
    void* user_data;
    struct EventListener* next;
} EventListener;

// Internal structures
struct DOMNode {
    DOMNodeType type;
    char* name;
    char* value;
    DOMNode* parent;
    DOMNode* first_child;
    DOMNode* last_child;
    DOMNode* next_sibling;
    DOMNode* prev_sibling;
    
    // For elements
    struct {
        char** attr_names;
        char** attr_values;
        int attr_count;
        int attr_capacity;
    } attributes;
    
    // Event listeners
    EventListener* event_listeners;
};

struct DOMElement {
    DOMNode node;
};

struct DOMDocument {
    DOMNode node;
    DOMElement* document_element;
};

DOMDocument* dom_document_create(void) {
    DOMDocument* doc = (DOMDocument*)calloc(1, sizeof(DOMDocument));
    if (!doc) {
        return NULL;
    }

    doc->node.type = NODE_DOCUMENT;
    doc->node.name = strdup("document");
    if (!doc->node.name) {
        free(doc);
        return NULL;
    }
    doc->document_element = NULL;

    return doc;
}

static void dom_node_destroy_recursive(DOMNode* node) {
    if (!node) {
        return;
    }

    // Destroy children
    DOMNode* child = node->first_child;
    while (child) {
        DOMNode* next = child->next_sibling;
        dom_node_destroy_recursive(child);
        child = next;
    }

    // Free attributes
    for (int i = 0; i < node->attributes.attr_count; i++) {
        free(node->attributes.attr_names[i]);
        free(node->attributes.attr_values[i]);
    }
    free(node->attributes.attr_names);
    free(node->attributes.attr_values);

    // Free event listeners
    EventListener* listener = node->event_listeners;
    while (listener) {
        EventListener* next = listener->next;
        free(listener->event_type);
        free(listener);
        listener = next;
    }

    // Free node data
    free(node->name);
    free(node->value);
    free(node);
}

void dom_document_destroy(DOMDocument* doc) {
    if (!doc) {
        return;
    }

    dom_node_destroy_recursive(doc->node.first_child);
    free(doc->node.name);
    free(doc);
}

DOMElement* dom_document_create_element(DOMDocument* doc, const char* tag_name) {
    if (!doc || !tag_name) {
        return NULL;
    }

    DOMElement* element = (DOMElement*)calloc(1, sizeof(DOMElement));
    if (!element) {
        return NULL;
    }

    element->node.type = NODE_ELEMENT;
    element->node.name = strdup(tag_name);
    if (!element->node.name) {
        free(element);
        return NULL;
    }
    
    // Initialize attributes
    element->node.attributes.attr_capacity = 8;
    element->node.attributes.attr_names = (char**)calloc(8, sizeof(char*));
    element->node.attributes.attr_values = (char**)calloc(8, sizeof(char*));
    
    if (!element->node.attributes.attr_names || !element->node.attributes.attr_values) {
        free(element->node.attributes.attr_names);
        free(element->node.attributes.attr_values);
        free(element->node.name);
        free(element);
        return NULL;
    }
    
    // Initialize event listeners
    element->node.event_listeners = NULL;

    // Set as document element if this is the first element
    if (!doc->document_element) {
        doc->document_element = element;
        element->node.parent = &doc->node;
        doc->node.first_child = &element->node;
        doc->node.last_child = &element->node;
    }

    return element;
}

DOMElement* dom_document_get_element(DOMDocument* doc) {
    if (!doc) {
        return NULL;
    }
    return doc->document_element;
}

// Helper function to search for element by ID recursively
static DOMElement* search_element_by_id(DOMNode* node, const char* id) {
    if (!node) {
        return NULL;
    }

    // Check if this is an element with matching ID
    if (node->type == NODE_ELEMENT) {
        DOMElement* elem = (DOMElement*)node;
        const char* elem_id = dom_element_get_attribute(elem, "id");
        if (elem_id && strcmp(elem_id, id) == 0) {
            return elem;
        }
    }

    // Search children
    DOMNode* child = node->first_child;
    while (child) {
        DOMElement* found = search_element_by_id(child, id);
        if (found) {
            return found;
        }
        child = child->next_sibling;
    }

    return NULL;
}

DOMElement* dom_document_get_element_by_id(DOMDocument* doc, const char* id) {
    if (!doc || !id) {
        return NULL;
    }

    // Start searching from document element
    return search_element_by_id(&doc->node, id);
}

int dom_node_append_child(DOMNode* parent, DOMNode* child) {
    if (!parent || !child) {
        return -1;
    }

    child->parent = parent;
    child->next_sibling = NULL;

    if (!parent->first_child) {
        parent->first_child = child;
        parent->last_child = child;
        child->prev_sibling = NULL;
    } else {
        child->prev_sibling = parent->last_child;
        parent->last_child->next_sibling = child;
        parent->last_child = child;
    }

    return 0;
}

DOMNodeType dom_node_get_type(DOMNode* node) {
    if (!node) {
        return NODE_DOCUMENT; // Default
    }
    return node->type;
}

int dom_element_set_attribute(DOMElement* element, const char* name, const char* value) {
    if (!element || !name || !value) {
        return -1;
    }

    DOMNode* node = &element->node;

    // Check if attribute already exists
    for (int i = 0; i < node->attributes.attr_count; i++) {
        if (strcmp(node->attributes.attr_names[i], name) == 0) {
            char* new_value = strdup(value);
            if (!new_value) {
                return -1;
            }
            free(node->attributes.attr_values[i]);
            node->attributes.attr_values[i] = new_value;
            return 0;
        }
    }

    // Add new attribute
    if (node->attributes.attr_count >= node->attributes.attr_capacity) {
        // Grow arrays
        int new_capacity = node->attributes.attr_capacity * 2;
        char** new_names = (char**)realloc(node->attributes.attr_names, new_capacity * sizeof(char*));
        char** new_values = (char**)realloc(node->attributes.attr_values, new_capacity * sizeof(char*));
        if (!new_names || !new_values) {
            return -1;
        }
        node->attributes.attr_names = new_names;
        node->attributes.attr_values = new_values;
        node->attributes.attr_capacity = new_capacity;
    }

    node->attributes.attr_names[node->attributes.attr_count] = strdup(name);
    node->attributes.attr_values[node->attributes.attr_count] = strdup(value);
    
    if (!node->attributes.attr_names[node->attributes.attr_count] || 
        !node->attributes.attr_values[node->attributes.attr_count]) {
        free(node->attributes.attr_names[node->attributes.attr_count]);
        free(node->attributes.attr_values[node->attributes.attr_count]);
        return -1;
    }
    
    node->attributes.attr_count++;

    return 0;
}

const char* dom_element_get_attribute(DOMElement* element, const char* name) {
    if (!element || !name) {
        return NULL;
    }

    DOMNode* node = &element->node;

    for (int i = 0; i < node->attributes.attr_count; i++) {
        if (strcmp(node->attributes.attr_names[i], name) == 0) {
            return node->attributes.attr_values[i];
        }
    }

    return NULL;
}

const char* dom_element_get_tag_name(DOMElement* element) {
    if (!element) {
        return NULL;
    }
    return element->node.name;
}

int dom_element_set_inner_html(DOMElement* element, const char* html) {
    if (!element || !html) {
        return -1;
    }

    // Destroy existing children
    DOMNode* child = element->node.first_child;
    while (child) {
        DOMNode* next = child->next_sibling;
        dom_node_destroy_recursive(child);
        child = next;
    }
    element->node.first_child = NULL;
    element->node.last_child = NULL;

    // For basic implementation, just store as a text node
    // A real implementation would parse the HTML
    DOMNode* text_node = (DOMNode*)calloc(1, sizeof(DOMNode));
    if (!text_node) {
        return -1;
    }

    text_node->type = NODE_TEXT;
    text_node->value = strdup(html);
    if (!text_node->value) {
        free(text_node);
        return -1;
    }
    
    return dom_node_append_child(&element->node, text_node);
}

int dom_element_add_event_listener(DOMElement* element, const char* event_type,
                                    DOMEventCallback callback, void* user_data) {
    if (!element || !event_type || !callback) {
        return -1;
    }

    EventListener* listener = (EventListener*)malloc(sizeof(EventListener));
    if (!listener) {
        return -1;
    }

    listener->event_type = strdup(event_type);
    if (!listener->event_type) {
        free(listener);
        return -1;
    }

    listener->callback = callback;
    listener->user_data = user_data;
    listener->next = element->node.event_listeners;
    element->node.event_listeners = listener;

    return 0;
}

int dom_element_trigger_event(DOMElement* element, const char* event_type) {
    if (!element || !event_type) {
        return -1;
    }

    EventListener* listener = element->node.event_listeners;
    while (listener) {
        if (strcmp(listener->event_type, event_type) == 0) {
            listener->callback(element, listener->user_data);
        }
        listener = listener->next;
    }

    return 0;
}

// Helper function for querySelector - checks if element matches selector
static int matches_selector(DOMElement* element, const char* selector) {
    if (!element || !selector) {
        return 0;
    }

    // ID selector (#id)
    if (selector[0] == '#') {
        const char* id = dom_element_get_attribute(element, "id");
        return id && strcmp(id, selector + 1) == 0;
    }

    // Class selector (.class)
    if (selector[0] == '.') {
        const char* class_attr = dom_element_get_attribute(element, "class");
        if (!class_attr) {
            return 0;
        }
        // Simple check - full implementation would split by spaces
        return strstr(class_attr, selector + 1) != NULL;
    }

    // Tag selector
    const char* tag = dom_element_get_tag_name(element);
    return tag && strcmp(tag, selector) == 0;
}

// Helper function to search DOM tree for querySelector
static DOMElement* search_query_selector(DOMNode* node, const char* selector) {
    if (!node) {
        return NULL;
    }

    // Check if this node matches
    if (node->type == NODE_ELEMENT) {
        DOMElement* elem = (DOMElement*)node;
        if (matches_selector(elem, selector)) {
            return elem;
        }
    }

    // Search children
    DOMNode* child = node->first_child;
    while (child) {
        DOMElement* found = search_query_selector(child, selector);
        if (found) {
            return found;
        }
        child = child->next_sibling;
    }

    return NULL;
}

DOMElement* dom_element_query_selector(DOMElement* element, const char* selector) {
    if (!element || !selector) {
        return NULL;
    }

    return search_query_selector((DOMNode*)element, selector);
}
