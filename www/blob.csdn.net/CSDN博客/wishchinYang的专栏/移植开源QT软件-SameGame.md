# 移植开源QT软件-SameGame - wishchinYang的专栏 - CSDN博客
2015年01月22日 09:39:54[wishchin](https://me.csdn.net/wishchin)阅读数：946
前言：
        QML是一种描述性的[脚本](http://baike.baidu.com/view/54.htm)语言，[文件格式](http://baike.baidu.com/view/1066926.htm)以.qml结尾。语法格式非常像CSS（参考后文具体例子），但又支持javascript形式的编程控制。我个人认为它结合了QtDesigner
 UI和QtScript的优点。QtDesigner可以设计出·ui界面文件，但是**不支持和Qt原生C++代码的交互**。不过也没关系，本身MVC分离了最好。
        QtScript可以和Qt原生代码进行交互，但是有一个缺点，如果要在[脚本](http://baike.baidu.com/view/54.htm)中创建一个继承于QObject的图形对象非常不方便，只能在Qt代码中创建图形对象，然后从QtScript中进行访问。而QML可以在[脚本](http://baike.baidu.com/view/54.htm)里创建图形对象，并且支持各种图形特效，以及[状态机](http://baike.baidu.com/view/1906565.htm)等，同时又能跟Qt写的C++代码进行方便的交互，使用起来非常方便。
        一般使用C++编写计算密集型模块，作为object插件的形式被QML使用，再使用JavaScript编写逻辑模块，做到模块分离。
移植QtSameGame：
[QtSameGame在Qt5-Sample帮助页](http://doc.qt.io/qt-5/qml-advtutorial.html)。
        QtSameGame在Qt5-Sample里面不是单独的工程创立的，不能原班移植。参考这篇文章：[一步一步学Qt（Qt Quick)/qml 开发第五篇](http://www.tuicool.com/articles/AFFzey)，建立一个基于Qt Quick Controls1.2
 的工程。不能基于ＱＭＬ，不知道为何。
　　依照　参考文章里面的指示添加Content里面的文件，修改：
```cpp
ApplicationWindow {
    //Rectangle {
```
       可以编译运行。
       但是失去了state属性，又一次修改为
```cpp
//ApplicationWindow {
    Rectangle {
```
        程序可以正常运行
可能性错误：[During sturtup program exited with code 0xc0000139](http://blog.csdn.net/love_007abc/article/details/39989241)
       解决方案：分清MinGW的DLL文件和Qt5里面的Dll文件；
                           若使用了第三方库，用所使用的QT版本重新编译一次，如qwt这样的第三方的库；
注意：
        将QML和其中的图片放入资源中,这里引用QML的时候需要以qrc开头,否则访问不到图片文件.
        资源添加过程中在哪个位置加入是个问题.
争议之处：
```cpp
import "content/samegame.js" as Logic
```
        使用JS作为控制系统效率问题？逻辑与计算分离。
参考文章：
**[Rectangle     属性列表页](http://qt-project.org/doc/qt-4.8/qml-rectangle-members.html)**
# [ApplicationWindow属性页](http://doc-snapshot.qt-project.org/qt5-release/qtquickcontrols/qml-qtquick-controls1-applicationwindow-members.html)
# [QML官方系列教程——Using the Qt Quick Particle System使用粒子系统](http://blog.csdn.net/cloud_castle/article/details/30764359)
# [QML官方系列教程——Scalability 使用ApplicationWindow：适配不同分辨率设备。](http://blog.csdn.net/cloud_castle/article/details/30057169)
Qt官方教程系列，此系列翻译得较为详细，非常值得参考：[Qt官方教程系列](http://blog.csdn.net/cloud_castle/article/category/2320127)
# Qt Quick新特性概览：[最明显的就是新的QtQuick.Controls模块的引入](http://blog.qt.digia.com/cn/2013/06/24/overview-of-the-new-features-in-qt-quick/)
此文章介绍了：**在窗口部件中嵌入Qt Quick**
        最终我们为在一个现有的基于窗口部件的应用程序中包含一个Qt Quick 2场景添加了支持。这是通过使用新的[QWidget::createWindowContainer()](http://doc-snapshot.qt-project.org/qt5-dev/qtwidgets/qwidget.html#createWindowContainer)函数来嵌入一个QQuickWindow的。
QML范例详解：[使用JS编写控制系统，使用C++进行计算。](http://blog.csdn.net/henreash/article/details/7952175)
[Rectangle详细解释](http://blog.csdn.net/foruok/article/details/28859415)
