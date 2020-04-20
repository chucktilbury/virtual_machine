#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "scanner.h"
#include "errors.h"

// This will grow as the assembler matures. It will have a way to interrogate the environment and
// other sources

int parse_include(void)
{
    char buffer[128];

    int tok = scanner_get_token(buffer, sizeof(buffer));

    if(tok == TOK_STRING_LITERAL)
    {
        printf("including \"%s\"\n", buffer);
    }
    else
    {
        expect("string literal for include", tok);
    }
    return 0;
}
