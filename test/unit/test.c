#include "../../deps/greatest.h"

#include "../../src/psh.h"
#include "../../src/reader.h"
#include "../../src/parser.h"

TEST reader_test() {
  char *line = "ls .";
  token_list *list = scan_line(line);
  tree*       tr   = parse_list(list);
  // Check the first in sequence is a command
  tree_command* tc = tr->sequence[0];
  ASSERT_EQ(tc->type, TCOMMAND);

  // Check length
  int command_len = command_length(tc);
  ASSERT_EQ(command_len, 2);

  // Check the tokens are all correct
  token_word *w;
  // First token:
  w = (token_word*)(tc->tokens[0]);
  ASSERT_STR_EQ(w->contents, "ls");
  // Second token:
  w = (token_word*)(tc->tokens[1]);
  ASSERT_STR_EQ(w->contents, ".");
  // Ensure last is NULL:
  ASSERT_EQ(tc->tokens[2], NULL);

  // Check the second (last) in sequence is null.
  ASSERT_EQ(tr->sequence[1], NULL);
  PASS();
}

SUITE(test_suite) {
  RUN_TEST(reader_test);
}

GREATEST_MAIN_DEFS();
int main(int argc, char *argv[]) {
  GREATEST_MAIN_BEGIN();
  RUN_SUITE(test_suite);
  GREATEST_MAIN_END();
  return 0;
}
