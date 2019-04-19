# Makefile不再编译已经编译过的未更新文件 - Koma Hub - CSDN博客
2019年03月06日 21:55:14[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：32
个人分类：[C/C++																[cmake/make/automake																[Linux](https://blog.csdn.net/Rong_Toa/article/category/7156254)](https://blog.csdn.net/Rong_Toa/article/category/8730065)](https://blog.csdn.net/Rong_Toa/article/category/7156199)
提到Makefile我就想起了中美合拍的《西游记》，偶，不对，提到Makefile不得不说是一种非常牛叉的编译工具了，他比cmake要易懂直观很多，可是，当我们编写大型软件时候，往往伴随着每次重新编译整个软件的问题消耗大量的编译时间，有没有一种方法可以只编译修改过得文件呢？这篇文章我们就来解决这个问题。
# 首先建立一个多文件项目
## 项目目录结构
```
.
└── software
    ├── debug
    │   ├── Makefile
    │   ├── module1
    │   │   └── src
    │   └── module2
    │       └── src
    ├── main.c
    ├── module1
    │   ├── include
    │   │   └── m1_api.h
    │   └── src
    │       └── m1.c
    └── module2
        ├── include
        │   └── m2_api.h
        └── src
            └── m2.c
12 directories, 8 files
```
## 项目源文件
### main.c
```cpp
#include <stdio.h>
#include "m1_api.h"
#include "m2_api.h"
int main()
{
	m1_func(NULL);
	m2_func(NULL);
	return 0;
}
```
### m1_api.h
```cpp
#ifndef _MODULE1_API_H
#define _MODULE1_API_H
typedef struct {
	int id;
	int fd;
	int tid;
	int pid;
}m1_t;
void m1_func(m1_t *t);
#endif /*_MODULE1_API_H*/
```
### m1.c
```cpp
#include <stdio.h>
#include "m1_api.h"
void m1_func(m1_t *t)
{
	printf("%s\n", __func__);
}
```
### m2_api.h
```cpp
#ifndef _MODULE2_API_H
#define _MODULE2_API_H
typedef struct {
	int id;
	int fd;
	int tid;
	int pid;
}m2_t;
void m2_func(m2_t *t);
#endif /*_MODULE2_API_H*/
```
### m2.c
```cpp
#include <stdio.h>
#include "m2_api.h"
void m2_func(m2_t *t)
{
	printf("%s\n", __func__); 
}
```
# Makefile建立
## 搭建一个Makefile框架
### 项目路径
```
SOFTROOT = /home/XXX/test/make/demo1/software
DEBUGROOT = $(SOFTROOT)/debug
TARGET = software.exe
```
我的用户名就不写出来了，换成自己的就行（目录自己修改），
### 需要的shell指令
```
CC = gcc
RM = rm -rf 
ECHO = echo
```
### makefile变量的声明
```
SRCS :=
INCLUDES :=
LIBS :=
OBJS :=
DEPS := 
CFLAGS :=
```
### 源文件列表
```
SRCS += $(SOFTROOT)/module1/src/m1.c
SRCS += $(SOFTROOT)/module2/src/m2.c
SRCS += $(SOFTROOT)/main.c
```
### 头文件列表
```
INCLUDES += -I$(SOFTROOT)/module1/include
INCLUDES += -I$(SOFTROOT)/module2/include
```
### 目标文件
这里的目标文件我们根据源文件和编译输出路径来转化，不由用户自己规定
```
OBJS += $(subst $(SOFTROOT),$(DEBUGROOT),$(patsubst %.c,%.o,$(SRCS)))
DEPS += $(subst $(SOFTROOT),$(DEBUGROOT),$(patsubst %.c,%.d,$(SRCS)))
```
整体思路：先将.c替换成.o（或者.d），然后将源文件路径替换为编译文件夹的路径。
### 编译目标文件
```
$(OBJS):
	@if [ ! -d $(dir $@) ]; then mkdir -p $(dir $@); fi;
	@$(ECHO) "\033[1;31mCompile\033[0m: $(notdir $@)"
	@$(CC) -o $@ $(CFLAGS) -c  $(patsubst %.o,%.c,$(subst $(DEBUGROOT),$(SOFTROOT),$@))
```
### 编译生成依赖文件列表
```
$(DEPS):
	@if [ ! -d $(dir $@) ]; then mkdir -p $(dir $@); fi; \
	set -e; $(RM) $@
	@$(ECHO) "\033[1;31mCreate\033[0m: $(notdir $@)"
	@$(CC) -MM $(CFLAGS) -c $(patsubst %.d,%.c,$(subst $(DEBUGROOT),$(SOFTROOT),$@)) -o $@.dep
	@sed 's,\($(notdir $(basename $@))\).o[ :]*,$(dir $@)\1.o $@ :,g' < $@.dep > $@;\
	$(RM) $@.dep
-include $(DEPS)
```
注意最后一句-include。
可以适量的添加makefile查找源文件的路径，制定查找路径
`vpath %.c $(dir %(SRCS))`
## 最终的Makefile
```
SOFTROOT = /home/rongtao/test/make/demo1/software
DEBUGROOT = $(SOFTROOT)/debug
TARGET = software.exe
CC = gcc
RM = rm -rf 
ECHO = echo 
SRCS :=
INCLUDES :=
LIBS :=
OBJS :=
DEPS := 
CFLAGS := 
SRCS += $(SOFTROOT)/module1/src/m1.c
SRCS += $(SOFTROOT)/module2/src/m2.c
SRCS += $(SOFTROOT)/main.c
INCLUDES += -I$(SOFTROOT)/module1/include
INCLUDES += -I$(SOFTROOT)/module2/include
OBJS += $(subst $(SOFTROOT),$(DEBUGROOT),$(patsubst %.c,%.o,$(SRCS)))
DEPS += $(subst $(SOFTROOT),$(DEBUGROOT),$(patsubst %.c,%.d,$(SRCS)))
CFLAGS += $(INCLUDES)
all:$(OBJS)
	@$(ECHO) "\033[1;31mFinish\033[0m"
	@$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)
$(OBJS):
	@if [ ! -d $(dir $@) ]; then mkdir -p $(dir $@); fi;
	@$(ECHO) "\033[1;31mCompile\033[0m: $(notdir $@)"
	@$(CC) -o $@ $(CFLAGS) -c  $(patsubst %.o,%.c,$(subst $(DEBUGROOT),$(SOFTROOT),$@))
$(DEPS):
	@if [ ! -d $(dir $@) ]; then mkdir -p $(dir $@); fi; \
	set -e; $(RM) $@
	@$(ECHO) "\033[1;31mCreate\033[0m: $(notdir $@)"
	@$(CC) -MM $(CFLAGS) -c $(patsubst %.d,%.c,$(subst $(DEBUGROOT),$(SOFTROOT),$@)) -o $@.dep
	@sed 's,\($(notdir $(basename $@))\).o[ :]*,$(dir $@)\1.o $@ :,g' < $@.dep > $@;\
	$(RM) $@.dep
-include $(DEPS)
vpath %.c $(dir %(SRCS))
	
clean:
	@$(ECHO) "\033[1;31mClean\033[0m"
	@$(RM) $(DEPS) $(OBJS) $(TARGET)
```
> 
源代码请搜索CSDN与本文同名的资源下载即可“Makefile不编译已经编译过的文件”。

