/* Inspired on https://github.com/llpilla/compiler_examples/blob/master/simple_ast/ast.h */
#ifndef DRD_SYNTAX_TREE_H
#define DRD_SYNTAX_TREE_H

#include <iostream>
#include <vector>

extern void yyerror(const char*, ...);

namespace SymTbl {
	class Symbol;
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
		Type _type;
		Node() {}
		Node(Type type) : _type(type) {}
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
		Declarable(std::string id, SymTbl::Symbol* symbol);
	};

	class Variable : public Declarable {
	public:
		Declarable* _next;
		Node* _value;
		Variable(std::string id, SymTbl::Symbol* symbol, Node* value, Declarable* next);
		void printTree();
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
		Declarable* _node;
		Declaration(Declarable* node);
		void printTree();
	};
	
}
#endif