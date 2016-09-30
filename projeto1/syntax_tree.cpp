/* Inspired on https://github.com/llpilla/compiler_examples/blob/master/simple_ast/ast.cpp */

#include "syntax_tree.h"
#include "symbol_table.h"

using namespace SyntaxTree;

extern SymTbl::SymbolTable symbolTable;

typedef SymTbl::Symbol Symbol;

BinaryOp::BinaryOp(Node* left, Operation op, Node* right) : Node(left->getType()) {
	_left = left;
	_right = right;
	_op = op;

	if(op == greater || op == less 
		|| op == greater_equal || op == less_equal 
		|| op == bool_and || op == bool_or) {
		setType(Type::t_bool);
	}

	if(!isValid(left, right, op)) {
		error("semantic", "%s operation expected %s but received %s\n", 
			Node::operationToString(op), 
			Symbol::typeToString(left->getType()), 
			Symbol::typeToString(right->getType()));
	}
}

bool BinaryOp::isValid() {
	return isValid(_left, _right, _op);
}

bool BinaryOp::isValid(Node* n1, Node* n2, Operation op) {
	coercion(n1, n2);

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

void BinaryOp::coercion(Node* n1, Node* n2) {
	Type t1 = n1->getType(), t2 = n2->getType();

	if(t1 == Type::t_float && t2 == Type::t_int) {
		Cast* cast = new Cast(Type::t_float, n2);
		_right = cast;
		setType(Type::t_float);
	} else if(t2 == Type::t_float && t1 == Type::t_int) {
		Cast* cast = new Cast(Type::t_float, n1);
		_left = cast;
		setType(Type::t_float);
	}
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

Cast::Cast(Type type, Node* node) : UnaryOp(node, casting) {
	setType(type);
}

Declaration::Declaration(Type type, Node* node) : Node(type) {
	_node = node;
	setType(type);
}

Conditional::Conditional(Node* condition, Block* ifBlock, Block* elseBlock) : 
	_condition(condition), _ifBlock(ifBlock), _elseBlock(elseBlock) {
	_ifBlock->_parent = this;
	if(_elseBlock != NULL) {
		_elseBlock->_parent = this;
	}
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
	symbolTable.setType(_id, type);

	if(!isValueValid()) {
		error("semantic", "attribution operation expected %s but received %s\n", 
			Symbol::typeToString(type), 
			Symbol::typeToString(_value->getType()));
	} else if(_value != NULL) {
		coercion();
	}
}

void Variable::coercion() {
	if(getType() == Type::t_int && _value->getType() == Type::t_float) {
		_value = new Cast(Type::t_int, _value);
	} else if(getType() == Type::t_float && _value->getType() == Type::t_int) {
		_value = new Cast(Type::t_float, _value);
	}
}

bool Variable::isValueValid() {
	 if(_value != NULL) { 
	 	Type t1 = _value->getType(), t2 = getType();

	 	bool numeric = (t1 == Type::t_int || t1 == Type::t_float) 
	 	&& (t2 == Type::t_int || t2 == Type::t_float);

	 	return t1 == t2 || numeric; 
	 } 

	 return true;
}

int Block::getLevel() { 
	if(_parent == NULL) {
		return 0;
	} else {
		return _parent->getLevel() + 1;
	}
}

void Block::append(Block* block) {
	_lines.push_back(block);
	block->_parent = this;
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
	int level = getLevel();

	if(level > 0) {
		level--;
	}

	std::string tab = std::string(level*2, ' ');

	for(Node* line : _lines) {
		std::cout << tab;
		line->printTree();
		std::cout << std::endl;
	}
}

void UnaryOp::printTree() {
	switch(_op) {
		case negative: std::cout << "-u "; break;
		case negation: std::cout << "! "; break;
		default: break;
	}
	_node->printTree();
}

void Declaration::printTree() {
	std::cout << Symbol::typeToString(getType()) << " ";
	std::cout << "var: ";
	_node->printTree();
}

void Cast::printTree() {
	std::cout << "[" << Symbol::typeToString(getType()) << "] ";
	_node->printTree();
}

void Conditional::printTree() {
	int level = getLevel();

	if(level > 0) {
		level--;
	}

	std::string tab = std::string(level*2, ' ');

	std::cout << tab << "if: ";
	_condition->printTree();
	std::cout << "\n" << tab << "then: " << std::endl;
	_ifBlock->printTree();
	if(_elseBlock != NULL) {
		std::cout << "\n" << tab << "else: " << std::endl;
		_elseBlock->printTree();
	}
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