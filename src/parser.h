#ifndef PARSER_H
#define PARSER_H

#define TOKEN_LIST_SIZE 1024
#define MAX_WORD_LENGTH 1024
#define TREE_SEQUENCE_SIZE 1024

typedef enum {
  PERR_GENERIC = 1,
  PERR_UNKOWN_TOKEN = 2,
  PARSE_ERRORS
} parse_error;

typedef enum {
  TKEYWORD,
  TWORD,
  TSEPARATOR,
  TOKENS
} token_type;

typedef enum {
  TTREE,
  TCOMMAND,
  TREES
} tree_type;

typedef enum {
  SCOLON,
  SEPARATORS
} separator_type;

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
typedef struct token_separator {
  token_type     type;
  separator_type separator;
} token_separator;

typedef struct token_list {
  token** tokens;
} token_list;


typedef struct tree_command {
  tree_type      type;
  token**        tokens;
} tree_command;
typedef struct tree_separator {
  tree_type      type;
  separator_type separator;
} tree_separator;

typedef struct tree {
  tree_type type;
  void**    sequence;
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
void print_token(token *t);
void print_token_list(token_list* tl);
void print_tree(tree* t);

int command_length(tree_command*);

const char* human_name_for_parse_error(parse_error p);
const char* string_for_token_type(token_type t);

#endif
