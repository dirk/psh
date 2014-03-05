#ifndef READER_H
#define READER_H

#define CESCAPE 27

#define LINE_BUFFER_SIZE 65536
char *reader_buffer;
int   reader_cursor;

void setup_reader();
void teardown_reader();

void read_line();

#define PSH_HISTORY ".psh_history"
// Computes (and memoizes) the path to the history file.
char *get_history_path();

#endif
