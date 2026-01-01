# Simple tests for basic functionality
add_executable(test_dom
    test_dom.c
)

target_link_libraries(test_dom
    just-browse-core
)

add_test(NAME DOMTest COMMAND test_dom)

# Engine integration test
add_executable(test_engine
    test_engine.c
)

target_link_libraries(test_engine
    just-browse-core
)

add_test(NAME EngineTest COMMAND test_engine)

# JavaScript integration test
add_executable(test_js_integration
    test_js_integration.c
)

target_link_libraries(test_js_integration
    just-browse-core
)

add_test(NAME JSIntegrationTest COMMAND test_js_integration)

# HTML parser test
add_executable(test_html_parser
    test_html_parser.c
)

target_link_libraries(test_html_parser
    just-browse-core
)

add_test(NAME HTMLParserTest COMMAND test_html_parser)
