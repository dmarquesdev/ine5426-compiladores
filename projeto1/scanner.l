/* Inspired on https://github.com/llpilla/compiler_examples/blob/master/simple_ast/scanner.l */

%{
#include <string>
#include <cstring>
#include "stdlib.h"
#include "symbol_table.h"
#include "syntax_tree.h"
#include "parser.h" /* Generated by Bison */

extern int yylineno;
void yyerror(const char* s, ...);
void error(const char* type, const char* s, ...);
SymTbl::Type to_type(const char* token);

%}

%option noyywrap nodefault yylineno

integer [0-9]+
float ({integer}\.{integer}*|{integer}*\.{integer})
boolean (true|false)

id [a-zA-Z]([a-zA-Z0-9_])*
type (int|float|bool)
cast \[{type}\]
not_valid [^a-zA-Z0-9\n \t\{\}\+\*\-\/\(\)\=\,\!\>\<\&\|\_]*

%%

[ \t]               ; //ignores spaces

{float}				{yylval.fval = std::atof(yytext); return T_FLOAT;}
{integer}           {yylval.ival = std::atoi(yytext); return T_INT;}
{boolean}			{yylval.bval = std::strncmp(yytext, "true", 4) == 0; return T_BOOL;}

{type}				{yylval.type = to_type(yytext); return T_TYPE;}
{cast}				{yylval.type = to_type(yytext); return T_CAST;}
"if"				{return T_IF;}
"then"				{return T_THEN;}
"else"				{return T_ELSE;}
"for"				{return T_FOR;}
"fun"				{return T_FUNC;}
"ret"				{return T_RETURN;}

\{					{return T_OPEN_CBRACK;}
\}					{return T_CLOSE_CBRACK;}

\+                 	{return T_PLUS;}
\*                  {return T_TIMES;}
\-                  {return T_SUB;}
\/                  {return T_DIV;}
\n                  {return T_NL;}
\(                  {return T_OPEN_PAR;}
\)                  {return T_CLOSE_PAR;}
\=					{return T_ATTRIB;}
\,					{return T_COMMA;}
\!					{return T_NEGATION;}
"=="				{return T_BOOL_EQ;}
"!="				{return T_BOOL_NEQ;}
">"					{return T_BOOL_GR;}
"<"					{return T_BOOL_LS;}
">="				{return T_BOOL_GE;}
"<="				{return T_BOOL_LE;}
\&					{return T_BOOL_AND;}
"|"					{return T_BOOL_OR;}

{id}				{yylval.cval = strdup(yytext); return T_ID;}
{not_valid}         {char buf[100]; strcpy(buf, "unknown symbol "); strcat(buf, yytext); strcat(buf, "\n"); error("lexical", buf);} /*complains about every unknown character*/

%%

SymTbl::Type to_type(const char* token) {
	SymTbl::Type type;

	if (strncmp(token, "int", 3) == 0 
		|| strncmp(token, "[int]", 5) == 0) { type = SymTbl::Type::t_int; }
	else if (strncmp(token, "float", 5) == 0 
		|| strncmp(token, "[float]", 7) == 0) { type = SymTbl::Type::t_float; }
	else if (strncmp(token, "bool", 4) == 0 
		|| strncmp(token, "[bool]", 6) == 0) { type = SymTbl::Type::t_bool; }

	return type;
}

void yyerror(const char *s, ...){
    va_list ap;
    va_start(ap, s);
    std::fprintf(stderr, "[Line %d]: ", yylineno);
    std::vfprintf(stderr, s, ap);

}

void error(const char* type, const char *s, ...){
	va_list ap;
    va_start(ap, s);

	std::string formatted;
	formatted = type + std::string(" error: ") + s;
	yyerror(formatted.c_str(), ap);
}