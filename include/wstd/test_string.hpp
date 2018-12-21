
#include "wstd/string.hpp"
#include <stdio.h>
#include <string.h>

#include "std/test_string_c.h"
using namespace wstd;

int test_string() {
  test_strsep();
  vector<string> ret;
  split(ret, "what tatoo"," t",true);
  const char* out[] = {"wha","a","oo"};
  for (int i=0; i<countof(out); ++i) {
    ASSERT(out[i]==ret[i]);
  }
  return 0;
}

#if 0
int test_string_path() {
  using namespace std;
  const char* str = "C:\\outpic\\aaa.jpg";
  assert("C:" == wstd::path_fmt(str, "d"));
  assert("\\outpic\\" == wstd::path_fmt(str, "p"));
  assert("aaa" == wstd::path_fmt(str, "n"));
  assert(".jpg" == wstd::path_fmt(str, "x"));
  assert("C:\\outpic\\" == wstd::path_fmt(str, "dp"));
  assert("C:\\outpic\\.jpg" == wstd::path_fmt(str, "dpx"));
  assert("C:aaa.jpg" == wstd::path_fmt(str, "dxn"));
  return 0;
}
#endif
