
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "scanner.h"
#include "errors.h"
#include "tokens.h"

/*
 * Brute force return a string for all tokens.
 */
const char* scanner_tok_str(int tok)
{
    switch (tok)
    {
        case TOK_ERROR:
            return "an error token";
        case TOK_END_INPUT:
            return "end of input";
        case TOK_NOP:
            return "a NOP instruction";
        case TOK_STZ:
            return "a STZ instruction";
        case TOK_CLZ:
            return "a CLZ instruction";
        case TOK_STC:
            return "a STC instruction";
        case TOK_CLC:
            return "a CLC instruction";
        case TOK_STN:
            return "a STN instruction";
        case TOK_CLN:
            return "a CLN instruction";
        case TOK_STV:
            return "a STV instruction";
        case TOK_CLV:
            return "a CLV instruction";
        case TOK_STT:
            return "a STT instruction";
        case TOK_CLT:
            return "a CLT instruction";
        case TOK_STE:
            return "a STE instruction";
        case TOK_CLE:
            return "a CLE instruction";
        case TOK_PAUSE:
            return "a PAUSE instruction";
        case TOK_RESUME:
            return "a RESUME instruction";
        case TOK_END:
            return "a END instruction";
        case TOK_LOAD:
            return "a LOAD instruction";
        case TOK_STORE:
            return "a STORE instruction";
        case TOK_MOV8:
            return "a MOV8 instruction";
        case TOK_MOV16:
            return "a MOV16 instruction";
        case TOK_MOV32:
            return "a MOV32 instruction";
        case TOK_MOV64:
            return "a MOV64 instruction";
        case TOK_MOV:
            return "a MOV instruction";
        case TOK_MOVB8:
            return "a MOVB8 instruction";
        case TOK_MOVB16:
            return "a MOVB16 instruction";
        case TOK_MOVB32:
            return "a MOVB32 instruction";
        case TOK_MOVB64:
            return "a MOVB64 instruction";
        case TOK_MOVB:
            return "a MOVB instruction";
        case TOK_PUSH:
            return "a PUSH instruction";
        case TOK_POP:
            return "a POP instruction";
        case TOK_IADD:
            return "a IADD instruction";
        case TOK_UADD:
            return "a UADD instruction";
        case TOK_FADD:
            return "a FADD instruction";
        case TOK_ISUB:
            return "a ISUB instruction";
        case TOK_USUB:
            return "a USUB instruction";
        case TOK_FSUB:
            return "a FSUB instruction";
        case TOK_IMUL:
            return "a IMUL instruction";
        case TOK_UMUL:
            return "a UMUL instruction";
        case TOK_FMUL:
            return "a FMUL instruction";
        case TOK_IDIV:
            return "a IDIV instruction";
        case TOK_UDIV:
            return "a UDIV instruction";
        case TOK_FDIV:
            return "a FDIV instruction";
        case TOK_IMOD:
            return "a IMOD instruction";
        case TOK_UMOD:
            return "a UMOD instruction";
        case TOK_FMOD:
            return "a FMOD instruction";
        case TOK_INEG:
            return "a INEG instruction";
        case TOK_UNEG:
            return "a UNEG instruction";
        case TOK_FNEG:
            return "a FNEG instruction";
        case TOK_FTU:
            return "a FTU instruction";
        case TOK_FTI:
            return "a FTI instruction";
        case TOK_ITF:
            return "a ITF instruction";
        case TOK_ITU:
            return "a ITU instruction";
        case TOK_UTF:
            return "a UTF instruction";
        case TOK_UTI:
            return "a UTI instruction";
        case TOK_INC:
            return "a INC instruction";
        case TOK_DEC:
            return "a DEC instruction";
        case TOK_SHL:
            return "a SHL instruction";
        case TOK_SHR:
            return "a SHR instruction";
        case TOK_ROL:
            return "a ROL instruction";
        case TOK_ROR:
            return "a ROR instruction";
        case TOK_AND:
            return "a AND instruction";
        case TOK_OR:
            return "a OR instruction";
        case TOK_XOR:
            return "a XOR instruction";
        case TOK_NOT:
            return "a NOT instruction";
        case TOK_CMP:
            return "a CMP instruction";
        case TOK_TST:
            return "a TST instruction";
        case TOK_JMPEQ:
            return "a JMPEQ instruction";
        case TOK_JMPNE:
            return "a JMPNE instruction";
        case TOK_JMPCS:
            return "a JMPCS instruction";
        case TOK_JMPCC:
            return "a JMPCC instruction";
        case TOK_JMPMI:
            return "a JMPMI instruction";
        case TOK_JMPPL:
            return "a JMPPL instruction";
        case TOK_JMPVS:
            return "a JMPVS instruction";
        case TOK_JMPVC:
            return "a JMPVC instruction";
        case TOK_JMPHI:
            return "a JMPHI instruction";
        case TOK_JMPLS:
            return "a JMPLS instruction";
        case TOK_JMPGE:
            return "a JMPGE instruction";
        case TOK_JMPLT:
            return "a JMPLT instruction";
        case TOK_JMPGT:
            return "a JMPGT instruction";
        case TOK_JMPLE:
            return "a JMPLE instruction";
        case TOK_JMPTE:
            return "a JMPTE instruction";
        case TOK_JMPEE:
            return "a JMPEE instruction";
        case TOK_JMP:
            return "a JMP instruction";
        case TOK_CALLEQ:
            return "a CALLEQ instruction";
        case TOK_CALLNE:
            return "a CALLNE instruction";
        case TOK_CALLCS:
            return "a CALLCS instruction";
        case TOK_CALLCC:
            return "a CALLCC instruction";
        case TOK_CALLMI:
            return "a CALLMI instruction";
        case TOK_CALLPL:
            return "a CALLPL instruction";
        case TOK_CALLVS:
            return "a CALLVS instruction";
        case TOK_CALLVC:
            return "a CALLVC instruction";
        case TOK_CALLHI:
            return "a CALLHI instruction";
        case TOK_CALLLS:
            return "a CALLLS instruction";
        case TOK_CALLGE:
            return "a CALLGE instruction";
        case TOK_CALLLT:
            return "a CALLLT instruction";
        case TOK_CALLGT:
            return "a CALLGT instruction";
        case TOK_CALLLE:
            return "a CALLLE instruction";
        case TOK_CALLTE:
            return "a CALLTE instruction";
        case TOK_CALLEE:
            return "a CALLEE instruction";
        case TOK_CALL:
            return "a CALL instruction";
        case TOK_EXCALLEQ:
            return "a EXCALLEQ instruction";
        case TOK_EXCALLNE:
            return "a EXCALLNE instruction";
        case TOK_EXCALLCS:
            return "a EXCALLCS instruction";
        case TOK_EXCALLCC:
            return "a EXCALLCC instruction";
        case TOK_EXCALLMI:
            return "a EXCALLMI instruction";
        case TOK_EXCALLPL:
            return "a EXCALLPL instruction";
        case TOK_EXCALLVS:
            return "a EXCALLVS instruction";
        case TOK_EXCALLVC:
            return "a EXCALLVC instruction";
        case TOK_EXCALLHI:
            return "a EXCALLHI instruction";
        case TOK_EXCALLLS:
            return "a EXCALLLS instruction";
        case TOK_EXCALLGE:
            return "a EXCALLGE instruction";
        case TOK_EXCALLLT:
            return "a EXCALLLT instruction";
        case TOK_EXCALLGT:
            return "a EXCALLGT instruction";
        case TOK_EXCALLLE:
            return "a EXCALLLE instruction";
        case TOK_EXCALLTE:
            return "a EXCALLTE instruction";
        case TOK_EXCALLEE:
            return "a EXCALLEE instruction";
        case TOK_EXCALL:
            return "a EXCALL instruction";
        case TOK_RETEQ:
            return "a RETEQ instruction";
        case TOK_RETNE:
            return "a RETNE instruction";
        case TOK_RETCS:
            return "a RETCS instruction";
        case TOK_RETCC:
            return "a RETCC instruction";
        case TOK_RETMI:
            return "a RETMI instruction";
        case TOK_RETPL:
            return "a RETPL instruction";
        case TOK_RETVS:
            return "a RETVS instruction";
        case TOK_RETVC:
            return "a RETVC instruction";
        case TOK_RETHI:
            return "a RETHI instruction";
        case TOK_RETLS:
            return "a RETLS instruction";
        case TOK_RETGE:
            return "a RETGE instruction";
        case TOK_RETLT:
            return "a RETLT instruction";
        case TOK_RETGT:
            return "a RETGT instruction";
        case TOK_RETLE:
            return "a RETLE instruction";
        case TOK_RETTE:
            return "a RETTE instruction";
        case TOK_RETEE:
            return "a RETEE instruction";
        case TOK_RET:
            return "a RET instruction";
        case TOK_TRAPEQ:
            return "a TRAPEQ instruction";
        case TOK_TRAPNE:
            return "a TRAPNE instruction";
        case TOK_TRAPCS:
            return "a TRAPCS instruction";
        case TOK_TRAPCC:
            return "a TRAPCC instruction";
        case TOK_TRAPMI:
            return "a TRAPMI instruction";
        case TOK_TRAPPL:
            return "a TRAPPL instruction";
        case TOK_TRAPVS:
            return "a TRAPVS instruction";
        case TOK_TRAPVC:
            return "a TRAPVC instruction";
        case TOK_TRAPHI:
            return "a TRAPHI instruction";
        case TOK_TRAPLS:
            return "a TRAPLS instruction";
        case TOK_TRAPGE:
            return "a TRAPGE instruction";
        case TOK_TRAPLT:
            return "a TRAPLT instruction";
        case TOK_TRAPGT:
            return "a TRAPGT instruction";
        case TOK_TRAPLE:
            return "a TRAPLE instruction";
        case TOK_TRAPTE:
            return "a TRAPTE instruction";
        case TOK_TRAPEE:
            return "a TRAPEE instruction";
        case TOK_TRAP:
            return "a TRAP instruction";
        case TOK_TRETEQ:
            return "a TRETEQ instruction";
        case TOK_TRETNE:
            return "a TRETNE instruction";
        case TOK_TRETCS:
            return "a TRETCS instruction";
        case TOK_TRETCC:
            return "a TRETCC instruction";
        case TOK_TRETMI:
            return "a TRETMI instruction";
        case TOK_TRETPL:
            return "a TRETPL instruction";
        case TOK_TRETVS:
            return "a TRETVS instruction";
        case TOK_TRETVC:
            return "a TRETVC instruction";
        case TOK_TRETHI:
            return "a TRETHI instruction";
        case TOK_TRETLS:
            return "a TRETLS instruction";
        case TOK_TRETGE:
            return "a TRETGE instruction";
        case TOK_TRETLT:
            return "a TRETLT instruction";
        case TOK_TRETGT:
            return "a TRETGT instruction";
        case TOK_TRETLE:
            return "a TRETLE instruction";
        case TOK_TRETTE:
            return "a TRETTE instruction";
        case TOK_TRETEE:
            return "a TRETEE instruction";
        case TOK_TRET:
            return "a TRET instruction";
        case TOK_RAISEEQ:
            return "a RAISEEQ instruction";
        case TOK_RAISENE:
            return "a RAISENE instruction";
        case TOK_RAISECS:
            return "a RAISECS instruction";
        case TOK_RAISECC:
            return "a RAISECC instruction";
        case TOK_RAISEMI:
            return "a RAISEMI instruction";
        case TOK_RAISEPL:
            return "a RAISEPL instruction";
        case TOK_RAISEVS:
            return "a RAISEVS instruction";
        case TOK_RAISEVC:
            return "a RAISEVC instruction";
        case TOK_RAISEHI:
            return "a RAISEHI instruction";
        case TOK_RAISELS:
            return "a RAISELS instruction";
        case TOK_RAISEGE:
            return "a RAISEGE instruction";
        case TOK_RAISELT:
            return "a RAISELT instruction";
        case TOK_RAISEGT:
            return "a RAISEGT instruction";
        case TOK_RAISELE:
            return "a RAISELE instruction";
        case TOK_RAISETE:
            return "a RAISETE instruction";
        case TOK_RAISEEE:
            return "a RAISEEE instruction";
        case TOK_RAISE:
            return "a RAISE instruction";
        case TOK_ERETEQ:
            return "a ERETEQ instruction";
        case TOK_ERETNE:
            return "a ERETNE instruction";
        case TOK_ERETCS:
            return "a ERETCS instruction";
        case TOK_ERETCC:
            return "a ERETCC instruction";
        case TOK_ERETMI:
            return "a ERETMI instruction";
        case TOK_ERETPL:
            return "a ERETPL instruction";
        case TOK_ERETVS:
            return "a ERETVS instruction";
        case TOK_ERETVC:
            return "a ERETVC instruction";
        case TOK_ERETHI:
            return "a ERETHI instruction";
        case TOK_ERETLS:
            return "a ERETLS instruction";
        case TOK_ERETGE:
            return "a ERETGE instruction";
        case TOK_ERETLT:
            return "a ERETLT instruction";
        case TOK_ERETGT:
            return "a ERETGT instruction";
        case TOK_ERETLE:
            return "a ERETLE instruction";
        case TOK_ERETTE:
            return "a ERETTE instruction";
        case TOK_ERETEE:
            return "a ERETEE instruction";
        case TOK_ERET:
            return "a ERET instruction";
        case TOK_R0:
            return "an R0 register";
        case TOK_R1:
            return "an R1 register";
        case TOK_R2:
            return "an R2 register";
        case TOK_R3:
            return "an R3 register";
        case TOK_R4:
            return "an R4 register";
        case TOK_R5:
            return "an R5 register";
        case TOK_R6:
            return "an R6 register";
        case TOK_R7:
            return "an R7 register";
        case TOK_R8:
            return "an R8 register";
        case TOK_R9:
            return "an R9 register";
        case TOK_R10:
            return "an R10 register";
        case TOK_R11:
            return "an R11 register";
        case TOK_R12:
            return "an R12 register";
        case TOK_R13:
            return "an R13 register";
        case TOK_R14:
            return "an R14 register";
        case TOK_R15:
            return "an R15 register";
        case TOK_R16:
            return "an R16 register";
        case TOK_R17:
            return "an R17 register";
        case TOK_R18:
            return "an R18 register";
        case TOK_R19:
            return "an R19 register";
        case TOK_R20:
            return "an R20 register";
        case TOK_R21:
            return "an R21 register";
        case TOK_R22:
            return "an R22 register";
        case TOK_R23:
            return "an R23 register";
        case TOK_R24:
            return "an R24 register";
        case TOK_R25:
            return "an R25 register";
        case TOK_R26:
            return "an R26 register";
        case TOK_R27:
            return "an R27 register";
        case TOK_R28:
            return "an R28 register";
        case TOK_R29:
            return "an R29 register";
        case TOK_R30:
            return "an R30 register";
        case TOK_R31:
            return "an R31 register";
        case TOK_CODE:
            return "the CODE keyword";
        case TOK_DATA:
            return "the DATA keyword";
        case TOK_END_SEC:
            return "the END_SEC keyword";
        case TOK_INCLUDE:
            return "the INCLUDE keyword";
        case TOK_INT8:
            return "the INT8 data type";
        case TOK_INT16:
            return "the INT16 data type";
        case TOK_INT32:
            return "the INT32 data type";
        case TOK_INT64:
            return "the INT64 data type";
        case TOK_UINT8:
            return "the UINT8 data type";
        case TOK_UINT16:
            return "the UINT16 data type";
        case TOK_UINT32:
            return "the UINT32 data type";
        case TOK_UINT64:
            return "the UINT64 data type";
        case TOK_PLUS:
            return "the '+' (add) operator";
        case TOK_MINUS:
            return "the '-' (subtract) operator";
        case TOK_STAR:
            return "the '*' (multiply) operator";
        case TOK_SLASH:
            return "the '/' (divide) operator";
        case TOK_BWSHL:
            return "the '<' (shift left) operator";
        case TOK_BWSHR:
            return "the '>' (shift right) operator";
        case TOK_BWAND:
            return "the '&' (and) operator";
        case TOK_BWOR:
            return "the '|' (or) operator";
        case TOK_BWXOR:
            return "the '^' (xor) operator";
        case TOK_BWNOT:
            return "the '~' (not) operator";
        case TOK_OCURLY:
            return "a '{'";
        case TOK_CCURLY:
            return "a '}'";
        case TOK_OPAREN:
            return "a '('";
        case TOK_CPAREN:
            return "a ')'";
        case TOK_OSQUARE:
            return "a '['";
        case TOK_CSQUARE:
            return "a ']'";
        case TOK_EQUAL:
            return "a '=' (assignment) operator";
        case TOK_PERIOD:
            return "a '.'";
        case TOK_COMMA:
            return "a ','";
        case TOK_STRING_LITERAL:
            return "a string literal";
        case TOK_IDENTIFIER:
            return "an identifier";
        case TOK_UNUM_LITERAL:
            return "an unsigned literal number";
        case TOK_INUM_LITERAL:
            return "a signed literal number";
        case TOK_FNUM_LITERAL:
            return "a floating point number";
        default:
            return "something unknown";
    }
}
