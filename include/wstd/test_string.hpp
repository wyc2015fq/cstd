
#include "wstd/string.hpp"
#include <stdio.h>
#include <string.h>

using namespace wstd;

int test_strsep() {
  char str[] = "abcdefg";
  char *p = str;
  char *key_point;
  while (p)
  {
    while (key_point = strsep(&p, "cd"))//关键字为c或d，它们连续出现了
    {
      //函数遇到c时，key_point指向c返回，遇到d时key_point指向d返回（注意此时d已经被改写为'\0'了）
      if (*key_point == 0)
        continue;//遇到连续的关键字，返回一个指向\0的指针，继续往后找就是
      else
        break;//分割出一个正常的字符串，快去打印吧！
    }
    printf("%s\n", key_point);
  }
  return 0;
}

int test_string() {
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
