# 在Makefile里如何将变量变成小写或者大写 - 2019跟着小虎玩着去软考 - CSDN博客
2011年01月21日 14:37:00[littletigerat](https://me.csdn.net/littletigerat)阅读数：4087
**在Makefile里如何将变量变成小写或者大写**
在Makefile编译脚本里如何将变量全部变成小写或者大写
例如如何将变量BOARD的内容变成大写：
BOARD = $(shell echo $(BOARD) | tr a-z A-Z)
当然啦！
如何将变量BOARD的内容变成小写，就是：
BOARD = $(shell echo $(BOARD) | tr A-Z a-z )
