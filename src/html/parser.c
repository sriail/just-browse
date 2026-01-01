#include "html/parser.h"
#include "dom/dom.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

// Simple HTML parser - handles basic tags and text nodes
// This is a simplified parser for demonstration purposes

typedef struct {
    const char* input;
    size_t pos;
    size_t length;
} Parser;

static void skip_whitespace(Parser* p) {
    while (p->pos < p->length && isspace(p->input[p->pos])) {
        p->pos++;
    }
}

static char peek_char(Parser* p) {
    if (p->pos < p->length) {
        return p->input[p->pos];
    }
    return '\0';
}

static char next_char(Parser* p) {
    if (p->pos < p->length) {
        return p->input[p->pos++];
    }
    return '\0';
}

static char* read_until(Parser* p, char delim) {
    size_t start = p->pos;
    while (p->pos < p->length && p->input[p->pos] != delim) {
        p->pos++;
    }
    
    size_t len = p->pos - start;
    char* result = (char*)malloc(len + 1);
    if (result) {
        memcpy(result, p->input + start, len);
        result[len] = '\0';
    }
    return result;
}

static char* read_tag_name(Parser* p) {
    size_t start = p->pos;
    while (p->pos < p->length && (isalnum(p->input[p->pos]) || p->input[p->pos] == '-')) {
        p->pos++;
    }
    
    size_t len = p->pos - start;
    char* result = (char*)malloc(len + 1);
    if (result) {
        memcpy(result, p->input + start, len);
        result[len] = '\0';
    }
    return result;
}

static int parse_attributes(Parser* p, DOMElement* element) {
    while (peek_char(p) != '>' && peek_char(p) != '/' && peek_char(p) != '\0') {
        skip_whitespace(p);
        
        if (peek_char(p) == '>' || peek_char(p) == '/') {
            break;
        }
        
        // Read attribute name
        char* attr_name = read_tag_name(p);
        if (!attr_name) {
            return -1;
        }
        
        skip_whitespace(p);
        
        char* attr_value = NULL;
        if (peek_char(p) == '=') {
            next_char(p); // consume '='
            skip_whitespace(p);
            
            char quote = peek_char(p);
            if (quote == '"' || quote == '\'') {
                next_char(p); // consume quote
                attr_value = read_until(p, quote);
                next_char(p); // consume closing quote
            } else {
                // Unquoted attribute value
                size_t start = p->pos;
                while (p->pos < p->length && !isspace(p->input[p->pos]) && 
                       p->input[p->pos] != '>' && p->input[p->pos] != '/') {
                    p->pos++;
                }
                size_t len = p->pos - start;
                attr_value = (char*)malloc(len + 1);
                if (attr_value) {
                    memcpy(attr_value, p->input + start, len);
                    attr_value[len] = '\0';
                }
            }
        } else {
            // Boolean attribute
            attr_value = strdup("");
        }
        
        if (attr_value) {
            dom_element_set_attribute(element, attr_name, attr_value);
            free(attr_value);
        }
        free(attr_name);
    }
    
    return 0;
}

// Forward declaration
static DOMElement* parse_element(Parser* p, DOMDocument* doc);

static int parse_children(Parser* p, DOMDocument* doc, DOMElement* parent) {
    while (peek_char(p) != '\0') {
        skip_whitespace(p);
        
        if (peek_char(p) == '\0') {
            break;
        }
        
        if (peek_char(p) == '<') {
            p->pos++; // consume '<'
            
            if (peek_char(p) == '/') {
                // Closing tag
                p->pos--; // put back '<'
                break;
            }
            
            // Parse element
            p->pos--; // put back '<'
            DOMElement* child = parse_element(p, doc);
            if (child) {
                dom_node_append_child((DOMNode*)parent, (DOMNode*)child);
            }
        } else {
            // Text content
            size_t start = p->pos;
            while (p->pos < p->length && p->input[p->pos] != '<') {
                p->pos++;
            }
            
            size_t len = p->pos - start;
            if (len > 0) {
                // Trim whitespace
                const char* text_start = p->input + start;
                const char* text_end = text_start + len;
                
                while (text_start < text_end && isspace(*text_start)) {
                    text_start++;
                }
                while (text_end > text_start && isspace(*(text_end - 1))) {
                    text_end--;
                }
                
                len = text_end - text_start;
                if (len > 0) {
                    char* text = (char*)malloc(len + 1);
                    if (text) {
                        memcpy(text, text_start, len);
                        text[len] = '\0';
                        dom_element_set_inner_html(parent, text);
                        free(text);
                    }
                }
            }
        }
    }
    
    return 0;
}

