
#define CESCAPE 27

#define LINE_BUFFER_SIZE 65536
char *reader_buffer;
int   reader_cursor;

void setup_reader();
void read_line();

