/* Inspirated by https://github.com/llpilla/compiler_examples/blob/master/simple_ast/st.cpp */

#include "symbol_table.h"

using namespace SymTbl;

extern SymbolTable symbolTable;

SyntaxTree::Node* SymbolTable::newVariable(std::string id, SyntaxTree::Node* value) {
	Symbol symbol(Type::unknown, k_var, (value != NULL));

	if (!contains(id)) {
		symbolList[id] = symbol;
	} else {
		yyerror("Variable redefinition! %s\n", id.c_str());
	}

	return new SyntaxTree::Variable(id, Type::unknown, value);
}

SyntaxTree::Node* SymbolTable::useVariable(std::string id) {
	if(!contains(id)) { yyerror("Variable not defined yet! %s\n", id.c_str()); }
	// if(!symbolList[id]._initialized) { yyerror("Variable not initialized yet! %s\n", id.c_str()); }

	return new SyntaxTree::Variable(id, symbolList[id]._type, NULL);
}

SyntaxTree::Node* SymbolTable::assignVariable(std::string id) {
	if(!contains(id)) { yyerror("Variable not defined yet! %s\n", id.c_str()); }
	symbolList[id]._initialized = true;

	return new SyntaxTree::Variable(id, symbolList[id]._type, NULL);
}

const char* Symbol::typeToString(Type type) {
	switch(type) {
		case SymTbl::Type::t_int: return "int"; 
		case SymTbl::Type::t_float: return "float";
		case SymTbl::Type::t_bool: return "bool";
		case SymTbl::Type::unknown: return "unknown";
	}
}