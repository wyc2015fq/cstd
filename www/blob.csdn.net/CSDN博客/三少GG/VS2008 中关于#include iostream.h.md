# VS2008 中关于#include "iostream.h" - 三少GG - CSDN博客
2009年11月21日 12:46:00[三少GG](https://me.csdn.net/scut1135)阅读数：1521标签：[语言																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)
C语言里头文件#include "iostream.h"
在vs2008中使用时，是C++的输入输出流。目前ANSI中不应该是这样写，
而应该写作：
#include<iostream>
并且要加上名称空间语句
using namespace std; 
cout是在std那个命名空间里面的，必须加个using namespace std，
或者直接std::cout
