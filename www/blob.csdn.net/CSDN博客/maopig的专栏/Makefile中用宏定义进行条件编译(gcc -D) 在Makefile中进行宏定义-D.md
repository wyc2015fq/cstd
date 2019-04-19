# Makefile中用宏定义进行条件编译(gcc -D)/在Makefile中进行宏定义-D - maopig的专栏 - CSDN博客
2012年02月03日 09:49:40[maopig](https://me.csdn.net/maopig)阅读数：44713
在源代码里面如果这样是定义的：
#ifdef   MACRONAME
//可选代码
#endif
那在makefile里面
gcc   -D   MACRONAME=MACRODEF
或者
gcc   -D   MACRONAME 
这样就定义了预处理宏，编译的时候可选代码就会被编译进去了。
对于GCC编译器，有如下选项：
        -D macro=string，等价于在头文件中定义：#define   macro   string。例如：-D TRUE=true，等价于：#define   TRUE   true
        -D macro，等价于在头文件中定义：#define   macro   1，实际上也达到了定义：#define   macro的目的。例如：-D LINUX，等价于：#define   LINUX   1（与#define   LINUX作用类似）。
        --define-macro   macro=string与-D macro=string作用相同。
如：
TEST.C 文件
#include <stdio.h>
#include <stdlib.h>
main()
{
#ifdef p1
  printf("Hello p1");
#else
  printf("Hello p2");
#endif
}
1.
编译： gcc -o test test.c
运行： ./test
输出： Hello p2
2.
编译： gcc -o test test.c -D p1
运行： ./test
输出： Hello p1
### [在Makefile中进行宏定义-D](http://blog.csdn.net/chaoqunz/article/details/6033663)
关键词: Make宏定义 Make传递宏定义 Makefile中添加宏定义 Makefile -D
在Makefile中我们可以通过宏定义来控制源程序的编译。只要在Makefile中的CFLAGS中通过选项-D来指定你于定义的宏即可。
如:
CFLAGS += -D _YUQIANG
在编译的时候加上此选项就可以了： $(CC) $(CFLAGS) $^ -o $@ 
下面是我写的一个测试文件：
例如：
**Makefile文件内容为:**
CC = gcc
RM = rm
CFLAGS += -D _YUQIANG
TARGETS := myapp
all:$(TARGETS)
$(TARGETS):main.c
$(CC) $(CFLAGS) $^ -o $@
clean:
-$(RM) -f *.o
-$(RM) -f $(TARGETS)
**main.c文件的内容为：**
#include <stdio.h>
int main()
{
#ifdef _YUQIANG
printf("Hello Yu Qiang, How are you?/n");
#else
printf("Sorry to lost you. /n");
#endif
return 0;
}
在端口中输入    make clean all
然后输入           ./myapp
结果                  Hello Yu Qiang, How are you?
