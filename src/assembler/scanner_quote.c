#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "scanner.h"
#include "errors.h"

/*
 * Scan a string that may contain escape characters. The escape characters are converted to their
 * binary form and inserted into the string. Escapes can also have the form of "\xNN" where NN are
 * hex digits.
 */
token_t scanner_dquote(char* str, size_t size)
{
    int finished = 0;
    int ch;
    char buffer[3];          // used when scanning a hex escape
    int buf_idx = 0;
    token_t tok = TOK_STRING_LITERAL;

    while(!finished)
    {
        ch = get_char();
        if(ch == '\"')
            finished++;
        else if(ch == '\n' || ch == END_INPUT)
        {
            scanner_error("unterminated string");
            finished++;
            tok = TOK_ERROR;
        }
        else
        {
            if(ch == '\\')
            {
                ch = get_char();
                switch (ch)
                {
                    case 'a':
                        add_char(0x07, str, size);
                        break;
                    case 'b':
                        add_char(0x08, str, size);
                        break;
                    case 'e':
                        add_char(0x1b, str, size);
                        break;
                    case 'f':
                        add_char(0x0c, str, size);
                        break;
                    case 'n':
                        add_char(0x0a, str, size);
                        break;
                    case 'r':
                        add_char(0x0d, str, size);
                        break;
                    case 't':
                        add_char(0x09, str, size);
                        break;
                    case 'v':
                        add_char(0x0b, str, size);
                        break;
                    case '\\':
                        add_char(0x5c, str, size);
                        break;
                    case '\"':
                        add_char(0x22, str, size);
                        break;
                    case 'x':
                    case 'X':
                        memset(buffer, 0, sizeof(buffer));
                        for(buf_idx = 0; buf_idx < 2; buf_idx++)
                        {
                            ch = get_char();
                            if(isxdigit(ch))
                                buffer[buf_idx] = ch;
                            else
                            {
                                // add_char(ch, str, size);
                                unget_char(ch);
                                scanner_error("malformed hex code in string: %s", str);
                                // finished++;
                                tok = TOK_ERROR;
                                break;
                            }
                        }
                        add_char((char)strtol(buffer, NULL, 16), str, size);
                        break;

                    default:
                        add_char(ch, str, size);
                        break;
                }
            }
            else
                add_char(ch, str, size);
        }
    }
    return tok;
}

/*
 * Scan a string with no character translation. The string is copied literally into the buffer.
 */
token_t scanner_squote(char* str, size_t size)
{
    int finished = 0;
    int ch;
    token_t tok = TOK_STRING_LITERAL;

    while(!finished)
    {
        ch = get_char();
        if(ch == '\'')
            finished++;
        else if(ch == '\n' || ch == END_INPUT)
        {
            scanner_error("unterminated string");
            finished++;
            tok = TOK_ERROR;
        }
        else
            add_char(ch, str, size);
    }
    return tok;
}
