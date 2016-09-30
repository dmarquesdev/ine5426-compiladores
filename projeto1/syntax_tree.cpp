/* Inspired on https://github.com/llpilla/compiler_examples/blob/master/simple_ast/ast.cpp */

#include "syntax_tree.h"
#include "symbol_table.h"

using namespace SyntaxTree;

extern SymTbl::SymbolTable symbolTable;

typedef SymTbl::Symbol Symbol;

BinaryOp::BinaryOp(Node* left, Operation op, Node* right) : Node(left->getType()) {
	if(!isValid(left, right, op)) {
		yyerror("%s operation expected %s but received %s\n", 
			Node::operationToString(op), 
			Symbol::typeToString(left->getType()), 
			Symbol::typeToString(right->getType()));
	}

	_left = left;
	_right = right;
	_op = op;

	if(op == greater || op == less 
		|| op == greater_equal || op == less_equal 
		|| op == bool_and || op == bool_or) {
		setType(Type::t_bool);
	}
}

bool BinaryOp::isValid() {
	return isValid(_left, _right, _op);
}

bool BinaryOp::isValid(Node* n1, Node* n2, Operation op) {
	Type t1 = n1->getType(), t2 = n2->getType();
	
	if(op == plus || op == minus || op == times || 
		op == division || op == greater || op == less 
		|| op == greater_equal || op == less_equal) {

		return t1 != Type::t_bool && t2 != Type::t_bool;

	} else if (op == bool_and || op == bool_or) {
		return t1 == Type::t_bool && t2 == Type::t_bool;
	} else if (op == assign) {
		return t1 == t2;
	}

	return true;
}

Variable::Variable(std::string id, Type type, Node* value) : Node(type) {
	_id = id;
	_value = value;
}

List::List(Node* node, Node* next) : Node(node->getType()) {
	_node = node;
	_next = next;
}

UnaryOp::UnaryOp(Node* node, UniOperation op) : Node(node->getType()) {
	_node = node;
	_op = op;
}

Declaration::Declaration(Type type, Node* node) : Node(type) {
	_node = node;
	setType(type);
}

Integer::Integer(int value) : Node(Type::t_int), _value(value) {}
Float::Float(float value) : Node(Type::t_float), _value(value) {}
Boolean::Boolean(bool value) : Node(Type::t_bool), _value(value) {}

void Declaration::setType(Type type) {
	Node::setType(type);

	_node->setType(type);
}

void List::setType(Type type) {
	Node::setType(type);

	_node->setType(type);

	if(_next != NULL) {
		_next->setType(type);
	}
}

void Variable::setType(Type type) {
	Node::setType(type);

	if(!isValueValid()) {
		yyerror("attribution operation expected %s but received %s\n", 
			Symbol::typeToString(type), 
			Symbol::typeToString(_value->getType()));
	}

	symbolTable.setType(_id, type);
}

bool Variable::isValueValid() {
	 if(_value != NULL) { 
	 	return _value->getType() == getType(); 
	 } 

	 return true;
}

void Variable::printTree() {
	std::cout << _id;
	if(_value != NULL) {
		std::cout << " = ";
		_value->printTree();
	}
}

void List::printTree() {
	if(_next != NULL) {
		_next->printTree();
		std::cout << ", ";
	}
	_node->printTree();
}

void BinaryOp::printTree() {
	std::cout << Node::operationToString(_op) << " ";

	_left->printTree();
	std::cout << " ";
	_right->printTree();
}

void Block::printTree() {
	for(Node* line : _lines) {
		line->printTree();
		std::cout << std::endl;
	}
}

void UnaryOp::printTree() {
	switch(_op) {
		case negative: std::cout << "-u "; break;
		case negation: std::cout << "! "; break;
	}
	_node->printTree();
}

void Declaration::printTree() {
	std::cout << Symbol::typeToString(getType()) << " ";
	std::cout << "var: ";
	_node->printTree();
}

const char* Node::operationToString(Operation op) {
	switch(op) {
		case plus: return "+";
		case minus: return "-";
		case times: return "*";
		case division: return "/";
		case assign: return "=";
		case equals: return "==";
		case different: return "!=";
		case greater: return ">";
		case less: return "<";
		case greater_equal: return ">=";
		case less_equal: return "<=";
		case bool_and: return "&";
		case bool_or: return "|";
	}
}