/* Inspirated by https://github.com/llpilla/compiler_examples/blob/master/simple_ast/st.h */

#ifndef DRD_SYMBOL_TABLE_H
#define DRD_SYMBOL_TABLE_H

#include <map>
#include <iostream>
#include "syntax_tree.h"

extern void error(const char* type, const char* msg, ...);

namespace SymTbl
{
	enum class Type { unknown, t_int, t_float, t_bool };

	enum Kind { k_var, k_func };

	class Symbol;

	typedef std::map<std::string, Symbol> SymbolList;
	typedef std::vector<Type> TypeList;

	class Symbol {
	public:
		Type _type;
		Kind _kind;
		bool _initialized;
		TypeList* _typeList;

		Symbol(Type type = Type::unknown, Kind kind = k_var, 
			TypeList* typeList = NULL, bool initialized = false) :
			_type(type), _kind(kind), _typeList(typeList), 
			_initialized(initialized) {}

		static const char* typeToString(Type type);
	};

	class SymbolTable {
	public:
		SymbolTable() {}
		SymbolTable(SymbolTable* parent) : _parent(parent) {}

		SymbolList symbolList;
		SymbolTable* _parent;

		SyntaxTree::Node* newVariable(std::string id, SyntaxTree::Node* value);
		SyntaxTree::Node* useVariable(std::string id);
		SyntaxTree::Node* assignVariable(std::string id);


		SyntaxTree::Block* newFunction(std::string id, Type type, 
			SyntaxTree::List* params, SyntaxTree::Block* body, 
			SyntaxTree::Node* returnValue);

		SyntaxTree::Node* callFunction(std::string id, SyntaxTree::List* params);

		void setType(std::string id, Type type);
		Symbol* find(std::string id, Kind kind = k_var, TypeList* typeList = NULL, bool local = false);
		SymbolTable* getParent() { return (_parent == NULL) ? this : _parent; }
	};
}

#endif