#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "scanner.h"
#include "keyword_map.h"
#include "errors.h"

static void str_lower(char* buf, size_t size, const char* str)
{
    memset(buf, 0, size);
    for(int i = 0; str[i] != 0 && i < size; i++)
        buf[i] = tolower(str[i]);
}

/*
 * Search the keyword map.
 */
static int binary_search(const char* keyword)
{

    int first = 0, last = num_keywords - 1;
    int index = (first + last) / 2;
    char buffer[50];

    str_lower(buffer, sizeof(buffer), keyword);
    while(first <= last)
    {
        if(strcmp(keyword_map[index].str, buffer) < 0)
            first = index + 1;
        else if(strcmp(keyword_map[index].str, buffer) > 0)
            last = index - 1;
        else
            return index;

        index = (first + last) / 2;
    }

    return -1;  // not found
}

/*
 * When this is entered, a character has been read that is acceptible in a
 * symbol. It is unknown if the symbol will be a keyword or the name of
 * something that is user defined. That is resolved after the word has been
 * isolated.
 */
token_t scanner_symbol(char* str, size_t size)
{
    int finished = 0;
    token_t tok = TOK_ERROR;

    while(!finished)
    {
        int ch = get_char();

        switch (get_char_type(ch))
        {
            case SYMBOL_CHAR:
            case NUMBERIC_CHAR:
                add_char(ch, str, size);
                break;
            case END_INPUT:
            default:
                unget_char(ch);
                finished++;
                break;
        }
    }

    int index = binary_search(str);

    if(index < 0)
        tok = TOK_IDENTIFIER;
    else
        tok = keyword_map[index].token;

    return tok;
}
