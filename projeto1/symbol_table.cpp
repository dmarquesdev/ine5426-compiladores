/* Inspirated by https://github.com/llpilla/compiler_examples/blob/master/simple_ast/st.cpp */

#include "symbol_table.h"

using namespace SymTbl;

extern SymbolTable symbolTable;

SyntaxTree::Node* SymbolTable::newVariable(std::string id, SyntaxTree::Node* next, SyntaxTree::Node* value) {
	if (!contains(id)) {
		Symbol newSymbol(integer, variable, (value != NULL));
		symbolList[id] = newSymbol;
	} else {
		yyerror("Variable redefinition! %s\n", id.c_str());
	}

	return new SyntaxTree::Variable(id, value, next);
}

SyntaxTree::Node* SymbolTable::useVariable(std::string id) {
	if(!contains(id)) { yyerror("Variable not defined yet! %s\n", id.c_str()); }
	if(!symbolList[id]._initialized) { yyerror("Variable not initialized yet! %s\n", id.c_str()); }

	return new SyntaxTree::Variable(id, NULL, NULL);
}

SyntaxTree::Node* SymbolTable::assignVariable(std::string id) {
	if(!contains(id)) { yyerror("Variable not defined yet! %s\n", id.c_str()); }
	symbolList[id]._initialized = true;

	return new SyntaxTree::Variable(id, NULL, NULL);
}