/* Inspired on https://github.com/llpilla/compiler_examples/blob/master/simple_ast/ast.cpp */

#include "syntax_tree.h"
#include "symbol_table.h"

using namespace SyntaxTree;

extern SymTbl::SymbolTable symbolTable;

typedef SymTbl::Symbol Symbol;

/*
* Contrutor operação binária.
* param left: Nodo de arvore, equivale ao operando a esquerda.
* param op: operador, operação entre os operandos.
* param rigth: Nodo de arvore, equivale ao operando a direita.
*
*/
BinaryOp::BinaryOp(Node* left, Operation op, Node* right) : Node(left->getType()) {
	_left = left;
	_right = right;
	_op = op;

	//verifica se os tipos dos operando satisfazem os tipos permitidos pelo operador.
	if(!isValid(left, right, op)) {
		error("semantic", "%s operation expected %s but received %s\n", 
			Node::operationToString(op), 
			Symbol::typeToString(left->getType()), 
			Symbol::typeToString(right->getType()));
	}

	//define o tipo da operação de acordo como operador.
	if(op == greater || op == less 
		|| op == greater_equal || op == less_equal 
		|| op == bool_and || op == bool_or) {
		setType(Type::t_bool);
	}
}

/*
* Verifica se a operação binaria é valida.
*
* Retorna um boolean
*/
bool BinaryOp::isValid() {
	return isValid(_left, _right, _op);
}

/*
* Verifica se a operação binaria é valida.
*
* param n1: nodo da arvore, equivale ao operando definido a esquerda da operação binária.
* param n2: nodo da arvore, equiva ao operando definido a direita da operação binária.
* param op: equivale ao operador da operação binária.
* return:  um boolean se a operação é ou não válida.
*/
bool BinaryOp::isValid(Node* n1, Node* n2, Operation op) {
	coercion(n1, n2);

	Type t1 = n1->getType(), t2 = n2->getType();
	
	//verifica os tipos permitidos por cada operador binario, 
	//caso os tipos dos operando não satisfaçam o operador retorna falso.
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

/*
* Faz a coersão dos tipos das variaveis não booleanas de operações binárias.
*
* param n1: nodo da arvore, variavel a ser testada e aplicada coersão.
* param n2: nodo da arvore, variavel a ser testada e aplicada coersão.
*/
void BinaryOp::coercion(Node* n1, Node* n2) {
	Type t1 = n1->getType(), t2 = n2->getType();

	//se uma das variaveis é integer e outra é float, faz casting para float da variavel integer.
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

/*
* Contrutor de nodo de variavel.
* Define id (nome) da variavel, valor da variavel e seu tipo.
*
* param id: id (nome) da variável.
* param type: tipo da variavel.
* param value: nodo que armazena o valor da variavel.
*/
Variable::Variable(std::string id, Type type, Node* value) : Node(type) {
	_id = id;
	_value = value;
}

/*
* Contrutor de nodo de lista.
*
* param node: nodo da árvore 
* param next: nodo da árvore, proximo nodo da lista.
*/
List::List(Node* node, Node* next) : Node(node->getType()) {
	_node = node;
	_next = next;
}

/*
* Contrutor de nodo operação unária.
*
* param node: nodo da árvore, variavel em questão.
* param op: operação unária.
*/
UnaryOp::UnaryOp(Node* node, UniOperation op) : Node(node->getType()) {
	_node = node;
	_op = op;
}

/*
* Contrutor cast.
*
* param type: tipo a ser feito cast.
* param node: nodo da árvore com a variavel.
*/
Cast::Cast(Type type, Node* node) : UnaryOp(node, casting) {
	setType(type);
}

/*
* Contrutor de declaração.
*
* param type: tipo da variavel
* param node: nodo da árvore com a veriavel.
*/
Declaration::Declaration(Type type, Node* node) : Node(type) {
	_node = node;
	setType(type);
}

/*
* Contrutor de condicional.
*
* param condition: nodo que contem a condição.
* param ifBlock: bloco que contem o trecho de código do if.
* param elseBlock: bloco que contem o trecho de código do else. Pode ser vazio.
* 
*/
Conditional::Conditional(Node* condition, Block* ifBlock, Block* elseBlock) : 
	_condition(condition), _ifBlock(ifBlock), _elseBlock(elseBlock) {
	_ifBlock->_parent = this;
	if(_elseBlock != NULL) {
		_elseBlock->_parent = this;
	}

	if(_condition->getType() != Type::t_bool) {
		error("semantic", "test operation expected boolean but received %s", 
			Symbol::typeToString(_condition->getType()));
	}
}

/*
* Contrutor do tipo integer
*
* param value: valor da variavel.
*/
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

/*
* Faz a coersao de valores, fazendo casting de tipos quando necessário.
*
*/
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

	 	//verifica se os tipos são pelo menos integer e float ou float e integer.
	 	bool numeric = (t1 == Type::t_int || t1 == Type::t_float) 
	 	&& (t2 == Type::t_int || t2 == Type::t_float);

	 	//retorna true se os tipos forem iguais ou no maximo integer e float.
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

/*
* Imprime variavel e seu valor
*
*/
void Variable::printTree() {
	std::cout << _id;
	if(_value != NULL) {
		std::cout << " = ";
		_value->printTree();
	}
}

/*
* Imprime uma lista.
*
*/
void List::printTree() {
	if(_next != NULL) {
		_next->printTree();
		std::cout << ", ";
	}
	_node->printTree();
}

/*
* Imprime uma operação binária, operador e operandos.
*
*/
void BinaryOp::printTree() {
	std::cout << Node::operationToString(_op) << " ";

	_left->printTree();
	std::cout << " ";
	_right->printTree();
}

/*
* Imprime o blocos de código, são os trechos de código dentro de condicionais e laços.
*
*/
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

/*
* Imprime operação unária, números nagtivos e negação de binarios e boolean.
*
*/
void UnaryOp::printTree() {
	switch(_op) {
		case negative: std::cout << "-u "; break;
		case negation: std::cout << "! "; break;
		default: break;
	}
	_node->printTree();
}

/*
* Imprime a declaração de variaveis.
*/
void Declaration::printTree() {
	std::cout << Symbol::typeToString(getType()) << " ";
	std::cout << "var: ";
	_node->printTree();
}

/*
* Imprime o casting de um tipo.
*/
void Cast::printTree() {
	std::cout << "[" << Symbol::typeToString(getType()) << "] ";
	_node->printTree();
}

/*
* Imprime condicionais if,then,else. Cada um é um bloco a ser printado.
*
*/
void Conditional::printTree() {
	int level = getLevel();

	if(level > 0) {
		level--;
	}

	std::string tab = std::string(level*2, ' ');

	std::cout << tab << "if: ";
	//printa a expressão de condição.
	_condition->printTree();
	std::cout << "\n" << tab << "then: " << std::endl;
	//printa o trecho de código dentro do if.
	_ifBlock->printTree();
	//caso exista o ramo else, printa o trecho de código dentro do else.
	if(_elseBlock != NULL) {
		std::cout << "\n" << tab << "else: " << std::endl;
		_elseBlock->printTree();
	}
}

/*
* Imprime e retorna a string de cada operação.
*
*/
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