static DOMElement* parse_element(Parser* p, DOMDocument* doc) {
    if (next_char(p) != '<') {
        return NULL;
    }
    
    // Handle comments
    if (p->pos + 2 < p->length && p->input[p->pos] == '!' && p->input[p->pos + 1] == '-' && p->input[p->pos + 2] == '-') {
        // Skip comment
        p->pos += 3;
        while (p->pos + 2 < p->length) {
            if (p->input[p->pos] == '-' && p->input[p->pos + 1] == '-' && p->input[p->pos + 2] == '>') {
                p->pos += 3;
                break;
            }
            p->pos++;
        }
        return NULL;
    }
    
    // Handle DOCTYPE
    if (p->pos < p->length && p->input[p->pos] == '!') {
        // Skip DOCTYPE
        while (peek_char(p) != '>' && peek_char(p) != '\0') {
            next_char(p);
        }
        if (peek_char(p) == '>') {
            next_char(p);
        }
        return NULL;
    }
    
    char* tag_name = read_tag_name(p);
    if (!tag_name) {
        return NULL;
    }
    
    DOMElement* element = dom_document_create_element(doc, tag_name);
    free(tag_name);
    
    if (!element) {
        return NULL;
    }
    
    // Parse attributes
    parse_attributes(p, element);
    
    skip_whitespace(p);
    
    // Check for self-closing tag
    if (peek_char(p) == '/') {
        next_char(p); // consume '/'
        if (peek_char(p) == '>') {
            next_char(p); // consume '>'
        }
        return element;
    }
    
    if (peek_char(p) == '>') {
        next_char(p); // consume '>'
    }
    
    // Check for void elements (self-closing in HTML5)
    const char* tag = dom_element_get_tag_name(element);
    if (tag && (strcmp(tag, "br") == 0 || strcmp(tag, "hr") == 0 || 
                strcmp(tag, "img") == 0 || strcmp(tag, "input") == 0 ||
                strcmp(tag, "meta") == 0 || strcmp(tag, "link") == 0)) {
        return element;
    }
    
    // Parse children
    parse_children(p, doc, element);
    
    // Parse closing tag
    skip_whitespace(p);
    if (peek_char(p) == '<') {
        next_char(p); // consume '<'
        if (peek_char(p) == '/') {
            next_char(p); // consume '/'
            char* closing_tag = read_tag_name(p);
            free(closing_tag); // We trust it matches for now
            skip_whitespace(p);
            if (peek_char(p) == '>') {
                next_char(p); // consume '>'
            }
        }
    }
    
    return element;
}

int html_parser_parse(DOMDocument* document, const char* html) {
    if (!document || !html) {
        return -1;
    }
    
    Parser parser;
    parser.input = html;
    parser.pos = 0;
    parser.length = strlen(html);
    
    // Parse all top-level elements
    while (parser.pos < parser.length) {
        skip_whitespace(&parser);
        
        if (peek_char(&parser) == '\0') {
            break;
        }
        
        if (peek_char(&parser) == '<') {
            DOMElement* element = parse_element(&parser, document);
            if (element) {
                // If this is the first real element, make it the document element
                if (!dom_document_get_element(document)) {
                    // Already set as document element in dom_document_create_element
                }
            }
        } else {
            // Skip unexpected text at top level
            parser.pos++;
        }
    }
    
    return 0;
}
