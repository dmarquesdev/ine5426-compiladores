/* Inspired on https://github.com/llpilla/compiler_examples/blob/master/simple_ast/parser.y */

%{
#include <iostream>
#include "symbol_table.h"
#include "syntax_tree.h"

SymTbl::SymbolTable symbolTable;
SyntaxTree::Block* programRoot;

extern int yylex();
extern void yyerror(const char* s, ...);


%}

%define parse.trace

/* yylval == %union
 * union informs the different ways we can store data
 */
%union {
    int ival;
    const char* cval;
    float fval;
    bool bval;
    SyntaxTree::Node* node;
    SyntaxTree::Block* block;
}

/* token defines our terminal symbols (tokens).
 */
%token T_TYPE T_PLUS T_TIMES T_SUB T_DIV T_NL T_OPEN_PAR 
T_CLOSE_PAR T_ATTRIB T_COMMA T_NEGATION T_BOOL_REL T_BOOL_BIN

%token <ival> T_INT

%token <cval> T_VAR_NAME 

%token <fval> T_FLOAT

%token <bval> T_BOOL

/* type defines the type of our nonterminal symbols.
 * Types should match the names used in the union.
 * Example: %type<node> expr
 */
%type <node> expr boolExpr aritmExpr varDecl line
%type <block> program lines

/* Operator precedence for mathematical operators
 * The latest it is listed, the highest the precedence
 * left, right, nonassoc
 */

 %left T_PLUS T_SUB
 %left T_TIMES T_DIV
 %left USUB

/* Starting rule 
 */
%start program

%%

program: 
    lines { programRoot = $1; } /* Start program root block with all lines */
    ;

lines: 
    /* Declares a new code block for that line and if line contains something, add it
     * to the other program lines
     */
    line { $$ = new SyntaxTree::Block(); if($1 != NULL) $$->_lines.push_back($1); }
    /* Add the line to the lines block in tree */
    | lines line { if($2 != NULL) $$->_lines.push_back($2); }
    ;

line: 
    T_NL { $$ = NULL; } /*nothing here to be used */
    | expr T_NL /*$$ = $1 when nothing is said*/
    | T_TYPE varDecl T_NL { $$ = new SyntaxTree::Declaration($2); } /* Variable declaration */
    | T_VAR_NAME T_ATTRIB expr T_NL { SyntaxTree::Node* node = symbolTable.assignVariable($1);
            $$ = new SyntaxTree::BinaryOp(node, SyntaxTree::assign, $3); }/*$$ = $1 when nothing is said*/
    ;

varDecl:
    T_VAR_NAME { $$ = symbolTable.newVariable($1, NULL, NULL); } /* Adds variable id to Symbol Table */
    | T_VAR_NAME T_ATTRIB expr { $$ = symbolTable.newVariable($1, NULL, $3); }
    | varDecl T_COMMA T_VAR_NAME { $$ = symbolTable.newVariable($3, $1, NULL); }
    | varDecl T_COMMA T_VAR_NAME T_ATTRIB expr { $$ = symbolTable.newVariable($3, $1, $5);}
    ;

expr:
    T_VAR_NAME { $$ = symbolTable.useVariable($1); }
    | aritmExpr 
    | boolExpr
    ;

aritmExpr: 
    T_INT { $$ = new SyntaxTree::Integer($1); }
    | T_SUB expr %prec USUB { $$ = new SyntaxTree::UnaryOp($2, SyntaxTree::negation); } 
    | T_OPEN_PAR expr T_CLOSE_PAR { $$ = $2; }
    | expr T_PLUS expr { $$ = new SyntaxTree::BinaryOp($1, SyntaxTree::plus, $3); }
    | expr T_TIMES expr { $$ = new SyntaxTree::BinaryOp($1, SyntaxTree::times, $3); }
    | expr T_DIV expr { $$ = new SyntaxTree::BinaryOp($1, SyntaxTree::division, $3); }
    | expr T_SUB expr { $$ = new SyntaxTree::BinaryOp($1, SyntaxTree::minus, $3); }
    ;

boolExpr:
    T_BOOL 
    | T_NEGATION expr
    | expr T_BOOL_REL expr
    | expr T_BOOL_BIN expr
    ;

%%


