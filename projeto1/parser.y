%{
#include <iostream>

extern int yylex();
extern void yyerror(const char* s, ...);


%}

%define parse.trace

/* yylval == %union
 * union informs the different ways we can store data
 */
%union {
    int ival;
    char* cval;
}

/* token defines our terminal symbols (tokens).
 */
%token <ival> T_INT

%token <cval> T_VAR_NAME

%token T_PLUS T_TIMES T_SUB T_DIV T_NL T_OPEN_PAR 
T_CLOSE_PAR T_ATTRIB T_TYPE T_COMMA

/* type defines the type of our nonterminal symbols.
 * Types should match the names used in the union.
 * Example: %type<node> expr
 */
%type <ival> program lines line expr 
varDecl 

/* Operator precedence for mathematical operators
 * The latest it is listed, the highest the precedence
 * left, right, nonassoc
 */

 %left T_PLUS T_SUB
 %left T_TIMES T_DIV

/* Starting rule 
 */
%start program

%%

program: 
    lines 
    ;

lines: 
    line 
    | lines line
    ;

line: 
    T_NL 
    | varDecl T_NL 
    | expr T_NL 
    ;

varDecl:
    T_VAR_NAME T_ATTRIB expr 
    | T_VAR_NAME 
    | varDecl T_COMMA varDecl 
    ;

expr: 
    T_INT 
    | T_VAR_NAME 
    | T_SUB T_INT  
    | T_SUB T_VAR_NAME 
    | T_OPEN_PAR expr T_CLOSE_PAR 
    | expr T_PLUS expr 
    | expr T_TIMES expr 
    | expr T_DIV expr 
    | expr T_SUB expr 
    ;

%%


