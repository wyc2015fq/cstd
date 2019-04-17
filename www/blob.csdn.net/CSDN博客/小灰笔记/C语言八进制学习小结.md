# C语言八进制学习小结 - 小灰笔记 - CSDN博客





2017年02月08日 23:52:38[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：551








读了一下《C语言陷阱与缺陷》，开篇的一个介绍便是八进制。说起来，这个问题在我工作的过程中还确实是遇到过。

在C语言中，以0开头的数字被识别为八进制。简单写一段代码如下：

  1 #include
"stdio.h"

  2 

  3 int var1 =
123;

  4 int var2 =
0123;

  5 

  6 int main(void)

  7 {

  8     printf("var1 =
%d\n",var1);

  9     printf("var2 =
%d\n",var2);

 10 return0;

 11 }




编译运行结果：




GreydeMac-mini:oct greyzhang$ gcc oct.c 

GreydeMac-mini:oct greyzhang$ ./a.out 

var1 = 123

var2 = 83




从书中看到，有些编译器会把0789这样的数值也会按照八进制的转换方式进行转换。最终的结果为9*(8**0) + 8*(8**1) + 7*(8**2)计算出来。不过在标准C中，这是不允许的。简单测试一下，修改代码：




  1 #include
"stdio.h"

  2 

  3 int var1 =
789;

  4 int var2 =
0789;

  5 

  6 int main(void)

  7 {

  8     printf("var1 =
%d\n",var1);

  9     printf("var2 =
%d\n",var2);

 10 return0;

 11 }




编译与运行：

GreydeMac-mini:oct greyzhang$ gcc oct.c 

**oct.c:4:14: ****error: ****invalid digit '8' in octal constant**

int var2 = 0789;

**             ^**

1 error generated.




查看一下编译器的版本信息：

GreydeMac-mini:oct greyzhang$ gcc -v

Configured with: --prefix=/Library/Developer/CommandLineTools/usr --with-gxx-include-dir=/usr/include/c++/4.2.1

Apple LLVM version 8.0.0 (clang-800.0.42.1)

Target: x86_64-apple-darwin16.4.0

Thread model: posix

InstalledDir: /Library/Developer/CommandLineTools/usr/bin




不像之前用过的gcc，这次没有gcc的版本信息，不过里面的信息在使用Python的时候倒是看到过。

[GCC 4.2.1 Compatible Apple LLVM 8.0.0 (clang-800.0.34)] on darwin




由以上信息可以看出，其实Mac里面提供的gcc编译器并不是gcc而是一个兼容gcc功能的一个编译器。



