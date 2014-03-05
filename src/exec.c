#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <strings.h>

#include "psh.h"
#include "exec.h"

static char *path[PATH_SIZE];

void parse_path() {
  char *op = getenv("PATH");
  
  int plen = strlen(op);
  int ps = 0, pi = 0;// Start of segment and current index
  int di = 0;// Destination (path) index
  while(pi < plen) {
    // If it's separator or end of string
    if((op[pi] == ':' || op[pi] == '\0') && pi > ps) {
      int segment_len = pi - ps;
      char *s = malloc(sizeof(char) * (segment_len + 1));
      s[segment_len] = '\0';
      strncpy(s, &op[ps], segment_len);
      // printf("segment: %s\n", s);
      ps = pi + 1;
      // Put it in the path
      path[di] = s;
      di += 1;
    }
    pi += 1;
  }
  path[di] = NULL;// Terminator
}
char *find_in_path(char *f, char *dest) {
  int i = 0;
  while(path[i] != NULL) {
    // Use dest as a buffer to build the path in.
    // TODO: Buffer overflow error/vuln. here.
    dest[0] = '\0';
    strcat(dest, path[i]);
    strcat(dest, "/");
    strcat(dest, f);
    // Test if can access and execute it.
    if(access(dest, X_OK) != -1) {
      return dest;
    }
    i += 1;
  }
  dest[0] = '\0';// Just to be safe.
  return NULL;
}

void setup_exec() {
  parse_path();
}


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
