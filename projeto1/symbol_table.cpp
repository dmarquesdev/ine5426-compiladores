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
SyntaxTree::Variable* SymbolTable::newVariable(std::string id, SyntaxTree::Node* value) {
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

SyntaxTree::Node* SymbolTable::declareFunction(std::string id, Type type, 
	SyntaxTree::List* params) {

	TypeList* tl;

	if(params != NULL) {
		tl = params->getTypeList();
	}

	Symbol symbol(type, k_func, tl, false);

	Symbol* symb = find(id, k_func, tl, true);

	if (symb == NULL) {
		symbolList[id] = symbol;
	} else {
		error("semantic", "re-definition of function %s\n", id.c_str());
	}

	return new SyntaxTree::FunctionDeclaration(id, type, params);
}

SyntaxTree::Block* SymbolTable::defineFunction(std::string id, Type type, 
	SyntaxTree::List* params, 
	SyntaxTree::Block* body, 
	SyntaxTree::Node* returnValue) {

	TypeList* tl;

	if(params != NULL) {
		tl = params->getTypeList();
	}

	Symbol* symb = find(id, k_func, tl, true);

	if(symb == NULL) {
		Symbol symbol(type, k_func, tl, false);

		symbolList[id] = symbol;
		symb = &symbol;
	}

	if(symb->_initialized) {
		error("semantic", "re-definition of function %s\n", id.c_str());
	}

	symbolList[id]._initialized = true;

	return new SyntaxTree::Function(
		new SyntaxTree::FunctionDeclaration(id, type, params), 
		body, returnValue);
}

SyntaxTree::Node* SymbolTable::callFunction(std::string id, SyntaxTree::List* params) {
	TypeList* tl;

	if(params != NULL) {
		tl = params->getTypeList();
	}

	Symbol* symb = find(id, k_func, NULL, false);

	if(symb == NULL) { 
		error("semantic", "undeclared function %s\n", id.c_str()); 
	} else if(!symb->_initialized) {
		error("semantic", "undefined function %s\n", id.c_str());
	} else if(tl != NULL && symb->_typeList != NULL) {
			checkParameters(id, symb->_typeList, tl);
	}


	SyntaxTree::FunctionCall* result = new SyntaxTree::FunctionCall(id, params);
	result->setType(symb->_type);

	return result;
}

void SymbolTable::checkUndefinedFunction() {
	for(auto i = symbolList.begin(); i != symbolList.end(); ++i) {
		if(i->second._kind == k_func && !i->second._initialized) {
			std::string id = i->first;
			error("semantic", "function %s is declared but never defined\n", id.c_str());
		}
	}
}

void SymbolTable::checkParameters(std::string id, TypeList* symbolTL, TypeList* callTL) {
	int symbolSize = symbolTL->size(), callSize = callTL->size();

	if(symbolSize != callSize) {
		error("semantic", "function %s expects %d parameters but received %d\n", 
			id.c_str(), symbolTL->size(), callTL->size());
	} else {
		for(int i = 0; i < symbolTL->size(); i++) {
			Type symbolType = (*symbolTL)[i], 
				callType = (*callTL)[i];

			if(symbolType != callType) {
				error("semantic", "parameter %d expected %s but received %s\n", 
					(i+1), Symbol::typeToString(symbolType), 
					Symbol::typeToString(callType));
			}
		}
	}
}

void SymbolTable::addParameters(SyntaxTree::List* params) {
	SyntaxTree::List* current = params;
	while(current != NULL) {
		Symbol symbol(current->getType(), k_var, NULL, true);

		SyntaxTree::Variable* var = current->_node;

		symbolList[var->_id] = symbol;

		current = current->_next;
	}
}

SymbolTable* SymbolTable::endScope() {
	checkUndefinedFunction();
	return (_parent == NULL) ? this : _parent;
}

const char* Symbol::typeToString(Type type) {
	switch(type) {
		case SymTbl::Type::t_int: return "int"; 
		case SymTbl::Type::t_float: return "float";
		case SymTbl::Type::t_bool: return "bool";
		case SymTbl::Type::unknown: return "unknown";
	}
}