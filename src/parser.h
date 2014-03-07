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

#ifdef YYTOKENTYPE
typedef YYTOKENTYPE token_type;
#else
typedef int token_type;
#endif

typedef enum {
  TRTREE,
  TRCOMMAND,
  TREXPRESSION,
  TRWORD,
  TRWORD_LIST,
  TRITEM,
  TRGROUP,
  TRSEQUENCE_LIST,
  TRBODY,
  TREES
} tree_type;

typedef enum {
  SCOLON,
  SLPAREN,
  SRPAREN,
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

typedef enum {
  EGROUP,
  ECALL,
  EXPRESSIONS
} expression_type;


typedef struct tr_word {
  tree_type type;
  char*     value;
  
  struct tr_word*  next;
  struct tr_word*  prev;
} tr_word;
tr_word *new_tr_word();

typedef struct tr_word_list {
  tree_type type;
  tr_word*  head;
} tr_word_list;
tr_word_list *new_tr_word_list();

typedef struct tr_command {
  tree_type      type;
  tr_word_list*  list;
} tr_command;
tr_command *new_tr_command();

typedef struct tr_expression {
  tree_type type;
  char*     keyword;
  void*     body;
} tr_expression;
tr_expression *new_tr_expression();

typedef struct tr_sequence {
  tree_type           type;// TRITEM or TRGROUP
  void*               item;
  struct tr_sequence* next;
  struct tr_sequence* prev;
} tr_sequence;
tr_sequence *new_tr_sequence();

typedef struct tr_sequence_list {
  tree_type    type;
  tr_sequence* head;
} tr_sequence_list;
tr_sequence_list *new_tr_sequence_list();

typedef struct tr_body {
  tree_type         type;
  tr_sequence_list* list;
} tr_body;
tr_body *new_tr_body();

typedef struct tree_separator {
  tree_type      type;
  separator_type separator;
} tree_separator;
typedef struct tree_expression {
  tree_type type;
  expression_type expression;
  token_list* list;// List of tokens that composes the expression
  struct tree_expression* next;// Expression following this one
} tree_expression;

typedef struct tree {
  tree_type type;
  void**    sequence;
} tree;

token_list *scan_line(char *line);
tree *parse_list(token_list *list);

tree *parse_line(char *line);

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

int command_length(tr_command*);

const char* human_name_for_parse_error(parse_error p);
const char* string_for_token_type(token_type t);



#define YYDEBUG 1

#define YYLTYPE_IS_DECLARED
typedef struct YYLTYPE {
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;

#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void* yyscan_t;
#endif

#endif
