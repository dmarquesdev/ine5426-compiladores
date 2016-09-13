/* Inspirated by https://github.com/llpilla/compiler_examples/blob/master/simple_ast/st.h */
#include <map.h>
#include <cstdlib>

namespace Compiler
{
	enum Type { INTEGER };

	typedef map<string, Symbol> SymbolList;

	class SymbolTable {
	public:
		SymbolTable() {}
		~SymbolTable() {}
		SymbolList symbolList;
		void add(string id, Symbol symbol);
		Symbol get(string id);
		bool contains(string id) {return symbolList.find(id) != symbolList.end()}
	}

	class Symbol {
	public:
		Symbol(string id, Type type, int64_t value, bool initialized) :
			_id(id), _type(type), _value(value), _initialized(initialized){}

		~Symbol(){}

		Type _type;
		string _id;
		int64_t _value;
		bool _initialized;
	}
}