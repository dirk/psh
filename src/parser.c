#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "psh.h"
#include "parser.h"

token_list *new_token_list() {
  token_list *tl = malloc(sizeof(token_list));
  tl->tokens = malloc(sizeof(void) * TOKEN_LIST_SIZE);
  tl->tokens[0] = NULL;
  return tl;
}
void free_token_list(token_list *tl) {
  // TODO: Free individual tokens too
  free(tl->tokens);
  free(tl);
}

tree* new_tree() {
  tree* t = malloc(sizeof(tree));
  t->sequence = malloc(sizeof(void) * TREE_SEQUENCE_SIZE);
  t->sequence[0] = NULL;
  return t;
}

int command_length(tree_command *cmd) {
  int length = 0;
  while(cmd->tokens[length] != NULL) {
    length += 1;
  }
  return length;
}

int parse_command(token ***tokens_ptr, void **command_ptr) {
  token **tokens = *tokens_ptr;
  token *t;
  
  t = tokens[0];
  if(t == NULL) return -1;
  if(t->type != TWORD) return -1;
  
  // Set up the command
  tree_command* c = malloc(sizeof(tree_command));
  c->type   = TCOMMAND;
  c->tokens = malloc(sizeof(void) * TREE_SEQUENCE_SIZE);
  *command_ptr = c;
  
  int i = 0;
  while((t = tokens[i]) && t != NULL) {
    if(t->type == TSEPARATOR) {
      break;
    }
    if(t->type != TWORD) {
      fprintf(stderr, "Unexpected token type: %d\n", t->type); // TODO: Return exception
      return 1; // TODO: Leaking
    }
    // token *t = *tokens;
    // printf("%d t: %p\n", i, t);
    c->tokens[i] = t;
    i += 1;
  }
  c->tokens[i] = NULL;
  // Update the parent's tokens pointer.
  *tokens_ptr = &tokens[i];
  return 0;
}

int parse_separator(token ***tokens_ptr, void **command_ptr) {
  token **tokens = *tokens_ptr;
  token *t;
  
  t = tokens[0];
  if(t == NULL) return -1;
  if(t->type != TSEPARATOR) return -1;
  
  // Add the command
  *command_ptr = t;
  // Then move the parent forwards one
  (*tokens_ptr)++;
  return 0;
}

// Parse a token list into a tree of commands/expressions/etc.
tree *parse_list(token_list *list) {
  tree *t = new_tree();

  token** tokens = list->tokens;
  int     err    = 0;
  int     ci     = 0; // Command index

  while(*tokens != NULL) {
    // Error codes:
    //   -1  = Didn't match
    //    0  = Matched
    //    1+ = Error

    // err = parse_group(&tokens, &t->sequence[ci]);
    // if(err > 0) break;
    // if(err == 0) goto tail;

    err = parse_separator(&tokens, &t->sequence[ci]);
    if(err > 0) break;
    if(err == 0) goto tail;

    err = parse_command(&tokens, &t->sequence[ci]);
    if(err > 0) break;
    if(err == 0) goto tail;

    if(err == -1) {
      fprintf(stderr, "Unable to match token type: %s (%d)\n", string_for_token_type((token_type)(*tokens)->type), (*tokens)->type);
      return (tree*)PERR_UNKOWN_TOKEN;
    }

  tail:
    // if(*tokens != NULL) print_token(*tokens);
    ci += 1;
  }
  if(err != 0) {
    fprintf(stderr, "Parse error: %d\n", err);
    return NULL;
  }
  // Ensure NULL-terminated
  t->sequence[ci] = NULL;
  return t;
}


token_list *scan_line(char *line) {
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
bool is_underscore(char c) {
  return c == '_';
}
bool is_number(char c) {
  return (c >= '0' && c <= '9');
}
bool is_parentheses(char c) {
  return (c == '(' || c == ')');
}

bool is_word_character(char c) {
  return (
    is_letter(c) ||
    is_underscore(c) ||
    is_number(c) ||
    is_parentheses(c) ||
    c == '-' ||
    c == '.' ||
    c == '#' ||
    c == '\'' ||
    c == '/'
  );
}
bool is_starting_word_character(char c) {
  return (is_word_character(c) && c != '/');
}

void consume_whitespace(char **pos_ptr) {
  char *pos = *pos_ptr;
  while(*pos == ' ' || *pos == '\t') { pos++; }
  *pos_ptr = pos;
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

token_word *consume_string(char **pos_ptr) {
  char *pos = *pos_ptr;
  if(pos[0] != '"') return NULL;
  pos++;
  
  // Allocate the string
  // TODO: Make it use dynamic strings
  char *s = malloc(sizeof(char) * MAX_WORD_LENGTH);
  int si = 0;
  while(*pos != '"' && si < (MAX_WORD_LENGTH - 1)) {
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
  s[si] = '\0'; // End the string
  pos++; // Move beyond closing quote
  *pos_ptr = pos;
  token_word* str = malloc(sizeof(token_word));
  str->type = TWORD;
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

token_word *consume_word(char **pos_ptr) {
  char *pos = *pos_ptr;
  // Check we have a word to start off with
  if(!is_starting_word_character(pos[0])) return NULL;
  // TODO: Dynamicize this string
  char *s = malloc(sizeof(char) * MAX_WORD_LENGTH);
  int   i = 0;
  while(is_word_character(pos[i]) && i < (MAX_WORD_LENGTH - 1)) {
    s[i] = pos[i];
    i++;
  }
  s[i] = '\0';
  *pos_ptr = &pos[i];
  token_word* str = malloc(sizeof(token_word));
  str->type = TWORD;
  str->contents = s;
  return str;
}

token_separator *consume_separator(char **pos_ptr) {
  char *pos = *pos_ptr;
  
  if(pos[0] == ';') {
    token_separator* s = malloc(sizeof(token_separator));
    s->type = TSEPARATOR;
    s->separator = SCOLON;
    *pos_ptr = &pos[1];
    return s;
  }
  return NULL;
}

bool parse_token(char **pos_ptr, token **token_ptr) {
  token* t;
  consume_whitespace(pos_ptr);
  
  // Try to consume difference tokens
  if((t = (token*)consume_keyword(pos_ptr)) && t != NULL) {
    // printf("keyword: %s (%p)\n", ((token_keyword*)t)->name, t);
  } else
  if((t = (token*)consume_string(pos_ptr)) && t != NULL) {
    
  } else
  if((t = (token*)consume_word(pos_ptr)) && t != NULL) {
    
  } else
  if((t = (token*)consume_separator(pos_ptr)) && t != NULL) {
    
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
  if(t->type == TWORD) {
    printf("\"%s\"", ((token_word*)t)->contents);
  } else if(t->type == TKEYWORD) {
    printf(":%s", ((token_keyword*)t)->name);
  } else if(t->type == TSEPARATOR) {
    printf("separator");
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

static const char* parse_error_human_names[] = {
  NULL,// 0
  "generic",// 1
  "unknown token"// 2
};
const char* human_name_for_parse_error(parse_error p) {
  return parse_error_human_names[(int)p];
}

static const char* token_type_strings[] = {
  "TKEYWORD",
  "TWORD",
  "TSEPARATOR",
};
const char* string_for_token_type(token_type t) {
  return token_type_strings[(int)t];
}
