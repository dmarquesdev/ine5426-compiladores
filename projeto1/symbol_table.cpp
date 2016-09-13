/* Inspirated by https://github.com/llpilla/compiler_examples/blob/master/simple_ast/st.cpp */
#include "symbol_table.h"
#include <cstdlib>

Compiler::SymbolTable::SymbolList symbolList;

void Compiler::SymbolTable::add(string id, Symbol symbol) {
	if (!contains(id)) {
		symbolList[id] = symbol;
	}
}

Compiler::Symbol Compiler::SymbolTable::get(string id) {
	if(contains(id)) {
		return symbolList[id];
	}
}