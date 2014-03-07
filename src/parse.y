%{
#include <stdio.h>
#include "psh.h"
#include "parser.h"

extern int yylex();
extern int yyparse();
extern int yylineno_extern;

void yyerror(YYLTYPE* loc, yyscan_t scanner, char* source, const char *err) {
  fprintf(stderr, "Parse error in %s on line %d: %s\n", source, yylineno_extern, err);
}

%}

%union {
  char* lstring;
  int   lint;
  float lfloat;
  void* lvoid;
}

%code requires {
//#define SOURCE(a) a->source = source; a->line = (yylloc.first_line);
}

%locations
%pure-parser
%lex-param { yyscan_t scanner }
// %parse-param { sl_s_expr_t **head }
%parse-param { yyscan_t scanner }
%parse-param { char* source }

%error-verbose

%token TWORD;
%token TKEYWORD;
%token TLPAREN;
%token TRPAREN;
%token TSEPARATOR;

// %type <tree> tree;
// %type <tree_expression> expr;
// %type <tree_command> cmd;
// %type <token_word> word;

%%
main: sequences { /* fprintf(stderr, "main exprs = %p\n", $1); *head = $1; */ };

//sequences: sequence separators sequences;
//sequences: sequence separators
sequences: sequence;

sequence: item;
sequence: TLPAREN sequences TRPAREN;

item: expr
    | cmd;

cmd: words;
words: word words;
words: word;
word: TWORD;

expr: TKEYWORD;
expr: TKEYWORD sequence;

// separators: separator separators;
// separators: separator;
// separator: TSEPARATOR;

%%


