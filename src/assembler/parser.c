
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include "scanner.h"
#include "errors.h"
#include "parse_section.h"
#include "parse_include.h"

/*
 * Top level entry point for the parse. Expects that a file is already open and the scanner
 * has been initialized.
 */
void parse_all(void)
{

    int tok;
    char str[50];
    int finished = 0;

    while(!finished)
    {
        tok = scanner_get_token(str, sizeof(str));
        switch (tok)
        {
            case TOK_END_INPUT:
                finished++; // only legal place for an end of file
                break;
            case TOK_INCLUDE:
                finished = parse_include();
                break;
            case TOK_CODE:
                finished = parse_code_section();
                break;
            case TOK_DATA:
                finished = parse_data_section();
                break;
            default:
                expect("an include or section", tok);
                finished++;
                break;
        }
    }
}
