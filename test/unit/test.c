#include "../../deps/greatest.h"

#include "../../src/psh.h"
#include "../../src/reader.h"
#include "../../src/parser.h"

TEST reader_test() {
  char *line = "ls .";
  token_list *list = scan_line(line);
  tree*       tr   = parse_list(list);
  
  tree_command* tc = tr->sequence[0];
  ASSERT_EQ(tc->type, TCOMMAND);
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
