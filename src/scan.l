%{
#include <string.h>
#include "psh.h"
#include "parse.h"


#define SAVE_STRING yylval->p_string = strdup(yytext);

int yylineno_extern = 1;
//#define YY_USER_ACTION yylineno_extern = yylineno;
#define YY_USER_ACTION yylineno_extern = yylloc->first_line = yylloc->last_line = yylineno;

char *strclone(char *str);

/*
"$"{variable}            { yylval = strclone(yytext); return TVARIABLE; }
*/

%}

%option header-file="src/scan.yy.h"
%option     outfile="src/scan.yy.c"
%option yylineno
%option noyywrap
%option reentrant
%option nounput
%option warn
%option never-interactive
%option bison-bridge bison-locations


keyword     :[A-Za-z_]
comment     [ \t]*#[^\n]*

hex       [0-9a-f]
unescaped [ -!#-\[\]-~]
escaped   \\["\\bfnrt/]
unicode   \\u{hex}{hex}{hex}{hex}
char     {unescaped}|{escaped}|{unicode}
chars    {char}+

variable [0-9A-Za-z_]+
word     [0-9A-Za-z][0-9A-Za-z_]*

%%
\r // Remove carriage returns
[ \t]*#[^\n]* // Remove comments

"\"\""|("\""{chars}"\"") { yylval = strclone(yytext); return TWORD; }
{word}                   { yylval = strclone(yytext); return TWORD; }

{keyword}                { return TKEYWORD; }

"("                      { return TLPAREN; }
")"                      { return TRPAREN; }
;                        { return TSEPARATOR; }

\n+                      { return TSEPARATOR; }
[ \t\r] // Remove whitespace
%%
char *strclone(char *str)
{
    int len = strlen(str);
    char *clone = (char *)malloc(sizeof(char)*(len+1));
    strcpy(clone,str);
    return clone;
}
