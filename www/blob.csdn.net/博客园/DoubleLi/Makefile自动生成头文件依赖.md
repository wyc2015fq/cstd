# Makefile自动生成头文件依赖 - DoubleLi - 博客园






## 前言

Makefile自动生成头文件依赖是很常用的功能，本文的目的是想尽量详细说明其中的原理和过程。

## Makefile模板

首先给出一个本人在小项目中常用的Makefile模板，支持自动生成头文件依赖。

```
CC      = gcc 
CFLAGS  = -Wall -O
INCLUDEFLAGS = 
LDFLAGS = 
OBJS    = seq.o
TARGETS = test_seq 

.PHONY:all 
all : $(TARGETS)

test_seq:test_seq.o $(OBJS)
    $(CC) -o $@ $^ $(LDFLAGS)

%.o:%.c
    $(CC) -o $@ -c $< $(CFLAGS) $(INCLUDEFLAGS)

%.d:%.c
    @set -e; rm -f $@; $(CC) -MM $< $(INCLUDEFLAGS) > $@.$$$$; \
    sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
    rm -f $@.$$$$

-include $(OBJS:.o=.d)

.PHONY:clean 
clean:
    rm -f $(TARGETS) *.o *.d *.d.*
```

## 基础知识

在进行下一步之前，首先需要了解make的执行步骤：
- 读入Makefile
- 读入被include的其它Makefile
- 初始化Makefile中的变量
- 推导隐晦规则，并分析所有规则
- 为所有目标创建依赖关系链
- 根据依赖关系，决定哪些目标需要重新生成
- 执行生成命令

## 如何动态生成依赖关系？

从上面make的执行过程中可看出，要动态生成依赖关系，只能利用第2步读入其它Makefile的机制。那么，我们是否可以先把生成的依赖关系保存到文件，然后再把该文件的内容包含进来？
答案是Yes! 只要利用include的机制。

include关键字是用于读入其它Makefile文件。当该文件不存在时，make会寻找是否有生成它的规则，如果有，则执行其生成命令，然后再尝试读入。在include前加减号"-"可以上make忽略其产生的错误，并不输出任何错误信息。

即是说，我们需要提供生成规则文件的规则。例如，我们可以这样动态生成头文件依赖关系：

```
seq.d : seq.c
    @echo “seq.o seq.d : seq.c seq.h" > $@

-include seq.d
```

当make执行时，Makefile中的内容将是这样子（指内存上的数据）：

```
seq.d : seq.c
    @echo “seq.o seq.d : seq.c seq.h" > $@

seq.o seq.d : seq.c seq.h
```

特别注意的是，由于对seq.c和seq.h的修改需要更新seq.d的内容（因为依赖关系可能已变化），因此seq.d也要在依赖关系的目标列表中。

## 自动生成头文件依赖

基于上面的例子，现在可以开始讨论如何自动生成头文件依赖。

### 自动生成依赖关系

大多数c/c++编译器提供了-M选项，可自动寻找源文件依赖的头文件，并生成依赖规则。对于gcc，需要使用-MM选项，否则它会把系统依赖的头文件也包含进来。例如执行下面一个命令：

```
gcc -MM seq.c
```

将输出：

```
seq.o : seq.c seq.h
```

但我们需要结果是seq.d也要包含在目标列表中，所以还需要对它进行文本处理。因此，上面的例子可改为：

```
seq.d : seq.c
    @set -e; \
    gcc -MM $< > $@.$$$$; \
    sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
    rm -f $@.$$$$

-include seq.d
```

### 生成规则中的执行命令解释

第一个命令`@set -e`。@关键字告诉make不输出该行命令；set -e的作用是，当后面的命令的返回值非0时，立即退出。

那么为什么要把几个命令写在”同一行“（是对于make来说，因为\的作用就是连接行），并用分号隔开每个命令？因为在Makefile这样做才能使上一个命令作用于下一个命令。这里是想要set -e作用于后面的命令。

第二个命令`gcc -MM $< > $@.$$$$`, 作用是根据源文件生成依赖关系，并保存到临时文件中。内建变量`$<`的值为第一个依赖文件（那seq.c)，`$$$$`为字符串`"$$"`，由于makefile中所有的$字符都是特殊字符（**即使在单引号之中！**），要得到普通字符$，需要用`$$`来转义; 而`$$`是shell的特殊变量，它的值为当前进程号；使用进程号为后缀的名称创建临时文件，是shell编程常用做法，这样可保证文件唯一性。

第三个命令作用是将目标文件加入依赖关系的目录列表中，并保存到目标文件。关于正则表达式部分就不说了，唯一要注意的是内建变量`$*`，`$*`的值为第一个依赖文件去掉后缀的名称（这里即是seq)。

第四个命令是将该临时文件删除。

如果把内建变量都替换成其值后，实际内容是这样子：

```
seq.d : seq.c
    @set -e; \
    gcc -MM seq.c > seq.d.$$$$; \
    sed 's,\(seq\)\.o[ :]*,\1.o seq.d : ,g' < seq.d.$$$$ > seq.d; \
    rm -f seq.d.$$$$

-include seq.d
```

### Makefile的模式匹配

最后，再把Makefile的模式匹配应用上，就完成自动生成头文件依赖功能了：

```
%.d : %.c
    @set -e; \
    gcc -MM $@ > $@.$$$$; \
    sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
    rm -f $@.$$$$

-include seq.d
```

## 参考资料

<跟我一起写Makefile> by 陈晧
GNU make官方文档 [http://www.gnu.org/software/make/manual/make.html](http://www.gnu.org/software/make/manual/make.html)









