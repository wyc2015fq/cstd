
#include "wstd/string.hpp"

int test_string() {
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

