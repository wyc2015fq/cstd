# makefile自动生成依赖关系 - DoubleLi - 博客园






手工编写依赖关系不仅工作量大而且极易出现遗漏，更新也很难及时，修改源或头文件后makefile可能忘记修改。为了解决这个问题，可以用gcc的-M选项自动生成目标文件和源文件的依赖关系。-M选项会把包含的系统头文件以及其所包含的其他系统头文件也找出来了，如果我们不需要输出系统头文件的依赖关系时，可以用-MM选项。

下面我们以一个简单的例子来说明如何自动生成依赖关系：

exm/

     main.c

     s.c

     s.h

makefile文件内容如下：

all:a

src=$(wildcard *.c)

obj:=$(patsubst %.c,%.o,$(src))

ifneq($(MAKECMDGOALS),clean)

-include$(src:.c=.d)

endif

a:$(obj)

gcc$(obj)-o $@

%.d:%.c

set-e;rm -f $@; \

gcc-MM$(CPPFLAGS) $< > $@.



; \



sed's,



> $@; \



rm-f$@.





%.o:%.c

@echo'Buildingfile: $<'

@echo'Invoking:GCC C Compiler'

gcc-O0-g3 -Wall -c -o "$@" "$<"

@echo'Finishedbuilding: $<'

@echo''



其中wildcard作用就是将指定目录下.c文件全部找出，所以这里src=main.cs.c

patsubst作用是把$(src)中的.c全部换为.o，于是obj=main.os.o

include$(src:.c=.d)相当于includemain.ds.d

由于此时这两个文件并不存在，所以会出现下面提示：

makefile:6:main.d:没有那个文件或目录

makefile:6:s.d:没有那个文件或目录

如果不想要这个提示，可以将include替换为-include

尽管一开始找不到.d文件，所以make会报警告。但是make会把include的文件名也当作目标来尝试更新，而这些目标适用模式规则%.d:%c

`注意，虽然在`Makefile`中这个命令写了四行，但其实是一条命令，`make`只创建一个`Shell`进程执行这条命令，这条命令分为`5`个子命令，用`;`号隔开，并且为了美观，用续行符`\`拆成四行来写。执行步骤为：`

1)set-e`命令设置当前`Shell`进程为这样的状态：如果它执行的任何一条命令的退出状态非零则立刻终止，不再执行后续命令。`@`表示`makefile`执行这条命令时不显示出来`

2)把原来的.d`文件`删掉。

3)$<依赖的目标集(即*.c), -MM:表示生成文件依赖关系,$@:表示生成的目标文件(即*.d),$$:表示本身的ProcessID。注意，在Makefile中$有特殊含义，如果要表示它的字面意思则需要写两个$，所以Makefile中的四个$传给Shell变成两个$，两个$在Shell中表示当前进程的id，一般用它给临时文件起名，以保证文件名唯一。

4)这个sed命令比较复杂，就不细讲了，主要作用是查找替换，并加入.d的依赖关系。

5)最后把临时文件删掉。



`不管是`Makefile`本身还是被它包含的文件，只要有一个文件在`make`过程中被更新了，`make`就会重新读取整个`Makefile`以及被它包含的所有文件，现在`main.d`、`stack.d`和`maze.d`都生成了，就可以正常包含进来了，相当于在`Makefile`中添了下面规则：`

`main.omain.d : main.c s.h`

`s.os.d : s.c s.h`

`当源或头文件修改时，如果依赖关系发生变化，执行`makefile`时将更新具有依赖关系的`.d`文件，而`.d`文件的更新又促使`make`重新读取`makefile`文件，把新的`.d`文件包括进来，于是新的依赖关系被建立。`

除了上面方法外，还可使用GCC的-MMD-MP -MF -MT选项，如下，可起到同样目的：

all:a

src=$(wildcard *.c)

obj:=$(patsubst %.c,%.o,$(src))

ifneq($(MAKECMDGOALS),clean)

-include$(src:.c=.d)

endif

a:$(obj)

gcc$(obj)-o $@

%.o:%.c

@echo'Buildingfile: $<'

@echo'Invoking:GCC C Compiler'

gcc-O0-g3 -Wall -c -fmessage-length=0 -MMD -MP-MF"$(@:%.o=%.d)"-MT"$(@:%.o=%.d)" -o "$@""$<"

@echo'Finishedbuilding: $<'

@echo''











