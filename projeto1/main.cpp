#include <iostream>
#include "syntax_tree.h"
#include "symbol_table.h"

extern SyntaxTree::Node* programRoot;
extern int yyparse();
extern int yydebug;

int main(int argc, char **argv)
{
    //yydebug = 1;              //remove comment to have verbose debug messages
    yyparse();                  //parses whole input
    std::cout << "##Printing the lines" << std::endl;
    programRoot->printTree();
    return 0;
}
