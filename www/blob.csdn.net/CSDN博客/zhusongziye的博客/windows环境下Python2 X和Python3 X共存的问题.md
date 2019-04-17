# windows环境下Python2.X和Python3.X共存的问题 - zhusongziye的博客 - CSDN博客





2017年08月28日 14:32:12[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：154








##  由于某些原因，我们有时需要python2.7与python3.6并存，python2与python3的pip

  下面介绍是如何在windows系统下安装双版本Python:

  了解python的人都知道python有2.x版本和3.x版本，而python3.x版本不向下兼容，但是根据具体的需要，有时候要2.x和3.x共存，python共存本身没有问题，只是需要设置一些环境变量和修改一些东西来让它更容易使用。




  首先是去python官网下载python安装包，我安装的都是目前最新的版本。 下载地址：[www.**python**.org/downloads](http://www.baidu.com/link?url=37qkpmtA0TWlOvFEUq9V4V8ejQs15N9ukcQiIf66S5K1Lq_aO2jx-sMFgt69hl3f)两个版本安装顺序无所谓,我先装的是python3，注意安装路径设置为C:\Python36（你可以根据你的需要安装到你的路径）

py2和3都安装结束后 接下来就是检查环境变量，缺少的我们需要添加。

在系统环境变量path中找以下4个变量

1.c:\Python27

2.c:\Python27\Scripts

3.c:\Python36

4.c:\Python36\Scripts

少哪个加哪个，注意分号隔开。




我的方法不需要到各自的文件夹中更改python.exe的名字！！



这个问题（双版本共存）Python社区早就给出了官方解决方案，甚至连pip都解决了双版本。



**一、python2.7与python3.6并存**

下面具体介绍：

进入cmd，输入py -2，直接进入了python2.7

而输入py -3，进入了python3.6

在具体py文件中，使用

#! python2

# coding: utf-8

可以指定解释器版本，这时只需py test.py，无需带-2或-3这种参数。



**二、python2与python3的pip并存**

pip一直是个问题，但现在这个问题也可以很轻易地被解决。

使用pip2.7命令调用python2.7的pip，如pip2.7
 install click

使用pip3.6命令调用python3.6的pip，如pip3.6
 pip click



网上还提供了另外一种调用pip的方法

命令如下：

py -2 -m pip install XXXX

-2 还是表示使用 Python2，-m pip
表示运行 pip
模块，也就是运行pip命令了。

如果是为Python3安装软件，那么命令类似的变成

py -3 -m pip install XXXX



      PS: 当然你在安装好Python后，也可以直接在Python的安装路径根下，直接把python.exe的名字修改为python2.exe或者python3.exe，

这样在命令行中输入Python2.exe即可进入Python2，看你喜欢哪个方法就用哪个方法吧，不用纠结哪个好！





﻿﻿



