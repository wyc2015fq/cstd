# 编写高质量的Makefile - maopig的专栏 - CSDN博客
2011年09月22日 20:09:39[maopig](https://me.csdn.net/maopig)阅读数：1154
### [](http://blog.csdn.net/tenfyguo/article/details/5879192)
分类： [c++/c研究](http://blog.csdn.net/tenfyguo/article/category/500636)[GNU&LINUX](http://blog.csdn.net/tenfyguo/article/category/500635)2010-09-12 15:31163人阅读[评论](http://blog.csdn.net/tenfyguo/article/details/5879192#comments)(0)收藏[举报](http://blog.csdn.net/tenfyguo/article/details/5879192#report)
源地址 ：[http://acm.hrbeu.edu.cn/forums/index.php?showtopic=1827&st=0&gopid=8924&#entry8924](http://acm.hrbeu.edu.cn/forums/index.php?showtopic=1827&st=0&gopid=8924&#entry8924)
一、前言
回想自己的第一个Makefile，是这个样子的
CODE
hello:hello.c
     gcc hello.c -o hello
后来有所进步，陆续地写了一些大都是这个样子的Makefile:
CODE
foobar:foo.o bar.o
     gcc -o foo.o bar.o
foo.o:foo.c
     gcc -c foo.c
bar.o:foo.c
     gcc -c bar.c
.PHONY:clean
clean:
     rm -rf *.o foobar
看上去还行，用起来也不错，但是随着程序规模的扩大，每次添加一个新文件，都要手动修改Makefile，实在是不厌其烦。
后来阅读了一些开源程序的Makefile源代码，当然，不是automake生成的那种，有了一些心得，几番进化，一段时间后，感觉对GNU make算是有了些初步的了解，在此总结一下，也算是温故而知新了。而且我记性比较差 ，放在这里算是记录一下，免得以后忘记。同时也免得大家再去翻那些繁复的手册，浪费不必要的时间。 
下文中makefile操作的对象有三个文件： foo.c , bar.c 和bar.h，内容分别如下：
foo.c
CODE
#include "bar.h"
int main(){
     print("Hello, makefile!");
     return 0;
}
bar.c
CODE
#include <stdio.h>
int print(char * msg){
     printf("%s/n",msg);
     return 0;
}
bar.h
CODE
int print(char * msg);
OK，该交代的都交代了，进入正题。
二、我的makefile模板
把上个项目的makefile整理了一下，感觉结构比较清晰，可以作为模板供以后使用。
文件内容大体是这个样子的：
CODE
CC = gcc
CFLAGS = -Wall -O
INCLUDE = -I/usr/include/mysql                             #其实在这里用不着这几个选项
LFLAGS = -L/usr/lib/mysql -lmysqlclient -lpthread   #仅做示意之用
TARGET = foobar
SOUCE_FILES = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SOUCE_FILES))
%.o : %.c %.h
     $(CC) -c $(CFLAGS) $< -o $@
%.o : %.c
     $(CC) -c $(CFLAGS) $< -o $@
$(TARGET): $(OBJS)
     $(CC) $^ -o $@ $(INCLUDE) $(LFLAGS)
.PHONY:clean
clean:
     $(RM) $(TARGET) $(OBJS)
解释：
前几行都是变量的定义，至于为什么要定义这些变量，理由和编程中使用宏定义是一样的，那就是改一个就可以使很多地方同时生效，避免了重复的工作。
按照惯例：
CC变量指定了使用的编译器
CFLAGS变量包含了所需的编译选项
INCLUDE是寻找头文件的路径
LFLAGS是加载外部库时的指定选项。
TARGET变量代表最终要生成的可执行程序
下面的内容就是关键了，我们将利用一些GNU make内置的函数与推导规则来完成我们的目标。
首先的任务是自动获得当前目录下所有的源文件，好让我们新添文件后不必再修改Makefile。
完成这个功能的是这行代码
SOUCE_FILES = $(wildcard *.c)
wildcard 是GNU make程序预定义的一个函数，作用便是获取匹配模式文件名，原型为$(wildcard PATTERN)。它的详细说明可以看这里。简单来说wildcard函数的参数只有一个，就是函数名之后的文件名模式，这里的模式使用shell可识别 的通配符，包括“?”（单字符）、“*”（多字符）等。现在我们的需求是获取当前目录下的所有.c文件，模式自然是*.c。
按照最基本的依赖规则，生成TARGET文件依赖于一系列的.o文件，那么如何获得这些.o文件的列表呢？答案是使用patsubst模式替换函数函数:
$(patsubst %.c,%.o,$(SOUCE_FILES))
模 式替换函数patsubst函数原型为$(patsubst PATTERN,REPLACEMENT,TEXT)，相比wildcard，它要复杂一些，顾名思义，三个参数依次代表了匹配模式，替换规则，替换目标 字符串。在这里，我们需要把所有.c替换成.o,所以写成上面的样子就可以了。
现在c源文件列表和obj文件列表都有了，下一步就该为每个源文件编写规则了。
其实很多源文件的编译规则都是一样的，就像最开始那个Makefile中那样
CODE
foo.o:foo.c
     gcc -c foo.c
bar.o:foo.c
     gcc -c bar.c
仅仅是文件名不同而已，因此就给了我们提取模式的某种可能性。我在一个关于winsock的makefile中找到了答案：
CODE
.SUFFIXES: .c .o
.c.o:
         $(CC) -c $(CFLAGS) $< -o $@
这个规则利用了GNU make的后缀规则。
在这里，当定义了一个目标是“.c.o”的规则时。它的含义是所有“.o”文件的依赖文件是对应的“.c”文件。因此在这条规则下，foo.c将被自动编译成foo，bar.c被编译成bar。
而特殊目标.SUFFIXES这句的作用是： 在默认后缀的基础上，增加了可以作为后缀的关键字符串。
其实.c.o是肯定在默认识别的规则中的，不过为了保险起见，还是显式地声明一下比较好。
可以看到，这个规则十分的晦涩，反正我第一眼真是没看明白。因此，新版本的GNU make已经使用模式规则替代了后缀规则。
同样的功能，利用模式规则实现如下：
CODE
%.o : %.c
         $(CC) -c $(CFLAGS) $< -o $@
这样看起来便清晰多了。如果考虑到头文件，完美的写法应该是这样的：
CODE
%.o : %.c %.h
         $(CC) -c $(CFLAGS) $< -o $@
在上面的规则中，还使用了一些GNU make的自动化变量，他们的含义分别如下：
$@ --- 目标文件
$< --- 第一个依赖文件
$^ --- 所有的依赖文件
更多的自动化变量可以参见[这里](http://www.linuxsir.org/main/doc/gnumake/GNUmake_v3.80-zh_CN_html/make-10.html#_gnu_make_10.5.3)
最后的规则就是生成可执行文件了，很普通，不再赘述。
为了方便调试，可以在makefile中定义一些伪目标。（[](http://www.linuxsir.org/main/doc/gnumake/GNUmake_v3.80-zh_CN_html/make-04.html#_gnu_make_4.6)[伪目标的解释和意义可以看](http://www.linuxsir.org/main/doc/gnumake/GNUmake_v3.80-zh_CN_html/make-10.html#_gnu_make_10.5.3)这里）
一般调试用的makefile中都会有两个伪目标，一个clean，一个debug
对 于clean，手册里说：“make存在一个内嵌隐含变量“RM”，它被定义为：“RM = rm –f”。因此在书写“clean”规则的命令行时可以使用变量“$(RM)”来代替“rm”，这样可以免出现一些不必要的麻烦！”虽然不知道“必要的麻 烦”是什么，但是小心不为过，照着手册做比较好。
对于debug，和正常模式不同的就是添加了一些编译选项，修改CFLAGS的内容就可以了。但目前还没搞明白怎么动态地在makefile里修改变量的内容。这个问题以后再说。
三、在多文件夹情况使用makefile组织代码
上一段中给出的makefile，对于一般的小程序已经足矣，但是如果代码文件越来越多，最后不得不放到几个文件夹中，这时又该怎么办？
比如说我们准备把bar.c中的函数整理成了一个函数库libbar放在主程序文件夹中的子文件夹libbar中，这时该如何利用makefile来组织这些文件？
比较好的办法是在libbar文件夹中放置一个独立的子makefile，然后在主makefile里调用它。
libbar/Makefile:
CODE
CC = gcc
CFLAGS = -Wall -O
AR = ar
AFLAGS = -r
INCLUDE = -I/usr/include/mysql
LFLAGS = -L/usr/lib/mysql -lmysqlclient -lpthread
TARGET = libbar.a
SOUCE_FILES = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SOUCE_FILES))
%.o : %.c %.h
     $(CC) -c $(CFLAGS) $< -o $@
%.o : %.c
     $(CC) -c $(CFLAGS) $< -o $@
$(TARGET): $(OBJS)
     $(AR) $(AFLAGS) $(TARGET) $(OBJS)
.PHONY:clean
clean:
     $(RM) $(TARGET) $(OBJS)
主Makefile:
CODE
CC = gcc
CFLAGS = -Wall -O
INCLUDE = -I./libbar
LFLAGS = -L./libbar -lbar
SHELL = /bin/bash
SUBDIRS = libbar
TARGET = foobar
SOUCE_FILES = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SOUCE_FILES))
%.o : %.c %.h
     $(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@
$(TARGET): $(OBJS) libs
     $(CC) $(OBJS) -o $@ $(INCLUDE) $(LFLAGS)
libs:
     @ for subdir in $(SUBDIRS); do /
         (cd $$subdir && $(MAKE)); /
     done
.PHONY:clean
clean:
     $(RM) $(TARGET) $(OBJS)
     @ for subdir in $(SUBDIRS); do /
         (cd $$subdir && $(MAKE) clean); /
     done
在主makefile中使用了shell的for语句，循环取出SUBDIRS中的子文件夹名，然后进入子文件夹执行make，然后返回。如果在子makefile中出错，编译过程将终止。
四、编译多个目标
不知你有没有遇到过这样的情况，那就是需要从很多的代码，生成很多的可执行文件。
例如编写了一堆小工具，而每个工具只有一个源文件，用foo.c生成foo，用bar.c生成bar。
一个一个编译肯定不现实，这时该怎么做？让我们用GNU make来解决吧！
仔细阅读手册，发现GNU make中的静态模式，正好可以满足这个要求。
方便阅读，直接将手册中关于静态模式的解释粘贴如下：
QUOTE
静态模式规则是这样一个规则：规则存在多个目标，并且不同的目标可以根据目标文件的名字来自动构造出依赖文件。静态模式规则比多目标规则更通用，它不需要多个目标具有相同的依赖。但是静态模式规则中的依赖文件必须是相类似的而不是完全相同的。
静态模式规则的基本语法：
TARGETS ...: TARGET-PATTERN: PREREQ-PATTERNS ...
COMMANDS
...
“TAGETS”列出了此规则的一系列目标文件。像普通规则的目标一样可以包含通配符。
“TAGET -PATTERN”和“PREREQ-PATTERNS”说明了如何为每一个目标文件生成依赖文件。从目标模式（TAGET-PATTERN）的目标名字 中抽取一部分字符串（称为“茎”）。使用“茎”替代依赖模式（PREREQ-PATTERNS）中的相应部分来产生对应目标的依赖文件。
对应我们的需求，应该是用符合%.c模式的文件，生成文件名为%的可执行文件，同时利用自动化变量，构造规则如下：
CODE
$(TARGET_FILES): % : %.cpp
     g++ $(CFLAGS) $< -o $@
其中$(TARGET_FILES)为最终的可执行文件名，可以用wildcard配合patsubs函数获得。
因为$(TARGET_FILES)不止一个，所以直接写这个命令的结果是只会编译出一个可执行文件，即第目标文件列表中的一个文件，要想成功编译出所有的，还需要伪目标的帮忙。
完整的makefile如下：
CODE
CC = gcc
CFLAGS = -Wall -O
SOUCE_FILES=$(wildcard *.c)
TARGET_FILES=$(patsubst %.c,%,$(SOUCE_FILES))
.PHONY:all
all:$(TARGET_FILES)
$(TARGET_FILES): % : %.c
     g++ $(CFLAGS) $< -o $@
clean:
     $(RM) $(TARGET_FILES)

这里介绍两种变量的高级使用方法，第一种是变量值的替换。
我们可以替换变量中的共有的部分，其格式是“$(var:a=b)”或是“${var:a=b}”，其意思是，把变量“var”中所有以“a”字串“结尾”的“a”替换成“b”字串。这里的“结尾”意思是“空格”或是“结束符”。
还是看一个示例吧：
    foo := a.o b.o c.o
    bar := $(foo:.o=.c)
这个示例中，我们先定义了一个“$(foo)”变量，而第二行的意思是把“$(foo)”中所有以“.o”字串“结尾”全部替换成“.c”，所以我们的“$(bar)”的值就是“a.c b.c c.c”。
另外一种变量替换的技术是以“静态模式”（参见前面章节）定义的，如：
    foo := a.o b.o c.o
    bar := $(foo:%.o=%.c)
这依赖于被替换字串中的有相同的模式，模式中必须包含一个“%”字符，这个例子同样让$(bar)变量的值为“a.c b.c c.c”。
