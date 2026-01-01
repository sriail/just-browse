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
