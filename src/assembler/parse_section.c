#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "scanner.h"
#include "errors.h"

/*
 * There are two kinds of section. CODE and DATA. A DATA section is where data structures are defined and
 * CODE is where the instructions are defined. The purpose of this module is to split out for the two different
 * kinds of sections.
 *
 * The names of the sections are used when referencing it. In the end, all references are simply indexes into the
 * contiguous section and the names are thrown away if no debugging information is being stored.
 */

/*
 * The CODE keyword has already been scanned.
 */
int parse_code_section(void)
{
    char buffer[128];

    int tok = scanner_get_token(buffer, sizeof(buffer));
    switch(tok) {
        case TOK_CODE:
            break;
        case TOK_DATA:
            break;
    }

    return 0;
}

/*
 * The DATA keyword has already been scanned.
 */
int parse_data_section(void)
{
    char buffer[128];

    int tok = scanner_get_token(buffer, sizeof(buffer));
    switch(tok) {
        case TOK_CODE:
            break;
        case TOK_DATA:
            break;
    }

    return 0;
}
