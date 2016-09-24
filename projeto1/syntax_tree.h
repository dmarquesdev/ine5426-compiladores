/* Inspired on https://github.com/llpilla/compiler_examples/blob/master/simple_ast/ast.h */
#ifndef DRD_SYNTAX_TREE_H
#define DRD_SYNTAX_TREE_H

#include <iostream>
#include <vector>

extern void yyerror(const char*, ...);

namespace SymTbl {
	class Symbol;
}

namespace SyntaxTree
{
	enum Operation { plus, minus, times, division, assign, 
		greater, greater_equal, equals, less, less_equal, 
		different, bool_and, bool_or };

	enum UniOperation { negative, negation };

	class Node;

	typedef std::vector<Node*> NodeList;

	class Node {
	public:
		virtual ~Node(){}
		virtual void printTree(){}
	};

	class Variable;

	class Declarable : public Node {
		friend class Variable;
		
	public:
		std::string _id;
		SymTbl::Symbol* _symbol;
		virtual void printTree() {}
	private:
		Declarable(std::string id, SymTbl::Symbol* symbol) : 
			_id(id), _symbol(symbol) {}
	};

	class Variable : public Declarable {
	public:
		Declarable* _next;
		Node* _value;
		Variable(std::string id, SymTbl::Symbol* symbol, Node* value, Declarable* next) : 
			Declarable(id, symbol), _value(value), _next(next) {}
		void printTree();
	};

	class BinaryOp : public Node {
	public:
		Operation _op;
		Node* _left;
		Node* _right;
		BinaryOp(Node* left, Operation op, Node* right) : 
			_left(left), _op(op), _right(right) {}
		void printTree();
	};

	class Block : public Node {
	public:
		NodeList _lines;
		Block(){}
		void printTree();
	};

	class Integer : public Node {
	public:
		int _value;
		Integer(int value) : _value(value) {}
		void printTree();
	};

	class Float : public Node {
	public:
		float _value;
		Float(float value) : _value(value) {}
		void printTree();
	};

	class Bool : public Node {
	public:
		bool _value;
		Bool(bool value) : _value(value) {}
		void printTree();
	};	

	class UnaryOp : public Node {
	public:
		Node* _node;
		UniOperation _op;
		UnaryOp(Node* node, UniOperation op) : 
			_node(node), _op(op) {}
		void printTree();
	};

	class Declaration : public Node {
	public:
		Declarable* _node;
		Declaration(Declarable* node) : _node(node) {}
		void printTree();
	};
	
}
#endif