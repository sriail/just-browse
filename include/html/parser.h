#ifndef JUST_BROWSE_HTML_PARSER_H
#define JUST_BROWSE_HTML_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "dom/dom.h"

/**
 * Parse HTML string and create DOM tree
 * @param document The document to populate
 * @param html The HTML string to parse
 * @return 0 on success, -1 on failure
 */
int html_parser_parse(DOMDocument* document, const char* html);

#ifdef __cplusplus
}
#endif

#endif // JUST_BROWSE_HTML_PARSER_H
