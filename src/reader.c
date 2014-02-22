#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <term.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "psh.h"
#include "reader.h"

void setup_reader() {
  // Readline
  using_history();
  int errno = read_history(PSH_HISTORY);
  // fprintf(stdout, "Read history: %d\n", errno);
}
void teardown_reader() {
  // fputs("Writing history... ", stdout);
  // fflush(stdout);
  write_history(PSH_HISTORY);
  // fputs("done.\n", stdout);
  // fflush(stdout);
}

void read_line() {
  char *line;
  while((line = readline("psh> ")) != NULL) {
    printf("echo: %s\n", line);
    
    add_history(line); free(line);
  }
}
