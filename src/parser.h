#ifndef PARSER_H
#define PARSER_H

#define TOKEN_LIST_SIZE 1024
#define MAX_WORD_LENGTH 1024


typedef enum {
  TKEYWORD,
  TWORD,
  TOKENS
} token_type;

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

token_list *parse_line(char *line);
bool parse_token(char**, token**);

token_word *consume_word(char**);
token_word *consume_string(char**);
token_keyword *consume_keyword(char**);

char consume_string_escape_sequence(char**);

void print_token_list(token_list* tl);

#endif
