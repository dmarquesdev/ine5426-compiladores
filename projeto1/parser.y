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

typedef SymTbl::Symbol Symbol;

extern int yylex();
extern void yyerror(const char* s, ...);
extern void error(const char* type, const char* s, ...);

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
    SymTbl::Type type;
    SyntaxTree::Reserved rword;
    SyntaxTree::Delimiter delimiter;
}

/* token defines our terminal symbols (tokens).
 */
%token T_PLUS T_TIMES T_SUB T_DIV T_NL T_OPEN_PAR 
T_CLOSE_PAR T_ATTRIB T_COMMA T_NEGATION 
T_BOOL_EQ T_BOOL_NEQ T_BOOL_GR T_BOOL_LS 
T_BOOL_GE T_BOOL_LE T_BOOL_AND T_BOOL_OR 

%token <ival> T_INT

%token <cval> T_VAR_NAME

%token <type> T_TYPE T_CAST

%token <fval> T_FLOAT

%token <bval> T_BOOL

%token <rword> T_IF T_THEN T_ELSE

%token <delimiter> T_OPEN_CBRACK T_CLOSE_CBRACK

/* type defines the type of our nonterminal symbols.
 * Types should match the names used in the union.
 * Example: %type<node> expr
 */
%type <node> expr line attr exprValue 
varDecl varList var 
%type <block> program lines conditional

/* Operator precedence for mathematical operators
 * The latest it is listed, the highest the precedence
 * left, right, nonassoc
 */

 %left T_CAST
 %left T_BOOL_AND T_BOOL_OR 
 %left T_BOOL_EQ T_BOOL_NEQ T_BOOL_GR T_BOOL_LS T_BOOL_GE T_BOOL_LE 
 %left T_NEGATION 
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
    | conditional 
    | lines conditional { $$->append($2); }
    ;

line: 
    T_NL { $$ = NULL; } /*nothing here to be used */
    | varDecl T_NL /* Variable declaration */
    | attr T_NL 
    ;


varDecl:
    T_TYPE varList { $$ = new SyntaxTree::Declaration($1, $2); }
    ;

varList:
    var { $$ = new SyntaxTree::List($1, NULL); }
    | varList T_COMMA var { $$ = new SyntaxTree::List($3, $1); }
    ;

var:
    T_VAR_NAME { $$ = symbolTable.newVariable($1, NULL); }
    | T_VAR_NAME T_ATTRIB expr { $$ = symbolTable.newVariable($1, $3); }
    ;

attr:
    T_VAR_NAME T_ATTRIB expr { SyntaxTree::Node* node = symbolTable.assignVariable($1);
            $$ = new SyntaxTree::BinaryOp(node, SyntaxTree::assign, $3); }

/* The expression grammar uses "stratified grammar" to
 * avoid ambiguity
 * https://courses.engr.illinois.edu/cs421/sp2010/lectures/lecture10.pdf
 * Page 9
 */
expr:
    expr T_TIMES expr { $$ = new SyntaxTree::BinaryOp($1, SyntaxTree::times, $3); }
    | expr T_DIV expr { $$ = new SyntaxTree::BinaryOp($1, SyntaxTree::division, $3); }
    | expr T_PLUS expr { $$ = new SyntaxTree::BinaryOp($1, SyntaxTree::plus, $3); }
    | expr T_SUB expr { $$ = new SyntaxTree::BinaryOp($1, SyntaxTree::minus, $3); }
    | expr T_BOOL_GR expr { $$ = new SyntaxTree::BinaryOp($1, SyntaxTree::greater, $3); }
    | expr T_BOOL_LS expr { $$ = new SyntaxTree::BinaryOp($1, SyntaxTree::less, $3); }
    | expr T_BOOL_GE expr { $$ = new SyntaxTree::BinaryOp($1, SyntaxTree::greater_equal, $3); }
    | expr T_BOOL_LE expr { $$ = new SyntaxTree::BinaryOp($1, SyntaxTree::less_equal, $3); }
    | expr T_BOOL_EQ expr { $$ = new SyntaxTree::BinaryOp($1, SyntaxTree::equals, $3); }
    | expr T_BOOL_NEQ expr { $$ = new SyntaxTree::BinaryOp($1, SyntaxTree::different, $3); }
    | expr T_BOOL_AND expr { $$ = new SyntaxTree::BinaryOp($1, SyntaxTree::bool_and, $3); }
    | expr T_BOOL_OR expr { $$ = new SyntaxTree::BinaryOp($1, SyntaxTree::bool_or, $3); }
    | exprValue
    | T_OPEN_PAR expr T_CLOSE_PAR { $$ = $2; }
    | T_SUB expr %prec USUB { $$ = new SyntaxTree::UnaryOp($2, SyntaxTree::negative); } 
    | T_NEGATION expr { $$ = new SyntaxTree::UnaryOp($2, SyntaxTree::negation); }
    | T_CAST expr { $$ = new SyntaxTree::Cast($1, $2); }
    ;

exprValue:
    T_INT { $$ = new SyntaxTree::Integer($1); }
    | T_FLOAT { $$ = new SyntaxTree::Float($1); }
    | T_BOOL { $$ = new SyntaxTree::Boolean($1); }
    | T_VAR_NAME { $$ = symbolTable.useVariable($1); }
    ;

conditional:
    T_IF expr T_NL T_THEN T_OPEN_CBRACK T_NL lines T_CLOSE_CBRACK { $$ = new SyntaxTree::Conditional($2, $7, NULL); }
    | T_IF expr T_NL T_THEN T_OPEN_CBRACK T_NL lines 
        T_CLOSE_CBRACK T_ELSE T_OPEN_CBRACK T_NL lines 
        T_CLOSE_CBRACK { $$ = new SyntaxTree::Conditional($2, $7, $12); }
    ;

%%