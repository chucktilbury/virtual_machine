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

# these are tokens that do not map to an opcode
exclude_list = [
    "TOK_ERROR",
    "TOK_END_INPUT",
    "TOK_STRING_LITERAL",
    "TOK_IDENTIFIER",
    "TOK_UNUM_LITERAL",
    "TOK_INUM_LITERAL",
    "TOK_FNUM_LITERAL",
    "TOK_R0",
    "TOK_R1",
    "TOK_R2",
    "TOK_R3",
    "TOK_R4",
    "TOK_R5",
    "TOK_R6",
    "TOK_R7",
    "TOK_R8",
    "TOK_R9",
    "TOK_R10",
    "TOK_R11",
    "TOK_R12",
    "TOK_R13",
    "TOK_R14",
    "TOK_R15",
    "TOK_R16",
    "TOK_R17",
    "TOK_R18",
    "TOK_R19",
    "TOK_R20",
    "TOK_R21",
    "TOK_R22",
    "TOK_R23",
    "TOK_R24",
    "TOK_R25",
    "TOK_R26",
    "TOK_R27",
    "TOK_R28",
    "TOK_R29",
    "TOK_R30",
    "TOK_R31",
    "TOK_SECTION",
    "TOK_INCLUDE",
    "TOK_INT8",
    "TOK_INT16",
    "TOK_INT32",
    "TOK_INT64",
    "TOK_UINT8",
    "TOK_UINT16",
    "TOK_UINT32",
    "TOK_UINT64",
    "TOK_GLOBAL",
    "TOK_CONST",
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
value = 255 - len(tok_list)
with open(args.outfile, 'w') as outfp:
    outfp.write("\n// This file is generated from tokens.h.\n// DO NOT EDIT\n")
    outfp.write("// Generated: %s\n"%(time.ctime()))
    outfp.write("\n#ifndef __OPCODES_H__\n#define __OPCODES_H__\n\n")
    #outfp.write("\n#include tokens.h\n\n");
    outfp.write("typedef enum = {\n")

    #tok_list.sort()
    for line in tok_list:
        outfp.write("    %s = 0x%02X,\n"%(line.replace("TOK_", "OP_"), value))
        count += 1
        value += 1

    outfp.write("} opcode_t;\n\n#endif\n\n")


print("Finished: Processed %d tokens"%(count))
