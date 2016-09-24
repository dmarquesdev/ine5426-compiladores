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
%type <node> expr boolExpr aritmExpr line attr
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
    | expr T_NL /*$$ = $1 when nothing is said*/
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

expr:
    aritmExpr 
    | boolExpr 
    ;

aritmExpr: 
    T_INT { $$ = new SyntaxTree::Integer($1); }
    | T_FLOAT { $$ = new SyntaxTree::Float($1); }
    | T_VAR_NAME { $$ = symbolTable.useVariable($1); }
    | T_OPEN_PAR aritmExpr T_CLOSE_PAR { $$ = $2; }
    | T_SUB aritmExpr %prec USUB { $$ = new SyntaxTree::UnaryOp($2, SyntaxTree::negative); } 
    | aritmExpr T_PLUS aritmExpr { $$ = new SyntaxTree::BinaryOp($1, SyntaxTree::plus, $3); }
    | aritmExpr T_TIMES aritmExpr { $$ = new SyntaxTree::BinaryOp($1, SyntaxTree::times, $3); }
    | aritmExpr T_DIV aritmExpr { $$ = new SyntaxTree::BinaryOp($1, SyntaxTree::division, $3); }
    | aritmExpr T_SUB aritmExpr { $$ = new SyntaxTree::BinaryOp($1, SyntaxTree::minus, $3); }
    ;

boolExpr:
    T_BOOL { $$ = new SyntaxTree::Bool($1); }
    | T_VAR_NAME { $$ = symbolTable.useVariable($1); }
    | T_OPEN_PAR boolExpr T_CLOSE_PAR { $$ = $2; }
    | T_NEGATION boolExpr { $$ = new SyntaxTree::UnaryOp($2, SyntaxTree::negation); }
    | expr T_BOOL_EQ expr { $$ = new SyntaxTree::BinaryOp($1, SyntaxTree::equals, $3); }
    | expr T_BOOL_NEQ expr { $$ = new SyntaxTree::BinaryOp($1, SyntaxTree::different, $3); }
    | aritmExpr T_BOOL_GR aritmExpr { $$ = new SyntaxTree::BinaryOp($1, SyntaxTree::greater, $3); }
    | aritmExpr T_BOOL_LS aritmExpr { $$ = new SyntaxTree::BinaryOp($1, SyntaxTree::less, $3); }
    | aritmExpr T_BOOL_GE aritmExpr { $$ = new SyntaxTree::BinaryOp($1, SyntaxTree::greater_equal, $3); }
    | aritmExpr T_BOOL_LE aritmExpr { $$ = new SyntaxTree::BinaryOp($1, SyntaxTree::less_equal, $3); }
    | boolExpr T_BOOL_AND boolExpr { $$ = new SyntaxTree::BinaryOp($1, SyntaxTree::bool_and, $3); } 
    | boolExpr T_BOOL_OR boolExpr { $$ = new SyntaxTree::BinaryOp($1, SyntaxTree::bool_or, $3); } 
    ;

%%


