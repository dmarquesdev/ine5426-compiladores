/* Inspired on https://github.com/llpilla/compiler_examples/blob/master/simple_ast/ast.cpp */

#include "syntax_tree.h"
#include "symbol_table.h"

using namespace SyntaxTree;

extern SymTbl::SymbolTable symbolTable;

void Variable::printTree() {
	if(_next != NULL) {
		_next->printTree();
		std::cout << ", ";
	}
	std::cout << _id;
	if(_value != NULL) {
		std::cout << " = ";
		_value->printTree();
	}
}

BinaryOp::BinaryOp(Node* left, Operation op, Node* right) {
	if(!isValid(left, right, op)) {
		yyerror("erro de operador");
	}
	_left = left;
	_right = right;
	_op = op;
}

bool BinaryOp::isValid() {
	return isValid(_left, _right, _op);
}

bool BinaryOp::isValid(Node* n1, Node* n2, Operation op) {
	// TODO make validation about binary op between two nodes
	return true;
}

void BinaryOp::printTree() {
	switch(_op) {
		case plus: std::cout << "+ "; break;
		case minus: std::cout << "- "; break;
		case times: std::cout << "* "; break;
		case division: std::cout << "/ "; break;
		case assign: std::cout << "= "; break;
		case equals: std::cout << "== "; break;
		case different: std::cout << "!= "; break;
		case greater: std::cout << "> "; break;
		case less: std::cout << "< "; break;
		case greater_equal: std::cout << ">= "; break;
		case less_equal: std::cout << "<= "; break;
		case bool_and: std::cout << "& "; break;
		case bool_or: std::cout << "| "; break;
	}

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

void Integer::printTree() {
	std::cout << _value;
}

void Float::printTree() {
	std::cout << _value;
}

void Bool::printTree() {
	std::cout << ((_value) ? "true" : "false");
}

void UnaryOp::printTree() {
	switch(_op) {
		case negative: std::cout << "-u "; break;
		case negation: std::cout << "! "; break;
	}
	_node->printTree();
}

void Declaration::printTree() {
	switch(_node->_symbol->_type) {
		case SymTbl::Type::t_int: std::cout << "int "; break;
		case SymTbl::Type::t_float: std::cout << "float "; break;
		case SymTbl::Type::t_bool: std::cout << "bool "; break;
	}
	std::cout << "var: ";
	_node->printTree();
}