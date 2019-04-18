# Qt 之 qwt 和 qwtpolar编译和安装 - L_Andy的专栏 - CSDN博客

2018年12月14日 16:45:54[卡哥](https://me.csdn.net/L_Andy)阅读数：129


# **1  Qwt **

  Qwt 全称为 Qt Widgets for Technical Applications，用于专业技术领域的可视化显示，如下所示：

  左图为自动控制领域，二阶系统的频率响应；中图为德国小城 Friedberg 2007年的温度统计；右图为2012年4月，Linux 各个发行版，Debian, Mint, Ubuntu, openSuse 等，每天的平均点击次数

![](https://images2017.cnblogs.com/blog/890227/201708/890227-20170804103944444-410329476.png)![](https://images2017.cnblogs.com/blog/890227/201708/890227-20170804104004944-1536647948.png)![](https://images2017.cnblogs.com/blog/890227/201708/890227-20170804104030834-1404862878.png)

  另外，还有一些不常用的，比如：左边的强度图（不同的颜色代表强度的不同）； 中间的各种仪表盘（指南针，速度计等）； 右边组合成的示波器等

![](https://images2017.cnblogs.com/blog/890227/201708/890227-20170804135639412-1292716510.png)![](https://images2017.cnblogs.com/blog/890227/201708/890227-20170804135824178-475682460.png)![](https://images2017.cnblogs.com/blog/890227/201708/890227-20170804140350412-1002107127.png)

  下面介绍在 Windows 系统下，如何下载、编译、安装 Qwt 到 Qt 中

## **1.1  下载**

　qwt-6.1.3 版本下载地址： [https://sourceforge.net/projects/qwt/files/qwt/](https://sourceforge.net/projects/qwt/files/qwt/) ，解压到  D:\Qt\qwt-6.1.3，更改 qwtconfig.pri 中的 #27

win32 {
    QWT_INSTALL_PREFIX    = D:/Qt/Qwt-$$QWT_VERSION
    # QWT_INSTALL_PREFIX = C:/Qwt-$$QWT_VERSION-qt-$$QT_VERSION
}
  注释掉 qwtbuild.pri 中的 #30

    #CONFIG        += debug_and_release
    CONFIG        += build_all
## **1.2  编译**

### **1.2.1  qmake**

  打开 Qt 5.9.1 64-bit for Desktop (MSVC 2015)，切换到 qwt 解压后的目录，输入 qmake  qwt.pro  进行编译，生成 Makefile 文件。

![](https://images2017.cnblogs.com/blog/890227/201708/890227-20170803154148147-2102850703.png)

  注意： 如果没有 qmake 命令，一般是环境变量没有配置，需要配置 qmake.exe 的所在路径，此处为 D:\Qt\Qt5.9.1\5.9.1\msvc2015_64\bin

  在执行该步骤时，提示 Project ERROR

![](https://images2017.cnblogs.com/blog/890227/201708/890227-20170803163626287-16017272.png)

  于是搜索到 cl 命令，发现其位于 D:\Program Files\Microsoft Visual Studio 14.0\VC\bin，则将该路径也配置到环境变量 path 中

### **1.2.2  nmake**

  打开 VS2015 x64 本机工具命令提示符，切换到 qwt 生成 Makefile 文件的目录，先输入 nmake，再输入 nmake install

![](https://images2015.cnblogs.com/blog/890227/201702/890227-20170206192959588-112171921.jpg)

### **1.2.3  examples**

  在 qwt 目录下，切换到 examples 目录， 先执行  qmake examples.pro，再执行 nmake

# **2  Qwtpolar**

  Qwtpolar 是 Qwt 的扩展，主要用来绘制极坐标下的函数图形，如下所示：

![](https://images2017.cnblogs.com/blog/890227/201708/890227-20170804141045631-307099784.png)

  qwtpolar-1.1.1 下载地址： [https://sourceforge.net/projects/qwtpolar/](https://sourceforge.net/projects/qwtpolar/)

  和 qwt 的安装步骤一致，解压后，首先修改 qwtpolarconfig.pri 和 qwtpolarbuild.pri 文件；再执行 qmake qwtpolar.pro；完毕后再依次执行 nmake，nmake install

# **3  配置**

## **3.1  额外配置工程**

  在每个 .pro 中，指定库文件(.lib) 和 头文件 (.h)

LIBS += -L$$PWD/../../Qt/qwt-6.1.3/lib/ -lqwtd
LIBS += -L$$PWD/../../Qt/qwtpolar-1.1.1/lib/ -lqwtpolard

INCLUDEPATH += $$PWD/../../Qt/qwt-6.1.3/src
INCLUDEPATH += $$PWD/../../Qt/qwtpolar-1.1.1/src
## **3.2  模块配置**

### **3.2.1  环境变量**

![](https://images2015.cnblogs.com/blog/890227/201702/890227-20170207103043276-640586881.png)

### **3.2.2  库文件**

  将 D:\Qt\qwt-6.1.3\lib 下的 qwt.lib 和 qwtd.lib 复制到 D:\Qt\Qt5.9.1\5.9.1\msvc2015_64\lib 目录下

  将 D:\Qt\qwt-6.1.3\lib 下的 qwt.dll 和 qwtd.dll 复制到 D:\Qt\Qt5.9.1\5.9.1\msvc2015_64\bin 目录下

  将 D:\Qt\qwt-6.1.3\designer\plugins\designer 下的 qwt_designer_plugin.dll 复制到 D:\Qt\Qt5.9.1\5.9.1\msvc2015_64\plugins\designer 目录下

  同理，将 qwtpolar-1.1.1 下的 文件复制到相同目录

### **3.2.3  头文件**

 在 D:\Qt\Qt5.9.1\5.9.1\msvc2015_64\include 目录下，新建 Qwt 文件夹，将 qwt 和 qwtpolar 下的头文件全部复制到此处

### **3.2.4  工程配置**

  以上步骤都执行完毕后，则当使用 qwt 和 qwtpolar 时，只需要添加对应的模块名称即可

CONFIG += qwt
CONFIG += qwtpolar

# **4  后续问题**

 按照以上步骤，在 Qt 5.7.1  Creator 中成功显示了 Qwt Widgets 和 QwtPolarPlot 的 ui 设计界面，而在 Qt 5.9.1 Creator 中却没有显示。

  另外，奇怪的是，在 Qt 5.9.1 中，单独打开  Designer，却可以显示二者，不知道是什么原因...

## 参考资料

  “Windows 环境下 Qwt 安装和使用”， [http://www.jianshu.com/p/d6fb96c3e6b0](http://www.jianshu.com/p/d6fb96c3e6b0)

  Qwt 安装笔记， [http://blog.csdn.net/liyuanbhu/article/details/40825065](http://blog.csdn.net/liyuanbhu/article/details/40825065)

