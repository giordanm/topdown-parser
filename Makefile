# Makefile for allocator

CFLAGS=-Wall

3726alloc:	alloc.o machine.o instruction.o hash.o lex.yy.o iloc.tab.o
		gcc $(CFLAGS) -o 3726alloc alloc.o machine.o instruction.o hash.o lex.yy.o iloc.tab.o

alloc.o:	alloc.c instruction.h machine.h alloc.h
		gcc $(CFLAGS) -c alloc.c

machine.o:	machine.c machine.h
		gcc $(CFLAGS) -c machine.c

instruction.o:	instruction.c instruction.h hash.h
		gcc $(CFLAGS) -c instruction.c

hash.o:		hash.c hash.h
		gcc $(CFLAGS) -c hash.c

lex.yy.o:	lex.yy.c
		gcc -g -c lex.yy.c

iloc.tab.o:	iloc.tab.c
		gcc -g -c iloc.tab.c

lex.yy.c:	iloc.l iloc.tab.c instruction.h
		flex iloc.l

iloc.tab.c:	iloc.y instruction.h
		bison -v -t -d iloc.y

clean:
		rm *.o
		rm lex.yy.c
		rm iloc.tab.c
		rm iloc.tab.h
		rm 3726alloc

wc:		
		wc iloc.y iloc.l hash.h hash.c instruction.h instruction.c machine.h machine.c alloc.h alloc.c
