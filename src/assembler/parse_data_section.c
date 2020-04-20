#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#include "assembler.h"
#include "scanner.h"
#include "errors.h"
#include "sections.h"

// This allows easy translation from an actual data
// type to a stream of bytes to write to the section.
typedef union
{
    uint8_t* uchars;
    uint16_t* ushorts;
    uint32_t* uints;
    uint64_t* ulongs;
    int8_t* chars;
    int16_t* shorts;
    int32_t* ints;
    int64_t* longs;
    double* floats;
} __attribute__((packed)) data_union_t;

// This tracks the section entry as we build it up.
typedef struct
{
    char name[MAX_SYMBOL];
    char sec_name[MAX_SYMBOL];
    int type;
    size_t each_item;
    size_t total;
    int nitems;
    data_union_t data;
} data_section_entry_t;

data_section_entry_t data_entry;

/*
 * There are two kinds of section. CODE and DATA. A DATA section is where data structures are defined and
 * CODE is where the instructions are defined. The purpose of this module is to split out for the two different
 * kinds of sections.
 *
 * The names of the sections are used when referencing it. In the end, all references are simply indexes into the
 * contiguous section and the names are thrown away if no debugging information is being stored.
 */

static void allocate_buffer(void)
{
    size_t size;

    switch (data_entry.type)
    {
        case TYPE_INT8:
        case TYPE_UINT8:
            size = sizeof(uint8_t);
            break;

        case TYPE_INT16:
        case TYPE_UINT16:
            size = sizeof(uint16_t);
            break;

        case TYPE_INT32:
        case TYPE_UINT32:
            size = sizeof(uint32_t);
            break;

        case TYPE_INT64:
        case TYPE_UINT64:
            size = sizeof(uint64_t);
            break;

        case TYPE_FLOAT:
            size = sizeof(double);
            break;
    }

    data_entry.data.chars = (int8_t *) calloc(data_entry.nitems, size);
    if(data_entry.data.chars == NULL)
        fatal_error("cannot allocate %d bytes for section entry", data_entry.nitems * size);

}

static void write_entry(void)
{
    // write the section to the database
    add_section_entry(data_entry.sec_name, data_entry.name);
    add_entry_bytes(data_entry.sec_name, data_entry.name, (void *)&data_entry.data, data_entry.total);
}

/*
 * Assignment count be a number, a string, a list of expressions, or a single
 * expression. This has to make sure that the assignment result and the type
 * are compatible.
 */
static int do_assignment(void)
{
    MARK();
    return 0;
}

/*
 * A subscript has the form "[number]". Expressions are also allowed.
 */
static int do_subscript(void)
{
    MARK();
    char buffer[MAX_SYMBOL];
    int tok = scanner_get_token(buffer, sizeof(buffer));

    switch (tok)
    {
        case TOK_UNUM_LITERAL:
            data_entry.nitems = (int)strtol(buffer, NULL, 16);
            if((errno == ERANGE && (data_entry.nitems == (int)LONG_MAX || data_entry.nitems == (int)LONG_MIN)) ||
               (errno != 0 && data_entry.nitems == 0))
            {
                syntax("cannot convert subscript \"%s\" to number: %s", buffer, strerror(errno));
                return 1;
            }
            allocate_buffer();
            break;
        case TOK_INUM_LITERAL:
            data_entry.nitems = (int)strtol(buffer, NULL, 10);
            if((errno == ERANGE && (data_entry.nitems == (int)LONG_MAX || data_entry.nitems == (int)LONG_MIN)) ||
               (errno != 0 && data_entry.nitems == 0))
            {
                syntax("cannot convert subscript \"%s\" to number: %s", buffer, strerror(errno));
                return 1;
            }
            allocate_buffer();
            break;
        default:
            expect("an integer or an unsigned", tok);
            return 1;
    }

    tok = scanner_get_token(buffer, sizeof(buffer));
    if(tok != TOK_CSQUARE)
    {
        expect("a close square brace", tok);
        return 1;
    }

    tok = scanner_get_token(buffer, sizeof(buffer));
    switch (tok)
    {
        case TOK_SEMICOLON:
            return 0;   // finished with the definition
        case TOK_EQUAL:
            return do_assignment();
        default:
            expect("semicolon or assignment", tok);
            return 1;
    }

    return 0;   // unreachable
}

static int do_data_definition(int type)
{
    MARK();
    char buffer[MAX_SYMBOL];

    data_entry.type = type;
    int tok = scanner_get_token(buffer, sizeof(buffer));

    if(tok != TOK_IDENTIFIER)
    {
        expect("an identifier", tok);
        return 1;
    }
    else
    {
        strncpy(data_entry.name, buffer, MAX_SYMBOL);

        // store the identifier and connect it to the 64 bit data element
        int tok = scanner_get_token(buffer, sizeof(buffer));

        if(tok == TOK_OSQUARE)
        {
            // parse a subscript with possible assignment
            return do_subscript();
        }
        else if(tok == TOK_EQUAL)
        {
            // parse an assignment expression
            return do_assignment();
        }
        else if(tok == TOK_SEMICOLON)
        {
            // finished the definition with no initializer
            return 0;
        }
        else
        {
            expect("a subscript, assignment, or semicolon", tok);
            return 1;
        }
    }

    return 0;
}

/*
 * The DATA keyword has already been scanned.
 */
int parse_data_section(void)
{
    MARK();
    char buffer[MAX_SYMBOL];
    int finished = 0;
    int state = 0;

    // get the contents of the data section
    while(!finished)
    {
        int tok = scanner_get_token(buffer, sizeof(buffer));

        switch (state)
        {
            case 0:
                switch (tok)
                {
                    case TOK_IDENTIFIER:
                        // create the new section and save the identifier
                        strncpy(data_entry.sec_name, buffer, MAX_SYMBOL);
                        add_section(data_entry.sec_name, SEC_TYPE_DATA);
                        printf("add section: %s\n", buffer);
                        state = 1;
                        break;
                    default:
                        expect("a name for the section", tok);
                        finished++;
                        break;
                }
                break;
            case 1:
                switch (tok)
                {
                    case TOK_INT8:
                        finished = do_data_definition(TYPE_INT8);
                        break;
                    case TOK_INT16:
                        finished = do_data_definition(TYPE_INT16);
                        break;
                    case TOK_INT32:
                        finished = do_data_definition(TYPE_INT32);
                        break;
                    case TOK_INT64:
                        finished = do_data_definition(TYPE_INT64);
                        break;
                    case TOK_UINT8:
                        finished = do_data_definition(TYPE_UINT8);
                        break;
                    case TOK_UINT16:
                        finished = do_data_definition(TYPE_UINT16);
                        break;
                    case TOK_UINT32:
                        finished = do_data_definition(TYPE_UINT32);
                        break;
                    case TOK_UINT64:
                        finished = do_data_definition(TYPE_UINT64);
                        break;
                    case TOK_FLOAT:
                        finished = do_data_definition(TYPE_FLOAT);
                        break;
                    case TOK_END_INPUT:
                        expect("a data definition or end of section", tok);
                    case TOK_END_SEC:
                        finished++;
                        break;
                    default:
                        expect("a data type", tok);
                        finished++;
                        break;
                }

                if(get_num_errors() == 0)
                {
                    write_entry();
                    if(data_entry.data.chars != NULL)
                        free((void *)data_entry.data.chars);
                }
                else
                    finished++;

                break;
            default:
                syntax("unreachable code while parsing a data section");
                exit(1);
                break;
        }
    }

    return 0;
}
