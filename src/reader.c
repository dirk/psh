#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <term.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <limits.h>

#include "psh.h"
#include "reader.h"
#include "parser.h"
#include "eval.h"

void setup_reader() {
  // Readline
  rl_prep_terminal(0);
  using_history();
  int errno = read_history(get_history_path());
  if(errno != 0) fprintf(stderr, "Error %d reading history!\n", errno);
}
void teardown_reader() {
  // fputs("Writing history... ", stdout);
  // fflush(stdout);
  write_history(get_history_path());
  rl_deprep_terminal();
  // fputs("done.\n", stdout);
  // fflush(stdout);
}

void read_line() {
  char *line;
  while((line = readline("psh> ")) != NULL) {
    // printf("echo: %s\n", line);
    
    token_list *l = scan_line(line); printf("\x1b[30;1;22m(scanned)\x1b[0m\n");
    tree*       t = parse_list(l);   printf("\x1b[30;1;22m(parsed: %p, %d)\x1b[0m\n", t, (int)t);

    // Check if it's an error.
    if((int)t >= 0 && (int)t <= PARSE_ERRORS) {
      fprintf(stderr, "Parse error: %s\n", human_name_for_parse_error((parse_error)t));
    } else {
      eval_tree(t);
    }
    
    //printf("\n");
    
    add_history(line); free(line);
  }
}

static char *hp = NULL;
char *get_history_path() {
  if(hp != NULL) return hp;
  hp = malloc(sizeof(char) * PATH_MAX);
  hp[0] = '\0';
  strcat(hp, getenv("HOME"));
  strcat(hp, "/");
  strcat(hp, PSH_HISTORY);
  return hp;
}
