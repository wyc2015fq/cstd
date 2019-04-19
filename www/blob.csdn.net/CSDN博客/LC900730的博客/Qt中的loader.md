# Qt中的loader - LC900730的博客 - CSDN博客
2017年08月30日 21:07:57[lc900730](https://me.csdn.net/LC900730)阅读数：514
### 使用Loader
Loader用来动态加载QML组件，可以看做是一种占位符，可以加载一个QML文件(使用source属性)或者一个组件对象(使用sourceComponent属性)，Loader主要用于延迟组件的创建，使一个组件的创建被延迟到真正需要的时候。 
如只有在MouseArea单击鼠标才加载Page1.qml文件
```
import QtQuick 2.2
Item {
    width:200;
    height:200;
    Loader{
        id:pageLoader
    }
    MouseArea{
        anchors.fill:parent
        onClicked:pageLoader.source="Page1.qml"
    }
}
```
### .item属性
//第一个文件
```
//Appl.qml
Item{
    width:100;
    height:100;
    Loader{
        id:myLoader;
        source:"MyItem.qml"
    }
    Connections{
        target:myLoader.item
        onMessage:console.log(msg)
    }
}
```
//第二个文件
```
//MyItem.qml
Rectangle {
    id:myItem;
    signal message(string msg);
    width:100
    height:100
    color:"red";
    MouseArea{
        anchors.fill:parent
        onClicked:myItem.message("clicked");
    }
}
```
在另一个qml中，直接引入Appl，则会将MyItem组件引入。 
当鼠标点击的时候，调用message信号，而Appl响应这个信号。因为MyItem.qml是在Loader的作用域中被加载的，所以它可以调用在Loader或者其父项目中的定义的任何函数。
//Loader的id是myLoader，myLoader.item实际上是这个MyItem组件。
