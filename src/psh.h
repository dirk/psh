#ifndef PSH_H
#define PSH_H

#define PSH_HISTORY ".psh_history"

typedef enum {false, true} bool;

void set_mode();

void setup();
void sig_handler(int s);

// Computes (and memoizes) the path to the history file.
char *get_history_path();

#endif
