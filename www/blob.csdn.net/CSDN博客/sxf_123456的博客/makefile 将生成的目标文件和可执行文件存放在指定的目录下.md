# makefile 将生成的目标文件和可执行文件存放在指定的目录下 - sxf_123456的博客 - CSDN博客
2017年02月23日 11:51:18[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：1258
                
.PHONY: all clean
CC = gcc
RM = rm
MKDIR = mkdir
CFLAGS = -Wall -std=gnu99
INC = -I ../include/hh_include
DIR_OBJS = ../obj
DIR_EXEC = ../exec
DIRS := $(DIR_OBJS) $(DIR_EXEC)
EXEC = prin
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
OBJS := $(addprefix $(DIR_OBJS)/,$(OBJS))
EXEC := $(addprefix $(DIR_EXEC)/,$(EXEC))
all: $(DIRS) $(EXEC)
$(DIRS):
$(MKDIR) $@
$(EXEC):$(OBJS)
$(CC) -o $@ $^
$(DIR_OBJS)/%.o:%.c
$(CC) $(CFLAGS) $(INC) -o $@ -c $^
clean:
$(RM) -rf  $(DIRS) $(EXEC)
解释：
1、.PHONY 修饰的目标就是只有规则没有依赖。由于makefile只能有一个目标，所以可以使用all来构造一个没有规则的终极目标。
并以可执行文件作为依赖。clean 清除所有的依赖和编译时生成的文件
2、CC RM MKDIR CFLAGS 作为变量，使用该变量的值使用$(变量名)
3、INC 指定的是编译源文件所需要的头文件的路径
4、./ 为当前目录 ../为当前目录的上一层目录
5、SRCS = $(wildcard *.c) 当前目录下的所有源文件
OBJS := $(addprefix $(DIR_OBJS)/,$(OBJS)) 生成目标文件存放的目录
EXEC := $(addprefix $(DIR_EXEC)/,$(EXEC)) 生成可执行文件存放的目录
