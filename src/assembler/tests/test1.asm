
##
    This is a multi-line comment
##

include "system_calls.asm" # this is a single line comment
include 'test2.asm'

DATA test1_section
    int32 items[4]; # = {1, 2, 3, 4+12}
    int8 str = "this is a string\x0a\x0A"
    int8 bacon = (1*2+3)
    int8 eggs = 123+234
END_SEC

{
# curly braces are optional for label blocks
a_label {

    eret
    mov R10, str
    mov R11, 3x1234
  hidden_by_scope
    mov R10, [R10+R11]
    INC r11
    cmp r10, 0
   name {
    JMPEQ end_loop
    jmp hidden
  end_loop
  }
     {mov R5, [str] }
    mov r6, r11
    ret
}
nop nop
######################
#  Print the string and return
########################
another_label {
    call a_label
    excall printf
    mov r5, 10  // new line
    call a_label
    mov r6, 1
    excall printf
    ret
}

nop
nop
}
