# 编译错误syntax error : missing ';' before 'type' - sandro_zhang的专栏 - CSDN博客
2011年09月16日 15:44:10[sandro_zhang](https://me.csdn.net/sandro_zhang)阅读数：800
大多此情况的解决方法是,在一个函数体中,变量定义要放在所有函数调用之前.
```cpp
#include <stdio.h>
int main()
{
 char a[100];
 char b;
 memset(&a, 0, 100);
 return 0;
}
```
