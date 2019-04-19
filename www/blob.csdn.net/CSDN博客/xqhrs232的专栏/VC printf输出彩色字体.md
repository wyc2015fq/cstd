# VC printf输出彩色字体 - xqhrs232的专栏 - CSDN博客
2013年04月28日 11:03:42[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：881
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://blog.csdn.net/huangkangying/article/details/8054447](http://blog.csdn.net/huangkangying/article/details/8054447)
在VC下使用SetConsoleTextAttribute（）函数可以改变当前控制台的前景色和背景色，从而达到输出彩色字体的效果。
使用的方法也很简单，具体代码如下：
**[cpp]**[view
 plain](http://blog.csdn.net/huangkangying/article/details/8054447#)[copy](http://blog.csdn.net/huangkangying/article/details/8054447#)
- #include <windows.h>
- #include <winnt.h>
- #include <stdio.h>
- 
- int main(int argc, char* argv[])  
- {  
- HANDLE hConsoleWnd;  
-     hConsoleWnd = GetStdHandle(STD_OUTPUT_HANDLE);  
-     SetConsoleTextAttribute(hConsoleWnd,FOREGROUND_RED);  
-     printf("I am red now!\n");  
-     SetConsoleTextAttribute(hConsoleWnd,FOREGROUND_INTENSITY);  
-     printf("I am gray now!\n");  
- return 0;  
- }  
