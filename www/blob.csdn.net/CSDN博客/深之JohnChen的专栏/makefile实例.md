# makefile实例 - 深之JohnChen的专栏 - CSDN博客

2018年03月16日 11:39:17[byxdaz](https://me.csdn.net/byxdaz)阅读数：284


Makefile的介绍

使用 GCC 的命令行进行程序编译在单个文件下是比较方便的，当工程中的文件逐渐增多，甚至变得十分庞大的时候，使用 GCC 命令编译就会变得力不从心。Linux 中的 make 工具提供了一种管理工程的功能，可以方便的进行程序的编译，对更新的文件进行重编译。

Makefile的基本格式为：

```
TARGET... : DEPENDEDS...
    COMMAND
    ...
    ...
```
- TARGET:规则所定义的目标，通常是最后生成的文件，也可以是一个“动作”，称之为“伪目标”。
- DEPENDEDS：执行此规则所必须的依赖条件。
- COMMAND：规则所执行的命令。命令可以是多个，每个命令占一行，以 Tab 开头。

使用make进行项目管理，需要编写Makefile。在编译时，make程序按照顺序从Makefile文件中读取指令，依次执行！

当需要编译工程时，直接在工程目录中执行`make`即可。如果想清除编译过程中生成的目标文件，执行`make clean`即可。

makefile两种执行方式：直接make与make -f 指定文件

如果在本地工程目录下有文件名为makefile的makefile文件，使用make执行编译，使用make clean执行清除目标文件。

如果在本地工程目录下有指定的makefile文件，比如：demo.makefile文件，使用make -f demo.makefile执行编译，使用make clean -f demo.makefile执行清除目标文件。

使用预定义变量的Makefile

在Makefile中还有一些变量是系统预定义的，用户可以直接使用。

Makefile中经常使用的变量及含义
|变量名|含 义|默 认 值|
|----|----|----|
|AR|生成静态库库文件的程序名称|ar|
|AS|汇编编译器的名称|as|
|CC|C语言编译器的名称|cc|
|CPP|C语言预编译器的名称|$(CC) -E|
|CXX|C++语言编译器的名称|g++|
|FC|FORTRAN语言编译器的名称|f77|
|RM|删除文件程序的名称|rm -f|
|ARFLAGS|生成静态库库文件程序的选项|无默认值|
|ASFLAGS|汇编语言编译器的编译选项|无默认值|
|CFLAGS|C语言编译器的编译选项|无默认值|
|CPPFLAGS|C语言预编译器的编译选项|无默认值|
|CXXFLAGS|C++语言编译器的编译选项|无默认值|
|FFLAGS|FORTRAN语言编译器的编译选项|无默认值|

因此，前面的Makefile文件可以改写成：

```
CFLAGS = -Iadd -Isub -O2
OBJS = add/add_int.o add/add_float.o \
       sub/sub_int.o sub/sub_float.o main.o
TARGET = cacu

$(TARGET):$(OBJS)
    $(CC) -o $(TARGET) $(OBJS) $(CFLAGS)

clean:
    -$(RM) $(TARGET) $(OBJS)
```

其中变量`$(CC) $(RM)`可以直接使用，默认值分别是`cc`和`rm -f`。另外`CFLAGS`等变量是调用编译器时的默认选项配置，在生成`main.o`时没有指定编译选项，make程序自动调用了文件中定义的`CFLAGS`选项来增加头文件的搜索路径。

使用自动变量的Makefile

还记得上面出现的 <和<和@ 吗？它们是Makefile中的自动变量，分别代表依赖项和目标项。下面是一些常见的自动变量及其含义：

Makefile 中常见的自动变量和含义
|变量|含义|
|----|----|
|*|表示目标文件的名称，不包含目标文件的扩展名|
|+|表示所有的依赖文件，这些依赖文件之间以空格分开，按照出现的先后为顺序，其中可能包含重复的依赖文件|
|<|表示依赖项中第一个依赖文件的名称|
|?|依赖项中，所有目标文件时间戳晚的依赖文件，依赖文件之间以空格分开|
|@|目标项中目标文件的名称|
|^|依赖项中，所有不重复的依赖文件，这些文件之间以空格分开|

由此，对上面的Makefile文件进行重写，代码如下：

```
CFLAGS = -Iadd -Isub -O2
OBJS = add/add_int.o add/add_float.o \
       sub/sub_int.o sub/sub_float.o main.o
TARGET = cacu

$(TARGET):$(OBJS)
    $(CC) $^ -o $@ $(CFLAGS)

$(OBJS):%o:%c
    $(CC) -c $< -o $@ $(CFLAGS)

clean:
    -$(RM) $(TARGET) $(OBJS)
```

**Makefile的实例**

实例

add.h

```cpp
#include <stdio.h>

int add(int a,int b);
```

add.cpp

```cpp
#include "add.h"

int add(int a,int b)
{
	int r = 0;
	r = a+b;
	return r;
}
```

main.cpp

```cpp
#include "add.h"

int main(int argc, char *argv[])
{
	printf("simple demo \n");
	printf("add(91,8)=%d\n",add(91,8));
	
	return 0;
}
```

main.makefile文件或makefile文件

```cpp
test:main.o add.o
	g++ main.o add.o -o test
	
main.o:main.cpp add.h
	g++ -c main.cpp -o main.o
add.o:add.cpp add.h
	g++ -c add.cpp -o add.o

.PHONY:clean
clean:
	rm -rf *.o test
```

文件名为makefile的makefile文件，使用make执行编译，使用make clean执行清除目标文件。

文件名为main.makefile文件，使用make -f main.makefile执行编译，使用make clean -f main.makefile执行清除目标文件。

简化后的makefile文件

```cpp
#这是简化后的makefile
CC=g++
OBJS=main.o add.o

test:$(OBJS)
	${CC} $^ -o $@
%.o:%.c
	${CC} -c $< -o $@
    
clean:
	rm -rf *.o test
```

执行makefile并编译，生成test文件。

最后执行文件，输入命令./test

结果输出：

simple demo

add(91,8)=99

makefile中的$@, $^, $< , $?, $%, $+, $*

$@  表示目标文件

$^  表示所有的依赖文件

$<  表示第一个依赖文件

$?  表示比目标还要新的依赖文件列表

$%    表示仅当目标是函数库文件中，表示规则中的目标成员名。例如，如果一个目标是“foo.a(bar.o)”，那么，“$%”就是“bar.o”，“$@”就是“foo.a”。如果目标不是函数库文件（Unix下是[.a]，Windows下是[.lib]），那么，其值为空。

$+ 这个变量很像“$^”，也是所有依赖目标的集合。只是它不去除重复的依赖目标。

$*    这个变量表示目标模式中“%”及其之前的部分。如果目标是“dir/a.foo.b”，并且目标的模式是“a.%.b”，那么，“$*”的值就是“dir/a.foo”。这个变量对于构造有关联的文件名是比较有较。如果目标中没有模式的定义，那么“$*”也就不能被推导出，但是，如果目标文件的后缀是make所识别的，那么“$*”就是除了后缀的那一部分。例如：如果目标是“foo.c”，因为“.c”是make所能识别的后缀名，所以，“$*”的值就是“foo”。这个特性是GNU make的，很有可能不兼容于其它版本的make，所以，你应该尽量避免使用“$*”，除非是在隐含规则或是静态模式中。如果目标中的后缀是make所不能识别的，那么“$*”就是空值。


查看目标的依赖关系

写 Makefile 的时候, 需要确定每个目标的依赖关系。GNU提供一个机制可以查看C代码文件依赖那些文件。

查看C文件的依赖关系

gcc -MM main.c

结果输出

main.o: main.cpp add.h

查看C++文件的依赖关系

g++ -MM main.cpp

结果输出

main.o: main.cpp add.h

