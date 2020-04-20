#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "scanner.h"
#include "errors.h"

/*
 * Eat the comment. A single line comment goes from a '#' to the end of a line, and
 * a multi line comment starts with "##" and ends with "##". Lines of '#' are treated
 * as if they are just 2 '#' in a row.
 */
void scanner_comment(void)
{
    int finished = 0;
    int state = 0;

    while(!finished)
    {
        int ch = get_char();

        switch (ch)
        {
            case END_OF_INPUT:
                scanner_error("unexpected end of file in comment");
                exit(1);
                break;
            case '#':
                switch (state)
                {
                        // have a multi line comment
                    case 0:
                        state = 1;
                        break;
                        // found one '#' in multi-line
                    case 2:
                        state = 3;
                        break;
                        // found second '#' in multi-line
                    case 3:
                        state = 5;
                        break;
                        // ignore '#' in other states.
                    default:
                        break;
                }
                break;
            case '\n':
                switch (state)
                {
                        // found break in a row of '#'
                    case 1:
                        state = 2;
                        break;
                        // found a single '#' while ignoring multi-line
                    case 3:
                        state = 2;
                        break;
                        // end a single line comment or multi-line
                    case 5:
                    case 4:
                        finished++;
                        break;
                        // ignore if not looking for end of single line.
                    default:
                        break;
                }
                break;
            default:
                switch (state)
                {
                        // have a single line comment
                    case 0:
                        state = 4;
                        break;
                        // must have another character other than '#'
                    case 1:
                        state = 2;
                        break;
                        // found a single '#' while ignoring multi-line
                    case 3:
                        state = 2;
                        break;
                        // finished a multi-line comment and a char is but up to it, like this:
                        // ### asd ###xxx We need to save that first 'x',
                    case 5:
                        unget_char(ch);
                        finished++;
                        break;
                        // ignore whatever we see here
                    default:
                        break;
                }
                break;
        }
    }
}
