# linux下通用Makefile写法 - maopig的专栏 - CSDN博客
2011年07月06日 23:21:19[maopig](https://me.csdn.net/maopig)阅读数：1139
linux编译多个源文件的程序比较麻烦，这下就需要通用的Makefile了，编译的时候执行一下make命令就OK，下面介绍通用makfile的写法。
假设现在有以下源文件：file1.h file1.c file2.h file2.c mainproc.c，程序的主函数在mainproc.c中。
#######通用Makefile模板########
# 目标文件名字
TARGET = Myproc
# c源文件
SRC = mainproc.c file1.c file2.c
# object文件
OBJS = $(SRC:.c=.o)
# 编译器
CC = gcc
# 头文件路径，可以任意指定头文件
INCLUDE = -I/usr/include/ -I/usr/include/ncurses
# 链接库
LINKPARAM = -lpthread -lncurses
# 编译选项
CFLAGS = -ansi -g -Wall
all:$(TARGET)
# 定义目标文件生成规则
$(TARGET):$(OBJS)
　　$(CC) -o $(TARGET) $(LINKPARAM) $(OBJS)
.SUFFIXES:.c
# 定义obejct文件生成规则
.c.o:
　　$(CC) $(LINKPARAM) $(INCLUDE) $(CFLAGS) -c $<
# 定义清理函数
clean:
　　rm $(OBJS) $(TARGET)
