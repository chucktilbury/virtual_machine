################################################################################
#
#	Make file for simple objects
#
################################################################################
PLATNAME := $(shell uname)

SRCS	=	assembler.c \
			../common/errors.c \
			parser.c \
			scanner.c \
			scanner_comment.c \
			scanner_number.c \
			scanner_quote.c \
			scanner_stopper.c \
			scanner_symbol.c \
			scanner_tok_str.c \
			keyword_map.c \
			parse_code_section.c \
			parse_data_section.c \
			sections.c \
			hash_table.c \
			parse_include.c \
			expressions.c

HEADERS	=	$(SRCS:.c=.h) \
			tokens.h

OBJS	=	$(SRCS:.c=.o)

TARGET	=	assemble

CC		=	clang
CARGS	=	-Wall -I../common -I.
DEBUG	=	-g -DTRACE

.c.o:
	$(CC) $(CARGS) $(DEBUG) -c $< -o $@

all: $(TARGET)

big: pretty $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CARGS) $(DEBUG) $(OBJS) -o $(TARGET) $(LIBS)

keyword_map.c: tokens.h
	../tools/gen_keyword_map.py -i tokens.h -o keyword_map.c

pretty: keyword_map.c
	indent $(SRCS) $(HEADERS)
	-rm -f *~

clean:
	-rm -f $(OBJS) $(TARGET) keyword_map.c

#			dynamic_array.c \
