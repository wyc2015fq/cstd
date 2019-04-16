# 数据结构和算法系列 - c语言线性查找算法 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年06月22日 12:28:49[initphp](https://me.csdn.net/initphp)阅读数：1314








```cpp
#include <stdio.h>
char strArr[] = "woshixixihaha";
int indexof(char str) {
  int i = 0;
  while (strArr[i] != '\0') {
     if (strArr[i] == str) {
       return i;
     }
     i++;
  }
  return -1;
}

int main() {
  printf("字符串的位置：%d", indexof('s'));
  return 0;
}
```





