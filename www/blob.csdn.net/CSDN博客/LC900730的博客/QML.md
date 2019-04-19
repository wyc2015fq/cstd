# QML - LC900730的博客 - CSDN博客
2017年08月18日 16:29:16[lc900730](https://me.csdn.net/LC900730)阅读数：214
## qml
QML文件的后缀是qml，其实是个文本文件。
```
#include "Map.hpp"
Map::Map(QObject *parent) : QObject(parent)
{
    mManager = std::make_shared<QNetworkAccessManager>();
    mMapProvider = std::make_shared<MapProvider>();
    connect(mManager.get(), &QNetworkAccessManager::finished, this, &Map::replaySlot);
}
MapProvider *Map::getProvider()
{
    return mMapProvider.get();
}
void Map::get(QUrl url)
{
    mManager->get(QNetworkRequest(QUrl(url)));
}
void Map::replaySlot(QNetworkReply *reply)
{
    mMapProvider->setData(reply->readAll());
    emit replayReady();
}
```
import QtQuick 2.0 导入qtquick2.0模块。 
Rectangle{}语句，定义了一个类型为Rectangle的对象。QML与json类似，对象要用一对花括号来描述。在花括号之间，是对象的属性描述，“property”：“value”形式。
QML中属性就是成员变量。
附加属性和类型
```
import QtQuick 2.0
    Item{
        width:100;
        height:100;
        focus:true;
        Keys.enabled:false;
        Keys.onReturnPressed:console.log("Return key was pressed");
    }
    //普通信号处理器，你需要先知道名字，在按照on{Signal}的语法来定义信号处理器的名字；而附加信号处理器，只需要通过附加类型名字引用它，把代码块赋值给它即可。
```
Qt Quick作为QML标准库，提供了很多基本元素和控件来帮助我们构建Qt Quick应用。
包含以下主题： 
- 可视化的Canvas
- 用户输入
- 元素定位与布局
- 状态、过度和动画
- 数据模型、视图和数据存储
- 粒子和图形特效
- 方便类型
要使用Qt Quick，必须知道怎样使用QML语言来撰写QML应用。QML是对js的一种扩展，是一种说明性的语言，用来描述基于Qt对象系统的用户界面。QML提供了高可读的、声明式的、类json的语法，支持动态属性绑定的js表达式。
### qrc文件
qt资源文件是一个与平台无关的方法将二进制文件保存到可执行程序内，对于你的应用程序如果常常需要一些文件(比如图像文件、译文等),并且不希望这些文件有丢失的危险，使用Qt资源文件是很有用的。
1.将图像资源等都包含在一个文件夹中，如images，在QtCreater中创建qrc文件方法
```
添加新文件--Qt---Qt资源文件
```
在资源文件中添加前缀、文件等。
### Qt quick
是与Qt元对象系统的结合。
```
Button{
    text:"Quit";
    anchors.centerIn:parent;
    onClicked:{
        Qt.quick();
    }
}
```
onClicked就是个信号处理器，其实等价于qt中的槽，这个代码块类似于匿名函数，除了定义的地方，没人能调用它，它是私有的。代码块就是一系列语句的组合，它的作用就是使得语句序列一起执行。
### Connections
一个Connections对象创建一个到QML信号的连接 
如果 
- 需要将多个对象连接到一个QML信号上
- 你需要在发出信号的对象的作用域之外哎建立连接
- 发射信号的对象没有在QML中定义(可能是C++中导出的)
Connections有一个属性名称为target，指向发出信号的对象
```
Button{
    id:changeButton;
    anchors.top:text2.bottom;
    text:"Change";
}
Connections{
    target:changeButton;
    onClicked:function or code block;
}
//在上述Connections对象中指定target对象为changeButton(Change按钮的id)，然后定义了一个onClicked信号处理器。即它是一个槽函数，响应changeButton这个对象的click事件
```
### 定义自己的信号
比如需要通知的别的对象你的状态发生了变化 
使用signal关键字。 
信号其实是个方法(函数),所以其发射还是通过调用信号为名的方法达成的 。
```
import QtQuick 2.0  
import QtQuick.Controls 1.1  
Rectangle {  
    width: 320;  
    height: 240;  
    color: "#C0C0C0";  
    Text {  
        id: coloredText;  
        anchors.horizontalCenter: parent.horizontalCenter;  
        anchors.top: parent.top;  
        anchors.topMargin: 4;  
        text: "Hello World!";  
        font.pixelSize: 32;  
    }  
    Component {  
        id: colorComponent;  
        Rectangle {  
            id: colorPicker;  
            width: 50;  
            height: 30;  
           signal  colorPicked(color clr);  
            MouseArea {  
                anchors.fill: parent  
                onPressed: colorPicker.colorPicked(colorPicker.color);  
            }  
        }  
    }  
    Loader{  
        id: redLoader;  
        anchors.left: parent.left;  
        anchors.leftMargin: 4;  
        anchors.bottom: parent.bottom;  
        anchors.bottomMargin: 4;  
        sourceComponent: colorComponent;  
        onLoaded:{  
            item.color = "red";  
        }  
    }
```
### 信号连接方法
信号处理器和信号是一对一的关系，而signal对象的这种连接方式，使得一个信号能够连接多个方法。
```
Rectangle {  
    id: relay;  
    signal messageReceived(string person, string notice);  
    Component.onCompleted: {  
        relay.messageReceived.connect(sendToPost);  
        relay.messageReceived.connect(sendToTelegraph);  
        relay.messageReceived.connect(sendToEmail);  
        relay.messageReceived("Tom", "Happy Birthday");  
    }  
    function sendToPost(person, notice) {  
        console.log("Sending to post: " + person + ", " + notice);  
    }  
    function sendToTelegraph(person, notice) {  
        console.log("Sending to telegraph: " + person + ", " + notice);  
    }  
    function sendToEmail(person, notice) {  
        console.log("Sending to email: " + person + ", " + notice);  
    }  
}
```
### 信号连接信号
```
Rectangle {  
    id: forwarder;  
    width: 100;   
    height: 100;  
    signal send();  
    onSend: console.log("Send clicked");  
    MouseArea {  
        id: mousearea;  
        anchors.fill: parent;  
        onClicked: console.log("MouseArea clicked");  
    }  
    Component.onCompleted: {  
        mousearea.clicked.connect(send);  
    }  
}
```
### Components(组件)
Components是由Qt框架或开发者封装好的，只暴露了必要接口的QML类型，可以重复利用。一个QML组件就像一个黑盒子，它通过属性、信号、函数和外部世界交互。
一个Component即可以定义在独立的qml文件中，也可以嵌入到其他qml文档中来定义。通常： 
一个component比较小，且只在某个qml文档中使用或者一个Component从逻辑上看从属于某个qml文档，那就可以采用嵌入的方式来定义该Component。
#### Loader
loader是专门用来动态创建组件的，它可以从qml文件中创建组件，也可以指定sourceComponent来创建。
