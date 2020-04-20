
/*
 * Scanner for the assembler.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include "scanner.h"
#include "scanner_symbol.h"
#include "scanner_comment.h"
#include "scanner_number.h"
#include "scanner_stopper.h"
#include "scanner_quote.h"

typedef struct file_stack_t
{
    FILE* fp;
    const char* name;
    int line_no;
    int col_no;
    struct file_stack_t* next;
} _file_stack_t;

static _file_stack_t* file_stack;
static char char_type_table[256];
static token_t unget_token = -1;

void add_char(int ch, char* str, size_t size)
{
    int len = strlen(str);

    if(len + 1 > size)
    {
        fprintf(stderr, "scanner buffer overrun!\n");
        exit(1);
    }
    str[len] = (char)ch;
}

int get_char_type(int ch)
{
    return char_type_table[ch];
}

int get_char(void)
{
    int ch = END_OF_INPUT;

    if(file_stack != NULL)
    {
        ch = fgetc(file_stack->fp);
        if(ch == '\n')
        {
            file_stack->line_no++;
            file_stack->col_no = 0;
        }
        else if(ch == EOF)
        {
            _file_stack_t* fs = file_stack;

            file_stack = fs->next;
            fclose(fs->fp);
            free((void *)fs->name);
            free(fs);

            // return the next char from the previous file in the stack or
            // end of input if the file stack os NULL.
            ch = get_char();
        }
        else
            file_stack->col_no++;
    }

    return ch;
}

void unget_char(int ch)
{
    ungetc(ch, file_stack->fp);
}

// this is quick and dirty. May need to add the buffer at some point, but as it stands, I only need
// to unget single character syntax.
void scanner_unget_token(token_t tok) {
    unget_token = tok;
}

/****************
 * Public interface
 */
token_t scanner_get_token(char* str, size_t size)
{
    int ch, finished = 0;
    token_t token;

    memset(str, 0, size);

    if(unget_token > 0) {
        token = unget_token;
        unget_token = -1;
        return token;
    }

    while(!finished)
    {
        ch = get_char();
        switch (get_char_type(ch))
        {
            case NEWLINE_CHAR:
            case WHITE_SP_CHAR:
                break;
            case HASH_CHAR:
                scanner_comment();
                break;
            case END_INPUT:
                token = TOK_END_INPUT;
                strncpy(str, "end of input", size);
                finished++;
                break;
            case SQUOTE_CHAR:
                token = scanner_squote(str, size);
                finished++;
                break;
            case DQUOTE_CHAR:
                token = scanner_dquote(str, size);
                finished++;
                break;
            case SYMBOL_CHAR:
                add_char(ch, str, size);
                token = scanner_symbol(str, size);
                finished++;
                break;
            case NUMBERIC_CHAR:
                unget_char(ch);
                token = scanner_number(str, size);
                finished++;
                break;
            case STOPPER_CHAR:
                unget_char(ch);
                token = scanner_stopper(str, size);
                finished++;
                break;
            default:
                fprintf(stderr, "unrecognized character in input: %02X\n", ch);
                finished++;
                token = TOK_ERROR;
                break;
        }
    }
    return token;
}

void scanner_init(void)
{

    for(int i = 0; i < 256; i++)
        char_type_table[i] = INVALID_CHAR;

    for(int i = '0'; i < '9'; i++)
        char_type_table[i] = NUMBERIC_CHAR;

    for(int i = 'a'; i <= 'z'; i++)
        char_type_table[i] = SYMBOL_CHAR;

    for(int i = 'A'; i <= 'Z'; i++)
        char_type_table[i] = SYMBOL_CHAR;

    char* str = "+-*/=&|^~<>[]{}(),.;";

    for(int i = 0; str[i] != 0; i++)
        char_type_table[(int)str[i]] = STOPPER_CHAR;

    str = " \t\r";
    for(int i = 0; str[i] != 0; i++)
        char_type_table[(int)str[i]] = WHITE_SP_CHAR;

    str = "@!$%_?";
    for(int i = 0; str[i] != 0; i++)
        char_type_table[(int)str[i]] = SYMBOL_CHAR;

    char_type_table['\''] = SQUOTE_CHAR;
    char_type_table['\"'] = DQUOTE_CHAR;
    char_type_table['\n'] = NEWLINE_CHAR;
    char_type_table['#'] = HASH_CHAR;
    char_type_table[END_OF_INPUT] = END_INPUT;
}

int scanner_open_infile(char* fname)
{
    _file_stack_t* file;

    if(NULL == (file = calloc(1, sizeof(_file_stack_t))))
    {
        fprintf(stderr, "FATAL: cannot allocate memory for file stack\n");
        exit(1);
    }

    if(NULL == (file->name = strdup(fname)))
    {
        fprintf(stderr, "FATAL: cannot allocate memory for file name\n");
        free(file);
        exit(1);
    }

    file->fp = fopen(fname, "r");
    if(file->fp == NULL)
    {
        fprintf(stderr, "ERROR: cannot open input file: \"%s\": %s\n", fname, strerror(errno));
        free((void *)file->name);
        free(file);
        return 1;
    }

    file->line_no = 0;
    file->col_no = 1;

    if(file_stack == NULL)
        file_stack = file;
    else
    {
        file->next = file_stack;
        file_stack = file;
    }

    return 0;
}

int scanner_get_column(void)
{
    if(file_stack != NULL)
        return file_stack->col_no;
    else
        return -1;
}

int scanner_get_line(void)
{
    if(file_stack != NULL)
        return file_stack->line_no;
    else
        return -1;
}

const char* scanner_get_file_name(void)
{
    if(file_stack != NULL)
        return file_stack->name;
    else
        return "no file open";
}
