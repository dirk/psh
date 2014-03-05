#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

#include "psh.h"
#include "parser.h"
#include "debug.h"
#include "exec.h"
#include "eval.h"

int eval_tree(tree* t) {
  print_tree(t);
  
  int status = 0;
  
  void **sequence = t->sequence;
  tree *item;
  while((item = *sequence) && item != NULL) {
    // printf("item: %p, type = %d\n", item, ((tree*)item)->type);
    if(item->type == TCOMMAND) {
      status = eval_command((tree_command*)item);
    }
    
    sequence++;
  }
  
  return status;
}

int eval_command(tree_command *cmd) {
  // Command is a sequence of TWORD tokens
  // token **tokens = cmd->tokens;
  int length = command_length(cmd);
  char *argv[length];
  for(int i = 0; i < length; i++) {
    token_word *word = (token_word*)(cmd->tokens[i]);
    argv[i] = word->contents;
  }
  
  char path[PATH_MAX];
  char *res;
  
  res = realpath(argv[0], path);
  if(res == NULL) {
    res = find_in_path(argv[0], path);
  }
  if(res == NULL) {
    printf("Not found: %s\n", argv[0]);
  }
  printf("path: %s\n", path);
  
  return 0;
}
