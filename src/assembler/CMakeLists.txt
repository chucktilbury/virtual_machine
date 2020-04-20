project(assembler)

add_executable(${PROJECT_NAME}
    assembler.c
    hash_table.c
    parse_code_section.c
    parse_include.c
    parse_section.c
    scanner_comment.c
    scanner_quote.c
    scanner_symbol.c
    sections.c
    expressions.c
    parse_data_section.c
    parser.c
    scanner.c
    scanner_number.c
    scanner_stopper.c
    scanner_tok_str.c
    keyword_map.c
)

set(KEYWORD_MAP ${CMAKE_CURRENT_SOURCE_DIR}/keyword_map.c)
set(TOKENS ${CMAKE_CURRENT_SOURCE_DIR}/tokens.h)

target_link_libraries(${PROJECT_NAME}
    common
)

add_custom_command(OUTPUT ${KEYWORD_MAP}
    DEPENDS ${TOKENS}
    PRE_BUILD
    COMMAND ../tools/gen_keyword_map.py -i tokens.h -o keyword_map.c
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/../include
        ${PROJECT_SOURCE_DIR}/../common
)

target_compile_options(${PROJECT_NAME} PRIVATE "-Wall" "-Wextra" "-g" "-DTRACE")

set_property(DIRECTORY PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/keyword_map.c"
)
