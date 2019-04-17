# Keil ARM编译器命令行编译中实现头文件依赖的自动推导 - 小灰笔记 - CSDN博客





2017年09月11日 19:04:13[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：731








            接触了几个编译器之后发现编译器基本上都有依赖文件的自动推导功能，最初掌握这个功能是在使用gcc的时候。后来发现其他的编译器基本上也有这个功能而且基本上都与gcc有着一样的语法。

            今天简单试探了一下Keil编译器，没有查什么文档，纯粹是按照之前的老路子输入猜测的命令试了一下，还真是存在！

            我在一个文件夹放了一个C文件一个头文件，C文件中包含了头文件。这算是一个最简单的应用场景了。C文件为test.c，而头文件为test.h。执行效果如下：

![](https://img-blog.csdn.net/20170911190331710?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


            有点没有弄明白这种格式的使用方法，不过有了这种格式的输出再写个简单的小脚本实现头文件依赖的makefile已经不再是什么难事儿了！



