
#include "cstd.h"
//#include "str/regex.inl"
#include "str/regex1.inl"
#define STATIC_STRLEN(str) (sizeof(str)-1)
int test_regex()
{
  uchar re[256] = {0};
#define TEST_CASE(x)
  //#define TEST_REGEXDEF_1(pat, str) {regex_t re[1] = {0}; re_comp(re, pat, strlen(pat), 1, 1); ASSERT(re_exec(re, str, 0, strlen(str))); }
#define TEST_REGEXDEF_1(pat, str) {int n = re_comp(pat, STATIC_STRLEN(pat), re, countof(re)); ASSERT(STATIC_STRLEN(str)==re_exec(re, n, str, 0, STATIC_STRLEN(str))); }
#include "test_regexdef.txt"
#undef TEST_REGEXDEF_1
#undef TEST_CASE
  if (0) {
    const char pat[] = "(a|b|c)*(a|b|c)*";
    int i, j, n = re_comp(pat, STATIC_STRLEN(pat), re, countof(re));
    char str[100] = {0};
    for (j = 0; j < 4; ++j) {
      char ss[] = "abce";
      printf("==================\n");
      memset(str, ss[j], 99);
      for (i = 1; i < 10; ++i) {
        int ret = re_exec(re, n, str, 0, i);
        printf("ret %d\n", ret);
      }
    }
  }
  return 0;
}

