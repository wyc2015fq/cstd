# Makefile 使用总结 - DoubleLi - 博客园






## **1. Makefile 简介**

Makefile 是和 make 命令一起配合使用的.

很多大型项目的编译都是通过 Makefile 来组织的, 如果没有 Makefile, 那很多项目中各种库和代码之间的依赖关系不知会多复杂.

Makefile的组织流程的能力如此之强, 不仅可以用来编译项目, 还可以用来组织我们平时的一些日常操作. 这个需要大家发挥自己的想象力.



本篇博客是基于 [{精华} 跟我一起写 Makefile](http://bbs.chinaunix.net/forum.php?mod=viewthread&tid=408225) 而整理的, 有些删减, 追加了一些示例.

非常感谢 gunguymadman_cu 提供如此详尽的Makefile介绍, 这正是我一直寻找的Makefile中文文档.



### **1.1 Makefile 主要的 5个部分 (显示规则, 隐晦规则, 变量定义, 文件指示, 注释)**

Makefile基本格式如下:

```
target ... : prerequisites ...
    command
    ...
    ...
```

其中,
- target        - 目标文件, 可以是 Object File, 也可以是可执行文件
- prerequisites - 生成 target 所需要的文件或者目标
- command       - make需要执行的命令 (任意的shell命令), Makefile中的命令必须以 [tab] 开头


- **显示规则 :: 说明如何生成一个或多个目标文件(包括 生成的文件, 文件的依赖文件, 生成的命令)**
- **隐晦规则 :: make的自动推导功能所执行的规则**
- **变量定义 :: Makefile中定义的变量**
- **文件指示 :: Makefile中引用其他Makefile; 指定Makefile中有效部分; 定义一个多行命令**
- **注释     :: Makefile只有行注释 "#", 如果要使用或者输出"#"字符, 需要进行转义, "\#"**



### **1.2 GNU make 的工作方式**
- 读入主Makefile (主Makefile中可以引用其他Makefile)
- 读入被include的其他Makefile
- 初始化文件中的变量
- 推导隐晦规则, 并分析所有规则
- 为所有的目标文件创建依赖关系链
- 根据依赖关系, 决定哪些目标要重新生成
- 执行生成命令



## **2. Makefile 初级语法**

### **2.1 Makefile 规则**

#### **2.1.1 规则语法**

规则主要有2部分: 依赖关系 和 生成目标的方法.

语法有以下2种:

```
target ... : prerequisites ...
    command
    ...
```

或者

```
target ... : prerequisites ; command
    command
    ...
```

***注*** command太长, 可以用 "\" 作为换行符



#### **2.1.2 规则中的通配符**
- *     :: 表示任意一个或多个字符
- ?     :: 表示任意一个字符
- [...] :: ex. [abcd] 表示a,b,c,d中任意一个字符, [^abcd]表示除a,b,c,d以外的字符, [0-9]表示 0~9中任意一个数字
- ~     :: 表示用户的home目录



#### **2.1.3 路径搜索**

当一个Makefile中涉及到大量源文件时(这些源文件和Makefile极有可能不在同一个目录中),

这时, 最好将源文件的路径明确在Makefile中, 便于编译时查找. Makefile中有个特殊的变量 **VPATH** 就是完成这个功能的.

指定了 **VPATH** 之后, 如果当前目录中没有找到相应文件或依赖的文件, Makefile 回到 **VPATH** 指定的路径中再去查找..

**VPATH** 使用方法:
- vpath <directories>            :: 当前目录中找不到文件时, 就从<directories>中搜索
- vpath <pattern> <directories>  :: 符合<pattern>格式的文件, 就从<directories>中搜索
- vpath <pattern>                :: 清除符合<pattern>格式的文件搜索路径
- vpath                          :: 清除所有已经设置好的文件路径



```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

# 示例1 - 当前目录中找不到文件时, 按顺序从 src目录 ../parent-dir目录中查找文件
VPATH src:../parent-dir   

# 示例2 - .h结尾的文件都从 ./header 目录中查找
VPATH %.h ./header

# 示例3 - 清除示例2中设置的规则
VPATH %.h

# 示例4 - 清除所有VPATH的设置
VPATH

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```



### **2.2 Makefile 中的变量**

#### **2.2.1 变量定义 ( = or := )**

```
OBJS = programA.o programB.o
OBJS-ADD = $(OBJS) programC.o
# 或者
OBJS := programA.o programB.o
OBJS-ADD := $(OBJS) programC.o
```

其中 = 和 := 的区别在于, := 只能使用前面定义好的变量, = 可以使用后面定义的变量

**测试 =**

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

# Makefile内容
OBJS2 = $(OBJS1) programC.o
OBJS1 = programA.o programB.o

all:
    @echo $(OBJS2)

# bash中执行 make, 可以看出虽然 OBJS1 是在 OBJS2 之后定义的, 但在 OBJS2中可以提前使用
$ make
programA.o programB.o programC.o

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```



**测试 :=**

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

# Makefile内容
OBJS2 := $(OBJS1) programC.o
OBJS1 := programA.o programB.o

all:
    @echo $(OBJS2)

# bash中执行 make, 可以看出 OBJS2 中的 $(OBJS1) 为空
$ make
programC.o

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```



#### **2.2.2 变量替换**

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

# Makefile内容
SRCS := programA.c programB.c programC.c
OBJS := $(SRCS:%.c=%.o)

all:
    @echo "SRCS: " $(SRCS)
    @echo "OBJS: " $(OBJS)

# bash中运行make
$ make
SRCS:  programA.c programB.c programC.c
OBJS:  programA.o programB.o programC.o

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```



#### **2.2.3 变量追加值 +=**

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

# Makefile内容
SRCS := programA.c programB.c programC.c
SRCS += programD.c

all:
    @echo "SRCS: " $(SRCS)

# bash中运行make
$ make
SRCS:  programA.c programB.c programC.c programD.c

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```



#### **2.2.4 变量覆盖 override**

作用是使 Makefile中定义的变量能够覆盖 make 命令参数中指定的变量

语法:
- override <variable> = <value>
- override <variable> := <value>
- override <variable> += <value>



下面通过一个例子体会 override 的作用：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

# Makefile内容 (没有用override)
SRCS := programA.c programB.c programC.c

all:
    @echo "SRCS: " $(SRCS)

# bash中运行make
$ make SRCS=nothing
SRCS:  nothing

#################################################

# Makefile内容 (用override)
override SRCS := programA.c programB.c programC.c

all:
    @echo "SRCS: " $(SRCS)

# bash中运行make
$ make SRCS=nothing
SRCS:  programA.c programB.c programC.c

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```



#### **2.2.5 目标变量**

作用是使变量的作用域仅限于这个目标(target), 而不像之前例子中定义的变量, 对整个Makefile都有效.

语法:
- <target ...> :: <variable-assignment>
- <target ...> :: override <variable-assignment> (override作用参见 变量覆盖的介绍)



示例:

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

# Makefile 内容
SRCS := programA.c programB.c programC.c

target1: TARGET1-SRCS := programD.c
target1:
    @echo "SRCS: " $(SRCS)
    @echo "SRCS: " $(TARGET1-SRCS)

target2:
    @echo "SRCS: " $(SRCS)
    @echo "SRCS: " $(TARGET1-SRCS)

# bash中执行make
$ make target1
SRCS:  programA.c programB.c programC.c
SRCS:  programD.c

$ make target2     <-- target2中显示不了 $(TARGET1-SRCS)
SRCS:  programA.c programB.c programC.c
SRCS:

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```



### **2.3 Makefile 命令前缀**

Makefile 中书写shell命令时可以加2种前缀 @ 和 -, 或者不用前缀.

3种格式的shell命令区别如下:
- 不用前缀 :: 输出执行的命令以及命令执行的结果, 出错的话停止执行
- 前缀 @   :: 只输出命令执行的结果, 出错的话停止执行
- 前缀 -   :: 命令执行有错的话, 忽略错误, 继续执行



示例:

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

# Makefile 内容 (不用前缀)
all:
    echo "没有前缀"
    cat this_file_not_exist
    echo "错误之后的命令"       <-- 这条命令不会被执行

# bash中执行 make
$ make
echo "没有前缀"             <-- 命令本身显示出来
没有前缀                    <-- 命令执行结果显示出来
cat this_file_not_exist
cat: this_file_not_exist: No such file or directory
make: *** [all] Error 1

###########################################################

# Makefile 内容 (前缀 @)
all:
    @echo "没有前缀"
    @cat this_file_not_exist
    @echo "错误之后的命令"       <-- 这条命令不会被执行

# bash中执行 make
$ make
没有前缀                         <-- 只有命令执行的结果, 不显示命令本身
cat: this_file_not_exist: No such file or directory
make: *** [all] Error 1

###########################################################

# Makefile 内容 (前缀 -)
all:
    -echo "没有前缀"
    -cat this_file_not_exist
    -echo "错误之后的命令"       <-- 这条命令会被执行

# bash中执行 make
$ make
echo "没有前缀"             <-- 命令本身显示出来
没有前缀                    <-- 命令执行结果显示出来
cat this_file_not_exist
cat: this_file_not_exist: No such file or directory
make: [all] Error 1 (ignored)
echo "错误之后的命令"       <-- 出错之后的命令也会显示
错误之后的命令              <-- 出错之后的命令也会执行

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```



### **2.4 伪目标**

伪目标并不是一个"目标(target)", 不像真正的目标那样会生成一个目标文件.

典型的伪目标是 Makefile 中用来清理编译过程中中间文件的 clean 伪目标, 一般格式如下:

```
.PHONY: clean   <-- 这句没有也行, 但是最好加上
clean:
    -rm -f *.o
```



### **2.5 引用其他的 Makefile**

语法: include <filename>  (filename 可以包含通配符和路径)

示例:

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

# Makefile 内容
all:
    @echo "主 Makefile begin"
    @make other-all
    @echo "主 Makefile end"

include ./other/Makefile

# ./other/Makefile 内容
other-all:
    @echo "other makefile begin"
    @echo "other makefile end"

# bash中执行 make
$ ll
total 20K
-rw-r--r-- 1 wangyubin wangyubin  125 Sep 23 16:13 Makefile
-rw-r--r-- 1 wangyubin wangyubin  11K Sep 23 16:15 makefile.org   <-- 这个文件不用管
drwxr-xr-x 2 wangyubin wangyubin 4.0K Sep 23 16:11 other
$ ll other/
total 4.0K
-rw-r--r-- 1 wangyubin wangyubin 71 Sep 23 16:11 Makefile

$ make
```









