#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "scanner.h"
#include "errors.h"

/*
 * Before this function is entered, the character was un-gotten, so the first character read is likely
 * to be the whole token. Tokens recognized are : "+-*=/&|^~<>[]{}(),."
 */
token_t scanner_stopper(char* str, size_t size)
{
    token_t tok = TOK_ERROR;
    int ch;

    ch = get_char();
    switch (ch)
    {
        case '+':
            add_char(ch, str, size);
            return TOK_PLUS;
            break;
        case '-':
            add_char(ch, str, size);
            return TOK_MINUS;
            break;
        case '*':
            add_char(ch, str, size);
            return TOK_STAR;
            break;
        case '=':
            add_char(ch, str, size);
            return TOK_EQUAL;
            break;
        case '/':
            add_char(ch, str, size);
            return TOK_SLASH;
            break;
        case '&':
            add_char(ch, str, size);
            return TOK_BWAND;
            break;
        case '|':
            add_char(ch, str, size);
            return TOK_BWOR;
            break;
        case '^':
            add_char(ch, str, size);
            return TOK_BWXOR;
            break;
        case '~':
            add_char(ch, str, size);
            return TOK_BWNOT;
            break;
        case ',':
            add_char(ch, str, size);
            return TOK_COMMA;
            break;
        case '.':
            add_char(ch, str, size);
            return TOK_PERIOD;
            break;
        case '[':
            add_char(ch, str, size);
            return TOK_OSQUARE;
            break;
        case ']':
            add_char(ch, str, size);
            return TOK_CSQUARE;
            break;
        case '{':
            add_char(ch, str, size);
            return TOK_OCURLY;
            break;
        case '}':
            add_char(ch, str, size);
            return TOK_CCURLY;
            break;
        case '(':
            add_char(ch, str, size);
            return TOK_OPAREN;
            break;
        case ')':
            add_char(ch, str, size);
            return TOK_CPAREN;
            break;
        case '<':
            add_char(ch, str, size);
            return TOK_BWSHL;
            break;
        case '>':
            add_char(ch, str, size);
            return TOK_BWSHR;
            break;
        case ';':
            add_char(ch, str, size);
            return TOK_SEMICOLON;
            break;

        default:
            scanner_error("unreachable code has been reached in do_stopper();");
            return TOK_ERROR;
            break;
    }
    return tok;
}
