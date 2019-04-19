# 一个通用Makefile的编写 - maopig的专栏 - CSDN博客
2011年07月06日 23:13:31[maopig](https://me.csdn.net/maopig)阅读数：952标签：[makefile																[wildcard																[sqlite																[gcc																[编译器																[工作](https://so.csdn.net/so/search/s.do?q=工作&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=gcc&t=blog)](https://so.csdn.net/so/search/s.do?q=sqlite&t=blog)](https://so.csdn.net/so/search/s.do?q=wildcard&t=blog)](https://so.csdn.net/so/search/s.do?q=makefile&t=blog)
                我们在[Linux](http://wiki.dzsc.com/info/4181.html)**Linux**
　　Linux是一套免费使用和自由传播的操作系统，它主要用于基于Intel系列CPU的计算机上。这个系统是由全世界各地的成千上万的程序员设计和实现的，其目的是建立不受任何商品化软件的版权制约的、全世界都能自由使用的Unix兼容产品。 
环境下开发程序，少不了要自己编写Makefile，一个稍微大一些的工程下面都会包含很多.c的源文件。如果我们用gcc去一个一个编译每一个源文件的话，效率会低很多，但是如果我们可以写一个Makefile，那么只需要执行一个make就OK了，这样大大提高了开发效率。但是Makefile的语法规则众多，而且缺乏参考资料，对于初学者来说，写起来还是有一定的难度，往往令很多人望而生畏。下面我们介绍一个比较通用而且简洁的Makefile，大家只要对它稍作修改就可以用在你们自己的工程里了。 
　　现在假设我们有一个工程叫my_project，工程源码目录下面有app1.c，app2.c，app3.c以及main.c这五个源文件。我们现在需要编译出app1.o,app2.o,app3.o以及main.o，然后再把这些.o文件链接成为一个ELF格式的可执行程序叫做my_app。我们先看一个最简单的Makefile如何编写：
　　my_app : main.o, app1.o, app2.o, app3.o, app4.o
　　gcc –o my_app main.o app1.o, app2.o, app3.o, app4.o
　　main.o : main.c
　　gcc –c main.c
　　app1.o : app1.c
　　gcc –c app1.c
　　app2.o : app2.c
　　gcc –c app2.c
　　app3.o : app3.c
　　gcc –c app3.c
　　clean :
　　rm main.o app1.o, app2.o, app3.o, app4.o
　　这是一个傻瓜式的Makefile，不灵活，而且不具备可复制性，想象一个如果我们的工程下面有50个源文件，那岂不是要一个一个写出来。我们的目标是写一个Makefile，只要稍作修改就可以在各个工程之间通用。
　　下面这个Makefile就可以满足这个要求：
　　SRCS = $(wildcard *.c)
　　BJS = $(SRCS:.c = .o)
　　CC = gcc
　　INCLUDES = -I/home/noah/build_sqlite3/include
　　LIBS = -L/home/noah/build_sqlite3/lib -lsqlite3
[CCFL](http://www.dzsc.com/product/searchfile/3694.html)**CCFL**
　　CCFL(Cold Cathode Fluorescent Lamps)
       AGS = -g -Wall -O0 
　　my_app : $(OBJS)
　　$(CC) $^ -o $@ $(INCLUDES) $(LIBS)
　　%.o : %.c
　　$(CC) -c $< $(CCFLAGS)
　　clean:
　　rm *.o
　　大家看这个Makefile和前一个比起来是不是简洁很多，当然理解起来不如上一个那么直观。实际上编写Makefile就是为了提高我们的工作效率，而不是增加我们的工作量。因此Makefile为我们提供了很多强大的功能，比如定义变量，使用通配符等等。只要合理利用，就可以达到事半功倍的效果。
　　下面我们一条一条分析这个Makefile：
　　SRCS = $(wildcard *.c)
　　这条语句定义了一个变量SRCS，它的值就是当前面目录下面所有的以.c结尾的源文件。
　　BJS = $(SRCS:.c = .o)
　　这里变量OBJS的值就是将SRCS里面所有.c文件编译出的.o目标文件
　　CC = gcc
　　变量CC代表我们要使用的编译器
　　INCLUDES = -I/home/noah/build_sqlite3/include
　　LIBS = -L/home/noah/build_sqlite3/lib -lsqlite3
　　这里指定除了编译器默认的头文件和库文件的路径之外需要额外引用的头文件路径以及库的路径。
　　CCFLAGS = -g -Wall -O0
　　CCFLAGS变量存放的是编译选项
　　my_app : $(OBJS)
　　$(CC) $^ -o $@ $(INCLUDES) $(LIBS)
　　my_app依赖于所有的.o文件，$^代表$(OBJS)，$@代表my_app
　　%.o : %.c
　　$(CC) -c $< $(CCFLAGS)
　　将所有的.c源代码编译成.o目标文件，这样写是不是很省事？
　　clean:
　　rm *.o
　　在执行make clean之后删除所有编译过程中生成的.o文件。
　　这个Makefile就具备灵活的通用性，我们只要对它稍作修改就可以用在自己的工程里面。当然Makefile还有很多强大的功能，需要我们进一步学习。
