/* Inspired on https://github.com/llpilla/compiler_examples/blob/master/simple_ast/main.cpp */

#include <iostream>
#include "syntax_tree.h"
#include "symbol_table.h"

extern SyntaxTree::Node* programRoot;
extern int yyparse();
extern int yydebug;

int main(int argc, char **argv)
{
	//yydebug = 1;
    yyparse();                  //parses whole input
    programRoot->printTree();
    return 0;
}
