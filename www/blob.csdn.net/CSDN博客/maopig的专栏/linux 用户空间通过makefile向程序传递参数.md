# linux 用户空间通过makefile向程序传递参数 - maopig的专栏 - CSDN博客
2017年01月20日 19:52:57[maopig](https://me.csdn.net/maopig)阅读数：1294

一. 用户空间
     因为实际上进行预处理的只是Gcc工具，而make工具只是一个解决依赖关系的工具。所以问题就简化成如何通过make向gcc传递参数。
     通过简单的例子来说明：
hello.c
- #include <stdio.h>
- 
- void main(void) {
- #ifdef DEBUG
-      printf("you ask for debug!\n");
- #endif
-      printf("we must say goodbye\n");
-      return;
- }
Makefile:
- ifeq ($(DEBUG),y)
- CFLAGS := $(CFLAGS) -DDEBUG
- endif
- 
- hello: hello.c
- $(CC) $(CFLAGS) $< -o $@
执行过程：
- [ville@localhost test]$ ls
- hello.c Makefile
- [ville@localhost test]$ make
- cc hello.c -o hello
- [ville@localhost test]$ ./hello 
- we must say goodbye
- [ville@localhost test]$ rm hello
- [ville@localhost test]$ ls
- hello.c Makefile
- [ville@localhost test]$ make DEBUG:=y
- cc -DDEBUG hello.c -o hello
- [ville@localhost test]$ ./hello
- you ask for debug!
- we must say goodbye
- [ville@localhost test]$
通过以上的简单例子说明如何通过宏开关进行条件编译。
