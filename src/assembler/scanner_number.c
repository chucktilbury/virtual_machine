#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "scanner.h"
#include "errors.h"

/*
 * When this is entered, the "0x" has been scanned.
 */
static token_t get_unsigned_literal(char* str, size_t size)
{
    token_t tok = TOK_UNUM_LITERAL;
    int finished = 0;

    while(!finished)
    {
        int ch = get_char();

        if(isxdigit(ch))
        {
            add_char(ch, str, size);
        }
        else
        {
            switch (get_char_type(ch))
            {
                case WHITE_SP_CHAR:
                case STOPPER_CHAR:
                case NEWLINE_CHAR:
                case END_INPUT:
                    unget_char(ch);
                    finished++;
                    break;
                default:
                    fprintf(stderr, "ERROR: malformed hex number\n");
                    unget_char(ch);
                    finished++;
            }
        }
    }

    return tok;
}

/*
 * When this is entered, part of the number has already been scanned. This is entered when
 * a '.' is scanned by get_signed_or_float().
 */
static token_t get_float(char* str, size_t size)
{
    token_t tok = TOK_FNUM_LITERAL;
    int finished = 0;
    int state = 0;

    while(!finished)
    {
        int ch = get_char();

        if(isdigit(ch))
            add_char(ch, str, size);
        else
        {
            switch (ch)
            {
                case 'e':
                case 'E':
                    if(state != 0)
                    {
                        scanner_error("malformed floating point number\n");
                        return TOK_ERROR;
                    }
                    else
                    {
                        add_char(ch, str, size);
                        state = 1;
                    }
                    break;
                case '-':
                case '+':
                    if(state != 1)
                    {
                        scanner_error("malformed floating point number\n");
                        return TOK_ERROR;
                    }
                    else
                    {
                        add_char(ch, str, size);
                        state = 2;
                    }
                    break;
                default:
                    unget_char(ch);
                    finished++;
                    break;
            }
        }
    }
    return tok;
}

/*
 * Don't know if we have a signed int or a float. If we hit a '.', then switch to a float.
 */
static token_t get_signed_or_float(char* str, size_t size)
{
    token_t tok = TOK_INUM_LITERAL;
    int finished = 0;

    while(!finished)
    {
        int ch = get_char();

        if(isdigit(ch))
        {
            add_char(ch, str, size);
        }
        else if(ch == '.')
        {
            add_char(ch, str, size);
            tok = get_float(str, size);
        }
        else
        {
            unget_char(ch);
            finished++;
        }
    }
    return tok;
}

/*
 * Scan a number from the input stream. When this function is entered, the number type could
 * have the form of an integer that has the form of "[0-9]+", an unsigned number that is given
 * in hex and starts with "0[xX][0-9A-Fa-f]+", or a floating point number that has a dot in it
 * and optionally has a power in the form of "[0-9]+\.[0-9]+([eE]+[-+]?[0-9]+)?".
 */
token_t scanner_number(char* str, size_t size)
{
    token_t tok = TOK_ERROR;

    int ch = get_char();

    switch (ch)
    {
        case '0':
            add_char(ch, str, size);
            ch = get_char();
            switch (ch)
            {
                case 'x':
                case 'X':
                    add_char(ch, str, size);
                    tok = get_unsigned_literal(str, size);
                    break;
                default:
                    unget_char(ch);
                    tok = get_signed_or_float(str, size);
                    break;
            }
            break;
        default:
            unget_char(ch);
            tok = get_signed_or_float(str, size);
            break;

    }
    return tok;
}
