#ifndef PARSER_H
#define PARSER_H

#define TOKEN_LIST_SIZE 1024
#define MAX_WORD_LENGTH 1024


typedef enum {
  TKEYWORD,
  TWORD,
  TOKENS
} token_type;

typedef enum {
  TSEQUENCE,
  TCOMMAND,
  TREES
} tree_type;

/*static const char *token_names[TOKENS] = {
  "keyword"
};*/

typedef struct token {
  token_type type;
} token;

typedef struct token_keyword {
  token_type type;
  char      *name;
} token_keyword;
typedef struct token_word {
  token_type type;
  char      *contents;
} token_word;

typedef struct token_list {
  token** tokens;
} token_list;


typedef struct tree_command {
  tree_type      type;
  token_list*    list;
} tree_command;
typedef struct tree_sequence {
  tree_type      type;
  tree_command** commands;
} tree_sequence;

typedef struct tree {
  tree_sequence** sequences;
} tree;

token_list *scan_line(char *line);
tree *parse_list(token_list *list);


// Scanning
bool parse_token(char**, token**);
token_word *consume_word(char**);
token_word *consume_string(char**);
token_keyword *consume_keyword(char**);

// Utilities
char consume_string_escape_sequence(char**);
void print_token_list(token_list* tl);

#endif
