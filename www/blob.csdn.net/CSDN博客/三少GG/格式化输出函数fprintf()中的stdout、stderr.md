# 格式化输出函数fprintf()中的stdout、stderr - 三少GG - CSDN博客
2012年02月29日 16:10:29[三少GG](https://me.csdn.net/scut1135)阅读数：6055
                
关于格式化输出函数fprintf()中的stdout、stderr
格式化输出函数 fprintf() 的原型如下：
#include<stdio.h>
int printf(const char *restrict format, ...);
int fprintf(FILE *restrict fp, const char *restrict format, ...);
两个函数的返回值：若成功则返回输出字节数，若输出出错则返回负值
**printf将格式化写到标准输出，fprintf写至指定的流。**
以下为测试fp为stdout和stderr的区别：
#vi fprint.c
在vi中输入下面几行：
-----------------------------------------------------------------
#include<stdio.h>
void main()
{
fprintf(stdout,"this is first!\n");
fprintf(stderr,"this is second!\n");
printf("this is third!\n");
}
------------------------------------------------------------------
保存退出后
#gcc fprint.c -0 fprint
#./fprint
会在终端输出
this is first!
this is second!
this is third!
若把输出重定向到tmp.txt文件
#./fpint>tmp.txt
this is second!
文件tmp.txt中的内容为：
this is first!
this is third!
说明：stdout标准输出、stderr标准错误输出，二者默认向屏幕输出。
如果重定向输出到磁盘文件，则stdout输出到该文件，而stderr仍输出到屏幕。
stderr是作为程序运行中的错误显示出来的，若要把它重定向到磁盘文件，需要运行如下命令：
#./fprint 2>tmp.txt
this is first!
this is third!
文件tmp.txt中的内容为：
this is second!
PS：关于为什么 "./fprint 2>tmp.txt" 中的 2 ；在UNIX系统中，标准输入、标准输出、标准错误输出分别被定义为0、1、2。
维基百科：            [标准流](http://zh.wikipedia.org/zh-cn/%E6%A8%99%E6%BA%96%E4%B8%B2%E6%B5%81)
