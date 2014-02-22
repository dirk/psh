#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <term.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "psh.h"
#include "reader.h"

void read_line() {
  char *line;
  while((line = readline("hello> ")) != NULL) {
    printf("echo: '%s'\n", line);
    
    add_history(line); free(line);
  }
}
