#ifndef PARSER_H
#define PARSER_H

#define TOKEN_LIST_SIZE 1024


typedef enum {
  KEYWORD,
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

typedef struct token_list {
  token** tokens;
} token_list;

token_list *parse_line(char *line);
bool parse_token(char**, token**);

#endif
