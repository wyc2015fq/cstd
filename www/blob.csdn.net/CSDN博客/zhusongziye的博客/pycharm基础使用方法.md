# pycharm基础使用方法 - zhusongziye的博客 - CSDN博客





2018年01月15日 21:52:02[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：26063










0、前言



Pycharm 作为一款针对 Python 的编辑器，配置简单、功能强大、使用起来省时省心，对初学者友好，这也是为什么编程教室一直推荐新手使用 Pycharm 的原因。

本文我们将介绍  pycharm 编辑器的基本使用方法，主要包括以下几个方面：
- 
下载安装

- 
新建项目流程

- 
配置 Pycharm

- 
Python控制台

- 
其他参考资料


1、下载安装

Pycharm 提供 免费的社区版 与 付费的专业版。专业版额外增加了一些功能，如项目模板、远程开发、数据库支持等。个人学习 Python 使用免费的社区版已足够。

pycharm社区版：http://www.jetbrains.com/pycharm/download/

百度云盘地址：https://pan.baidu.com/s/1bpqWA2F

安装过程照着提示一步步操作就可以了。注意安装路径尽量不使用带有 中文或空格 的目录，这样在之后的使用过程中减少一些莫名的错误。

2、新建项目

安装好软件之后，我们开始创建第一个项目，界面如下

![](https://img-blog.csdn.net/20180115215231046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

左侧导航栏选择 Pure Python  ，右侧的 Location 选择项目的路径， Interpreter  通过下拉栏选择 Python版本 ，这里直接使用 Python 的安装路径即可。

选择完成之后，点击 Create 按钮，进入界面。这时就可以创建文件了，步骤如下图所示：

![](https://img-blog.csdn.net/20180115215258441?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这里我们以刚刚创建的 Crossin-practices 文件夹为例，依次点击

Crossin-practices  →  New  →   Python File

得到了如下的结果

![](https://img-blog.csdn.net/20180115215321736?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在 Name 一栏输入文件名即可，记得添加 .py 后缀，点击 OK 之后就可以开始写下
`print('hello,world')`
然后在界面点击 右键 → Run example

![](https://img-blog.csdn.net/20180115215339855?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

出现这样的结果：

![](https://img-blog.csdn.net/20180115215353660?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

对于同一个脚本，第一次运行使用  右键 → Run example ，之后可以直接点击右上角或者左下角的 绿三角 。如下图：

![](https://img-blog.csdn.net/20180115215431193?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

注意：更改文件运行的时候，三角和快捷键运行项目不会自动更改。所以常会运行了错误的文件而没发现。所以我们推荐第一次运行使用右键的方式，将脚本切换之后再使用绿三角。

到此，建立项目，运行脚本文件的流程都介绍完毕了

3、配置 Pycharm

Pycharm 提供的配置很多，这里讲几个比较重要的配置

编码设置：

Python 的编码问题由来已久，为了避免一步一坑，Pycharm 提供了方便直接的解决方案

![](https://img-blog.csdn.net/20180115215452298?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在 IDE Encoding 、Project Encoding 、Property Files 三处都使用 UTF-8 编码，同时在文件头添加

`#-*- coding: utf-8 -*`

 这样在之后的学习过程中，或多或少会避免一些编码坑。

解释器设置：

当有多个版本安装在电脑上，或者需要管理虚拟环境时，Project Interpreter 提供方便的管理工具。

![](https://img-blog.csdn.net/20180115215519435?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在这里可以方便的切换 Python 版本，添加卸载库等操作。

修改字体：

在 Editor → Font 选项下可以修改字体，调整字体大小等功能。

![](https://img-blog.csdn.net/20180115215536239?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

快捷键设置：

在 windows 下一些最常用的默认快捷键：

![](https://img-blog.csdn.net/20180115215606510?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

Pycharm 也为不同平台的用户提供了定制的快捷键方案，习惯了用emacs、vim、vs的同学，可以直接选择对应的方案。

![](https://img-blog.csdn.net/20180115215835986?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

同时，Pycharm 也提供了自定义快捷键的功能。

![](https://img-blog.csdn.net/20180115215857481?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

修改完成之后就去试试效果吧！

4、调试

强大的 Pycharm 为我们提供了方便易用的断点调试功能，步骤如下图所示：

![](https://img-blog.csdn.net/20180115215913556?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

简单介绍一下调试栏的几个重要的按钮作用：



![](https://img-blog.csdn.net/20180115220435759?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) Resume Program：断点调试后，点击按钮，继续执行程序；

![](https://img-blog.csdn.net/20180115220508071?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) Step Over  ：在单步执行时，在函数内遇到子函数时不会进入子函数内单步执行，而是将子函数整个执行完再停止，也就是把子函数整个作为一步。有一点,经过我们简单的调试,在不存在子函数的情况下是和Step Into效果一样的（简而言之，越过子函数，但子函数会执行）；

![](https://img-blog.csdn.net/20180115220527738?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) Step Into：单步执行，遇到子函数就进入并且继续单步执行（简而言之，进入子函数）；

![](https://img-blog.csdn.net/20180115220941198?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)Step Out ： 当单步执行到子函数内时，用step out就可以执行完子函数余下部分，并返回到上一层函数。

如果程序在某一步出现错误，程序会自动跳转到错误页面，方便我们查看错误信息

5、Python 控制台

为了方便用户，Pycharm 提供了另一个贴心的功能，将 Python shell 直接集成在软件中，调出方法如下：

![](https://img-blog.csdn.net/20180115220551039?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



6、一些网上收集的教程(参考)

Pycharm官方教程：http://www.jetbrains.com/pycharm/documentation/

Pycharm toolbar window：https://www.jetbrains.com/help/pycharm/2016.3/debug-tool-window.html#steptoolbar

Pycharm 皮肤主题及个性化设置：http://blog.csdn.net/garfielder007/article/details/53873787

Pycharm 更换主题：http://blog.csdn.net/felcon/article/details/38491413

快捷键大全：http://www.cnblogs.com/kaituorensheng/p/5371366.html

码代码不容易，分享一个支付宝小红包给大家，算是给小编的一点鼓励吧，谢谢！

![](https://img-blog.csdn.net/20181020163804827?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![](https://img-blog.csdn.net/20181020163956700?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



