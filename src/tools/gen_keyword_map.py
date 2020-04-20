#!/usr/bin/env python

'''
This stand-alone program generates the keyword_map data structure
from the tokens.h file.
'''

import sys
import argparse
import time

parser = argparse.ArgumentParser(description="Process the tokens file")
parser.add_argument('-i', dest='infile', type=str, help="specify the full name of the input file", required=True)
parser.add_argument('-o', dest='outfile', type=str, help="specify the full name of the output file", required=True)
args = parser.parse_args()

# these are tokens that do not map to a keyword
exclude_list = [
    "TOK_ERROR",
    "TOK_END_INPUT",
    "TOK_STRING_LITERAL",
    "TOK_IDENTIFIER",
    "TOK_UNUM_LITERAL",
    "TOK_INUM_LITERAL",
    "TOK_FNUM_LITERAL",
    "TOK_PLUS",
    "TOK_MINUS",
    "TOK_STAR",
    "TOK_SLASH",
    "TOK_BWSHL",
    "TOK_BWSHR",
    "TOK_BWAND",
    "TOK_BWOR",
    "TOK_XOR",
    "TOK_BWNOT",
    "TOK_OCURLY",
    "TOK_CCURLY",
    "TOK_OPAREN",
    "TOK_CPAREN",
    "TOK_OSQUARE",
    "TOK_CSQUARE",
    "TOK_EQUAL",
    "TOK_PERIOD",
    "TOK_COMMA",
]
tok_list = []

with open(args.infile, 'r') as infp:

    for line in infp:
        line = line.strip()
        if(line[:4] == 'TOK_'):
            line = line.replace(',', '')
            if not line in exclude_list:
                tok_list.append(line)

count = 0
with open(args.outfile, 'w') as outfp:
    outfp.write("\n// This file is generated from tokens.h.\n// DO NOT EDIT\n")
    outfp.write("// Generated: %s\n"%(time.ctime()))
    outfp.write("\n#include <stdlib.h>\n\n");
    outfp.write("\n#include \"tokens.h\"\n\n");
    outfp.write("\n#include \"keyword_map.h\"\n\n");
    outfp.write("keyword_map_t keyword_map[] = {\n")

    tok_list.sort()
    for line in tok_list:
        outfp.write("    {%s, \"%s\"},\n"%(line, line.replace("TOK_", "").lower()))
        count += 1

    outfp.write("};\n\n")
    outfp.write("const size_t num_keywords = (sizeof(keyword_map)/sizeof(keyword_map_t));\n\n")

print("Finished: Processed %d tokens"%(count))
