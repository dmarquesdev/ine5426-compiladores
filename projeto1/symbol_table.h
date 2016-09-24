/* Inspirated by https://github.com/llpilla/compiler_examples/blob/master/simple_ast/st.h */
#ifndef DRD_SYMBOL_TABLE_H
#define DRD_SYMBOL_TABLE_H

#include <map>
#include <iostream>
#include "syntax_tree.h"

extern void yyerror(const char* msg, ...);

namespace SymTbl
{
	enum class Type { t_int, t_float, t_bool };

	enum Kind { k_var, k_func };

	class Symbol;

	typedef std::map<std::string, Symbol> SymbolList;

	class Symbol {
	public:
		Type _type;
		Kind _kind;
		bool _initialized;

		Symbol(Type type, Kind kind, bool initialized) :
			_type(type), _kind(kind), _initialized(initialized) {}
		Symbol() {_type = Type::t_int; _kind = k_var; _initialized = false;}
	};

	class SymbolTable {
	public:
		SymbolTable() {}
		SymbolList symbolList;
		SyntaxTree::Declarable* newVariable(std::string id, Symbol* symbol, SyntaxTree::Declarable* next, SyntaxTree::Node* value);
		SyntaxTree::Declarable* useVariable(std::string id);
		SyntaxTree::Declarable* assignVariable(std::string id);
		bool contains(std::string id) {return symbolList.find(id) != symbolList.end(); }
	};
}

#endif