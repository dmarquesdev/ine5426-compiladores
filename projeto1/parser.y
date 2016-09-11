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
}

/* token defines our terminal symbols (tokens).
 */
%token <ival> T_INT T_VAR_NAME

%token T_PLUS T_TIMES T_SUB T_DIV T_NL T_OPEN_PAR 
T_CLOSE_PAR T_ATTRIB T_TYPE 

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

program: /*use ctrl+d to stop*/
    lines /*$$ = $1 when nothing is said*/
    ;

lines: 
    line /*$$ = $1 when nothing is said*/
    | lines line
    ;

line: 
    T_NL { $$ = 0 /*NULL*/; } /*nothing here to be used */
    | varDecl T_NL 
    | expr T_NL 
    ;

varDecl:
    T_VAR_NAME T_ATTRIB
    | T_VAR_NAME { $$ = 0; } 
    ;

expr: 
    T_INT { $$ = $1; } 
    | T_VAR_NAME { $$ = $1; }
    | T_SUB T_INT { $$ = -$2; } 
    | T_SUB T_VAR_NAME { $$ = -$2; }
    | T_OPEN_PAR expr T_CLOSE_PAR { $$ = $2; }
    | expr T_PLUS expr { $$ = $1 + $3; }
    | expr T_TIMES expr { $$ = $1 * $3; }
    | expr T_DIV expr { $$ = $1 / $3; }
    | expr T_SUB expr { $$ = $1 - $3; }
    ;

%%


