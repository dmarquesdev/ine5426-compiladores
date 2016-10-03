/* Inspired on https://github.com/llpilla/compiler_examples/blob/master/simple_ast/ast.h */
#ifndef DRD_SYNTAX_TREE_H
#define DRD_SYNTAX_TREE_H

#include <iostream>
#include <vector>

extern void error(const char* type, const char* msg, ...);

namespace SymTbl {
	enum class Type;
	class SymbolTable;
}

extern SymTbl::SymbolTable* symbolTable;

namespace SyntaxTree
{
	enum Operation { plus, minus, times, division, assign, 
		greater, greater_equal, equals, less, less_equal, 
		different, bool_and, bool_or };

	enum UniOperation { negative, negation, casting };

	class Node;

	typedef std::vector<Node*> NodeList;

	typedef SymTbl::Type Type;
	typedef SymTbl::SymbolTable SymbolTable;
	typedef std::vector<Type> TypeList;

	static SymbolTable* CURRENT_ST;

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
		virtual void setType(Type type);
		TypeList* getTypeList();
		int getSize();
	private:
		TypeList _typeList;
	};

	class Variable : public Node {
	public:
		std::string _id;
		Node* _value;
		Variable(std::string id, Type type, Node* value);
		void printTree();
		virtual void setType(Type type);
		bool isValueValid();
	private:
		void coercion();
	};

	class BinaryOp : public Node {
	public:
		Operation _op;
		Node* _left;
		Node* _right;
		BinaryOp(Node* left, Operation op, Node* right);
		void printTree();
	private:
		bool isValid(Node* n1, Node* n2, Operation op);
		void coercion(Node* n1, Node* n2);
	};

	class Block : public Node {
	public:
		NodeList _lines;
		Block* _parent;
		SymbolTable* _symbolTable;

		Block() : _symbolTable(symbolTable) {}
		Block(SymbolTable* symbolTable) : 
			_symbolTable(symbolTable) {}

		virtual void printTree();
		int getLevel();
		void append(Block* block);
	};

	class Conditional : public Block {
	public:
		Node* _condition;
		Block* _ifBlock;
		Block* _elseBlock;
		Conditional(Node* condition, 
			Block* ifBlock, Block* elseBlock);
		void printTree();
	};

	class ForLoop : public Block {
	public:
		Node* _initialization;
		Node* _test;
		Node* _iteration;
		ForLoop(Node* initialization, Node* test, 
			Node* iteration);
		void printTree();
		void setForBlock(Block* block);
		Block* getForBlock() { return _forBlock; }
	private:
		Block* _forBlock;
	};

	class Function : public Block {
	public:
		std::string _id;
		List* _parameters;
		Block* _body;
		Node* _returnValue;
		Function(std::string id, Type type, List* parameters, Block* body, Node* returnValue);
		void printTree();
	};

	class FunctionCall : public Node {
	public:
		std::string _id;
		List* _parameters;
		FunctionCall(std::string id, List* parameters);
		void printTree();
	};

	class UnaryOp : public Node {
	public:
		Node* _node;
		UniOperation _op;
		UnaryOp(Node* node, UniOperation op);
		virtual void printTree();
	};

	class Cast : public UnaryOp {
	public:
		Cast(Type type, Node* node);
		void printTree();
	};

	class Declaration : public Node {
	public:
		Node* _node;
		Declaration(Type type, Node* node);
		void printTree();
		virtual void setType(Type type);
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