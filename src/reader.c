#include "psh.h"
#include "reader.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include <term.h>

void setup_reader() {
  reader_buffer = malloc(sizeof(char) * LINE_BUFFER_SIZE);
  
  int result;
  setupterm(NULL, STDOUT_FILENO, &result);
  if(result <= 0) {
    fprintf(stderr, "Error setting up terminal: %d\n", result);
    exit(1);
  }
}

bool is_printable(char c) {
  if(c >= 32 && c <= 126) return true;
  return false;
}
bool is_escape(char c) {
  return (c == CESCAPE);
}

// Read single character
char read_char() {
  static char cbuff[1];
  read(STDIN_FILENO, cbuff, 1);
  return cbuff[0];
}

void cursor_backward(int n) {
  int npos = reader_cursor - n;
  if(npos < 0) {
    n = reader_cursor;
    npos = 0;
  }
  reader_cursor = npos;
  
  // int i;
  // for(i = 0; i < n; i++) {
  //   fputs(cursor_left, stdout);
  // }
  // fflush(stdout);
  fprintf(stdout, "\e[%dG", reader_cursor + 1);
  fflush(stdout);
}
void cursor_forward(int n) {
  int npos = reader_cursor + n;
  int maxpos = strlen(reader_buffer) + 1;
  if(npos >= maxpos) {
    n = (maxpos - reader_cursor) - 1;
    npos = maxpos - 1;
  }
  reader_cursor = npos;
  
  // int i;
  // for(i = 0; i < n; i++) {
  //   fputs(cursor_right, stdout);
  // }
  // fflush(stdout);
  fprintf(stdout, "\e[%dG", reader_cursor + 1);
  fflush(stdout);
}

void read_line() {
  reader_buffer[0] = '\0';
  reader_cursor = 0;
  
  while(1) {
    char c = read_char();
    if(c == 3) sig_handler(SIGINT);
    if(is_printable(c)) {
      if(reader_buffer[reader_cursor] != '\0') {
        int lastchar = strlen(reader_buffer) - 1;
        // Set the new end
        reader_buffer[lastchar + 2] = '\0';
        // Then advance all characters after the cursor
        int i;
        for(i = lastchar; i >= reader_cursor; i--) {
          reader_buffer[i + 1] = reader_buffer[i];
        }
      } else {
        reader_buffer[reader_cursor + 1] = '\0';
      }
      reader_buffer[reader_cursor] = c;
      reader_cursor += 1;
      fprintf(stdout, "\r%s", reader_buffer);
      fprintf(stdout, "\e[%dG", reader_cursor + 1);
      fflush(stdout);
      continue;
    }
    if(is_escape(c)) {
      char csi = read_char();
      if(csi != '[') {
        fprintf(stderr, "Unexpected CSI: %c\n", csi);
        exit(1);
      }
      char code = read_char();
      if(code == 'D') {
        cursor_backward(1);
      } else if(code == 'C') {
        cursor_forward(1);
      } else {
        fprintf(stderr, "Unexpected code: %c\n", code);
        exit(1);
      }
      continue;
      // printf("code: %c\n", code);
    }
    printf("c = %d\n", c);
  }
}

