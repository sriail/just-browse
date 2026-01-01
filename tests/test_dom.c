#include "dom/dom.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void test_document_creation() {
    printf("Testing document creation...\n");
    DOMDocument* doc = dom_document_create();
    assert(doc != NULL);
    dom_document_destroy(doc);
    printf("  PASSED\n");
}

void test_element_creation() {
    printf("Testing element creation...\n");
    DOMDocument* doc = dom_document_create();
    assert(doc != NULL);

    DOMElement* elem = dom_document_create_element(doc, "div");
    assert(elem != NULL);
    assert(strcmp(dom_element_get_tag_name(elem), "div") == 0);

    dom_document_destroy(doc);
    printf("  PASSED\n");
}

void test_attributes() {
    printf("Testing element attributes...\n");
    DOMDocument* doc = dom_document_create();
    assert(doc != NULL);

    DOMElement* elem = dom_document_create_element(doc, "div");
    assert(elem != NULL);

    // Set attribute
    assert(dom_element_set_attribute(elem, "id", "test-id") == 0);
    assert(dom_element_set_attribute(elem, "class", "test-class") == 0);

    // Get attribute
    const char* id = dom_element_get_attribute(elem, "id");
    assert(id != NULL);
    assert(strcmp(id, "test-id") == 0);

    const char* class_attr = dom_element_get_attribute(elem, "class");
    assert(class_attr != NULL);
    assert(strcmp(class_attr, "test-class") == 0);

    // Non-existent attribute
    const char* missing = dom_element_get_attribute(elem, "missing");
    assert(missing == NULL);

    dom_document_destroy(doc);
    printf("  PASSED\n");
}

void test_inner_html() {
    printf("Testing innerHTML...\n");
    DOMDocument* doc = dom_document_create();
    assert(doc != NULL);

    DOMElement* elem = dom_document_create_element(doc, "div");
    assert(elem != NULL);

    assert(dom_element_set_inner_html(elem, "<p>Test content</p>") == 0);

    dom_document_destroy(doc);
    printf("  PASSED\n");
}

int main() {
    printf("Running DOM tests...\n\n");

    test_document_creation();
    test_element_creation();
    test_attributes();
    test_inner_html();

    printf("\nAll DOM tests passed!\n");
    return 0;
}
