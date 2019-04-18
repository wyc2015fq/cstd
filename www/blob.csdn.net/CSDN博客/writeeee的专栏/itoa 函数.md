# itoa 函数 - writeeee的专栏 - CSDN博客
2017年04月18日 16:32:51[writeeee](https://me.csdn.net/writeeee)阅读数：222标签：[itoa](https://so.csdn.net/so/search/s.do?q=itoa&t=blog)
个人分类：[c++](https://blog.csdn.net/writeeee/article/category/5634855)
```cpp
#include <stdio.h>
#include <stdlib.h>
int main ()
{
  int i;
  char buffer [33];
  printf ("Enter a number: ");
  scanf ("%d",&i);
  itoa (i,buffer,10);//十进制
  printf ("decimal: %s\n",buffer);
  itoa (i,buffer,16);//十六进制
  printf ("hexadecimal: %s\n",buffer);
  itoa (i,buffer,2);//二进制
  printf ("binary: %s\n",buffer);
  return 0;
}
```
