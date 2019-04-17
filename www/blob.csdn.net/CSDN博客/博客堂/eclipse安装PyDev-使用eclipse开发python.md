# eclipse安装PyDev-使用eclipse开发python - 博客堂 - CSDN博客





2018年01月03日 14:46:24[最小森林](https://me.csdn.net/u012052268)阅读数：5615








# eclipse安装PyDev-使用eclipse开发python



- [eclipse安装PyDev-使用eclipse开发python](#eclipse安装pydev-使用eclipse开发python)- [简介](#1简介)
- [安装教程](#2安装教程)- [1 简易在线安装](#21-简易在线安装)
- [2 离线安装](#22-离线安装)

- [PyDev的配置](#3pydev的配置)
- [例子 实战](#4例子-实战)





## 1.简介

Eclipse是一款基于Java的可扩展开发平台。其官方下载中包括J2EE方向版本、Java方向版本、C/C++方向版本、移动应用方向版本等诸多版本。除此之外，Eclipse还可以通过安装插件的方式进行诸如Python、Android、PHP等语言的开发。

其中PyDev是一个功能强大的 Eclipse插件，使用户可用 Eclipse 来进行 Python 应用程序的开发和调试。PyDev 插件的出现方便了众多的 Python 开发人员，它提供了一些很好的功能，如：语法错误提示、源代码编辑助手、Quick Outline、Globals Browser、Hierarchy View、运行和调试等等。

本文将介绍eclipse的pydev插件的安装与配置。

## 2.安装教程
- 首先下载jdk，安装，配置环境变量；
- 再下载eclipse，配置编译器；
- 写一个helloworld；（具体过程请专门看教程。）

这样，基础环境就搭好了。下面安装PyDev，有两种安装方法，分别介绍之：

### 2.1 简易在线安装

这是官网安装，下载安装很方便，但可能由于网络原因用时过长或失败。

官网安装过程如下：
- Help - Install New Software

![image](https://upload-images.jianshu.io/upload_images/2119738-363608745a6b3ef2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
- 
点击 - Add

- 
输入 Name 和 Location，Name 随意，Location 为 [http://pydev.org/updates](http://pydev.org/updates) ； 点击OK


把【connect all update sites during install to find required software】的勾选去掉，否则在安装新插件时会联网寻找所有可能的更新站点搜索，导致安装时间不可预估，并可能导致安装失败。

确定后可以看到一个Pending过程，然后得到如下图所示的插件：

![image](https://upload-images.jianshu.io/upload_images/2119738-5d897f93b234e869.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)

勾选后，点击Next进行安装。

不过，由于网络的原因，这种方法安装PyDev极有可能失败，提示网络连接错误等。需要vpn。

### 2.2 离线安装

通常官网安装并不如人意，你懂的… 推荐离线安装，步骤如下
- 
下载PyDev离线安装包，我的云盘地址：[http://pan.baidu.com/s/1pJ1HQKb](http://pan.baidu.com/s/1pJ1HQKb)

- 
将解压后的features和plugins两个文件夹分别拷贝到Eclipse安装目录下的features和plugins目录中


## 3.PyDev的配置

　不论官网安装还是离线安装，都需要配置PyDev，步骤如下
- 
启动Eclipse，打开window->Preferences

- 
选择Interpreter-Python，然后选择New

- 
点击【New】，添加一个系统里已有的Python解释器的位置。确定后会经过短暂的处理，得到它的Libraries、Buildins等。 

(我写的地址是提前安装好的annaconda。)

- 
最后，单击Reference界面下的 OK ; 等待后即可在Eclipse中写Python了。


## 4.例子 实战

前面就已经配置好了Python的开发环境，下面新建一个项目，来测试一下，确实可以运行。

点击【File】-【New】-【Other】，找到【PyDev】，选择【PyDev Project】，点击Next。取一个项目名称，比如helloPython，如下图所示：

![image](https://upload-images.jianshu.io/upload_images/2119738-b329a80a1e2e469b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/527)

点击【Finish】，完成项目创建。然后你会进入PyDev视图，进行Python开发。

右键项目的src目录，选择【New】-【PyDev Package】，创建一个Python包，此处也命名为helloPython。

再右键该package，【New】-【PyDev Module】（python的module 就是java的calss），此处也命名为helloPython。 （pydev提供了一些模板，这边暂选 Empty）

双击打开 helloPython .py，添加如下代码。
`print'hello python!'`
右键项目，选择【Run As】-【Python Run】，或Ctrl+F11运行项目。

此时，可以在下方的console窗口，看到项目的运行结果。



