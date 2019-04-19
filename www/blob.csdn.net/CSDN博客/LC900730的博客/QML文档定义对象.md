# QML文档定义对象 - LC900730的博客 - CSDN博客
2017年08月31日 16:08:40[lc900730](https://me.csdn.net/LC900730)阅读数：197
### 通过QML文档定义对象类型
```xml
将一个QML文档放置到一个以<TypeName>.qml命名的文本文件中，这里<TypeName>是类型的名称，必须以大写字母开头，这个文档会自动被引擎识别为一个QML类型的定义。
//引擎解析QML类型名称时候，需要搜索相同的目录，所以这种方式定义的类型，同一目录中的其他QML文件会被自动设置为可用的。
```
如2个QML文件，下
```
//SquareButton.qml
import QtQuick 2.2
Rectangle{
    width:100；height:100;color:"red";
    MouseArea{
        anchors.fill:parent
        onClicked:console.log("Button clicked");
    }
}
```
由于文件名称是SquareButton.qml，因此可以被同一目录下的其他QML文件作为SqureButton类型使用。如
```java
//myapplication.qml
import QtQuick 2.2
SquareButton{}
```
当myapplication.qml文档被引擎加载时，它会将SquareButton.qml作为一个组件进行加载，并对其实例化来创建一个SquareButton对象。QML引擎从这个类型实例出一个SquareButton对象。
### 自定义类型访问特性
如SquareButton.qml文件的根对象类型是Rectangle，这意味着在Rectangle类型中定义的所有的属性都可以被SquareButton对象修改。
### 自定义属性、方法、信号
```
//SquareButton.qml
import QtQuick 2.2
Rectangle{
    id:root;
    property bool pressed:mouseArea.pressed;
    signal buttonClicked(real xPos , real yPos);
    function randomizeColor(){
      root.color= Qt.rgba(Math.random(),Math.random(),Math.random(),1);
    }
    width:100；height:100;color:"red";
    MouseArea{
        anchors.fill:parent
        onClicked:console.log("Button clicked");
    }
}
```
所有的SquareButton都可以使用上面定义的pressd属性、buttonClicked信号、randomizeColor()方法。 
如：
```
import QtQuick 2.0
SquareButton{
    id:squareButton;
    onButtonClicked:{
        console.log("Clicked",xPos,yPos);
        randomizeColor()
    }
    Text {
        text:squareButton.pressed?"Down":"Up";
    }
}
```
SquareButton.qml中定义的任何一个id值都不能在SquareButton对象中进行访问。因为id只能在组件作用域中进行访问。
