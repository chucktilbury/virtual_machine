# Assembler

[TOC]

## General

This is the design document for the assembler.  The assembler is very simple and defines the input to the virtual machine.  There is no linker. All of the program is included into the main module using the include directive. The links are fixed up after all of the includes have been processed.  

Directives, instructions and registers are not case sensitive. Symbols (as in the name the programmer gives things) are case sensitive.

There is generally no notion of a function as such. 

## Sections

A section is an area of memory that has a certain format. Sections are not defined by default and different kinds of data must be specified as to what section it goes into. There can be many sections defined, but in the end they are simply concatenated to the output. Naming them may help the programmer to keep things straight.  Sections have well defined types.

In the program running in the VM, the location of objects in the sections are given as zero based. They are translated to actual memory by the VM.

Read/write and constant sections are a fixed size, which is calculated at compile time. 

Heap and stack space sizes are specified by the programmer, but they may grow if needs must. This happens transparently to the running program.

A named section is used to manage name spaces. When the module is compiled, the name is discarded and replaced with an address, which is the index into the section. 

### Constant data

Constant data sections are signified by the "const" keyword. This is data such as strings and numbers that are not immediate in the instruction stream.

#### Example

```asm
section const my_constant_data
uint8 str = "this is the string in question\x00"
ends
# ...
# move a pointer to str into R12
MOV32 R12, my_constant_data.str
```

### Read/Write data

This type of data is intended to be used for things like buffers and data structures where the location and size of object is known at compile time. For example, global variables would go here. This section type is designated by the keyword "global".

#### Example

```assembly
section global my_global_data
uint32 x	# Define a uninitialized single 32 bit unsigned int
uint8 buffer[128] # Define an uninitialized string buffer
# Define an initialized data structure of 4 register sized words.
int64 floop[4] = (0, 0, 0, 0) 
ends
# ...
# move the 3rd element in floop into register R18
MOV64 R18, my_global_data.floop[2]
```

### Heap space

This data type is simply given a size and no initialization is done on it before the program starts. data that is allocated by the equivalents of malloc() and free() is taken from this section. This section is not stored in the output object. Only one heap is defined for a program.  Heap space is designated by the "heap" keyword.

#### Example

```assembly
section heap my_heap_data 1024 * 64 # constant expression that evaluated to unsigned long
ends # optional .ends 
```

### Stack space

Stack space is allocated at compile time. When the VM starts up, the stack pointer is initialized to this location. This section is not stored in the output object. Only one stack is defined for a program. Stack space has no name and cannot be accessed except using VM instructions.

### Code space

Code space is defined at compile time and is read-only. It has no name to reference.

## Data types

Typed data may be declared inside a section. Data declared outside of a section is a syntax error. The assembler uses the data types to decide how to encode the operands of instructions. 

### Keywords

Keywords to define data types is straight forward as follows

* ```uint8, int8``` - Defines an 8 bit word. 
* ```uint16, int16``` - Defines a 16 bit word.
* ```uint32, int32``` - Defines a 32 bit word.
* ```uint64, int64``` - Defines a 64 bit word.
* ```float``` - Defines a register sized float. On my system it's a double.
* ```pointer``` - Defines a pointer to something else. Remember here pointers are an index into the section in which they are defined. A pointer should not be initialized except to zero. A pointer defined in constant memory can only point to another section if that section and the object that it points to has already been defined.

### Arrays

An array of any size data item can be defined by appending square braces [] with an optional number, for example, [10], defined 10 items of the given type.

### Initialization

A memory object can be initialized using an expression if the value of the expression can be determined at compile time. Pointers to objects can be used. Indexed elements to another array, for example, cannot be used unless the value indicated is in constant (read-only) memory.

#### Examples

```assembly
section global bacon
int8 str = "this is an initialized string\x00"
int16 str1 = "this" # init to "\0t\0h\0i\0s"
uint32 items[4] = (0,0,0,0)
ends
```

## Instruction formats

What formats a given instruction can accept is documented in the virtual machine document. This describes the syntax of those formats.

Operands that are taken as values have no special decorations. 

```assembly
NEG R4 # R4 contains a valure to the artihmetically negated
MOV R4, 12345 # The value 12345 is loaded into R4
MOV R22, R3 # The value in R3 is copied into R22
MOV R18, some_section.foobar # The value of some_section.foobar is loaded into R18
```

Operands that specify an address are specified using square brackets. Immediate values are not allowed as pointers.

```assembly
MOV R12, [R0] # Move the value that R0 points to into R12
MOV R4, [my_sect.barbaz] # The item my_sect.barbaz is a pointer to a value.
```

Operands that specify an address with an index respect data types. 

```assembly
section global sauce
uint64 items[4] = (0,0,0,0)
ends
MOV R31, [sauce.items + 2] # The 3rd item in the array is a pointer to something
MOV R30, sauce.items # move the address of the array into the R30
MOV R31, [R30+2] # equivalent to the first emample in this block
```



## Blocks and scope

A block is a section of code that was it's own name space. It applies to labels only.  If a label is defined before a block then it is visible inside the block. If a label is defined inside a block, then it is not visible outside of that block. It can be considered "local" in scope to that block. This exists to ease name collisions in a long piece of contiguous code. 

Since there is no linker, and source code is concatenated together by file, each file is treated as a block. So, a label can be exposed by using a ```.public``` keyword. This makes the symbol visible across all of the blocks.

Blocks are denoted by curly braces. "{}". Inside the braces are a new namespace 

### Keywords

Keywords controlling the scope of a label

* ```public```  -- Makes the label visible across all files that make up the program.
* local``` -- Makes the label visible in this file only.  Note that if you never use blocks in a file, all labels are locally scoped. This is only needed to expose labels defined in a nested scope.

### Examples

```assembly
# File 1
public my_lable:
  # some instructions here
```

```
# File 2
@include file1.asm
my_label # Name collision
your_lable # a new name
{
my_label # Name collision, Refers to the block in file 1
CALL my_label # refers to the label defined in file 1
your_lable # name collision: syntax error
CALL your_lable # refers to the first lable with this name.
    {
    some_lable # no name collision
    lable_123
    }
    {
    some_lable # not a name collision
    CALL lable_123 # name not found. The label is not in this block. If the label is 
                   # defined as .public, the is is visible across all modules. 
    }
}
```

## Directives

Symbols that are not instruction codes or labels are directives. All directives start with a '@'', which is part of  the keyword. This is a comprehensive list of all directives and their arguments.

* ```section```  ```ends```  -- Defines a name space for a section. 
* ```include``` -- Causes a file to be read in as if it were a part of the current file. A file may be read into the compile exactly once. File names are monitored, looking for circular includes.
* public``` -- Creates a label with a global scope across all file that are read into this compile.
* ```local``` -- Creates a label that has a scope limited to the current file, no matter what block the label is defined in.

## Macros



## File format

