
#include "string_c.h"
#include <stdio.h>

int test_strsep()
{
  char str[] = "abcdefg";
  char* p = str;
  char* key_point;
  while (p) {
    //关键字为c或d，它们连续出现了
    while (key_point = strsep_c(&p, "cd")) {
      //函数遇到c时，key_point指向c返回，遇到d时key_point指向d返回（注意此时d已经被改写为'\0'了）
      if (*key_point == 0) {
        continue;  //遇到连续的关键字，返回一个指向\0的指针，继续往后找就是
      } else {
        break;  //分割出一个正常的字符串，快去打印吧！
      }
    }
    printf("%s\n", key_point);
  }
  return 0;
}
