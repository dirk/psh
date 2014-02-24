#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <term.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "psh.h"
#include "reader.h"
#include "parser.h"

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
    
    token_list *l = scan_line(line);
    tree*       t = parse_list(l);
    
    //print_token_list(l);
    //printf("\n");
    
    add_history(line); free(line);
  }
}
