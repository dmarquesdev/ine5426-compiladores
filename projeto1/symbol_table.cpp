/* Inspirated by https://github.com/llpilla/compiler_examples/blob/master/simple_ast/st.cpp */

#include "symbol_table.h"

using namespace SymTbl;

/* 
 * Crate a new variable node at syntax tree 
 * param id: variable id
 * param value: variable value
 *
 * It returns a Syntax Tree's variable
 */
SyntaxTree::Node* SymbolTable::newVariable(std::string id, SyntaxTree::Node* value) {
	Symbol symbol(Type::unknown, k_var, NULL, (value != NULL));

	Symbol* symb = find(id, k_var, NULL, true);

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

Symbol* SymbolTable::find(std::string id, Kind kind, TypeList* typeList, bool local) {
	SymbolTable* current = this;

	do {
		SymbolList sl = current->symbolList;
		auto result = sl.find(id);

		if(result != sl.end() && 
			result->second._kind == kind &&
			(typeList == NULL || typeList == result->second._typeList)) {
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

SyntaxTree::Block* SymbolTable::newFunction(std::string id, Type type, 
	SyntaxTree::List* params, SyntaxTree::Block* body, 
	SyntaxTree::Node* returnValue) {

	TypeList* tl;

	if(params != NULL) {
		tl = params->getTypeList();
	}

	Symbol symbol(type, k_func, tl, (body != NULL));

	Symbol* symb = find(id, k_func, tl, true);

	if (symb == NULL || !symb->_initialized) {
		symbolList[id] = symbol;
	} else if(symb->_initialized) {
		error("semantic", "re-declaration of function %s\n", id.c_str());
	}

	return new SyntaxTree::Function(id, type, params, body, returnValue);
}

SyntaxTree::Node* SymbolTable::callFunction(std::string id, SyntaxTree::List* params) {
	TypeList* tl;

	if(params != NULL) {
		tl = params->getTypeList();
	}

	Symbol* symb = find(id, k_func, tl, false);

	if(symb == NULL) { error("semantic", "undeclared function %s\n", id.c_str()); }

	SyntaxTree::FunctionCall* result = new SyntaxTree::FunctionCall(id, params);
	result->setType(symb->_type);

	return result;
}

const char* Symbol::typeToString(Type type) {
	switch(type) {
		case SymTbl::Type::t_int: return "int"; 
		case SymTbl::Type::t_float: return "float";
		case SymTbl::Type::t_bool: return "bool";
		case SymTbl::Type::unknown: return "unknown";
	}
}