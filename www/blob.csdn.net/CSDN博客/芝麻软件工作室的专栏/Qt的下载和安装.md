
# Qt的下载和安装 -  芝麻软件工作室的专栏 - CSDN博客


2016年06月16日 17:23:32[seven-soft](https://me.csdn.net/softn)阅读数：6819


Qt 的下载和安装都非常简单，可以到官网下载：最新版下载地址为：[http://www.qt.io/download-open-source/\#section-2](http://www.qt.io/download-open-source/#section-2)
老版本下载地址为：[http://download.qt.io/archive/qt/](http://download.qt.io/archive/qt/)
Qt 有三种安装方式，可以在线安装，可以下载离线安装包，也可以自己在本地编译源码。简单起见，推荐大家下载离线安装包。
Qt 基于C++语言，对于Windows，编译器可以是 MinGW，也可以是 VS。如下图所示：![](http://c.biancheng.net/cpp/uploads/allimg/151206/1-151206193533434.png)
Linux 的编译环境移植到 Windows 下的版本就叫做 MinGW，你可以认为 MinGW 主要包含了 GCC 编译器。
> GCC 虽然可以在 Windows 下使用，但不要期待它有优异的表现，发布软件时建议用 VS 编译。
安装 Qt 需要用账号登陆，如果没有，免费注册即可。
安装后的 Qt 主要包含以下几部分：
1) Qt Library：也就是 Qt 的库，这是 Qt 的核心。
2) Qt Creator：基于 Qt 开发的一款轻量级 IDE，是 Qt 被 Nokia 收购后推出的，目前最新版是 3.5.1，非常好用。
虽然也可以用 VS 等其他IDE 来开发 Qt 程序，但我们依然推荐使用 Qt Creator，它是官方专门为 Qt 开发打造的一款 IDE。Qt Creator 是快平台的，更换平台后不需要重新学习 IDE 的使用。
3) Qt Designer：Qt 程序的 UI 设计器。借助 Qt Designer，即使不编写代码，拖拖拽拽也可以开发简单的 GUI 程序，并且可以及时预览程序界面（无需编译）。
4) Qt Assistant：Qt 帮助工具，包含了 Qt 教程、示例、类参考手册、模块介绍等，是 Qt 的官方资料，类似 MSDN。学习 Qt，一定要会使用 Assistant 查找资料。
5) Qt Linguist：Qt 语言家，是 Qt 的国际化工具，借助它可以很方便的将界面上的文本翻译成其他语言，从而让程序支持多种语言，面向全球用户。

