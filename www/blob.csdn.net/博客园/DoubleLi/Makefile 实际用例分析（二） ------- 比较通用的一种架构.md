# Makefile 实际用例分析（二） ------- 比较通用的一种架构 - DoubleLi - 博客园






之前已经讲了这一篇文章：[Makefile实际用例分析(一)-----比较通用的一种架构](http://blog.csdn.net/shanshanpt/article/details/17198413)

现在这篇其实和那个差的不是很多，只是在布局上有些差别(这个makefile也是论坛上一起讨论过的，囧，忘了哪个论坛)



还是先看看基本的文件布局：

![](http://img.blog.csdn.net/20131208122959187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbnNoYW5wdA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

介绍：

debug是调试版本的binary文件夹

release是发行版本binary文件夹

src是所有的源文件文件夹、

lib是引用库

include一般是引用库头文件之类，或者其他头文件

obj所有.o文件和.d文件



src中：依然使用之前的那个ir_tree的例子

![](http://img.blog.csdn.net/20131208123544421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbnNoYW5wdA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



在lib中有一个外来的引用库，命名为libnew.a，

![](http://img.blog.csdn.net/20131208123852468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbnNoYW5wdA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



在include中是这个库的头文件new.h：

![](http://img.blog.csdn.net/20131208123940109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbnNoYW5wdA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



我们对于最基本的文件分别已经清楚了，这种[架构](http://lib.csdn.net/base/architecture)下只使用了一个makefile，下面我们具体看看：



---------------->>>>>> 我无语了，对csdn格式彻底无语了！这是第三次修改代码格式了，

算了，直接贴成文字吧。。。。。。。。。。。。。。。。。。。。。。。。。。。



**********温馨提示：由于csdn格式问题，下面的命令前面的TAB都没有了，所以自己写的时候就得加上!**



#################################
# 常见的配置，不多说
#
SHELL=/bin/sh 
CC=gcc
MAKE=make

#################################
# 下面定义的是一些目录路径
# 
MAKE_DIR=.                                           # 当前文件夹
SRC_DIR=$(MAKE_DIR)/src/ # 源文件夹
OBJ_DIR=$(MAKE_DIR)/obj/ # obj文件夹
LIB_DIR=$(MAKE_DIR)/lib/ # 引用库文件夹
INCLUDE_DIR=$(MAKE_DIR)/include/ # include文件夹
DEBUG_DIR=$(MAKE_DIR)/debug/ # debug文件夹

RELEASE_DIR=$(MAKE_DIR)/release/# release文件夹

EXEC_DIR= # 最终的binary文件夹


#################################
# 下面是include路径和库的路径
# 

INCLUDE=-I$(INCLUDE_DIR) -I$(SRC_DIR)
LIB=$(LIB_DIR)/libnew.a -L$(OBJ_DIR) -lm      # 引入库，包括自己的库文件( 如果你将libnew.a放到/usr/lib中了， )

  # 那么直接-lnew就可以了

#################################
# 下面是可执行名称
# 
EXEC=ir_tree

#################################
# 
# 注意：当我们下面出现%.c的时候，先在当前文件夹寻找，如果找不到，那么
#       到下面指定的文件夹中寻找！！！
# 
# 说白就是：如果依赖文件在本文件夹找不到，那么到下面文件夹寻找！仅仅是依赖文件！
# 

vpath %.h $(INCLUDE_DIR)
vpath %.c $(SRC_DIR)
vpath %.o $(OBJ_DIR)
vpath %.d $(OBJ_DIR)

#################################
# 下面是指定SRC  OBJ  DEP
# 注意：都是不带目录的basename
# 
SRC_TMP:=$(wildcard $(SRC_DIR)*.c)
C_SRCS:=$(notdir $(SRC_TMP))                            # 源文件
C_OBJS:=$(patsubst %.c,%.o,$(C_SRCS)) # o文件
C_DEPS:=$(patsubst %.c,%.d,$(C_SRCS)) # deps


#################################
# 编译选项！
#
FLAG_DEBUG=-g
CFLAGS=-O2 -Wall -c

# 下面判断是debug还是release
# 
DEBUG:=1
ifeq ($(DEBUG),1)
EXEC_DIR:=$(DEBUG_DIR)
CFLAGS:=$(CFLAGS) $(FLAG_DEBUG)
else
EXEC_DIR:=$(RELEASE_DIR)
endif


# 最终binary的名称( 路径+名称 )
# 
EXEC:=$(EXEC_DIR)$(EXEC)


################################
# 下面是终极目标all
#

#################################################################
# 关于下面的执行： 
#
# 首先-include $(addprefix $(OBJ_DIR),$(C_DEPS))
# 目的是为了将所有的.d文件包含进来，那么.d文件里面是所有的.o的
# 完整的依赖，那么即使.h被修改了，那么也是可以识别编译的！在第一次
# 处理时，没有.d文件，那么需要找.d的生成规则。因为我们include的是
# $(addprefix $(OBJ_DIR),$(C_DEPS))，那么需要找的依赖是$(OBJ_DIR)%.d
# ，那么OK，这个地方必须注意！如果你include的.d是例如card.d，那么
# 规则必须是：%.d: %.c，而不是$(OBJ_DIR)%.d: %.c。好！现在生成.d文件了
# 然后执行all:$(EXEC)，那么需要找依赖$(C_OBJS)，本文件夹没有，那么到
# vpath %.o $(OBJ_DIR)中寻找！那么可以因为开始.o是不存在(或者过期的)，
# 那么需要寻找生成规则：%.o: %.c %.d，OK生成！
# 等所有的.o处理OK，链接成可执行！
#
#################################################################

#
# 重要理解：
#    1: 你有什么样的依赖，那么就是什么样的一个子规则的目标！
#           例如：$(C_OBJS)是不带目录路径的.o的集合，例如a.o b.o c.o
#           那么，我们需要寻找生成他们的规则，那么肯定有一个子伪目标
#           名称是：(%.o：依赖)，而不是（$(OBJ_DIR)%.o:依赖），所以
#           要理解哦！
# 
#    2: 注意vpath用途，当“依赖”在本文件夹下找不到的时候，去指定
#           文件夹寻找！
#
#    3:注意Include是将.d文件中的内容加载到当前文件夹中！那么，如果.d
#           里的是例如：card.o: card.c card.h，那么$(C_OBJS)也应该是不带目录
#           路径的*.o形式！！！
#

all:$(EXEC)

$(EXEC): $(C_OBJS)
@echo 'start building  $(notdir $@) ...'
@$(CC)  $(addprefix $(OBJ_DIR),$(notdir $^)) $(LIB) -o $@

#
# 注意关系：每次makefile的时候，需要加载.d文件，那么所有的依赖被加进来
# 但是也必须有$(OBJ_DIR)%.d: %.c，这个是为了当我们的.c改变的时候，例如
# 可能心增加一个include，那么可以改变.d文件！那么后面的处理又是连带关系！
#

# 与上一篇说的一样

%.o: %.c %.d
@echo 'start building $(notdir $@)...'
@$(CC) $(CFLAGS) $< $(INCLUDE) -o $(OBJ_DIR)$@

$(OBJ_DIR)%.d: %.c
@echo 'start building $(notdir $@)...'
@$(CC) $< $(INCLUDE) -MM -MD -o $@

# 将所有的d文件引入，作用，为了更新~
-include $(addprefix $(OBJ_DIR),$(C_DEPS))

clean:
@$(RM) $(OBJ_DIR)*
@$(RM) $(EXEC)
@clear

run:
@$(EXEC)

debug:
@echo $(C_OBJS)
@echo $(C_DEPS)

.PHONY:all clean debug run



OK，现在你可以make，然后make run看结果、、、



如果你需要这个工程，同样可以免费下载：[ir_tree](http://download.csdn.net/detail/shanshanpt/6678441)

from:http://blog.csdn.net/shanshanpt/article/details/17199695









