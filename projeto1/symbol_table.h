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

	class Symbol {
	public:
		Type _type;
		Kind _kind;
		bool _initialized;

		Symbol(Type type = Type::unknown, Kind kind = k_var, bool initialized = false) :
			_type(type), _kind(kind), _initialized(initialized) {}

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
		void setType(std::string id, Type type);
		Symbol* find(std::string id, bool local = false);
		SymbolTable* getParent() { return (_parent == NULL) ? this : _parent; }
	};
}

#endif