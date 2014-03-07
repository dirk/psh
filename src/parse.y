%{
#include <stdio.h>
#include "psh.h"
#include "parser.h"

extern int yylex();
extern int yyparse();
extern int yylineno_extern;

void yyerror(YYLTYPE* loc, tr_body **main, yyscan_t scanner, char* source, const char *err) {
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
%parse-param { tr_body **main }
%parse-param { yyscan_t scanner }
%parse-param { char* source }

%error-verbose

%token <lstring> TWORD;
%token <lstring> TKEYWORD;
%token TLPAREN;
%token TRPAREN;
%token TSEPARATOR;

// %type <tree> tree;
%type <ptree> item;
%type <ptree> cmd;
%type <ptree> word;
%type <ptree> words;
%type <ptree> expr;
%type <ptree> sequence;
%type <ptree> body;
%type <ptree> main;

%%
main: body { *main = $1; $$ = $1; };

body: sequence separators body {
  tr_body  *body = $3;
  tr_sequence_list *sl = body->list;
  
  tr_sequence *left = $1;
  tr_sequence *right = sl->head;
  
  left->next  = right;
  right->prev = left;
  sl->head    = left;
  $$ = body;
};
body: sequence separators | sequence {
  tr_body        *body = new_tr_body();
  tr_sequence_list *sl = new_tr_sequence_list();
  tr_sequence       *s = $1;
  body->list = sl;
  sl->head   = s;
  $$ = body;
};

sequence: item {
  tr_sequence *s = new_tr_sequence();
  s->type = TRITEM;
  s->item = $1;
  $$ = s;
};
sequence: TLPAREN body TRPAREN {
  tr_sequence *s = new_tr_sequence();
  s->type = TRGROUP;
  s->item = $2;
  $$ = s;
};

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

expr: TKEYWORD {
  tr_expression *expr = new_tr_expression();
  expr->keyword = $1;
  $$ = expr;
};
expr: TKEYWORD sequence {
  tr_expression *expr = new_tr_expression();
  expr->keyword = $1;
  expr->body = $2;
  $$ = expr;
};

separators: separator separators;
separators: separator;
separator: TSEPARATOR;

%%


