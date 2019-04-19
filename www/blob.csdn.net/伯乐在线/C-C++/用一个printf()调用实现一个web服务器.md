# 用一个printf()调用实现一个web服务器 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [欣仔](http://www.jobbole.com/members/randomli) 翻译。未经许可，禁止转载！
英文出处：[tinyhack](http://tinyhack.com/2014/03/12/implementing-a-web-server-in-a-single-printf-call/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
一个小伙伴转发了一个可能我们都知道的Jeff Dean的[笑话](http://blog.jobbole.com/51607/)。每次我读到这个列表的时候，这一部分就会跳出来：
 Jeff Dean有次用一句printf()实现了一个web服务器，而其他工程师添加了数千行注释但是仍然不能完全弄清楚它是如何工作的。而这个程序正是如今的Google Search首页。
使用一句printf调用来实现一个web服务器是很有可能的，但是我还没发现其他人做到。所以这次我读到这个列表时，我决定实现它。这里是它的代码，一个纯粹单一的printf调用，没有任何附加的变量或者宏（不用担心，我将会解释这段代码是如何工作的）。

C
```
#include <stdio.h>
    int main(int argc, char *argv[])
    {
     printf("%*c%hn%*c%hn"
      "\xeb\x3d\x48\x54\x54\x50\x2f\x31\x2e\x30\x20\x32"
     "\x30\x30\x0d\x0a\x43\x6f\x6e\x74\x65\x6e\x74\x2d"
      "\x74\x79\x70\x65\x3a\x74\x65\x78\x74\x2f\x68\x74"
     "\x6d\x6c\x0d\x0a\x0d\x0a\x3c\x68\x31\x3e\x48\x65"
      "\x6c\x6c\x6f\x20\x57\x6f\x72\x6c\x64\x21\x3c\x2f"
      "\x68\x31\x3e\x4c\x8d\x2d\xbc\xff\xff\xff\x48\x89"
     "\xe3\x48\x83\xeb\x10\x48\x31\xc0\x50\x66\xb8\x1f"
      "\x90\xc1\xe0\x10\xb0\x02\x50\x31\xd2\x31\xf6\xff"
     "\xc6\x89\xf7\xff\xc7\x31\xc0\xb0\x29\x0f\x05\x49"
     "\x89\xc2\x31\xd2\xb2\x10\x48\x89\xde\x89\xc7\x31"
      "\xc0\xb0\x31\x0f\x05\x31\xc0\xb0\x05\x89\xc6\x4c"
     "\x89\xd0\x89\xc7\x31\xc0\xb0\x32\x0f\x05\x31\xd2"
      "\x31\xf6\x4c\x89\xd0\x89\xc7\x31\xc0\xb0\x2b\x0f"
     "\x05\x49\x89\xc4\x48\x31\xd2\xb2\x3d\x4c\x89\xee"
     "\x4c\x89\xe7\x31\xc0\xff\xc0\x0f\x05\x31\xf6\xff"
     "\xc6\xff\xc6\x4c\x89\xe7\x31\xc0\xb0\x30\x0f\x05"
      "\x4c\x89\xe7\x31\xc0\xb0\x03\x0f\x05\xeb\xc3",
     ((((unsigned long int)0x4005c8 + 12) >> 16) & 0xffff), 
     0, 0x00000000006007D8 + 2, 
      (((unsigned long int)0x4005c8 + 12) & 0xffff)-
      ((((unsigned long int)0x4005c8 + 12) >> 16) & 0xffff), 
      0, 0x00000000006007D8 );
    }
```
这段代码只能在独有Linux AMD64位编译器（gcc版本是4.8.2（Debian 4.8.2-16））的系统上运行，编译命令如下：

Shell
```
gcc -g web1.c -O webserver
```
可能有些人会这样猜测：我用一个[特殊格式的字符串](https://en.wikipedia.org/wiki/Uncontrolled_format_string)来作弊。这段代码可能不能在你的机器上运行，因为我对两个地址使用了硬编码。
下面这个版本是更加用户友好化的（更容易改变），但是你仍旧要改变两个值：FUNCTION_ADDR和DESTADDR，稍后我会解释：

C
```
#include <stdio.h>
    #include <stdlib.h>
    #include <stdint.h>
    #define FUNCTION_ADDR ((uint64_t)0x4005c8 + 12)
    #define DESTADDR 0x00000000006007D8
    #define a (FUNCTION_ADDR & 0xffff)
    #define b ((FUNCTION_ADDR >> 16) & 0xffff)
    int main(int argc, char *argv[])
    {
       printf("%*c%hn%*c%hn"
        "\xeb\x3d\x48\x54\x54\x50\x2f\x31\x2e\x30\x20\x32"
        "\x30\x30\x0d\x0a\x43\x6f\x6e\x74\x65\x6e\x74\x2d"
        "\x74\x79\x70\x65\x3a\x74\x65\x78\x74\x2f\x68\x74"
        "\x6d\x6c\x0d\x0a\x0d\x0a\x3c\x68\x31\x3e\x48\x65"
        "\x6c\x6c\x6f\x20\x57\x6f\x72\x6c\x64\x21\x3c\x2f"
        "\x68\x31\x3e\x4c\x8d\x2d\xbc\xff\xff\xff\x48\x89"
        "\xe3\x48\x83\xeb\x10\x48\x31\xc0\x50\x66\xb8\x1f"
        "\x90\xc1\xe0\x10\xb0\x02\x50\x31\xd2\x31\xf6\xff"
        "\xc6\x89\xf7\xff\xc7\x31\xc0\xb0\x29\x0f\x05\x49"
        "\x89\xc2\x31\xd2\xb2\x10\x48\x89\xde\x89\xc7\x31"
        "\xc0\xb0\x31\x0f\x05\x31\xc0\xb0\x05\x89\xc6\x4c"
        "\x89\xd0\x89\xc7\x31\xc0\xb0\x32\x0f\x05\x31\xd2"
        "\x31\xf6\x4c\x89\xd0\x89\xc7\x31\xc0\xb0\x2b\x0f"
        "\x05\x49\x89\xc4\x48\x31\xd2\xb2\x3d\x4c\x89\xee"
        "\x4c\x89\xe7\x31\xc0\xff\xc0\x0f\x05\x31\xf6\xff"
        "\xc6\xff\xc6\x4c\x89\xe7\x31\xc0\xb0\x30\x0f\x05"
        "\x4c\x89\xe7\x31\xc0\xb0\x03\x0f\x05\xeb\xc3"
    , b, 0, DESTADDR + 2, a-b, 0, DESTADDR );
    }
```
我将解释这段代码如何通过一系列简短的C编码来工作。第一段代码将解释如何不使用函数调用，就能运行另一段代码。看看下面这段简单的代码：

C
```
#include <stdlib.h>
    #include <stdio.h>
    #define ADDR 0x00000000600720
    void hello()
    {
        printf("hello world\n");
    }
    int main(int argc, char *argv[])
    {
        (*((unsigned long int*)ADDR))= (unsigned long    int)hello;
     }
```
你可以编译它，但是它可能不能在你的系统上运行，你需要按如下步骤来做：
1.编译这段代码：


```
gcc run-finalizer.c -o run-finalizer
```
2.检查fini_array的地址


```
objdump -h -j .fini_array run-finalizer
```
然后从中找到VMA：


```
run-finalizer:     file format elf64-x86-64
    Sections:
    Idx Name          Size      VMA               LMA               File off  Algn
     18 .fini_array   00000008  00000000006007200000000000600720  00000720  2**3
          CONTENTS, ALLOC, LOAD, DATA
```
你需要一个最新版的GCC来编译才能发现它，旧版本的GCC使用不同的存储终结器原理。
3. 改变代码中ADDR的值为正确的地址。
4.重新编译代码
5.运行它
现在你就会看到你的屏幕上输出“hello world”，而它实际上是如何运行的呢？：
依据[Chapter 11 of Linux Standard Base Core Specification 3.1](http://refspecs.linuxbase.org/LSB_3.1.1/LSB-Core-generic/LSB-Core-generic/specialsections.html)（译注：Linux标准基础核心规范3.1第11章）
       .fini_array
这部分保存了一个函数指针数组，它贡献出一个终止数组给这个可执行的或可共享的、包含这个部分的对象。
为了让hello函数被调用而不是调用默认的处理函数，我们要重写这个数组。如果尝试编译这个web服务器代码，ADDR的值以同样的方式获取（使用objdump）。
好了，现在我们清楚了如何通过覆盖一个确定的地址来执行一个函数，还需要知道如何**使用printf****来覆盖一个地址**。可以找到很多关于利用格式化字符串漏洞的教程，但是我将给出一个简短的解释。
printf函数有这样一个特性，使用“%n”格式可以让我们知道有多少个字符输出。


```
#include <stdio.h>
    int main(){
        int count;
        printf("AB%n", &count);
        printf("\n%d characters printed\n", count);
    }
```
可以看到输出如下：


```
AB
    2 characters printed
```
当然我们用任何计数指针的地址来重写这个地址。但是为了用一个大数值来覆盖地址，需要输出大量的文本。幸运的是，有另外一个格式化字符串“%hn”作用于short而不是int。每次可以用2个字节排列成一个我们需要的4字节值来覆盖这个值。
试着用两个printf调用放置我们需要的a¡值（在这个例子中是指“hello”函数的指针）到fini_array：

C
```
#include <stdio.h>
    #include <stdlib.h>
    #include <stdint.h>
    #define FUNCTION_ADDR ((uint64_t)hello)
    #define DESTADDR 0x0000000000600948
    void hello()
    {
        printf("\n\n\n\nhello world\n\n");
    }
    int main(int argc, char *argv[])
     {
     short a= FUNCTION_ADDR & 0xffff;
     short b = (FUNCTION_ADDR >> 16) & 0xffff;
     printf("a = %04x b = %04x\n", a, b);fflush(stdout);
     uint64_t *p = (uint64_t*)DESTADDR;
     printf("before: %08lx\n", *p); fflush(stdout);
     printf("%*c%hn", b, 0, DESTADDR + 2 );fflush(stdout);
     printf("after1: %08lx\n", *p); fflush(stdout);
     printf("%*c%hn", a, 0, DESTADDR);fflush(stdout);
     printf("after2: %08lx\n", *p); fflush(stdout);
     return 0;
    }
```
导入的行是：


```
short a= FUNCTION_ADDR & 0xffff;
    short b = (FUNCTION_ADDR >> 16) & 0xffff;
    printf("%*c%hn", b, 0, DESTADDR + 2 );
    printf("%*c%hn", a, 0, DESTADDR);
```
a和b都只是函数地址的一半，可以构造一个a和b长度的字符串传入printf，但是我选择使用“%*”这个格式，它可以通过参数来控制输出的长度。
例如这段代码：


```
printf("%*c", 10, 'A');
```
将会在A后面输出9个空格，所以一共输出10字符。
如果只想用一个printf，就需要考虑到b字节已经被打印，而我们又需要打印另一个b-a字节（这个计数器是累加的）。


```
printf("%*c%hn%*c%hn", b, 0, DESTADDR + 2, b-a, 0, DESTADDR );
```
目前我们是调用这个“hello”函数，但是其实我们是可以调用任何函数的（或者任何地址）。我写过一个就像web服务器的shellcode（译注：填充数据），但是它只是输出“Hello world”。以下是我写的填充数据：


```
unsigned char hello[] = 
        "\xeb\x3d\x48\x54\x54\x50\x2f\x31\x2e\x30\x20\x32"
        "\x30\x30\x0d\x0a\x43\x6f\x6e\x74\x65\x6e\x74\x2d"
        "\x74\x79\x70\x65\x3a\x74\x65\x78\x74\x2f\x68\x74"
        "\x6d\x6c\x0d\x0a\x0d\x0a\x3c\x68\x31\x3e\x48\x65"
        "\x6c\x6c\x6f\x20\x57\x6f\x72\x6c\x64\x21\x3c\x2f"
        "\x68\x31\x3e\x4c\x8d\x2d\xbc\xff\xff\xff\x48\x89"
        "\xe3\x48\x83\xeb\x10\x48\x31\xc0\x50\x66\xb8\x1f"
        "\x90\xc1\xe0\x10\xb0\x02\x50\x31\xd2\x31\xf6\xff"
        "\xc6\x89\xf7\xff\xc7\x31\xc0\xb0\x29\x0f\x05\x49"
        "\x89\xc2\x31\xd2\xb2\x10\x48\x89\xde\x89\xc7\x31"
        "\xc0\xb0\x31\x0f\x05\x31\xc0\xb0\x05\x89\xc6\x4c"
        "\x89\xd0\x89\xc7\x31\xc0\xb0\x32\x0f\x05\x31\xd2"
        "\x31\xf6\x4c\x89\xd0\x89\xc7\x31\xc0\xb0\x2b\x0f"
        "\x05\x49\x89\xc4\x48\x31\xd2\xb2\x3d\x4c\x89\xee"
        "\x4c\x89\xe7\x31\xc0\xff\xc0\x0f\x05\x31\xf6\xff"
        "\xc6\xff\xc6\x4c\x89\xe7\x31\xc0\xb0\x30\x0f\x05"
        "\x4c\x89\xe7\x31\xc0\xb0\x03\x0f\x05\xeb\xc3";
```
如果移除hello函数然后插入这个填充数据，这段代码将会被调用。
这段代码其实就是一个字符串，所以可以给它添加“%*c%hn%*c%hn”格式化字符串。这个字符串还未命名，所以需要在编译后找到它的地址，而为了获得这个地址，我们需要编译这段代码，然后反汇编它：


```
objdump -d webserver
```


C
```
00000000004004fd <main>:
     4004fd:   55                      push   %rbp
     4004fe:   48 89 e5                mov    %rsp,%rbp
     400501:   48 83 ec 20             sub    $0x20,%rsp
     400505:   89 7d fc                mov     %edi,-0x4(%rbp)
     400508:   48 89 75 f0             mov    %rsi,-0x10(%rbp)
     40050c:   c7 04 24 d8 07 60 00    movl   $0x6007d8,(%rsp)
     400513:   41 b9 00 00 00 00       mov    $0x0,%r9d
     400519:   41 b8 94 05 00 00       mov    $0x594,%r8d
     40051f:   b9 da 07 60 00          mov    $0x6007da,%ecx
     400524:   ba 00 00 00 00          mov    $0x0,%edx
     400529:   be 40 00 00 00          mov    $0x40,%esi
     40052e:   bf c8 05 40 00          mov    $0x4005c8,%edi
     400533:   b8 00 00 00 00          mov    $0x0,%eax
     400538:   e8 a3 fe ff ff          callq  4003e0     <printf@plt>
     40053d:   c9                      leaveq 
     40053e:   c3                      retq   
     40053f:   90                      nop
```
其实只需要关心这行：


```
mov    $0x4005c8,%edi
```
这就是我们需要的地址：


```
#define FUNCTION_ADDR ((uint64_t)0x4005c8 + 12)
```
+12是非常必要的，因为我们的填充数据是从12个字符长度的“%*c%hn%*c%hn”字符串后面开始的。
如果你的对填充数据很好奇，其实它是由以下的C代码创建的：

C
```
#include<stdio.h>
    #include<string.h>
    #include<stdlib.h>
    #include<unistd.h>
    #include<sys/types.h>
    #include<sys/stat.h>
    #include<sys/socket.h>
    #include<arpa/inet.h>
    #include<netdb.h>
    #include<signal.h>
    #include<fcntl.h>
    int main(int argc, char *argv[])
    {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    bzero((char *)&serv_addr, sizeof(serv_addr));      
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(8080);
    bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    listen(sockfd, 5);
    while (1) {
        int cfd  = accept(sockfd, 0, 0);
        char *s = "HTTP/1.0 200\r\nContent-type:text/html\r\n\r\n<h1>Hello world!</h1>"; 
        if (fork()==0) {
            write(cfd, s, strlen(s));
            shutdown(cfd, SHUT_RDWR);
            close(cfd);
        }   
    }
    return 0;
    }
```
我做了额外的工作（即使在这个例子中并不是十分必要的）来移除这个填充数据中的所有NUL字符（因为我没有从X86-64上的[Shellcodes](http://repo.shell-storm.org/shellcode/)[数据库](http://repo.shell-storm.org/shellcode/)中找到一个NUL字符）。
Jeff Dean曾经使用一个printf()调用实现了一个web服务器。其他的工程师添加了数千行的注释，但是仍然没有弄清楚它是如何工作的。**而这个程序正是如今的****Google Search****首页**。
这给读者留下了一道练习题，如果要评测web服务器，可以处理Google search的负载。
这部分的代码可以从[这里](https://github.com/yohanes/printf-webserver)获得。
对于认为这样做是无用的人：它确实是没有用的。我只是碰巧喜欢这种挑战，而它为以下主题更新了我的记忆和知识：编写填充代码（已经很多年没有写过了），AMD64装配（调用惯例，寄存器保护等等），系统调用，objdump，fini_array（最近一次我检测的时候，GCC依然使用.dtors），printf格式化利用，gdb技巧（例如将内存块写入文件），还有低阶的socket编程（过去几年中我使用过boost）。
**更新**：Ubuntu[增加了一个安全特性](https://wiki.ubuntu.com/ToolChain/CompilerFlags#A-Wl.2C-z.2Crelro)，这个特性提供了在最终的ELF表区域中只读重定位，为了能够在ubuntu中运行这个例子，在编译的时候添加以下命令行：


```
-Wl,-z,norelro
```
比如：


```
gcc -Wl,-z,norelro test.c
```
> 
**打赏支持我翻译更多好文章，谢谢！**
[打赏译者](#rewardbox)
#### 打赏支持我翻译更多好文章，谢谢！
![](http://jbcdn2.b0.upaiyun.com/2016/08/647babf96cf39feeea99ca6771f37763.jpg)
