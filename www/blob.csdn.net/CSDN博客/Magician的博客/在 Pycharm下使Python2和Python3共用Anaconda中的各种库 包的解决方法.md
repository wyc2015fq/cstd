
# 在 Pycharm下使Python2和Python3共用Anaconda中的各种库-包的解决方法 - Magician的博客 - CSDN博客


2018年06月26日 10:23:03[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：1402


参考：[https://www.cnblogs.com/MoonST/p/7610460.html](https://www.cnblogs.com/MoonST/p/7610460.html)
[目录：](#目录)[前言：](#前言)
[1、同时下载两个版本的anaconda](#1同时下载两个版本的anaconda)
[2、主版本conda的安装](#2主版本conda的安装)
[3、辅助版本Anaconda的安装](#3辅助版本anaconda的安装)


# 目录：
## 前言：
最近在看一些机器学习方面的教程，里面的一些实践Demo用的是py2的环境并且其中用到一些各种第三方的库，我想运行这些Demo，就需要满足以下两个条件：
1、py2条件
2、py2条件下有各种第三方的库
py2环境条件可以在Pycharm中创建（这也是该篇文章的前提条件，
详情参考：[https://blog.csdn.net/qq_16633405/article/details/80036721](https://blog.csdn.net/qq_16633405/article/details/80036721)）
但是py2下的各种第三方库需要自己重新下载，这里自己想省事所以就想能不能让py2共享py3版本的下Anaconda的各种库，一顿折腾后，发现这个方法行不通，但是可以曲线救国。通过在Pycharm中创建虚拟坏境，将Python2版本的Anaconda即Anaconda2装入对应的虚拟环境中，即可在py2条件下使用Anaconda中的各种第三方的库。
整个过程如下：
Anaconda是Python的一个发行版 其中有非常方便的包管理工具conda
官方下载地址[https://www.anaconda.com/download/](https://www.anaconda.com/download/)
![这里写图片描述](https://img-blog.csdn.net/20180626100827490?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 1、同时下载两个版本的anaconda
**以下Python2简称py2 Python3亦然，anaconda简称conda**
pycharm是非常方便的Python IDE工具但是其包管理却是不区分py2和py3的有时候无法找到合适版本的包会提示错误信息（安装了conda无法用pycharm进行包管理）
## 2、主版本conda的安装
先安装主版本的conda 比如你是使用Py2开发的就先把conda2安装了 安装过程就是一直的下一步
到这一步要同时勾选这两个选项
第一是把conda添加进系统变量里面
第二是把它设为主要的Py版本
![这里写图片描述](https://img-blog.csdn.net/20180626101022768?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180626101022768?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
所以想把py3作为主要版本的此处要先安装py3
[

](https://img-blog.csdn.net/20180626101022768?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 3、辅助版本Anaconda的安装
[
](https://img-blog.csdn.net/20180626101022768?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)安装完conda2之后，**conda3的安装路径需要在conda2的envs目录里面 这一步很重要**，因为这样以后可以用命令创建一个虚拟的Py3的运行环境。
![这里写图片描述](https://img-blog.csdn.net/20180626101401474?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**所以这一步的两个勾都要去掉避免和conda2冲突**
![这里写图片描述](https://img-blog.csdn.net/20180626101509975?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180626101509975?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
完成之后就同时有了py2和py3 可以在pycharm里面随意选择2和3进行开发
[
](https://img-blog.csdn.net/20180626101509975?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)打开pycharm之后 File→Setting→project interpreter点齿轮按钮添加编译器路径
![这里写图片描述](https://img-blog.csdn.net/20180626101544757?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180626101544757?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
创建项目可以选择py的版本
![这里写图片描述](https://img-blog.csdn.net/20180626101558882?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这样你就可以随便的切换环境，并且能共享Anaconda中的各种第三方库。

[
](https://img-blog.csdn.net/20180626101544757?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
