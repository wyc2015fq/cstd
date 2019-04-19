# Qt Quick学习---------------构建一个简单的qml程序 - xqhrs232的专栏 - CSDN博客
2017年07月26日 10:53:23[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1752
原文地址::[http://blog.csdn.net/u011619422/article/details/48050939](http://blog.csdn.net/u011619422/article/details/48050939)
相关文章
1、QML和C++混合编程小例子----[http://download.csdn.net/detail/yeqingbo2010/9090749](http://download.csdn.net/detail/yeqingbo2010/9090749)
2、体验QML演示程序----[http://blog.sina.com.cn/s/blog_d01f29a10101m8b3.html](http://blog.sina.com.cn/s/blog_d01f29a10101m8b3.html)
3、
## Qt UI Demos
 ----[http://blog.sina.com.cn/s/blog_d01f29a10101m8b2.html](http://blog.sina.com.cn/s/blog_d01f29a10101m8b2.html)
4、QML Qt4.7超炫经典实例1 源代码----[http://www.linuxidc.com/Linux/2010-08/27447.htm](http://www.linuxidc.com/Linux/2010-08/27447.htm)
5、[Qt之QML开发常用知识](http://www.cnblogs.com/yanhuiw/p/3972692.html)----[http://www.cnblogs.com/yanhuiw/p/3972692.html](http://www.cnblogs.com/yanhuiw/p/3972692.html)
6、**QML程序的发布----[http://cpp51.blog.51cto.com/5346598/1427566](http://cpp51.blog.51cto.com/5346598/1427566)**
我是在工作过程中才开始学习Qt Quick的，写这个的原因就是为了以后方便自己查找需要的资料。
qml的语法有点类似css，但是又可以像[JavaScript](http://lib.csdn.net/base/javascript)那样的流程控制。在界面开发中，能够做到快速[敏捷](http://lib.csdn.net/base/agile)的开发（ps：虽然我也是在书上看到的）。
在本次的学习中，我们要学会如何去构建并运行一个简单的qml程序。
1.打开Qt Crteater，选择新建文件和项目
2.选择Qt Quick Appliction 
3.修改项目名称，以及路径
4.Qt Quick版本选择2.2
就完成了对一个简单的程序的创建了。
这个程序自动生成了以下的东西：
deployment.pri   类似Qt中的.pro文件，都是用来包含库等东西的 
main.cpp   用来运行qml窗口的
main.qml
下面来看下代码：
首先是main.cpp
**[cpp]**[view
 plain](http://blog.csdn.net/u011619422/article/details/48050939#)[copy](http://blog.csdn.net/u011619422/article/details/48050939#)
- #include <QGuiApplication>
- #include <QQmlApplicationEngine>
- 
- int main(int argc, char *argv[])  
- {  
-     QGuiApplication app(argc, argv);  
- 
-     QQmlApplicationEngine engine;  
-     engine.load(QUrl(QStringLiteral("qrc:/main.qml")));  
- 
- return app.exec();  
- }  
包含了qml库，窗口库等。
创建了一个一个窗口， 创建了一个qml窗口，挂起窗口等待qml窗口
下面就是重点了，main.qml
**[cpp]**[view
 plain](http://blog.csdn.net/u011619422/article/details/48050939#)[copy](http://blog.csdn.net/u011619422/article/details/48050939#)
- import QtQuick 2.2  
- import QtQuick.Window 2.1  
- 
- Window {  
-     visible: true
-     width: 360  
-     height: 360  
- 
-     MouseArea {  
-         anchors.fill: parent  
-         onClicked: {  
-             Qt.quit();  
-         }  
-     }  
- 
-     Text {  
-         text: qsTr("Hello QtQuick App")  
-         anchors.centerIn: parent  
-     }  
- }  
1.inport QtQuick 2.2  类似于[Java](http://lib.csdn.net/base/java)中导入包的写法
2. Window{} 创建了一个窗口，在里面包含了窗口的大小，是否可见，    以及鼠标事件处理等，  最后还在窗口中心添加了一个Hello QtQuick App的文本
以上就是一个简单的qml程序的全部了
