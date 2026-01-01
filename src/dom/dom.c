#include "dom/dom.h"
#include <stdlib.h>
#include <string.h>

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
    
    // Initialize attributes
    element->node.attributes.attr_capacity = 8;
    element->node.attributes.attr_names = (char**)calloc(8, sizeof(char*));
    element->node.attributes.attr_values = (char**)calloc(8, sizeof(char*));

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

DOMElement* dom_document_get_element_by_id(DOMDocument* doc, const char* id) {
    if (!doc || !id) {
        return NULL;
    }

    // Simple recursive search (would need optimization for real use)
    // For now, return NULL as this is a basic implementation
    return NULL;
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
            free(node->attributes.attr_values[i]);
            node->attributes.attr_values[i] = strdup(value);
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
    
    return dom_node_append_child(&element->node, text_node);
}
