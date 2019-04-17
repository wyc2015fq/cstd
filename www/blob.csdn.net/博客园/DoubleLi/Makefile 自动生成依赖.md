# Makefile 自动生成依赖 - DoubleLi - 博客园






虽然以前对Makefile有个基本概念，但是真正到自己去写一个哪怕是简单的Makefile时也会遇到不少的麻烦。
    现在我有如下文件 dList.h dList.c memory.c debug.c debug.h test.c aaron.h 其中包含关系如下：
    aaron.h-->dList.h debug.h
    dList.c-->aaron.h
    debug.c-->aaron.h
    test.c-->aaron.h
    memory.c-->aaron.h
    第一次写Makefile如下：
    OBJS := test.o debug.o memory.o dList.o
    MACRO = DEBUGALL
    CFLAGS+= -g  -D$(MACRO)
    CC = gcc
    main: $(OBJS)
           $(CC) $(OBJS)  -D$(MACRO) $(CFLAGS) -o main  
    clean:
        rm -f $(OBJS)
        rm -f main
    第一次看看似乎没什么问题啊。真正运行发现这个Makefile没有反应出.c文件对.h文件的依赖性，所以当.h文件发生变化时Makefile执行并不会发生变化。于是有了第二版：
    OBJS := test.o debug.o memory.o dList.o
    MACRO = DEBUGALL
    CFLAGS+= -g  -D$(MACRO)
    CC = gcc
    main: $(OBJS)
           $(CC) $(OBJS)  -D$(MACRO) $(CFLAGS) -o main  
    %o: aaron.h
    clean:
        rm -f $(OBJS)
        rm -f main
    这样当aaron.h发生变化时，所有.o都会被更新，从而main也会被更新。但是问题还是有，aaron是依赖于dList.h, debug.h的，如果这两个头文件发生变化在这个Makefile里仍然不能使main重新编译。继续改造：
    OBJS := test.o debug.o memory.o dList.o
    MACRO = DEBUGALL
    CFLAGS+= -g  -D$(MACRO)
    CC = gcc
    main: $(OBJS)
           $(CC) $(OBJS)  -D$(MACRO) $(CFLAGS) -o main  
    %o: aaron.h dList.h debug.h
    clean:
        rm -f $(OBJS)
        rm -f main
    这下看上去能满足我的要求。但是仔细一想就会发现如果我这个工程很大，靠手工去找到每个.c文件对应的所有头文件然后来完成这个Makefile似乎是不太可能而且也很难维护。于是想着Makefile应该有机制能自动生成依赖关系吧。
    于是找出宝典《GNU make中文手册》搜索了下果然有自动生成依赖相关内容。主要是利用gcc 的编译选项-M和-MM。不过让我看了半天也没有看明白其中的玄机。最后又重新翻看了改宝典众多章节总算理解了。先把代码贴出来：

    MACRO = DEBUGALL
    CFLAGS+= -g -w -D$(MACRO)
    SOURCES = $(wildcard *.c)
    OBJS := $(patsubst %.c, %.o,$(SOURCES))

    CC = gcc
    main: $(OBJS)
           @echo "source files:" $(SOURCES)
           @echo "object files:" $(OBJS)
           $(CC) $(OBJS)  -D$(MACRO) $(CFLAGS) -o main  
    sinclude $(SOURCES:.c=.d)
    %d: %c
          @echo "create depend"
          $(CC) -MM $(CFLAGS) $< > $@.$$$$; \
          sed 's,\($*\)\.o[ :]*,\1.o $@ ,g' < $@.$$$$ > $@; \
          $(RM) $@.$$$$

  clean:
          rm -rf $(OBJS)
          rm -f main
    下面一行一行来分析下这个Makefile.
    前面两行很简单就是定义编译变量和编译选项。
    SOURCES = $(wildcard *.c) 这句话意思是定义一个变量SOURCES，它的值包含当前目录下所有.c文件。 在我的例子里我把这个值打印出来了就是dList.c memory.c test.c debug.c
    $(wildcard PATTEN) 是Makefile内建的一个函数：
    函数名称：获取匹配模式文件名函数—wildcard
    函数功能：列出当前目录下所有符合模式“PATTERN”格式的文件名。
    返回值：空格分割的、存在当前目录下的所有符合模式“PATTERN”的文件名。
    函数说明：“PATTERN”使用shell可识别的通配符，包括“?”（单字符）、“*”（多字符）等


    OBJS := $(patsubst %.c, %.o,$(SOURCES)) 这一行是定义了一个变量OBJS，它的值是将变量SOURCES里的内容以空格分开，将所有.c文件替换成.o. 在我的例子里打印出来就是dList.o memory.o test.o debug.o。
    $(patsubst PATTEN, REPLACEMENT, TEXT）也是内建函数
    函数名称：模式替换函数—patsubst。
    函数功能：搜索“TEXT”中以空格分开的单词，将否符合模式“TATTERN”替换为“REPLACEMENT”


    sinclude $(SOURCES:.c=.d) 这一行是非常关键的，它在当前Makefile里去include另外的Makefile. 这里“另外”的Makefile是将SOURCES变量里所有.c替换成.d。 在我的例子里就是dList.d memory.d test.d debug.d. 意思就是执行到这里
    的时候先去依次执行dList.d memory.d test.d debug.d. 这里的.d文件就是包含了每个.c文件自动生成的对头文件的依赖关系。这个依赖关系将由下面的%d:%c来完成。

    %d: %c
    此规则的含义是：所有的.d文件依赖于同名的.c文件。
第一行；使用c编译器自自动生成依赖文件（$<）的头文件的依赖关系，并输出成为一个临时文件，“$$$$”表示当前进程号。如果$(CC)为GNU的c编译工具，产生的依赖关系的规则中，依赖头文件包括了所有的使用的系统头文件和用户定义的头文件。如果需要生成的依赖描述文件不包含系统头文件，可使用“-MM”代替“-M”。
第二行；使用sed处理第二行已产生的那个临时文件并生成此规则的目标文件。经过这一行后test.d里内容如下：test.o: test.c aaron.h dList.h debug.h 其他.d里以此类推。
第三行；删除临时文件。

    到这里基本的意义弄明白了，但是让我不解的是%d: %c这个依赖的规则怎么能被执行到的？按照我的理解Makefile在执行时首先检查终极目标main是否存在，如果不存在则建立（根据main的依赖规则），如果存在在需要查看
    main的依赖文件是否存在并且是最新的，这我的例子里就是要看test.o dList.o memory.o debug.o是否存在且最新。这样追下去是否没有%d: %c什么事啊， .d文件也应该不存在或者说是空的。尽管我们include了.d文件，但是没有依赖规则去执行它啊。后来仔细阅读了
    Makefile文件的重建才明白了。
    Makefile如果由其它文件重建（这里我的Makefile include了所有.d文件，.d也可以看成是一个Makefile），Makefile在读入所有其他makefile文件（.d）之后，首先将所读取的每个makefile（.d）作为一个目标，寻找更新它们的规则。同样
    如果此目标不存在则根据依赖规则重新创建。在例子里其实.d文件开始并不存在，所以当Makefile在include这些.d文件时首先看.d存在不，不存在就要去寻找.d的依赖文件和规则。这里就找到了%d: %c从而创建出真正的.d文件。其实这里的关键点就是对于
    include了理解，它是把include的文件首先当成一个目标，然后要去寻找其依赖文件和规则的，而不是我事先想象的简单的把其他文件的内容包含过来。
    到此，问题解决，基本达到预期。









