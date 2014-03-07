%{
#include <string.h>
#include "psh.h"
#include "parser.h"
#include "parse.tab.h"


#define SAVE_STRING yylval->p_string = strdup(yytext);

int yylineno_extern = 1;
//#define YY_USER_ACTION yylineno_extern = yylineno;
#define YY_USER_ACTION yylineno_extern = yylloc->first_line = yylloc->last_line = yylineno;

char *strclone(char *str);

/*
"$"{variable}            { yylval = strclone(yytext); return TVARIABLE; }
*/

#define YY_NO_INPUT

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


keyword     :[A-Za-z_]+
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

"\"\""|("\""{chars}"\"") { yylval->lstring = strclone(yytext); return TWORD; }
{word}                   { fprintf(stderr, "word: %s\n", yytext); yylval->lstring = strclone(yytext); return TWORD; }

{keyword}                { fprintf(stderr, "kw: %s\n", yytext); return TKEYWORD; }

"("                      { return TLPAREN; }
")"                      { return TRPAREN; }
;                        { fprintf(stderr, "sep: ;\n");  return TSEPARATOR; }

\n+                      { fprintf(stderr, "sep: \\n\n"); return TSEPARATOR; }
[ \t\r] // Remove whitespace
%%
char *strclone(char *str)
{
    int len = strlen(str);
    char *clone = (char *)malloc(sizeof(char)*(len+1));
    strcpy(clone,str);
    return clone;
}
