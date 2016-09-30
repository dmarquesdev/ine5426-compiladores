/* Inspired on https://github.com/llpilla/compiler_examples/blob/master/simple_ast/ast.h */
#ifndef DRD_SYNTAX_TREE_H
#define DRD_SYNTAX_TREE_H

#include <iostream>
#include <vector>

extern void yyerror(const char*, ...);

namespace SymTbl {
	enum class Type;
}

namespace SyntaxTree
{
	enum Operation { plus, minus, times, division, assign, 
		greater, greater_equal, equals, less, less_equal, 
		different, bool_and, bool_or };

	enum UniOperation { negative, negation };

	class Node;

	typedef std::vector<Node*> NodeList;

	typedef SymTbl::Type Type;

	class Node {
	public:
		Node() {}
		Node(Type type) {
			setType(type);
		}
		virtual ~Node(){}
		virtual void printTree(){}
		virtual void setType(Type type) { _type = type; }
		Type getType() { return _type; }
		static const char* operationToString(Operation op);
	private:
		Type _type;
	};

	class List : public Node {
	public:
		Node* _node;
		Node* _next;
		void printTree();
		List(Node* node, Node* next);
		void setType(Type type);
	};

	class Variable : public Node {
	public:
		std::string _id;
		Node* _value;
		Variable(std::string id, Type type, Node* value);
		void printTree();
		void setType(Type type);
		bool isValueValid();
	};

	class BinaryOp : public Node {
	public:
		Operation _op;
		Node* _left;
		Node* _right;
		BinaryOp(Node* left, Operation op, Node* right);
		void printTree();
		bool isValid();
	private:
		bool isValid(Node* n1, Node* n2, Operation op);
	};

	class Block : public Node {
	public:
		NodeList _lines;
		Block() {}
		void printTree();
	};

	class UnaryOp : public Node {
	public:
		Node* _node;
		UniOperation _op;
		UnaryOp(Node* node, UniOperation op);
		void printTree();
	};

	class Declaration : public Node {
	public:
		Node* _node;
		Declaration(Type type, Node* node);
		void printTree();
		void setType(Type type);
	};

	class Integer : public Node {
	public:
		int _value;
		Integer(int value);
		void printTree() { std::cout << _value; }
	};

	class Float : public Node {
	public:
		float _value;
		Float(float value);
		void printTree() { std::cout << _value; }
	};

	class Boolean : public Node {
	public:
		bool _value;
		Boolean(bool value);
		void printTree() { std::cout << ((_value) ? "true" : "false"); }
	};
	
}
#endif