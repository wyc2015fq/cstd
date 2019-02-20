

#include "std/string_c.h"

int test_str() {
  char aa[256] = "四儿具发射点发而阿道夫";
  replace_str(aa, -1, "四儿", -1, "四川", -1, 1);
  replace_str(aa, 10, "具", -1, "县", -1, 1);
  replace_str(aa, 10, "巿", -1, "市", -1, 1);
  replace_str(aa, 10, "渐", -1, "浙", -1, 1);
  return 0;
}