/* Inspirated by https://github.com/llpilla/compiler_examples/blob/master/simple_ast/st.cpp */

#include "symbol_table.h"

using namespace SymTbl;

extern SymbolTable symbolTable;

SyntaxTree::Declarable* SymbolTable::newVariable(std::string id, Symbol* symbol, SyntaxTree::Declarable* next, SyntaxTree::Node* value) {
	if (!contains(id)) {
		symbolList[id] = *symbol;
	} else {
		yyerror("Variable redefinition! %s\n", id.c_str());
	}

	return new SyntaxTree::Variable(id, symbol, value, next);
}

SyntaxTree::Declarable* SymbolTable::useVariable(std::string id) {
	if(!contains(id)) { yyerror("Variable not defined yet! %s\n", id.c_str()); }
	// if(!symbolList[id]._initialized) { yyerror("Variable not initialized yet! %s\n", id.c_str()); }

	return new SyntaxTree::Variable(id, NULL, NULL, NULL);
}

SyntaxTree::Declarable* SymbolTable::assignVariable(std::string id) {
	if(!contains(id)) { yyerror("Variable not defined yet! %s\n", id.c_str()); }
	symbolList[id]._initialized = true;

	return new SyntaxTree::Variable(id, NULL, NULL, NULL);
}