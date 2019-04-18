# makefile文件的编写规则及实例 (转) - gauss的专栏 - CSDN博客
2015年01月25日 12:35:53[gauss](https://me.csdn.net/mathlmx)阅读数：491

1.一个简单的makefile例子
假设一个程序有两个文件file1.c,file2.c,每个文件都包含head.h,生成file可执行文件
file:file1.o file2.o                  附属行(文件的依存关系)
    gcc -o file1.o file2.o            命令行
file1.o:file1.c head.h
    gcc -c file1.c
file2.o:file2.c head.h
    gcc -c file2.c
从file最终的目标文件开始倒推,依次列出文件的依存关系,make在执行时:
(1)判断file可执行文件是否存在,若不存在,则执行命令行,向下寻找依存关系
(2)若file存在,则检查依靠文件,是否存在更新,若存在更新则执行命令行,若没有更新则给出提示:
    make:'file' is up to date.
2.makefile中的宏定义及内部变量
宏定义:
OBJS = file1.o file2.o
CC = gcc
CFLAGS = -wall -O -g
引用:
file:$(OBJS)
    $(CC) $(OBJS) -o file
file1.o:file1.c head.h
    $(CC) $(FLAGS) -c file1.c  
file2.o:file2.c head.h
    $(CC) $(FLAGS) -c file2.c
内部变量: 
$@:当前规则的目的文件名
$<:依靠列表中的第一个依靠文件
$^:整个依靠列表
file:$(OBJS)
    $(CC) $^ -o $@
file1.o:file1.c head.h
    $(CC) $(FLAGS) -c $< -o $@
file2.o:file2.c head.h
    $(CC) $(FLAGS) -c $< -o $@
"$(CC) $(FLAGS) -c $< -o $@"是隐含规则,可以不写,默认使用此规则
3.假象
假设一个项目要生成两个可执行文件file1和file2,这两个文件是相与独立的,则在makefile开始处:
all:file1 file2
make总是假设all要生成,去检查它的依赖文件
4.清除由make产生的文件
clean:
    rm *.o
    rm file
执行:
make clean
则会清除由make生成的*.o和file文件
如果有clean文件存在,则清除不会执行(因clean没有可依赖的文件,永远是最新的)
使用PHONY目标,避免同名文件相冲突,不会检查clean文件存在与否,都要执行清除操作
.PHONY : clean
clean:
    rm *.o
    rm file
5.makefile函数
搜索当前目录,生成由*.c结尾的文件列表,wildcard--函数名
SOURCE = $(wildcard *.c)
用%.o替换$(SOURCE)中的%.c文件
OBJS = $(patsubst %.c,%.O,$(SOURCE))
6.产生新规则
SOURCE = $(wildcard *.c)
depends:$(SOURCE)
    gcc -M $(SOURCE) > depends
(为每一个.c文件产生规则,c文件和相关头文件为依靠)
在makefile文件中:
include depends
7.一个有效的makefile文件
可以完成大部分我们所需要的依靠检查,不用做太多的修改就可用在大多数项目里
功能:搜索当前目录,寻找源码文件,放入SOURCE变量里,利用patsubst产生目标文件(*.o)
CC = gcc
CFLAGS = -Wall -O -g
SOURCE = $(wildcard *.c,*.cc)
OBJS = $(patsubst %.c,%.o,$(patsubst,%.cc,%.o,$(SOURCE)))
file:$(OBJS)
    $(CC) $^ -o $@
用默认规则产生目标文件(*.o)
1：编译可执行程序。2：编译lib库 3：编译so库
本博针对上面三种目的各自写出了makefile模版，希望对大家有所帮助。
一.编译可执行程序
当前目录下制定文件编译成可执行文件（连接外部库的话只需要更改INC和LIB即可）
CXX = g++
TARGET = bitmaploctest
C_FLAGS +=-g -Wall
LIB_FLAGS =-pthread
all: $(TARGET)
bitmaploctest: bitmaploctest.o bitmaploc.o file_lock.o
    $(CXX) -o $@ $^ $(LIB_FLAGS) $(LIB) $(C_FLAGS)
.cpp.o:
    $(CXX) -c -o $*.o $(INC) $(C_FLAGS) $*.cpp
.cc.o:
    $(CXX) -c -o $*.o $(INC) $(C_FLAGS) $*.cc
clean:
-rm -f *.o $(TARGET)
二.编译成lib库
当前目录下指定文件编译成lib库(一般lib库在编译的时候不会将使用的外部库编译进来，而是等编译成可执行程序时或者.so时)
INC_DIR= ./
SRC_DIR= ./
OBJ_DIR= ./
LIB_DIR= ./
H_DIR= ./
OBJ_EXT= .o
CXXSRC_EXT= .cpp
CSRC_EXT= .c
LIB_EXT= .a
H_EXT= .h
OBJECTS = $(OBJ_DIR)bitmaploc$(OBJ_EXT) \
          $(OBJ_DIR)file_lock$(OBJ_EXT)
LIB_TARGET = $(LIB_DIR)libbitmaploc$(LIB_EXT)
$(OBJ_DIR)%$(OBJ_EXT): $(SRC_DIR)%$(CXXSRC_EXT)
    @echo
    @echo “Compiling $<==> $@…”
    $(CXX) $(INC) $(C_FLAGS) -c $<-o $@
    $(OBJ_DIR)%$(OBJ_EXT): $(SRC_DIR)%$(CSRC_EXT)
    @echo
    @echo “Compiling $<==> $@…”
    $(CC) -I./ $(INC) $(C_FLAGS) -c $<-o $@
    all:$(LIB_TARGET)
    $(LIB_TARGET): $(OBJECTS)
    all: $(OBJECTS)
    @echo
    $(AR) rc $(LIB_TARGET) $(OBJECTS)
    @echo “ok”
clean:
    rm -f $(LIB_TARGET) $(OBJECTS)
三.编译成so库
当前目录下指定文件编译成so库（必须将所有引用的外部库都编译进来）
CC = gcc
CXX = g++
CFLAGS  =-Wall -pipe -DDEBUG -D_NEW_LIC -g -D_GNU_SOURCE -shared -D_REENTRANT
LIB     =-lconfig -ldl -lrt -L../../lib -lttc -g
INCLUDE =-I../spp_inc
OO    = service.o tinystr.o tinyxml.o tinyxmlerror.o tinyxmlparser.o uin_conf.o stat.o
TARGETS = ../../lib/libRanch.so
all: $(TARGETS)
    stat:tool_stat.cpp
    $(CXX) $(INCLUDE) tool_stat.cpp -o tool_stat stat.o tinystr.o tinyxml.o tinyxmlerror.o tinyxmlparser.o -g
    cp tool_stat ../../bin
    $(TARGETS): $(OO)
    $(CXX) $(CFLAGS) $(INCLUDE) $(OO) -o $@ $(LIBDIR) $(LIB)
.c.o:
    $(CC)  $(CFLAGS) -c $(INCLUDE) $<
    echo $@
.cpp.o:
    $(CXX) $(CFLAGS) -c $(INCLUDE) $<
    echo $@
%:%.c
    $(CC) $(CFLAGS) -o $@ $< $(OO) $(LDFLAGS)
    echo $@
clean:
    rm -f *.o
    rm -f $(TARGETS)
    rm -f tool_stat
CC=cc -g
PROC=proc
CFLAGS=-DPRECOMP -I$(ORACLE_HOME)/precomp/public \
        -I$(ORACLE_HOME)/xdk/include -I.
FLAGS= -D_ALL_SOURCE=1 -D_LINUX -g  -I../../incl -D_GNU_SOURCE -D_IS_EAB=1 -D__USE_GNU=1 -D__GCC_296 -I/usr/include/libxml2 -I../csrc -I../../csrc/ -DPOSIX=1 -DLINUX
LIBHOME=$(ORACLE_HOME)/lib
LLIBSQL= `cat $(LIBHOME)/sysliblist` \
         `cat $(LIBHOME)/ldflags` \
         -lclntsh
LIBS= -L$(LIBHOME) $(LLIBSQL) -lmylib
.SUFFIXES: .sqc .c .o
%.c:%.sqc
        $(PROC) $(PROCPLSFLAGS) iname=$^ hold_cursor=yes
%.o:%.c
        $(CC) -c $(FLAGS) $(LIBS) $^
test:test.o test1.o test1.o
        cc -o $@ $(FLAGS) $(LIBS) $^
        rm -rf $^
haha:test.o test1.o test1.o
        @echo "+="$+
        @echo "?="$?
        @echo "^="$^
        @echo "<="$<
        @echo "@="$@
        @echo "*="$*
        @echo "%="$%
2．说明
2．1一般makefile编写有3个步骤
    　　 1．宏定义。主要功能是定义一些宏变量已替代较长的编译支持信息。一般情况下针对编译所需要得.h头文件，.a/.so的库文件路径。比如例子中的CFLAGS是oracle预编译需要数据库支持的头文件路径。LIBHOME是oracle环境编译需要数据库库文件路径。
　　     2．源文件之间的相互依赖关系。列出需要产生目标文件编译依赖的文件。比如例子中的test目标，其产生时会检测（test.o test1.o test1.o）这些依赖文件的变化，如果依赖文件有变化会自动先编译依赖文件。
　　     3．可执行的命令.即针对目标关系，所作出的编译行为。比如test其检测完依赖文件后执行（cc -o $@ $(CFLAGS) $(LIBS) $^）编译链接产生目标执行文件test.
         4.宏使用时用(),{}来确认宏名称例如${LIB}加{}会查找LIB的宏内容，$LIB会查找L的宏内容。
   2.2 ：常用编译项说明
            1． -I:制定头文件搜索的路径
            2． -L:连接需要的库文件路径
            3.–l:连接需要的库文件(比如：libmylib.so写作 –lmylib)
   2.3: 自动化变量说明：
$+ :所有的依赖文件，以空格分开，并以出现的先后为序，可能包含重复的依赖文件。
$?:所有的依赖文件，以空格分开，这些依赖文件的修改日期比目标的创建日期晚
$^ :所有的依赖文件，以空格分开，不包含重复的依赖文件。
$< :第一个依赖文件的名称。 
$@ :目标的完整名称。
$* :不包含扩展名的目标文件名称。
$% :如果目标是归档成员，则该变量表示目标的归档成员名称。
具体对照使用以上makefile，执行make haha得到如下对照信息：
$ makehaha
test.otest1.o test1.o
test.otest1.o
test.otest1.o
test.o
haha
   2.4:后缀规则：
            .SUFFIXES:.sqc .c .o制定新的后缀规则。(%.c:%.sqc),(%.o:%.c)即规则行为。就是将所有.sqc转为.c,.c再转为.o。
           .c.o:等价于%.o:%.c
3．注意：
    分行符\后面不能再有其他任何内容。
    对齐行最好采用^I（tab键）
    注释符号#
    include包含编译文件
**1. 概述**
Makefile，what？？很多windows程序可能都没听说过
简单的说，Makefile是Unix/Linux环境下描述了整个工程的编译、连接等规则的文件，其主要包括三点： 
1) 工程中的哪些源文件需要编译以及如何编译 
2) 依赖库以及库所在的位置 
3) 想得到什么：可执行文件？静态库？动态库？ 
项目中，我们会有很多源文件、头文件、依赖库文件、配置文件等等，通过Makefile定义规则来制定编译顺序，编译规则，编译依赖，甚至更复杂的功能，将极大的方便我们的开发，其最大的好处就是”自动化编译“，通过‘make’就可以方便的进行整个项目的编译工作。
**2. 编译链接**
从源码到可执行文件，具体步骤： 
源码--->预处理--->编译--->汇编--->链接 
我们常常把预处理、编译和汇编三个阶段统称为编译阶段，在这个阶段，编译器会检查程序语法、函数与变量是否声明等。 
经过编译之后，unix/linux下，将得到.o文件（一般来说，每个源文件都能生成一个对应的.o文件），即object file（windows下即.obj文件），.o不能直接运行，我们需要将其合成可执行文件，这个过程就叫链接。在链接过程，链接器会在所有的.o文件中找寻函数的实现，如果找不到，则会报链接错误。
**3.Makefile规则**
**Makefile只有一个规则：**
target： prerequisites
command
target：即目标，它可以是可执行文件、可以是.o文件，也可以是一个标签，简单的说，它就是你要做的事情。
prerequisites：生成target所需要的条件，它可以是一个文件，也可以是另外一个target
command：具体执行的命令
解释如下：target这个目标依赖于prerequisites中的文件，其生成规则定义在command中。
更简单一点表达：如果prerequisites中任何一个文件的时间要比target文件更新的话，command所定义的命令就会执行。
eg：
=====makefile=====
#第一个规则
test： main.o hello.o
gcc main.o hello.o –o test
#第二个规则
main.o： main.c
gcc –c main.c
#第三个规则
hello.o： hello.c hello.h
gcc –c hello.c
#第四个规则
clean :
rm –rf *.o
rm –rf test
=====makefile=====
第一个规则:
test就是target， main.o和hello.o是prerequisites，'gcc main.o hello.o –o test’是command。
即：要生成test，就需要有main.o和hello.o，如果main.o或者hello.o文件的时间比test新（或者test文件不存在），则会执行命令’gcc main.o hello.o –o test’,
第二个规则：
target是main.o，main.c是prerequisites，’gcc –c main.c’是command
即：要生成test，就需要main.c，如果main.c文件的时间比main.o新，则会执行’gcc –c main.c’
第三个规则和第二个规则类似
第四个规则：
这里，target是clean，此处，clean并不是一个文件，而是一个动作的名字，它的执行，需要显示的在make命令后制定，例如，此处执行‘make clean’，将调用其后的command，即‘rm –rf *.o rm –rf test’,另外，此处没有prerequisites，即任何时候都执行command
我们来看下如果执行'make'会做什么：
1、make会在当前目录下找名字叫“Makefile”或“makefile”的文件。
2、如果找到，它会找文件中的第一个目标文件（target），在上面的例子中，他会找到“test”这个文件，并把这个文件作为最终的目标文件。
3、如果test文件不存在，或是test所依赖的后面的[.o]文件的文件修改时间要比test这个文件新，那么，他就会执行后面所定义的命令来生成test这个文件。
4、如果test所依赖的.o文件存在，那么make会在当前文件中找目标为.o文件的依赖性，如果找到则再根据那一个规则生成.o文件。
5、依次类推，直到生成的所有的target都是最新的。
