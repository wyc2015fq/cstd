# 对stdin，stdout 和STDOUT_FILENO，STDIN_FILENO的学习 - h13 - 博客园
在unix系统调用中,标准输入描述字用stdin,标准输出用stdout,标准出错用stderr表示,但在一些调用函数,引用了STDIN_FILENO表示标准输入才,同样,标准出入用STDOUT_FILENO,标准出错用STDERR_FILENO.
他们的区别：
stdin等是FILE *类型，属于标准I/O，在<stdio.h>。
STDIN_FILENO等是文件描述符，是非负整数，一般定义为0, 1, 2，属于没有buffer的I/O，直接调用系统调用，在<unistd.h>。 
下面一个例子是对STDOUT_FIFENO 和STDIN_FILENO的应用：
#include<unistd.h>
#define SIZE 100
int main(void)
{
        int n;
        char buf[SIZE];
        while(n=read(STDIN_FILENO,buf,SIZE))   //读取标准输入到buf中，返回读取字节数。
      {
                if(n!=write(STDOUT_FILENO,buf,n)) 把buf 写到标准输出中
                        perror("write error");
        }
        if(n<0) perror("read error");   
        return 0;
}
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
如果按照常规的使用方法：
[fan@fan:~/arm$](mailto:fan@fan:~/arm$) gcc -o app stdout.c 
[fan@fan:~/arm$](mailto:fan@fan:~/arm$) ./app
fdfe
fdfe
fefefef
fefefef
//输入一行，马上输出一行，也就是按照原样输出
===========================================================================
使用重定向来运行
[fan@fan:~/arm$](mailto:fan@fan:~/arm$) ./app> data
fefe
fdadfdf
adfdfef
//输入的字符串并不是输出到终端，而是输出到data文件中。
==========================================================================
[fan@fan:~/arm$](mailto:fan@fan:~/arm$) ./app <stdout.c >data
[fan@fan:~/arm$](mailto:fan@fan:~/arm$)
//这一句相当于把stdout.c复制到data中去
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++=
STDOUT_FILENO和STDIN_FILENO在默认的情况下是终端，
使用ctrl+D来输入文件结束符
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/xiaoxi2xin/archive/2010/04/24/5524769.aspx](http://blog.csdn.net/xiaoxi2xin/archive/2010/04/24/5524769.aspx)
