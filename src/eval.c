#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

#include "psh.h"
#include "parser.h"
#include "debug.h"
#include "exec.h"
#include "eval.h"

int eval_tree(tree* t) {
  printf("\x1b[30;1;22m");//grey
  print_tree(t);
  printf("\x1b[0m");//reset
  fflush(stdout);

  int status = 0;

  void **sequence = t->sequence;
  tree *item;
  while((item = *sequence) && item != NULL) {
    // printf("item: %p, type = %d\n", item, ((tree*)item)->type);
    if(item->type == TRCOMMAND) {
      status = eval_command((tree_command*)item);
    } else {
      fprintf(stderr, "Don't know how to eval item with type %d\n", (int)(item->type));
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
  argv[length] = NULL;

  char path[PATH_MAX];
  char *res;

  res = realpath(argv[0], path);
  if(res == NULL) {
    res = find_in_path(argv[0], path);
  }
  if(res == NULL) {
    printf("Not found: %s\n", argv[0]);
  } else {
    printf("\x1b[30;1;22m");//grey
    printf("(path: %s)\n", path);
    printf("\x1b[0m");//reset
    fflush(stdout);

    exec_command(path, argv);
  }


  return 0;
}
