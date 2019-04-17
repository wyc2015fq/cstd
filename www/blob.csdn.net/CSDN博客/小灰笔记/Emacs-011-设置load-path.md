# Emacs-011-设置load-path - 小灰笔记 - CSDN博客





2018年01月02日 09:00:31[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：776








 Emacs-011-设置load-path

     找了Emacs的插件，但是有时候不知道怎么用。能够用的一个前提就是要知道现在Emacs的load-path或者知道如何设置load-path。类似的功能，我们可以拿两个其他的使用场景类比一下。第一个是Windows命令类工具环境变量的添加，如果添加了Path环境变量，相应的命令就自然能够被索引。这个类似稍微有一点不够形象，如果要有个更为类似的类比，应该是MATLAB的add to path操作。只要在MATLAB的path目录中，MATLAB就能够进行相应的搜索定位。如果没有重复的程序文件，MATLAB会找到一个正确的程序。Emacs的加载目录与这个相似度非常高。

       我使用的Emacs是Widnows版本，如何在Widnows中使用Emacs的load-path呢？

       最初，我直接使用类似Linux的方式，因为习惯性创建了HOME目录并通过环境变量模拟了Linux的部分行为。结果这个尝试在坎坷中失败。后来发现，其实目录的加载在不同操作系统上的实现也是大同小异。不同的地方在于不同操作系统上的目录地址有不同的表示而已，而相应的设置指令都是相同的。

       实现方式，在.emacs文件中增加如下配置：

(add-to-list 'load-path"C:/Grey/.emacs.d/lisp")

       通过测试我自己的主题加载文件，上述方式是可以行的。



