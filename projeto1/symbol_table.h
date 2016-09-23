/* Inspirated by https://github.com/llpilla/compiler_examples/blob/master/simple_ast/st.h */

/* 
 * Symbol Table model, to use in compilers parser
 */

#pragma once

#include <map>
#include "syntax_tree.h"

extern void error(const char* type, const char* msg, ...);

namespace SymTbl
{
	enum Type { integer };
	enum Kind { variable };

	class Symbol;

	typedef std::map<std::string, Symbol> SymbolList;

	class Symbol {
	public:
		Type _type;
		Kind _kind;
		bool _initialized;
		Symbol(Type type, Kind kind, bool initialized) :
			_type(type), _kind(kind), _initialized(initialized) {}
		Symbol() {_type = integer; _kind = variable; _initialized = false;}
	};

	class SymbolTable {
	public:
		SymbolTable() {}
		SymbolList symbolList;
		SyntaxTree::Node* newVariable(std::string id, SyntaxTree::Node* next, SyntaxTree::Node* value);
		SyntaxTree::Node* useVariable(std::string id);
		SyntaxTree::Node* assignVariable(std::string id);
		bool contains(std::string id) {return symbolList.find(id) != symbolList.end(); }
	};
}