#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "psh.h"
#include "parser.h"

token_list *new_token_list() {
  token_list *tl = malloc(sizeof(token_list));
  tl->tokens = malloc(sizeof(void) * TOKEN_LIST_SIZE);
  return tl;
}

token_list *parse_line(char *line) {
  token_list *tl = new_token_list();
  // Position in the token list
  int    token_index = 0;
  token* token_ptr   = tl->tokens[token_index];
  // Position in the line
  char  *pos = line;
  
  while(parse_token(&pos, &token_ptr)) {
    // Move to the next token slot
    token_index += 1;
    if(token_index == TOKEN_LIST_SIZE) {
      fprintf(stderr, "Out of token slots\n");
      // TODO: Free the list
      return NULL;
    }
    token_ptr = tl->tokens[token_index];
  }
  return tl;
}

bool is_letter(char c) {
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

token_keyword *consume_keyword(char **pos_ptr) {
  char *pos = *pos_ptr;
  if(pos[0] == ':') {
    int i = 1;
    while(is_letter(pos[i])) i += 1;
    if(i == 1) {
      fprintf(stderr, "Error parsing keyword!\n");
      return NULL;
    }
    // Copy the name
    char *name = malloc(sizeof(char) * (i + 1));
    name[i] = '\0';
    strncpy(name, pos + 1, i);
    // Build the keyword
    token_keyword* kw = malloc(sizeof(token_keyword));
    kw->name = name;
    kw->type = KEYWORD;
    *pos_ptr = &pos[i + 1];
    return kw;
  }
  return NULL;
}
void consume_whitespace(char **pos_ptr) {
  char *pos = *pos_ptr;
  while(*pos == ' ' || *pos == '\t') { pos++; }
  *pos_ptr = pos;
}

bool parse_token(char **pos_ptr, token **token_ptr) {
  token* t;
  
  consume_whitespace(pos_ptr);
  
  if((t = (token*)consume_keyword(pos_ptr)) && t != NULL) {
    printf("keyword: %s\n", ((token_keyword*)t)->name);
  }
  
  if(t != NULL) {
    *token_ptr = t;
    return true;
  } else {
    return false;
  }
}//parse_token
