CC = gcc
CFLAGS = -Wall -g

all: minilang

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y

lex.yy.c: lexer.l parser.tab.h
	flex lexer.l

minilang: parser.tab.c lex.yy.c symtab.c ast.c tac.c report.c
	$(CC) $(CFLAGS) -o minilang parser.tab.c lex.yy.c symtab.c ast.c tac.c report.c

clean:
	rm -f minilang parser.tab.c parser.tab.h lex.yy.c

test: minilang
	./minilang -v test_valid.ml

.PHONY: all clean test