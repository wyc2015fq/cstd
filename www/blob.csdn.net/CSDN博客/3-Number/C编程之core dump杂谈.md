
# C编程之core dump杂谈 - 3-Number - CSDN博客


置顶2015年06月05日 16:39:14[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：551标签：[c																](https://so.csdn.net/so/search/s.do?q=c&t=blog)[core dump																](https://so.csdn.net/so/search/s.do?q=core dump&t=blog)[gdb																](https://so.csdn.net/so/search/s.do?q=gdb&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=core dump&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=c&t=blog)个人分类：[C编程																](https://blog.csdn.net/pangyemeng/article/category/5429119)
[
																								](https://so.csdn.net/so/search/s.do?q=c&t=blog)


## 0x01简单示例
开始讲前先学习C程序各内存分配情况。请看如下程序：

```python
#include <stdio.h>
#include <stdlib.h>
int a = 0;          //a在全局已初始化数据区 
char *p1 = NULL;    //p1在BSS区（未初始化全局变量） 
int main() 
{
        int b;            //b在栈区
        char s[] = "abc"; //s为数组变量，存储在栈区，
                          //"abc"为字符串常量，存储在已初始化数据区
        char *p1 = NULL, *p2 = NULL;  //p1、p2在栈区
        char *p3 = "123456";          //123456\0在已初始化数据区，p3在栈区 
        static int c =0;              //C为全局（静态）数据，存在于已初始化数据区
                                      //另外，静态数据会自动初始化
        p1 = (char *)malloc(10);      //分配得来的10个字节的区域在堆区
        if(p1 == NULL)
        {   
                printf("malloc error\n");
                return -1; 
        }   
        p2 = (char *)malloc(20);     //分配得来的20个字节的区域在堆区
        if(p1 == NULL)
        {   
                printf("malloc error\n");
                return -1; 
        }   
        free(p1);
        free(p2);
        return 0;
}
```

![](https://img-blog.csdn.net/20150605164109380?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

可以以看出，此可执行程序在存储时（没有调入到内存）分为代码区（text）、数据区（data）和未初始化数据区（bss）3个部分。
（1）代码区（text segment）。存放CPU执行的机器指令（machine instructions）。通常，代码区是可共享的（即另外的执行程序可以调用它），因为对于频繁被执行的程序，只需要在内存中有一份代码即可。代码区通常是只读的，使其只读的原因是防止程序意外地修改了它的指令。另外，代码区还规划了局部变量的相关信息。
（2）全局初始化数据区/静态数据区（initialized data segment/data segment）。该区包含了在程序中明确被初始化的全局变量、静态变量（包括全局静态变量和局部静态变量）和常量数据（如字符串常量）。
（3）未初始化数据区。亦称BSS区（uninitialized data segment），存入的是全局未初始化变量。BSS这个叫法是根据一个早期的汇编运算符而来，这个汇编运算符标志着一个块的开始。BSS区的数据在程序开始执行之前被内核初始化为0或者空指针（NULL）。![](https://img-blog.csdn.net/20150605164429793?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
（1）代码区（text segment）。代码区指令根据程序设计流程依次执行，对于顺序指令，则只会执行一次（每个进程），如果反复，则需要使用跳转指令，如果进行递归，则需要借助栈来实现。
代码区的指令中包括操作码和要操作的对象（或对象地址引用）。如果是立即数（即具体的数值，如5），将直接包含在代码中；如果是局部数据，将在栈区分配空间，然后引用该数据地址；如果是BSS区和数据区，在代码中同样将引用该数据地址。
（2）全局初始化数据区/静态数据区（Data Segment）。只初始化一次。
（3）未初始化数据区（BSS）。在运行时改变其值。
（4）栈区（stack）。由编译器自动分配释放，存放函数的参数值、局部变量的值等。其 操作方式类似于数据结构中的栈。每当一个函数被调用，该函数返回地址和一些关于调用的信息，比如某些寄存器的内容，被存储到栈区。然后这个被调用的函数再 为它的自动变量和临时变量在栈区上分配空间，这就是C实现函数递归调用的方法。每执行一次递归函数调用，一个新的栈框架就会被使用，这样这个新实例栈里的 变量就不会和该函数的另一个实例栈里面的变量混淆。
（5）堆区（heap）。用于动态内存分配。堆在内存中位于bss区和栈区之间。一般由程序员分配和释放，若程序员不释放，程序结束时有可能由OS回收。
之所以分成这么多个区域，主要基于以下考虑：
1、一个进程在运行过程中，代码是根据流程依次执行的，只需要访问一次，当然跳转和递归有可能使代码执行多次，而数据一般都需要访问多次，因此单独开辟空间以方便访问和节约空间。
2、临时数据及需要再次使用的代码在运行时放入栈区中，生命周期短。
3、 全局数据和静态数据有可能在整个程序执行过程中都需要访问，因此单独存储管理。
4、堆区由用户自由分配，以便管理。
## 0x02系统支持coredump配置
1、在/etc/profile中加入以下一行，这将允许生成coredump文件
ulimit -c unlimited
2、在rc.local中加入以下一行，这将使程序崩溃时生成的coredump文件位于/tmp目录下:
echo /tmp/core.%e.%p > /proc/sys/kernel/core_pattern
/tmp/也可以是其它的目录位置。最佳位置应当满足以下需求：
* 对所有用户可写
* 空间容量足够大
*掉电后文件不丢失
## 0x03 core dump
## 出现的情况
先聊聊C编程中常导致coredump的情况：
1，内存访问越界
a) 由于使用错误的下标，导致数组访问越界。
b) 搜索字符串时，依靠字符串结束符来判断字符串是否结束，但是字符串没有正常的使用结束符。
c) 使用strcpy, strcat, sprintf, strcmp,strcasecmp等字符串操作函数，将目标字符串读/写爆。应该使用strncpy, strlcpy, strncat, strlcat, snprintf, strncmp, strncasecmp等函数防止读写越界。
2，多线程程序使用了线程不安全的函数。
应该使用下面这些可重入的函数，它们很容易被用错：
asctime_r(3c) gethostbyname_r(3n) getservbyname_r(3n)ctermid_r(3s) gethostent_r(3n) getservbyport_r(3n) ctime_r(3c) getlogin_r(3c)getservent_r(3n) fgetgrent_r(3c) getnetbyaddr_r(3n) getspent_r(3c)fgetpwent_r(3c) getnetbyname_r(3n) getspnam_r(3c) fgetspent_r(3c)getnetent_r(3n) gmtime_r(3c) gamma_r(3m) getnetgrent_r(3n) lgamma_r(3m) getauclassent_r(3)getprotobyname_r(3n) localtime_r(3c) getauclassnam_r(3) etprotobynumber_r(3n)nis_sperror_r(3n) getauevent_r(3) getprotoent_r(3n) rand_r(3c) getauevnam_r(3)getpwent_r(3c) readdir_r(3c) getauevnum_r(3) getpwnam_r(3c) strtok_r(3c) getgrent_r(3c)getpwuid_r(3c) tmpnam_r(3s) getgrgid_r(3c) getrpcbyname_r(3n) ttyname_r(3c)getgrnam_r(3c) getrpcbynumber_r(3n) gethostbyaddr_r(3n) getrpcent_r(3n)
3，多线程读写的数据未加锁保护。
对于会被多个线程同时访问的全局数据，应该注意加锁保护，否则很容易造成coredump
4，非法指针
a) 使用空指针
b) 随意使用指针转换。一个指向一段内存的指针，除非确定这段内存原先就分配为某种结构或类型，或者这种结构或类型的数组，否则不要将它转换为这种结构或类型的指针，而应该将这段内存拷贝到一个这种结构或类型中，再访问这个结构或类型。这是因为如果这段内存的开始地址不是按照这种结构或类型对齐的，那么访问它时就很容易因为bus error而core dump。
5，堆栈溢出
不要使用大的局部变量（因为局部变量都分配在栈上），这样容易造成堆栈溢出，破坏系统的栈和堆结构，导致出现莫名其妙的错误。
## 0x04 core dump
## 调试方法
利用gdb、dmesg两种方式。
1、Dmesg
当出现core dump时，利用dmesg可以查看内核的信息。如下所示：
Error 7，下面解释一下这个参数：
这里首先说明一下执行阶段错误码error code由三个位组成，从高到底分别为bit2 bit1 bit0，所以它的取值范围是0~7。
bit2：值为1表示是用户态程序内存访问越界，值为0表示是内核态程序内存访问越界
bit1：值为1表示是写操作导致内存访问越界，值为0表示是读操作导致内存访问越界
bit0：值为1表示没有足够的权限访问非法地址的内容，值为0表示访问的非法地址根本没有对应的页面，也就是无效地址。
7直观翻译为“用户态写一个没有权限的非法地址”。
2、Gdb
gdb ./test -f core.xx
where、bt等常用的gdb操作。
## 0x05 举例
列举个人工作中所犯的一个错误，memcpy函数的使用。
```python
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
char *str = "pangyemeng love yaorong"; //(1) str 指向一个字符串常量的全局指针 
//char str[] = "pangyemeng love yaorong"; //(2) str 全局字符串数组
int main()
{
        //char str[] = "pangyemeng love yaorong"; //(3)str 局部 字符串数组
        //char *str = "pangyemeng love yaorong";  //(4)str 局部指针 指向一个字符串常量
        char sub_str[] = "520";
        /* find love to replace 520*/
        char *start = str;
        char *str_start = NULL;
        int offset = 0;
        //printf("str = %d, str_b = %d\n", sizeof(str), sizeof(str_b));
        if ((str_start = strstr(start, "love")) == NULL)
        {
                printf("Can't find love\n");
                return -1;
        }
        printf("%s\n", str_start);
        offset = str_start - str;
        //memcpy(str_start, sub_str, 3);
        memcpy(str + offset, sub_str, 3);
        printf("%s\n", str);
        return 0;
}
```
（1）（4）两种情况会存在core dump，原因是未分配空间。
示例2：
引用别人的：http://blog.csdn.net/lux_veritas/article/details/8227386

望大家多指正！





