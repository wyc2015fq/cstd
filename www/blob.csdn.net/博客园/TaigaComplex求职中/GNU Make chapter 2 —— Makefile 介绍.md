# GNU Make chapter 2 —— Makefile 介绍 - TaigaComplex求职中 - 博客园







# [GNU Make chapter 2 —— Makefile 介绍](https://www.cnblogs.com/TaigaCon/p/3979930.html)





Makefile是由一系列的rule规则组成，这些rule都遵循以下形式:

target ... : prerequisites ...

    command

    ...

    ...



# target（目标）

一般来说是需要生成的程序（模块）的名字，也可以是要执行的动作的名字，这种情况叫做（Phony Target）



# prerequisites（依赖）

用于生成target，一个target一般会依赖于多个文件



# command（命令）

由make来执行的动作，一个规则下可以包含多个command，需要注意的一点是command前必须有一个tab用于区分command所在的行与Makefile的其他行



# rule

表明了如何（command）以及何时（prerequisites）生成指定文件（target）。一旦prerequisites新加入到工程或者更新后，则会调用command来生成target。

rule的形式是以prerequisites为原料并通过调用command来生成target文件。不过并不是所有的target都是文件，在这种情况下就不需要prerequisites了（如make clean）。



Makefile可能会包含除了rule以外的其他文本，不过再复杂的Makefile，rule都是其核心部分。



以下有个简单的Makefile，我们可以对他展开分析：



edit : main.o kdb.o command.o display.o \
        insert.o search.o files.o utils.o
    cc -o edit main.o kbd.o command.o display.o \
                  insert.o search.o files.o utils.o

main.o : main.c defs.h
    cc -c main.c
kbd.o : kbd.c defs.h command.h
    cc -c kbd.c
command.o : command.c defs.h command.h
    cc -c command.c
display.o : display.c defs.h buffer.h
    cc -c display.c
insert.o : insert.c defs.h buffer.h
    cc -c insert.c
search.o : search.c defs.h buffer.h
    cc -c search.c
files.o : files.c defs.h buffer.h command.h
    cc -c files.c
utils.o : utils.c defs.h
    cc -c utiles.c

clean :
    rm edit main.o kbd.o command.o display.o \
                insert.o search.o files.o utils.o







在该Makefile中，
- target                 有edit，main.o等各种“.o”文件，最后还有clean
- prerequisites       有main.c等各种“.c”文件，defs.h等各种“.h”文件，以及main.o等各种“.o”文件
- command            有cc -o ..., cc -c ...等命令



当prerequisites文件被改变的时候，会重新调用command来生成新的target。command表明该如何由prerequisites生成新的target文件。但是实际上make并不知道command所做的是什么动作，因为command都是shell命令，最终会由shell调用，因此理论上command可以是任何的shell命令。

 在Makefile中，clean不是一个文件，而仅仅是一个动作的名字。由于clean不是其他target的prerequisites，因此除非你手动指定make来调用clean(make clean)，否则它不会被调用。另外，由于clean也没有任何的prerequisites，因此调用clean只会执行到其下方的command，这种只为执行某种动作的target叫做phony target。



# make如何处理Makefile

对于上方的那个简单Makefile，当我们敲make的时候，make会首先确定Makefile的goal（目标），默认goal就是该Makefile文件中第一个不包含‘.’前缀的target，即edit。

 但是make不会立即去执行edit下的command，因为它的rule规定了edit依赖于它的.o，而.o文件的rule又规定了它们依赖于.h，.c文件，因此会先由.c文件先生存.o文件，后再由.o文件生成edit。

clean这种不在edit分支内的target则需要手动执行（make clean）。

command只在特定情况下才会被执行：
- target文件不存在
- prerequisites文件时间比target文件新



# 使Makefile更简洁

#### 1.使用变量

上方的Makefile在edit的prerequisites处列出来一串.o文件，并且需要在command中再次列出来。这种长列表的重复会带来维护艰难的问题。因此可以用变量来取代长列表。变量允许一次定义，多次调用。



对于上方的.o长串，可以写成如下方式：

objects = main.o kbd.o command.o display.o \

              insert.o search.o files.o utils.o



在需要调用的时候写成“$(objects)”这种形式

上方Makefile可以简化成



objects = main.o kbd.o command.o display.o \
        insert.o search.o files.o utils.o

edit : $(objects)
    cc -o edit $(objects)

main.o : main.c defs.h
    cc -c main.c
kbd.o : kbd.c defs.h command.h
    cc -c kbd.c
command.o : command.c defs.h command.h
    cc -c command.c
display.o : display.c defs.h buffer.h
    cc -c display.c
insert.o : insert.c defs.h buffer.h
    cc -c insert.c
search.o : search.c defs.h buffer.h
    cc -c search.c
files.o : files.c defs.h buffer.h command.h
    cc -c files.c
utils.o : utils.c defs.h
    cc -c utils.c
clean :
    rm edit $(objects)

 







#### 2.让make推导command

make有一条隐性规则：由.c文件生成相应文件名的.o文件所用的command “cc -c”可以省略，因为make可以自动识别并调用该command。

.c文件也可以省略，因为make会自动加载.o相应的.c文件到prerequisites。

因此，Makefile还可以简化成以下形式：

objects = main.o kbd.o command.o display.o \
        insert.o search.o files.o utils.o
 
edit : $(objects)
    cc -o edit $(objects)

main.o : defs.h
kbd.o : defs.h command.h
command.o : defs.h command.h
display.o : defs.h command.h
insert.o : defs.h buffer.h
search.o : defs.h buffer.h
files.o : defs.h buffer.h command.h
utils.o : defs.h

.PHOMY : clean
clean :
    rm edit $(objects)

 



#### 3.multi target one prerequisites

objects = main.o kbd.o command.o display.o \
        insert.o search.o files.o utils.o

edit : $(objects)
    cc -o edit $(objects)

$(objects) : defs.h
kbd.o command.o files.o : command.h
display.o insert.o search.o files.o : buffer.h



但是这种方式相对于上面的one target multi prerequisite 的方式没那么直观。可以根据自己喜好选择。



# clean 的方式

clean有两种写法：

1.

clean :
       rm edit $(objects)

一般我们都采用这种方式，但是这会引起一个问题，如果存在一个clean文件的话，那么make clean会被混淆，以实际存在的clean文件为target来执行，而不会去rm。所以下面这种方式就是避免了这种问题而存在的。



2.

.PHONY : clean

   clean :
       rm edit $(objects)

.PHONY声明了clean是一个虚拟的目标，并不包含实际文件，当执行make clean时就会把clean当做虚拟目标来执行，不会影响实际存在的文件。



另外需要注意的是，不要把clean写在Makefile的开头，因为我们一般不会把clean当做Makefile的默认goal












