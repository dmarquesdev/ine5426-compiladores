/* Inspirated by https://github.com/llpilla/compiler_examples/blob/master/simple_ast/st.cpp */

#include "symbol_table.h"

using namespace SymTbl;

SyntaxTree::Node* SymbolTable::newVariable(std::string id, SyntaxTree::Node* value) {
	Symbol symbol(Type::unknown, k_var, (value != NULL));

	Symbol* symb = find(id, true);

	if (symb == NULL) {
		symbolList[id] = symbol;
	} else {
		error("semantic", "re-declaration of variable %s\n", id.c_str());
	}

	return new SyntaxTree::Variable(id, Type::unknown, value);
}

/* 
 * Handle the use of a variable 
 * param id: variable id
 *
 * It returns a Syntax Tree's variable
 */
SyntaxTree::Node* SymbolTable::useVariable(std::string id) {
	Symbol* symb = find(id);
	if(symb == NULL) { error("semantic", "undeclared variable %s\n", id.c_str()); }

	return new SyntaxTree::Variable(id, symb->_type, NULL);
}

/* 
 * Handle the assignment of a variable 
 * param id: variable id
 *
 * It returns a Syntax Tree's variable
 */
SyntaxTree::Node* SymbolTable::assignVariable(std::string id) {
	Symbol* symb = find(id);
	if(symb == NULL) { error("semantic", "undeclared variable %s\n", id.c_str()); }
	symb->_initialized = true;

	return new SyntaxTree::Variable(id, symb->_type, NULL);
}

Symbol* SymbolTable::find(std::string id, bool local) {
	SymbolTable* current = this;

	do {
		SymbolList sl = current->symbolList;
		auto result = sl.find(id);

		if(result != sl.end()) {
			return &result->second;
		}
		current = current->_parent;
	} while (current != NULL && !local);

	return NULL;
}

void SymbolTable::setType(std::string id, Type type) {
	if(find(id) != NULL) {
		symbolList[id]._type = type;
	}
}

const char* Symbol::typeToString(Type type) {
	switch(type) {
		case SymTbl::Type::t_int: return "int"; 
		case SymTbl::Type::t_float: return "float";
		case SymTbl::Type::t_bool: return "bool";
		case SymTbl::Type::unknown: return "unknown";
	}
}