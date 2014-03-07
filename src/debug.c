#include <stdlib.h>
#include <stdio.h>

#include "psh.h"
#include "parser.h"
#include "debug.h"

int _indent;
void _print_indent() {
  int i;
  for(i = 0; i < _indent; i++) {
    printf(" ");
  }
}

void print_sequence(void **seq_ptr) {
  void **seq = seq_ptr;
  while(*seq != NULL) {
    _print_indent();
    tree *s = (tree*)*seq;
    if(s->type == TRCOMMAND) {
      print_command((tree_command*)s);
    } else if(s->type == TSEPARATOR) {
      printf("separator:"); print_token((token*)s);
    } else {
      printf("Unknown type: %d", s->type);
    }
    printf("\n");
    seq++;
  }
}

void print_tree(tree* t) {
  printf("tree(\n");
  _indent++;
  print_sequence(t->sequence);
  _indent--;
  printf(")\n");
}

void print_command(tree_command *command) {
  printf("cmd(");
  token **tokens = command->tokens;
  while(*tokens != NULL) {
    token *t = *tokens;
    print_token(t);
    printf(" ");
    tokens++;
  }
  printf("\b)");
}

