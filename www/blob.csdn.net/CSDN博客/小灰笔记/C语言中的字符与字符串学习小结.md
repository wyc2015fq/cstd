# C语言中的字符与字符串学习小结 - 小灰笔记 - CSDN博客





2017年02月09日 00:28:34[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：1159








软件编程到底难不难呢？简单的写个小玩意儿似乎也不需要什么太多的知识或者技巧，但是往深了看，每一门编程语言似乎精细奥妙的多。C语言便是一个我用了许久但是始终没有搞明白的一门编程语言。

今天学习了解到的新知识点，关于字符与字符串：

1，用单引号引起的字符，本质上代表的是一个数字；

2，用双引号引起的字符串代表的是一个指向无名数组起始字符的指针，该数组被双引号之间的字符以及一个额外的二进制为0的字符串’\0’初始化。

测试代码：




  1 #include "stdio.h"

  2 

  3 char var_char =
'f';

  4 char var_str[] = {'a','b','c',0};

  5 char var_str_s[] =
"abc";

  6 

  7 int main(void)

  8 {

  9     printf("%d\n",var_char);

 10     printf("%s\n",var_str);

 11     printf("%s\n",var_str_s);

 12 return0;

 13 }




var_str与var_str_s其实是等效的两种方式。



