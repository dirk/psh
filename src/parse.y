%{
#include <stdio.h>
#include "psh.h"
#include "parser.h"

extern int yylex();
extern int yyparse();
void yyerror(YYLTYPE* loc, sl_s_expr_t **head, yyscan_t scanner, char* source, const char *err);
extern int yylineno_extern;

/*
%union {
  char* p_string;
  int   p_int;
  float p_float;
  void* p_node;
}
*/

%}

%code requires {
#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void* yyscan_t;
#endif

//define SOURCE(a) a->source = source; a->line = (yylloc.first_line);
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
main: sequences { /* DEBUG("main exprs = %p", $1); *head = $1; */ };

sequences: sequence separators sequences;
sequences: sequence separators | sequence;

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

separators: separator separators;
separators: separator;
separator: TSEPARATOR;

%%

void yyerror(YYLTYPE* loc, sl_s_expr_t **head, yyscan_t scanner, char* source, const char *err) {
  DEBUG("Parse error in %s on line %d: %s", source, yylineno_extern, err);
}
