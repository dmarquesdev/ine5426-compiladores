/* Inspirated by https://github.com/llpilla/compiler_examples/blob/master/simple_ast/st.cpp */
#include "symbol_table.h"

using namespace SymTbl;

extern SymbolTable symbolTable;

SyntaxTree::Node* SymbolTable::newVariable(std::string id, SyntaxTree::Node* next, bool initialized) {
	if (!contains(id)) {
		Symbol newSymbol(integer, variable, 0, initialized);
		symbolList[id] = newSymbol;
	} else {
		yyerror("Variable redefinition! %s\n", id.c_str());
	}

	return new SyntaxTree::Variable(id, next);
}

SyntaxTree::Node* SymbolTable::useVariable(std::string id) {
	if(!contains(id)) { yyerror("Variable not defined yet! %s\n", id.c_str()); }
	if(!symbolList[id]._initialized) { yyerror("Variable not initialized yet! %s\n", id.c_str()); }

	return new SyntaxTree::Variable(id, NULL);
}

SyntaxTree::Node* SymbolTable::assignVariable(std::string id) {
	if(!contains(id)) { yyerror("Variable not defined yet! %s\n", id.c_str()); }
	symbolList[id]._initialized = true;

	return new SyntaxTree::Variable(id, NULL);
}