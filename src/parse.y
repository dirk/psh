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
  void* ptree;
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

%token <lstring> TWORD;
%token TKEYWORD;
%token TLPAREN;
%token TRPAREN;
%token TSEPARATOR;

// %type <tree> tree;
%type <ptree> item;
%type <ptree> cmd;
%type <ptree> word;
%type <ptree> words;

%%
main: sequences { /* fprintf(stderr, "main exprs = %p\n", $1); *head = $1; */ };

sequences: sequence separators sequences;
sequences: sequence separators
sequences: sequence;

sequence: item;
sequence: TLPAREN sequences TRPAREN;

item: expr { $$ = NULL; }
    | cmd { $$ = $1; };

cmd: words {
  tr_command *cmd = new_tr_command();
  cmd->list = $1;
  $$ = cmd;
}
words: word words {
  tr_word_list *list = $2;
  tr_word *left = $1;
  tr_word *right = list->head;
  left->next = right;
  right->prev = left;
  list->head = left;
  $$ = list;
}
words: word {
  tr_word_list* list = new_tr_word_list();
  list->head = $1;
  $$ = list;
};
word: TWORD {
  tr_word* w = new_tr_word();
  w->value = $1;
  $$ = w;
};

expr: TKEYWORD;
expr: TKEYWORD sequence;

separators: separator separators;
separators: separator;
separator: TSEPARATOR;

%%


