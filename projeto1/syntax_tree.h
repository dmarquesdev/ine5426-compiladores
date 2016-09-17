/* Inspired on https://github.com/llpilla/compiler_examples/blob/master/simple_ast/ast.h */
#pragma once

#include <iostream>
#include <vector>

extern void yyerror(const char*, ...);

namespace SyntaxTree
{
	enum Operation { plus, minus, times, division, assign };
	enum UniOperation { negation };

	class Node;

	typedef std::vector<Node*> NodeList;

	class Node {
	public:
		virtual ~Node(){}
		virtual void printTree(){}
	};

	class Variable : public Node {
	public:
		std::string _id;
		Node* _next;
		Variable(std::string id, Node* next) : 
			_id(id), _next(next) {}
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

	class UnaryOp : public Node {
	public:
		Node* _node;
		UniOperation _op;
		UnaryOp(Node* node, UniOperation op) : 
			_node(node), _op(op) {}
		void printTree();
	};

	/*
	class Definition : public Node {
	public:
		SymTbl::Symbol _symbol;
		std::string _id;
		Definition(std::string id, SymTbl::Symbol symbol) :
			_id(id), _symbol(symbol){}
		void printTree();
	}
	*/
}