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
  int token_index = 0;
  // Position in the line
  char *pos = line;
  
  while(parse_token(&pos, &tl->tokens[token_index])) {
    // Move to the next token slot
    token_index++;
    if(token_index == TOKEN_LIST_SIZE) {
      fprintf(stderr, "Out of token slots\n");
      // TODO: Free the list
      return NULL;
    }
  }
  tl->tokens[token_index] = NULL; // Make sure it's terminated
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
    kw->type = TKEYWORD;
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

token_string *consume_string(char **pos_ptr) {
  char *pos = *pos_ptr;
  if(pos[0] != '"') return NULL;
  pos++;
  
  // Allocate the string
  // TODO: Make it use dynamic strings
  char *s = malloc(sizeof(char) * MAX_STRING_LENGTH);
  int si = 0;
  while(*pos != '"' && si < (MAX_STRING_LENGTH - 1)) {
    char c = *pos;
    pos++;
    
    char sc;
    if(c == '\\') {
      sc = consume_string_escape_sequence(&pos);
    } else {
      sc = c;
    }
    s[si] = sc;
    si++; // Advance destination index
  }
  pos++; // Move beyond closing quote
  *pos_ptr = pos;
  token_string* str = malloc(sizeof(token_string));
  str->type = TSTRING;
  str->contents = s;
  return str;
}
char consume_string_escape_sequence(char **src_ptr) {
  char *src = *src_ptr;
  (*src_ptr)++;
  char c = *src;
  if(c == 'u') {
    fprintf(stderr, "Cannot handle unicode yet!");
    return '\0';
  } else if(c == 'b') {
    return '\b';
  } else if(c == 'f') {
    return '\f';
  } else if(c == 'n') {
    return '\n';
  } else if(c == 'r') {
    return '\r';
  } else if(c == 't') {
    return '\t';
  } else {
    return c;
  }
  return '\0';
}

bool parse_token(char **pos_ptr, token **token_ptr) {
  token* t;
  consume_whitespace(pos_ptr);
  
  // Try to consume difference tokens
  if((t = (token*)consume_keyword(pos_ptr)) && t != NULL) {
    // printf("keyword: %s (%p)\n", ((token_keyword*)t)->name, t);
  } else
  if((t = (token*)consume_string(pos_ptr)) && t != NULL) {
    
  }
  // If a token was found then update the token pointer.
  // TODO: Refactor to return token pointer instead of bool.
  if(t != NULL) {
    *token_ptr = t;
    return true;
  }
  return false;
}//parse_token

void print_token(token *t) {
  if(t->type == TSTRING) {
    printf("\"%s\"", ((token_string*)t)->contents);
  } else if(t->type == TKEYWORD) {
    printf(":%s", ((token_keyword*)t)->name);
  } else {
    printf("unknown");
  }
}

void print_token_list(token_list* tl) {
  token **t_ptr = tl->tokens;
  while(*t_ptr != NULL) {
    token *t = *t_ptr;
    print_token(t); printf(" ");
    t_ptr++;
  }
}
