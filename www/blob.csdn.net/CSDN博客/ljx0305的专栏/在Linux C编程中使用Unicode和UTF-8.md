# 在Linux C编程中使用Unicode和UTF-8 - ljx0305的专栏 - CSDN博客
2009年07月19日 20:51:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1787标签：[linux																[c																[编程																[character																[终端																[编译器](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=终端&t=blog)](https://so.csdn.net/so/search/s.do?q=character&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[C																[C++](https://blog.csdn.net/ljx0305/article/category/380566)](https://blog.csdn.net/ljx0305/article/category/394717)
在Linux C编程中使用Unicode和UTF-8
目前各种Linux发行版都支持UTF-8编码，当前系统的语言和字符编码设置保存在一些环境变量中，可以通过locale命令查看：
$ locale
LANG=en_US.UTF-8
LC_CTYPE="en_US.UTF-8"
LC_NUMERIC="en_US.UTF-8"
LC_TIME="en_US.UTF-8"
LC_COLLATE="en_US.UTF-8"
LC_MONETARY="en_US.UTF-8"
LC_MESSAGES="en_US.UTF-8"
LC_PAPER="en_US.UTF-8"
LC_NAME="en_US.UTF-8"
LC_ADDRESS="en_US.UTF-8"
LC_TELEPHONE="en_US.UTF-8"
LC_MEASUREMENT="en_US.UTF-8"
LC_IDENTIFICATION="en_US.UTF-8"
LC_ALL=
常用汉字也都位于BMP中，所以一个汉字的存储通常占3个字节。
例如编辑一个C程序：
#include <stdio.h>
int main(void)
{
 printf("你好/n");
 return 0;
}
源文件是以UTF-8编码存储的：
$ od -tc nihao.c 
0000000   #   i   n   c   l   u   d   e       <   s   t   d   i   o   .
0000020   h   >  /n  /n   i   n   t       m   a   i   n   (   v   o   i
0000040   d   )  /n   {  /n  /t   p   r   i   n   t   f   (   " 344 275
0000060 240 345 245 275   /   n   "   )   ;  /n  /t   r   e   t   u   r
0000100   n       0   ;  /n   }  /n
0000107
其中八进制的344 375 240（十六进制e4 bd a0）就是“你”的UTF-8编码，八进制的345 245 275（十六进制e5 a5 bd）就是“好”。
把它编译成目标文件，"你好/n"这个字符串就成了这样一串字节：e4 bd a0 e5 a5 bd 0a 00，汉字在其中仍然是UTF-8编码的，一个汉字占3个字节，这种字符在C语言中称为多字节字符（Multibyte Character）。
运行这个程序相当于把这一串字节write到当前终端的设备文件。如果当前终端的驱动程序能够识别UTF-8编码就能打印出汉字，如果当前终端的驱动程序不能识别UTF-8编码（比如一般的字符终端）就打印不出汉字。
也就是说，像这种程序，识别汉字的工作既不是由C编译器做的也不是由libc做的，C编译器原封不动地把源文件中的UTF-8编码复制到目标文件中，libc只是当作以0结尾的字符串原封不动地write给内核，识别汉字的工作是由终端的驱动程序做的。
但是仅有这种程度的汉字支持是不够的，有时候我们需要在C程序中操作字符串里的字符，比如求字符串"你好/n"中有几个汉字或字符，用strlen就不灵了，因为strlen只看结尾的0字节而不管字符串里存的是什么，求出来的是字节数7。
为了在程序中操作Unicode字符，C语言定义了宽字符（Wide Character）类型wchar_t和一些库函数。
在字符常量或字符串字面值前面加一个L就表示宽字符常量或宽字符串，
例如定义wchar_t c = L'你';，变量c的值就是汉字“你”的31位UCS编码，而L"你好/n"就相当于{L'你', L'好', L'/n', 0}，wcslen函数就可以取宽字符串中的字符个数。
看下面的程序：
#include <stdio.h>
#include <locale.h>
int main(void)
{
 if (!setlocale(LC_CTYPE, "")) 
 {
  fprintf(stderr, "Can't set the specified locale! "
   "Check LANG, LC_CTYPE, LC_ALL./n");
  return 1;
 }
 printf("%ls", L"你好/n");
 return 0;
}
宽字符串L"你好/n"在源代码中当然还是存成UTF-8编码的，
但编译器会把它变成4个UCS编码0x00004f60 0x0000597d 0x0000000a 0x00000000保存在目标文件中，
按小端存储就是60 4f 00 00 7d 59 00 00 0a 00 00 00 00 00 00 00，用od命令查看目标文件应该能找到这些字节。
$ gcc hihao.c
$ od -tx1 a.out
printf的%ls转换说明表示把后面的参数按宽字符串解释，不是见到0字节就结束，而是见到UCS编码为0的字符才结束，
但是要write到终端仍然需要以多字节编码输出，这样终端驱动程序才能识别，所以printf在内部把宽字符串转换成多字节字符串再write出去。
事实上，C标准并没有规定多字节字符必须以UTF-8编码，也可以使用其它的多字节编码，在运行时根据环境变量确定当前系统的编码，
所以在程序开头需要调用setlocale获取当前系统的编码设置，如果当前系统是UTF-8的，printf就把UCS编码转换成UTF-8编码的多字节字符串再write出去。
一般来说，程序在做内部计算时通常以宽字符编码，如果要存盘或者输出给别的程序，或者通过网络发给别的程序，则采用多字节编码。
关于Unicode和UTF-8本节只介绍了最基本的概念，读者可进一步参考[Unicode FAQ]。
[http://www.yayu.org/book/Linux_c_study_html/bi01.html#bibli.unicodefaq](http://www.yayu.org/book/Linux_c_study_html/bi01.html#bibli.unicodefaq)
