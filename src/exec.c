#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#include "psh.h"
#include "exec.h"

int exec_command(char *filename, char** argv) {
  int pid;
  if((pid = fork()) < 0) {
    fprintf(stderr, "Error forking: %d\n", pid);
    return -1;
  }
  // CHILD --------------------------------------------------------------------
  if(pid == 0) {
    int err = execve(filename, argv, environ);
    exit(err);
  }
  // PARENT -------------------------------------------------------------------
  int status;
  while(wait(&status) != pid);
  // Returning the exit status of the child.
  // (refer to: http://linux.die.net/man/2/wait)
  if(WIFEXITED(status) == true) {
    return WEXITSTATUS(status);
  }
  // Non-normal exit (terminated, signaled, etc.)
  return 1;
}
