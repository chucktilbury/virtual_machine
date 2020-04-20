
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "assembler.h"
#include "scanner.h"
#include "parser.h"
#include "errors.h"

int main(int argc, char** argv)
{
    init_errors(10, stdout);
    scanner_init();
    init_sections();
    scanner_open_infile(argv[1]);

    parse_all();

    int errors = get_num_errors();

    if(errors != 0)
        printf("\nparse failed: %d errors: %d warnings\n", errors, get_num_warnings());
    else
        printf("\nparse succeeded: %d errors: %d warnings\n", errors, get_num_warnings());

    destroy_all_sections();
    return errors;
}
