#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>

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
  attr.c_lflag = attr.c_lflag & ~(ICANON | ECHO);
  attr.c_cc[VMIN] = 1;
  attr.c_cc[VTIME] = 0;
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &attr);
}

void setup() {
  // Switch to raw mode
  //system("/bin/stty raw");
  set_mode();
}
void teardown() {
  // Return to regular mode
  // TODO: Record original TTY (/bin/stty -g) on shell start and return to that on shell exit.
  //system("/bin/stty sane");
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
  
  char *c = malloc(sizeof(char));
  while(1) {
    // Read single character
    read(STDIN_FILENO, c, 1);
    if(*c == 3) sig_handler(SIGINT);
    printf("c = %d\n", *c);
  }
  
  return 0;
}
