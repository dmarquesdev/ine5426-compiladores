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
}

void BinaryOp::printTree() {
	_left->printTree();
	switch(_op) {
		case plus: std::cout << " + "; break;
		case minus: std::cout << " - "; break;
		case times: std::cout << " * "; break;
		case division: std::cout << " / "; break;
		case assign: std::cout << " = "; break;
	}
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

void UnaryOp::printTree() {
	switch(_op) {
		case negation: std::cout << " - ";
	}
	_node->printTree();
}