%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"
#include "report.h"

int verbose_tokens = 0; /* controls console token trace; HTML report always records tokens */
extern int error_count;

static void report_token(const char *name, const char *text) {
    report_add_token(yylineno, name, text);
    if (verbose_tokens) {
        printf("  [line %2d] %-10s '%s'\n", yylineno, name, text);
    }
}
%}

%option noyywrap
%option yylineno

DIGIT     [0-9]
LETTER    [a-zA-Z_]
ID        {LETTER}({LETTER}|{DIGIT})*
INT_LIT   {DIGIT}+
FLOAT_LIT {DIGIT}+"."{DIGIT}+
STRING_LIT \"([^\"\n\\]|\\.)*\"

%%

"//".*                         { /* line comment: skip */ }
[ \t\r]+                       { /* skip whitespace */ }
\n                              { /* newline handled by yylineno */ }

"int"                           { report_token("KEYWORD", yytext); return INT_KW; }
"float"                         { report_token("KEYWORD", yytext); return FLOAT_KW; }
"string"                        { report_token("KEYWORD", yytext); return STRING_KW; }
"if"                             { report_token("KEYWORD", yytext); return IF; }
"else"                           { report_token("KEYWORD", yytext); return ELSE; }
"while"                          { report_token("KEYWORD", yytext); return WHILE; }
"print"                          { report_token("KEYWORD", yytext); return PRINT; }

"=="                             { report_token("RELOP", yytext); return EQ; }
"!="                             { report_token("RELOP", yytext); return NEQ; }
"<="                             { report_token("RELOP", yytext); return LE; }
">="                             { report_token("RELOP", yytext); return GE; }
"<"                              { report_token("RELOP", yytext); return LT; }
">"                              { report_token("RELOP", yytext); return GT; }

"+"                              { report_token("OP", yytext); return '+'; }
"-"                              { report_token("OP", yytext); return '-'; }
"*"                              { report_token("OP", yytext); return '*'; }
"/"                              { report_token("OP", yytext); return '/'; }
"="                              { report_token("ASSIGN", yytext); return '='; }
"("                              { report_token("PUNCT", yytext); return '('; }
")"                              { report_token("PUNCT", yytext); return ')'; }
"{"                              { report_token("PUNCT", yytext); return '{'; }
"}"                              { report_token("PUNCT", yytext); return '}'; }
";"                              { report_token("PUNCT", yytext); return ';'; }

{FLOAT_LIT}                      { report_token("FLOAT_LIT", yytext); yylval.fval = atof(yytext); return FLOAT_LIT; }
{INT_LIT}                        { report_token("INT_LIT", yytext); yylval.ival = atoi(yytext); return INT_LIT; }
{STRING_LIT}                     { report_token("STRING_LIT", yytext); yylval.sval = strdup(yytext); return STRING_LIT; }
{ID}                              { report_token("ID", yytext); yylval.sval = strdup(yytext); return ID; }

.                                 {
    fprintf(stderr, "Lexical error at line %d: unexpected character '%s'\n", yylineno, yytext);
    char msg[64];
    snprintf(msg, sizeof(msg), "unexpected character '%s'", yytext);
    report_add_error(yylineno, "Lexical", msg);
    error_count++;
}

%%
