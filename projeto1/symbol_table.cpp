/* Inspirated by https://github.com/llpilla/compiler_examples/blob/master/simple_ast/st.cpp */

#include "symbol_table.h"

using namespace SymTbl;

extern SymbolTable symbolTable;

/* 
 * Register a new Variable in the Symbol Table 
 * param id: variable id
 * param next: In case of multiple declaration, the next declaration node
 * param value: The value attributed to the variable
 *
 * It returns a Syntax Tree's variable
 */
SyntaxTree::Node* SymbolTable::newVariable(std::string id, SyntaxTree::Node* next, SyntaxTree::Node* value) {
	if (!contains(id)) {
		Symbol newSymbol(integer, variable, (value != NULL));
		symbolList[id] = newSymbol;
	} else {
		error("semantic", "re-declaration of variable %s\n", id.c_str());
	}

	return new SyntaxTree::Variable(id, value, next);
}

/* 
 * Handle the use of a variable 
 * param id: variable id
 *
 * It returns a Syntax Tree's variable
 */
SyntaxTree::Node* SymbolTable::useVariable(std::string id) {
	if(!contains(id)) { error("semantic", "undeclared variable %s\n", id.c_str()); }
	// if(!symbolList[id]._initialized) { yyerror("Variable not initialized yet! %s\n", id.c_str()); }

	return new SyntaxTree::Variable(id, NULL, NULL);
}

/* 
 * Handle the assignment of a variable 
 * param id: variable id
 *
 * It returns a Syntax Tree's variable
 */
SyntaxTree::Node* SymbolTable::assignVariable(std::string id) {
	if(!contains(id)) { error("semantic", "undeclared variable %s\n", id.c_str()); }
	symbolList[id]._initialized = true;

	return new SyntaxTree::Variable(id, NULL, NULL);
}