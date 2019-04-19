# 从头学Qt Quick（1） --体验快速构建动态效果界面 - xqhrs232的专栏 - CSDN博客
2017年07月26日 10:23:46[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：193
原文地址::[http://www.cnblogs.com/csulennon/p/4483711.html](http://www.cnblogs.com/csulennon/p/4483711.html)
相关文章
1、[从头学Qt
 Quick（2）-- QML语法从一个简单的例子说起](http://www.cnblogs.com/csulennon/p/4485768.html)----[http://www.cnblogs.com/csulennon/p/4485768.html](http://www.cnblogs.com/csulennon/p/4485768.html)
自2005年Qt4发布以来，Qt已经为成千上万的应用程序提供了框架服务，现在Qt已经基本上支持所有的开发平台了，这里面既包含了桌面、嵌入式领域，也包括了Android、IOS、WP等移动操作平台，甚至支持最新的Ubuntu Phone。
Qt Quick是一个UI技术组，Qt Quick本身主要包含了QML、JavaScript、 Qt C++三种技术。其中的主角就是QML（Qt Declarative Module）也是我打算在这个系列里面描述的主要对象。QML的主要作用我理解就是将界面设计与程序逻辑解耦，通常来说前端需求的变动远超过后台逻辑，因此界面与逻辑分离不仅有利于开发人员之间的分工，也提供了更快速的迭代速度的可能性，也会大大降低程序的后期维护成本。
这套技术的主要框架如下
![image](http://images.cnitblog.com/blog/426469/201505/070051264545105.png)
QML是一种简单的脚本语言，语法和CSS比较接近，因此学起来相当简单。QML最早出现是在Qt4.7版本中，从4.x过度到5.x算起来一共经过了十年的时间。最新的QML相对于4.x时代的QML有了很大的改进（下面列举都是Qt Quick 2.0的特性）：
> 
（1）基于OpenGL(ES)的场景实现，提高图形绘制的效率。
（2）QML和JavaScript主导UI的创建，后台图形绘制采用C++。高效，灵活，可扩展性强。
（3）跨平台：这里不能说是完整意义上的跨平台，这里的跨平台是指编译的跨平台“一次编写，到处编译”
这里我们首先创建一个简单的Qt Quick应用，对QML有一个初步的印象。
我们的目标是要创建一个鼠标点击风车图片后能在屏幕上转起来的程序，下面的代码可以随便看一下，看不懂没关系，按照我给出的步骤做出来体验一下就行。
![a](http://images.cnitblog.com/blog/426469/201505/070051304542417.gif)
新建一个QtQuick工程：
![image](http://images.cnitblog.com/blog/426469/201505/070051317829444.png)
![image](http://images.cnitblog.com/blog/426469/201505/070051332202673.png)
选择Qt Quick2.x
![image](http://images.cnitblog.com/blog/426469/201505/070051349541642.png)
创建完了大概是这个样子，点那个绿色的按钮就会出现一个简单的窗口：
![image](http://images.cnitblog.com/blog/426469/201505/070051377825411.png)
将两张图片素材放到工程的目录下
![image](http://images.cnitblog.com/blog/426469/201505/070051399542781.png)
将资源添加到Qt工程去，首先 创建一个资源前缀：
![image](http://images.cnitblog.com/blog/426469/201505/070051421269151.png)
![image](http://images.cnitblog.com/blog/426469/201505/070051433927963.png)
![image](http://images.cnitblog.com/blog/426469/201505/070051456576305.png)
将两张图片加进去
![image](http://images.cnitblog.com/blog/426469/201505/070051469854333.png)
编辑main.qml
```
import QtQuick 2.0
import QtQuick.Window 2.0
Window {
    id:root;
    visible: true;
    width: 600;
    height: 400;
    Image {
        id: bg
        width: root.width;
        height:root.height;
        source: "qrc:///images/bg.png";// 路径根据自己设置的修改
    }
    Image {
        id: wheel;
        anchors.centerIn: parent;
        source: "qrc:///images/wheel.png";// 路径根据自己设置的修改
        Behavior on rotation {
            NumberAnimation {
                duration: 5000
            }
        }
        MouseArea {
            width: wheel.width;
            height: wheel.height;
            onClicked: {
                wheel.rotation = 360;
            }
        }
    }
}
```
运行，是不是一个漂亮的窗口程序出现在了眼前呢？点击中间的风车就会转起来。
源代码下载：[http://download.csdn.net/detail/csulennon/8670283](http://download.csdn.net/detail/csulennon/8670283)
在接下来的文章中会介绍QML的基本语法。

