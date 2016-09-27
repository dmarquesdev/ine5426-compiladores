/* Inspired on https://github.com/llpilla/compiler_examples/blob/master/simple_ast/parser.y */

/* Expression ambiguity removal inspired on 
 * https://courses.engr.illinois.edu/cs421/sp2010/lectures/lecture10.pdf
 */

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

%code requires {
    namespace SymTbl {
        enum class Type;
    }
}

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
    SyntaxTree::Declarable* decl;
    SymTbl::Type type;
    SyntaxTree::Operation op;
}

/* token defines our terminal symbols (tokens).
 */
%token T_PLUS T_TIMES T_SUB T_DIV T_NL T_OPEN_PAR 
T_CLOSE_PAR T_ATTRIB T_COMMA T_NEGATION 
T_BOOL_EQ T_BOOL_NEQ T_BOOL_GR T_BOOL_LS 
T_BOOL_GE T_BOOL_LE T_BOOL_AND T_BOOL_OR 

%token <ival> T_INT

%token <cval> T_VAR_NAME

%token <type> T_TYPE 

%token <fval> T_FLOAT

%token <bval> T_BOOL

/* type defines the type of our nonterminal symbols.
 * Types should match the names used in the union.
 * Example: %type<node> expr
 */
%type <node> expr line attr
exprTerm1 exprTerm2 exprTerm3 exprTerm4 exprValue
%type <op> mult_op add_op bool_bin_op bool_rel_op bool_cmp_op
%type <block> program lines
%type <decl> varDecl

/* Operator precedence for mathematical operators
 * The latest it is listed, the highest the precedence
 * left, right, nonassoc
 */

 %left T_NEGATION 
 %left T_BOOL_EQ T_BOOL_NEQ T_BOOL_GR T_BOOL_LS T_BOOL_GE T_BOOL_LE 
 %left T_BOOL_AND T_BOOL_OR 
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
    | varDecl T_NL { $$ = new SyntaxTree::Declaration($1); } /* Variable declaration */
    | attr T_NL 
    ;

attr:
    T_VAR_NAME T_ATTRIB expr { SyntaxTree::Declarable* node = symbolTable.assignVariable($1);
            $$ = new SyntaxTree::BinaryOp(node, SyntaxTree::assign, $3); }

varDecl:
    T_TYPE T_VAR_NAME { SymTbl::Symbol* symbol = 
                        new SymTbl::Symbol($1, SymTbl::k_var, false); 
                        $$ = symbolTable.newVariable($2, symbol, NULL, NULL); }

    | T_TYPE T_VAR_NAME T_ATTRIB expr { 
        SymTbl::Symbol* symbol = 
            new SymTbl::Symbol($1, SymTbl::k_var, true); 
        $$ = symbolTable.newVariable($2, symbol, NULL, $4); }

    | varDecl T_COMMA T_VAR_NAME { 
        SymTbl::Symbol* symbol = 
            new SymTbl::Symbol($1->_symbol->_type, SymTbl::k_var, false); 
        $$ = symbolTable.newVariable($3, symbol, $1, NULL); }

    | varDecl T_COMMA T_VAR_NAME T_ATTRIB expr { 
        SymTbl::Symbol* symbol = 
            new SymTbl::Symbol($1->_symbol->_type, SymTbl::k_var, true); 
        $$ = symbolTable.newVariable($3, symbol, $1, $5); }
    ;

/* The expression grammar uses "stratified grammar" to
 * avoid ambiguity
 * https://courses.engr.illinois.edu/cs421/sp2010/lectures/lecture10.pdf
 * Page 9
 */
expr:
    expr bool_cmp_op exprTerm1 { $$ = new SyntaxTree::BinaryOp($1, $2, $3); }
    | exprTerm1
    ;

exprTerm1:
    exprTerm1 bool_bin_op exprTerm2 { $$ = new SyntaxTree::BinaryOp($1, $2, $3); }
    | exprTerm2
    ;

exprTerm2:
    exprTerm2 bool_rel_op exprTerm3 { $$ = new SyntaxTree::BinaryOp($1, $2, $3); }
    | exprTerm3
    ;

exprTerm3:
    exprTerm3 add_op exprTerm4 { $$ = new SyntaxTree::BinaryOp($1, $2, $3); }
    | exprTerm4
    ;

exprTerm4:
    exprTerm4 mult_op exprValue { $$ = new SyntaxTree::BinaryOp($1, $2, $3); }
    | exprValue
    ;

exprValue:
    T_INT { $$ = new SyntaxTree::Node(SymTbl::Type::t_int); }
    | T_FLOAT { $$ = new SyntaxTree::Node(SymTbl::Type::t_float); }
    | T_BOOL { $$ = new SyntaxTree::Node(SymTbl::Type::t_bool); }
    | T_VAR_NAME { $$ = symbolTable.useVariable($1); }
    | T_OPEN_PAR expr T_CLOSE_PAR { $$ = $2; }
    | T_SUB expr %prec USUB { $$ = new SyntaxTree::UnaryOp($2, SyntaxTree::negative); } 
    | T_NEGATION expr { $$ = new SyntaxTree::UnaryOp($2, SyntaxTree::negation); }
    ;

mult_op:
    T_TIMES { $$ = SyntaxTree::times; }
    | T_DIV { $$ = SyntaxTree::division; }
    ;

add_op:
    T_PLUS { $$ = SyntaxTree::plus; }
    | T_SUB { $$ = SyntaxTree::minus; }
    ;

bool_rel_op:
    T_BOOL_GR { $$ = SyntaxTree::greater; }
    | T_BOOL_LS { $$ = SyntaxTree::less; }
    | T_BOOL_GE { $$ = SyntaxTree::greater_equal; }
    | T_BOOL_LE { $$ = SyntaxTree::less_equal; }
    ;

bool_bin_op:
    T_BOOL_AND { $$ = SyntaxTree::bool_and; }
    | T_BOOL_OR { $$ = SyntaxTree::bool_or; }
    ;

bool_cmp_op:
    T_BOOL_EQ { $$ = SyntaxTree::equals; }
    | T_BOOL_NEQ { $$ = SyntaxTree::different; }
    ;

%%


