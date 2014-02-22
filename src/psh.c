#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void sig_handler(int s) {
  if(s == SIGINT) {
    // Return to regular mode
    // TODO: Record original TTY (/bin/stty -g) on shell start and return to that on shell exit.
    system("/bin/stty sane");
    printf("\nGoodbye.\n");
    exit(0);
  }
}

int main(int argc, char **argv) {
  if(signal(SIGINT, sig_handler) == SIG_ERR) {
    printf("Can't catch SIGINT!\n");
    return 1;
  }
  // Switch to raw mode
  system("/bin/stty raw");
  
  int c;
  while(c = getchar()) {
    printf("c = %d\r", c);
    if(c == 3) sig_handler(SIGINT);
  }
  
  
  return 0;
}
