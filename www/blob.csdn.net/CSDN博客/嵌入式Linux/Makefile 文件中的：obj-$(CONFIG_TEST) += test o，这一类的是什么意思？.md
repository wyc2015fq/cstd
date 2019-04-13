
# Makefile 文件中的：obj-$(CONFIG_TEST) += test.o，这一类的是什么意思？ - 嵌入式Linux - CSDN博客

2019年02月21日 09:38:39[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：85个人分类：[Linux																](https://blog.csdn.net/weiqifa0/article/category/1388863)



# 1、obj-$
**$(CONFIG_TEST) 是一个整体，$(bbb)表示引用变量 bbb**
`比如定义 CONFIG_TEST=y`
`$(CONFIG_TEST) 就是 y`
`obj-$(CONFIG_TEST) 就是 obj-y`
`又比如定义 CONFIG_TEST=m`
`$(CONFIG_TEST) 就是 m`
`obj-$(CONFIG_TEST) 就是 obj-m`
obj-y += foo.o 该例子告诉Kbuild在这目录里，有一个名为foo.o的目标文件。foo.o将从foo.c 或foo.S文件编译得到。
**例子：**
obj-$(CONFIG_FOO) += foo.o $(CONFIG_FOO)可以为y(编译进内核) 或m(编译成模块)。如果CONFIG_FOO不是y 和m,那么该文件就不会被编译联接了
除了y、m以外的obj-x 形式的目标都不会被编译。
除了obj-形式的目标以外，还有lib-y library 库、hostprogs-y 主机程序等。

# 2、objs用法
obj-y = main.o
main-objs := a.o   \  b.o \     c.o
将a.c b.c c.c三个文件编译后链接生成main.o
![](https://img-blog.csdnimg.cn/20190221092920559.png)


