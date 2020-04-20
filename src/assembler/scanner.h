#ifndef __SCANNER_H__
#  define __SCANNER_H__

#  include "tokens.h"

typedef enum char_types_t
{
    INVALID_CHAR,
    NUMBERIC_CHAR,
    SYMBOL_CHAR,
    SQUOTE_CHAR,
    DQUOTE_CHAR,
    HASH_CHAR,
    WHITE_SP_CHAR,
    NEWLINE_CHAR,
    STOPPER_CHAR,
    END_INPUT,
} _char_types_t;

// character giving the end of input
#  define END_OF_INPUT    255

void scanner_init(void);
token_t scanner_get_token(char* str, size_t size);
int scanner_open_infile(char* fname);
int scanner_get_column(void);
int scanner_get_line(void);
const char* scanner_get_file_name(void);
void add_char(int ch, char* str, size_t size);
int get_char_type(int ch);
int get_char(void);
void unget_char(int ch);
const char* scanner_tok_str(int tok);
void scanner_unget_token(token_t tok);

#endif
