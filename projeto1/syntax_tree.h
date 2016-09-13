/* Inspired on https://github.com/llpilla/compiler_examples/blob/master/simple_ast/ast.h */

#include <cstdlib>

namespace Compilers
{
	class Node {
	public:
		virtual ~Node(){}
	}

	class Variable : public Node {
	public:
		Variable(string id) : _id(id) {}
		string _id;
	}
}