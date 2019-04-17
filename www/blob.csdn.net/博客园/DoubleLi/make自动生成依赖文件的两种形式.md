# make自动生成依赖文件的两种形式 - DoubleLi - 博客园






> 
最近编译源文件发现当修改头文件后，make并不会自动把包含此头文件的所有源文件重新编译，而每次都是需要把对应的中间文件清除才能重新编译，非常的麻烦。因此需要make自动对源文件所依赖的头文件进行管理，即make自动生成依赖文件。鉴于本人的刚开始写的博客，很多方面经验不足，比如如何介绍我所用到的知识等，现在只是对我在过程中遇到的问题进行记录，相关的知识可以查看gnu make中文文档，上网等等。

遇到的问题记录如下：1、make在生成依赖文件后并不正确：原因是生成的依赖文件中的目标文件（.d与.o）并不与所要的编译的源文件对应（一个在当前目录，一个在子目录下），对makefile的依赖文件的生成规则进行了查看，发现是使用了gcc -MM默认的生成方式，于是想到看有没有指定的依赖目标的生成规则。于是在网上搜索看到了这么一个帖子（http://bbs.chinaunix.net/thread-930718-1-1.html）：楼主的问题是：

|gcc -M生成依赖关系的时候，默认的形式是：xxx.o: xxx.c但是如果我想生成objs/xxx.o: xxx.c的形式，用什么参数呢？|
|----|


看到了四楼的回答发现可以使用gcc的-MT(或者-MQ)的参数来改变target.




看到了这里继续对-MT进行资料的搜索，看到这么一个博客（http://blog.sina.com.cn/s/blog_717794b70101gjca.html），里面就包含了利用gcc生成依赖文件的规则：


%.d : %.c
        $(CC) $(CCFLAGS) -MF"$@" -MG -MM -MP -MT"$@" -MT"$(<:.c=.o)" "$<".

> 其中-MF是输出依赖文件名，而-MT就是说在输出的依赖文件中的目标, 楼主的例子里为-MT"$@" -MT"$(<:.c=.o)"
就是说在输出的依赖文件中目标是 "$@" 和 "$(<:.c=.o)"。于是得出两种生成依赖文件的处理方式，以下便是我修改后的makefile文件：

> 


`SUBDIRS=$(shell ls -l |grep ^d | awk '{if($$9 == "uart") print $$9}')`

`SRC_FILE = $(wildcard avdecc/*.c) `#编译子目录下的源文件使用的方法

`DEPS = $(patsubst %.c, %.d, $(SRC_FILE))// 替换成.d文件`

`OBJS = $(patsubst %.c, %.o, $(SRC_FILE))`

` `

`LIB=libavdecc-host.a`

`all:LIB`

` `

`LIB:$(OBJS)`

`	$(AR) rc $(LIB) $(OBJS)`

` `

`$(OBJS):%.o:%.c`

`	$(CC) -MM $(CFG_INC) -MT"$*.d" -MT"$(<:.c=.o)" $< > $*.d #生成对应的依赖文件，实际上是一个Makefile文件`

`	$(CC) -c $(CFG_INC) $(CFLAGS) $< -o $@`



`sinclude $(DEPS) #包含依赖文件到当前Makefile文件，这是个关键的步骤。因为make会把DEPS文件都包含进本makefile文件中，把它当做目标，并且检查其的生成规则。`

` `

`$(SUBDIRS):ECHO`

`	make -C $@ #递归编译子目录`

` `

`ECHO:`

`	@echo $(SUBDIRS)`

` `

`.PHONY:clean`

`clean:`

`	@$(RM) $(OBJS) $(LIB) $(DEPS) .*.sw?`

`	make -C $(SUBDIRS) clean`





`CUR_SOURCE = $(wildcard ./*.c) #编译当前的源文件，使用gnu make手册的依赖文件的生成方式`

`CUR_OBJS = $(patsubst %.c, %.o, $(CUR_SOURCE))`

`DEPS = $(patsubst %.c, %.d, $(CUR_SOURCE))`

` `

`BIN=avdecc`

` `

`all:$(CUR_OBJS) BIN`

` `

`$(CUR_OBJS):%.o:%.c`

`	$(CC) $(CFLAGS) $(CFG_INC) -c  -o $@ $<`

` `

`BIN:$(CUR_OBJS)`

`	$(CC) -o $(BIN) $(CUR_OBJS) $(CFLAGS) $(CFG_INC)`

` `

`sinclude $(CUR_OBJS:.o=.d) #包含依赖文件到当前makefile文件`

`%.d:%.c`

`	@set -e; rm -f $@; \`

`	$(CC) -MM $(CFG_INC) $< > $@.$$$$; \ #gcc-MM生成依赖的默认形式`

`	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \`

`	rm -f $@.$$$$`

` `

`.PHONY:clean`

`clean:`

`	@$(RM) $(CUR_OBJS) $(BIN) $(DEPS) .*.sw？`












