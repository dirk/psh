#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <strings.h>

#include "psh.h"
#include "parser.h"
#include "debug.h"
#include "reader.h"

struct termios previous_attributes;
void reset_mode() {
  tcsetattr(STDIN_FILENO, TCSANOW, &previous_attributes);
}
void set_mode() {
  struct termios attr;
  if(!isatty(STDIN_FILENO)) {
    fprintf(stderr, "Not a TTY.\n");
    exit(1);
  }
  // Save old attributes
  tcgetattr(STDIN_FILENO, &previous_attributes);
  atexit(reset_mode);
  
  tcgetattr(STDIN_FILENO, &attr);
  // Disable canonical and echo
  attr.c_lflag &= ~(ICANON | ECHO);
  attr.c_iflag &= ~(IXON | IXOFF);
  attr.c_cc[VMIN] = 1;
  attr.c_cc[VTIME] = 0;
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &attr);
}

void setup() {
  setup_reader();
}
void teardown() {
  teardown_reader();
}

char *get_history_path() {
  static char *hp;
  if(hp != NULL) return hp;
  hp = malloc(sizeof(char) * 1024); // TODO: Maybe fix hard-coding
  hp[0] = '\0';
  strcat(hp, getenv("HOME"));
  strcat(hp, "/");
  strcat(hp, PSH_HISTORY);
  return hp;
}

void sig_handler(int s) {
  if(s == SIGINT) {
    teardown();
    printf("\nGoodbye.\n");
    exit(0);
  }
}

int main(int argc, char **argv) {
  if(signal(SIGINT, sig_handler) == SIG_ERR) {
    printf("Can't catch SIGINT!\n");
    return 1;
  }
  setup();
  
  while(true) {
    read_line();
  }
  
  return 0;
}
