#ifndef EXEC_H
#define EXEC_H

// Maximum number of strings in the path
#define PATH_SIZE 128

extern char **environ;
void setup_exec();
char *find_in_path(char *file, char *dest);

int exec_command(char *filename, char** argv);

#endif
