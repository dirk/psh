#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>

#include "psh.h"
#include "reader.h"

struct termios previous_attributes;
void reset_mode() {
  return;
  tcsetattr(STDIN_FILENO, TCSANOW, &previous_attributes);
}
void set_mode() {
  return;
  
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
  set_mode();
  setup_reader();
}
void teardown() {
  teardown_reader();
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
  
  while(1) {
    read_line();
  }
  
  return 0;
}
