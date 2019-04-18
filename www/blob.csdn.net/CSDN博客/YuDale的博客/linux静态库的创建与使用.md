# linux静态库的创建与使用 - YuDale的博客 - CSDN博客
2015年09月21日 21:52:40[YuDale](https://me.csdn.net/YuDale)阅读数：240
上图为参数法
首先创建今天学习的文件夹“2015921”
创建两个文件夹main.c，hello.c
给hello.c文件编写程序
给 main.c文件编程
编译成目标文件，即.o文件
创建静态库
用参数法将静态库链接到main.c文件中
测试成功
问题：
1.为什么目标问价是.o文件
2.静态库链接进去的是不是一整段程序？这样会和main.c问价中的一些重复，eg. #include<stdio.h>这部分，如何处理？
