# Inspired on https://github.com/llpilla/compiler_examples/blob/master/simple_ast/Makefile
# My Little Makefile
FLEX=flex
BISON=bison
CC=clang++
PARAM=-std=c++11 -fcxx-exceptions -Wno-deprecated-register
OUTPUT=lukacompiler

all: flex yacc clang

clang: *.cpp *.h
	${CC} -o ${OUTPUT} *.cpp ${LLVMPARAM} ${PARAM}

yacc: parser.y
	${BISON} --defines=parser.h --output=parser.cpp parser.y -v

flex: scanner.l
	${FLEX} -o scanner.cpp scanner.l 

clean:
	rm parser.cpp parser.h scanner.cpp ${OUTPUT} parser.output

test: all
	./${OUTPUT} < tests/v0.2/input/2.in
