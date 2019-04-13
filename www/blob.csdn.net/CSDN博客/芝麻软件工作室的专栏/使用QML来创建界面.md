
# 使用QML来创建界面 -  芝麻软件工作室的专栏 - CSDN博客


2016年06月16日 17:26:06[seven-soft](https://me.csdn.net/softn)阅读数：1366


前面两节介绍了 Qt Widgets Application 工程，可以使用纯C++代码，或C++和XML结合的方式来创建GUI程序。这节来看看如何使用 QML。
Qt Quick Application、Qt Quick Controls Application 和 Qt Canvas 3D Application 工程都可以使用 QML 来创建界面。Quick Controls
 比 Quick 多一些控件，暂时也不会涉及3D动画，所以我们选择 Quick Controls 工程，如下图所示：![](http://c.biancheng.net/cpp/uploads/allimg/151210/1-151210113032B5.png)
与 Widgets 工程相比，Quick Control 多出了几个文件：![](http://c.biancheng.net/cpp/uploads/allimg/151210/1-151210115136153.png)
main.qml 是 QML 文件，它的后缀就是`.qml`。qml.qrc
 是资源文件，主要包含了 QML 文件的路径，以确保 QML 引擎能够找到文件。deployment.pri 是工程文件 QuickControlsDemo.pro 的辅助文件。
切换到设计模式，就可以从左侧的面板中看到 QML 的基本元素、控件、布局等，这些都可以拖拽到中间的编辑区。
main.qml 是可以编辑的，打开 main.qml，将下面的代码复制进去：import QtQuick 2.3
import QtQuick.Controls 1.2
ApplicationWindow {
    visible: true
    width: 400
    height: 210
    color: "\#ffffff"
    title: qsTr("QuickControlsDemo")
    Text {
        id: text1
        x: 47
        y: 35
        width: 80
        height: 30
        text: qsTr("用户名：")
        horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 14
    }
    TextField {
        id: textField1
        x: 137
        y: 35
        width: 200
        height: 30
        placeholderText: qsTr("请输入用户名")
    }
    Text {
        id: text2
        x: 47
        y: 85
        width: 80
        height: 30
        text: qsTr("密  码：")
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 14
        horizontalAlignment: Text.AlignRight
    }
    TextField {
        id: textField2
        x: 137
        y: 85
        width: 200
        height: 30
        echoMode: 2
        placeholderText: qsTr("请输入密码")
    }
    Button {
        id: button1
        x: 71
        y: 145
        width: 116
        height: 36
        text: qsTr("登 录")
    }
    Button {
        id: button2
        x: 224
        y: 145
        width: 110
        height: 36
        text: qsTr("取 消")
    }
}再切换到设计模式，就可以看到一个简单的登录框。运行一下就可以看到效果：![](http://c.biancheng.net/cpp/uploads/allimg/151210/1-1512101226302G.png)
上面我们只是用 QML 绘制了界面，并没有使用 JavaScript。

