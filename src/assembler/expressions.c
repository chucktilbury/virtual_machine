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

/*
 * This expression evaluator is for data definition expressions only. Expressions
 * that involve pointers for instructions are evaluated elsewhere.
 *
 * An expression can be a number or a simple arithmetic expression consisting of
 * add, subtract, multiply, or divide operations. The binary operations are also
 * permitted on non-float values. The value of each term in the expression must
 * be a constant value. No symbolic values or registers are allowed in these
 * expressions.
 *
 * After the expression is evaluated, the resulting value is stored in a data
 * structure which can be retrieved by the data_section code.
 *
 * The first token that is read is expected to be a number.
 *
 * This implements the shunting yard algorithm.
 */

// operator precidences, where higher precidence is a higher number
enum
{
    BOR = 1,
    BXOR = 2,
    BAND = 3,
    BSHL = 4,
    BSHR = 4,

    ADD = 5,
    SUB = 5,

    MUL = 6,
    DIV = 6,

    BUNOT = 7,
    UMINUS = 7,
    UPLUS = 7,

    OPAR = 8,
    CPAR = 8,
};

typedef struct
{
    int token;
    int prec;
    union
    {
        uint64_t unum;
        int64_t inum;
        double fnum;
    } __attribute__((packed)) value;
} stack_item_t;

typedef struct
{
    stack_item_t* stack;
    size_t size;
    size_t capacity;
    int top;
} value_stack_t;

static value_stack_t oper_stack = {.stack = NULL,.size = 0,.capacity = 1,.top = 0 };
static value_stack_t out_stack = {.stack = NULL,.size = 0,.capacity = 1,.top = 0 };

static void init_stacks(void)
{
    if(oper_stack.stack != NULL)
        free(oper_stack.stack);

    if(out_stack.stack != NULL)
        free(out_stack.stack);

    oper_stack.size = 0;
    oper_stack.top = 0;
    oper_stack.capacity = 1;
    oper_stack.stack = NULL;

    out_stack.size = 0;
    out_stack.top = 0;
    out_stack.capacity = 1;
    out_stack.stack = NULL;
}

static void grow_stack(value_stack_t * stack)
{
    if(stack->size + 2 > stack->capacity)
        stack->capacity = stack->capacity << 1;

    stack->stack = (stack_item_t *) realloc(stack->stack, stack->capacity * sizeof(stack_item_t));
    if(stack->stack == NULL)
        fatal_error("cannot allocate %lu bytes for expression stack", stack->capacity * sizeof(stack_item_t));
}

static void push(value_stack_t * stack, stack_item_t * val)
{
    grow_stack(stack);
    memcpy((void *)&stack->stack[stack->top], (void *)val, sizeof(stack_item_t));
    stack->top++;
}

static stack_item_t* pop(value_stack_t * stack)
{
    stack->top--;
    if(stack->top < 0)
    {
        stack->top = 0;
        // syntax("expression stack underrun");
        return NULL;
    }
    return &stack->stack[stack->top];
}

static stack_item_t* peek(value_stack_t * stack)
{
    if(stack->top <= 0)
        return &stack->stack[0];
    else
        return &stack->stack[stack->top-1];
}

// returns 1 if the parsing should stop; for example if there is an error. Otherwise, return 0.
int parse_expression(void)
{
    stack_item_t item;
    int finished = 0;
    int tok;
    char buffer[MAX_SYMBOL];
    // this is set when the item read was an operator and cleared when it's not.
    // It's used to detect if an operator is unary.
    int flag = 1;

    init_stacks();

    while(!finished) {
        tok = scanner_get_token(buffer, sizeof(buffer));
        // fill in the item
        switch(tok) {
            case TOK_UNUM_LITERAL:
                item.token = tok;
                item.value.unum = strtol(buffer, NULL, 16);
                item.prec = 0;
                push(&out_stack, &item);
                flag = 0;
                break;
            case TOK_INUM_LITERAL:
                item.token = tok;
                item.value.inum = strtol(buffer, NULL, 10);
                item.prec = 0;
                push(&out_stack, &item);
                flag = 0;
                break;
            case TOK_FNUM_LITERAL:
                item.token = tok;
                item.value.fnum = strtod(buffer, NULL);
                item.prec = 0;
                push(&out_stack, &item);
                flag = 0;
                break;

            case TOK_OPAREN:
                item.token = tok;
                item.value.unum = 0;
                item.prec = OPAR;
                push(&out_stack, &item);
                flag = 1;
                break;
            case TOK_CPAREN:
                break;

            case TOK_PLUS:
                if(flag) {
                    while(peek(&oper_stack)->prec > UPLUS) {
                    }
                }
                else {
                    while(peek(&oper_stack)->prec > ADD) {
                    }
                }
                break;
            case TOK_MINUS:
                if(flag) {
                    while(peek(&oper_stack)->prec > UMINUS) {
                    }
                }
                else {
                    while(peek(&oper_stack)->prec > SUB) {
                    }
                }
                break;
            case TOK_BWNOT:
                if(flag) {
                    while(peek(&oper_stack)->prec > BUNOT) {
                    }
                }
                else {
                    // not unary star is a syntax error
                    expect("number or a semicolon", tok);
                    return 1;
                }
                break;

            case TOK_STAR:
                if(flag) {
                    // unary star is a syntax error
                    expect("number or a semicolon", tok);
                    return 1;
                }
                else {
                    while(peek(&oper_stack)->prec > MUL) {
                    }
                }
                break;
            case TOK_SLASH:
                if(flag) {
                    // unary slash is a syntax error
                    expect("number or a semicolon", tok);
                    return 1;
                }
                else {
                    while(peek(&oper_stack)->prec > DIV) {
                    }
                }
                break;

            case TOK_BWSHL:
                if(flag) {
                    // unary operator is a syntax error
                    expect("number or a semicolon", tok);
                    return 1;
                }
                else {
                    while(peek(&oper_stack)->prec > BSHL) {
                    }
                }
                break;
            case TOK_BWSHR:
                if(flag) {
                    // unary operator is a syntax error
                    expect("number or a semicolon", tok);
                    return 1;
                }
                else {
                    while(peek(&oper_stack)->prec > BSHR) {
                    }
                }
                break;
            case TOK_BWAND:
                if(flag) {
                    // unary operator is a syntax error
                    expect("number or a semicolon", tok);
                    return 1;
                }
                else {
                    while(peek(&oper_stack)->prec > BAND) {
                    }
                }
                break;
            case TOK_BWOR:
                if(flag) {
                    // unary operator is a syntax error
                    expect("number or a semicolon", tok);
                    return 1;
                }
                else {
                    while(peek(&oper_stack)->prec > BOR) {
                    }
                }
                break;
            case TOK_BWXOR:
                if(flag) {
                    // unary operator is a syntax error
                    expect("number or a semicolon", tok);
                    return 1;
                }
                else {
                    while(peek(&oper_stack)->prec > BXOR) {
                    }
                }
                break;


            case TOK_COMMA:
                break;

            case TOK_SEMICOLON:
                break;

            default:
                expect("number or arithmetic binary operator", tok);
                return 1;
        }
    }
}
