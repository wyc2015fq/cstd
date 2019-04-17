# makefile 自动处理头文件的依赖关系 （zz） - DoubleLi - 博客园






现在我们的Makefile写成这样：

all: main

main: main.o stack.o maze.o
gcc $^ -o $@

main.o: main.h stack.h maze.h
stack.o: stack.h main.h
maze.o: maze.h main.h

clean:
-rm main *.o

.PHONY: clean
按照惯例，用all做缺省目标。现在还有一点比较麻烦，在写main.o、stack.o和maze.o这三个目标的规则时要查看源代码，找出它们依赖于哪些头文件，这很容易出错，一是因为有的头文件包含在另一个头文件中，在写规则时很容易遗漏，二是如果以后修改源代码改变了依赖关系，很可能忘记修改Makefile的规则。为了解决这个问题，可以用gcc的-M选项自动生成目标文件和源文件的依赖关系：

$ gcc -M main.c
main.o: main.c /usr/include/stdio.h /usr/include/features.h \
/usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
/usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
/usr/lib/gcc/i486-linux-gnu/4.3.2/include/stddef.h \
/usr/include/bits/types.h /usr/include/bits/typesizes.h \
/usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
/usr/lib/gcc/i486-linux-gnu/4.3.2/include/stdarg.h \
/usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h main.h \
stack.h maze.h
-M选项把stdio.h以及它所包含的系统头文件也找出来了，如果我们不需要输出系统头文件的依赖关系，可以用-MM选项：

$ gcc -MM *.c
main.o: main.c main.h stack.h maze.h
maze.o: maze.c maze.h main.h
stack.o: stack.c stack.h main.h
接下来的问题是怎么把这些规则包含到Makefile中，GNU make的官方手册建议这样写：



**[plain]**[view plain](http://blog.csdn.net/dragon101788/article/details/30580349)[copy](http://blog.csdn.net/dragon101788/article/details/30580349)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)



- all: main  
- 
- main: main.o stack.o maze.o  
- gcc $^ -o $@  
- 
- clean:  
- -rm main *.o  
- 
- .PHONY: clean  
- 
- sources = main.c stack.c maze.c  
- 
- include $(sources:.c=.d)  
- 
- %.d: %.c  
- set -e; rm -f $@; \  
- $(CC) -MM $(CPPFLAGS) $< > $@.


; \  
- sed 's,\.o[ :]*,\1.o $@ : ,g' < $@.


- rm -f $@.






sources变量包含我们要编译的所有.c文件，$(sources:.c=.d)是一个变量替换语法，把sources变量中每一项的.c替换成.d，所以include这一句相当于：

include main.d stack.d maze.d
类似于C语言的#include指示，这里的include表示包含三个文件main.d、stack.d和maze.d，这三个文件也应该符合Makefile的语法。如果现在你的工作目录是干净的，只有.c文件、.h文件和Makefile，运行make的结果是：

$ make
Makefile:13: main.d: No such file or directory
Makefile:13: stack.d: No such file or directory
Makefile:13: maze.d: No such file or directory
set -e; rm -f maze.d; \
cc -MM maze.c > maze.d.




> maze.d; \
rm -f maze.d.



; \
sed 's,\.o[ :]*,\1.o stack.d : ,g' < stack.d.




set -e; rm -f main.d; \
cc -MM main.c > main.d.



> main.d; \
rm -f main.d.$$
cc -c -o main.o main.c
cc -c -o stack.o stack.c
cc -c -o maze.o maze.c
gcc main.o stack.o maze.o -o main
一开始找不到.d文件，所以make会报警告。但是make会把include的文件名也当作目标来尝试更新，而这些目标适用模式规则%.d: %c，所以执行它的命令列表，比如生成maze.d的命令：



set -e; rm -f maze.d; \
cc -MM maze.c > maze.d.



> maze.d; \
rm -f maze.d.$$
注意，虽然在Makefile中这个命令写了四行，但其实是一条命令，make只创建一个Shell进程执行这条命令，这条命令分为5个子命令，用;号隔开，并且为了美观，用续行符\拆成四行来写。执行步骤为：



set -e命令设置当前Shell进程为这样的状态：如果它执行的任何一条命令的退出状态非零则立刻终止，不再执行后续命令。

把原来的maze.d删掉。

重新生成maze.c的依赖关系，保存成文件maze.d.1234（假设当前Shell进程的id是1234）。注意，在Makefile中$有特殊含义，如果要表示它的字面意思则需要写两个$，所以Makefile中的四个$传给Shell变成两个$，两个$在Shell中表示当前进程的id，一般用它给临时文件起名，以保证文件名唯一。

这个sed命令比较复杂，就不细讲了，主要作用是查找替换。maze.d.1234的内容应该是maze.o: maze.c maze.h main.h，经过sed处理之后存为maze.d，其内容是maze.o maze.d: maze.c maze.h main.h。

最后把临时文件maze.d.1234删掉。

不管是Makefile本身还是被它包含的文件，只要有一个文件在make过程中被更新了，make就会重新读取整个Makefile以及被它包含的所有文件，现在main.d、stack.d和maze.d都生成了，就可以正常包含进来了（假如这时还没有生成，make就要报错而不是报警告了），相当于在Makefile中添了三条规则：

main.o main.d: main.c main.h stack.h maze.h
maze.o maze.d: maze.c maze.h main.h
stack.o stack.d: stack.c stack.h main.h
如果我在main.c中加了一行#include "foo.h"，那么：

1、main.c的修改日期变了，根据规则main.o main.d: main.c main.h stack.h maze.h要重新生成main.o和main.d。生成main.o的规则有两条：

main.o: main.c main.h stack.h maze.h
%.o: %.c
# commands to execute (built-in):
$(COMPILE.c) $(OUTPUT_OPTION) $<
第一条是把规则main.o main.d: main.c main.h stack.h maze.h拆开写得到的，第二条是隐含规则，因此执行cc命令重新编译main.o。生成main.d的规则也有两条：

main.d: main.c main.h stack.h maze.h
%.d: %.c
set -e; rm -f $@; \
$(CC) -MM $(CPPFLAGS) $< > [$@.](mailto:$@.$$$$)



; \
sed 's,\.o[ :]*,\1.o $@ : ,g' < [$@.](mailto:$@.$$$$)> $@; \
rm -f [$@.](mailto:$@.$$$$)
因此main.d的内容被更新为main.o main.d: main.c main.h stack.h maze.h foo.h。



2、由于main.d被Makefile包含，main.d被更新又导致make重新读取整个Makefile，把新的main.d包含进来，于是新的依赖关系生效了。









