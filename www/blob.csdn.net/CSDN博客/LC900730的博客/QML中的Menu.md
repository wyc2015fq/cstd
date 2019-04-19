# QML中的Menu - LC900730的博客 - CSDN博客
2017年09月24日 14:56:58[lc900730](https://me.csdn.net/LC900730)阅读数：1408
menu必须依附于MenuBar的存在，否则不会显示。
### 动态添加菜单项
如给id为testitem的Menu添加一个item
`testitem.additem(qsTr("item3"));`
### 动态添加的菜单绑定事件
```java
//类似于绑定一个匿名函数代码块
testitem.additem(qsTr("item3")).triggered.connected(function(){
});
```
```
import QtQuick 2.6
import QtQuick.Controls 1.4
import Qtquick.Window 2.2
Window {
    visible:true;
    width:640
    height:480
    title:qsTr("Hello world");
    MenuBar {
        Menu{
            id:testitem;
            title:"hello";
            MenuItem{
                text:'item1';
            }
            MenuItem{
                text:'item2';
            }
        }
    }
}
MouseArea{
    anchors.fill:parent
    onClicked:{
        testitem.addItem(qsTr("item3")).triggered.connected(function(){
            console.log("hello world");
        });
    }
}
```
注意：如果一个menu初始化没有一个menuitem，那么会不显示。即使使用动态additem也不会起作用。
