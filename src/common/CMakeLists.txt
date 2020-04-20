project(common)

add_library(${PROJECT_NAME} STATIC
    errors.c
    array_manager.c
    hash_table.c
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/../include
        ${PROJECT_SOURCE_DIR}/../assembler
)

target_compile_options(${PROJECT_NAME} PRIVATE "-Wall" "-g" "-DTRACE")
