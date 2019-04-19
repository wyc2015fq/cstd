# 如何在Linux内核里增加一个系统调用 - xqhrs232的专栏 - CSDN博客
2012年03月04日 15:12:35[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：634标签：[linux内核																[测试																[嵌入式																[linux](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=嵌入式&t=blog)](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=linux内核&t=blog)
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://blog.csdn.net/chenjieb520/article/details/7317629](http://blog.csdn.net/chenjieb520/article/details/7317629)
作者:chenjieb520
**给linux内核增加一个系统调用的方法**
 为了更加好地调试linux内核，笔者的实验均在mini6410的arm板上运行的。这样做的原因，第一是因为本人是学嵌入式的，第二是这样可以在arm板上进行实验，可以提高实验的效率。
**（1****）、首先定义你所要添加的系统调用名**，然后在Linux-2.6.x/kernel/创建一个文件，文件名为：mysyscall.c文件的内容为：
![](http://hi.csdn.net/attachment/201203/4/0_1330836176XH1m.gif)
**（2）、在Linux-2.6.x/arch/x86/include/asm/unistd_32.h****中为系统调用定义一个索引号**，这个索引号必须在索引列表的最后一个的后面。
![](http://hi.csdn.net/attachment/201203/4/0_1330836219HYTB.gif)
**（3）、在unistd_32.h中同时添加了系统调用的个数**
        #define __NR_syscalls 295   //add by chenjieb
![](http://hi.csdn.net/attachment/201203/4/0_1330836259KpP0.gif)
**（4）、在Linux-2.6.x/arch/x86/kernel/syscall_table.S****中**，这个表按系统调用号（即前面提到的__NR_name）排列了所有系统调用函数的指针，以供系统调用入口函数查找。从这张表看得出，linux给它所支持的系统调用函数取名叫sys_name。
     .long sys_mysyscall
![](http://hi.csdn.net/attachment/201203/4/0_1330836305rzYt.gif)
**（5）、在Linux-2.6.x/kernel/Makefile****中添加如下的语句**，这是用于内核的编译使用的。
    obj-y += mysyscall.o
![](http://hi.csdn.net/attachment/201203/4/0_1330836354rdjl.gif)
**（6）、写一个测试的程序。测试上述的西系统调用是否可以用。**
 #include <errno.h>
 #include <sys/syscall.h>
 #include <unistd.h>
 #include <stdio.h>
 #include <stdlib.h>
long errno_one;
#define __NR_mysyscall 294
int main() 
{
errno_one=mysyscall(12,24);
printf("%d\n",errno_one);
return 0;
}
**运行结果：**首先先对增加了新的系统调用的内核进行编译，将编译后的内核下载到arm板上，然后编译自己的测试程序。
**1、编译内核结果如下：**
![](http://hi.csdn.net/attachment/201203/4/0_1330836452o5oU.gif)
**2****、运行的结果：**如果在自己的PC板上运行的话，需要超级用户，而我是在arm板上运行，arm板上的系统默认是超级用户，所以我这里只需要一般的运行即可，结果如下：
![](http://hi.csdn.net/attachment/201203/4/0_1330836477t26k.gif)
希望这个教程对大家学习Linux内核有用！欢迎大家批评指教！
