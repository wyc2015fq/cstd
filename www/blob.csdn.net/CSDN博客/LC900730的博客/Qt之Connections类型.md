# Qt之Connections类型 - LC900730的博客 - CSDN博客
2017年08月30日 17:14:45[lc900730](https://me.csdn.net/LC900730)阅读数：267
### 使用Connections类型
QtQuick模块提供了Connections类型，用于连接外部对象的信号。Connections对象可以接受指定目标(target)的任意信号。如下面的代码没有在发出clicked信号的MouseArea内响应这个信号，而是通过Connections对象，在mouseArea外部处理信号。
```
import QtQuick 2.2
Rectangle {
    id:rect;
    width:100；
    height:100
    MouseArea{
        id:mouseArea
        anchors.fill:parent
    }
    Connections{
//Connections的target属性是mouseArea，因而这个Connections对象可以接收来自mouseArea的任意信号
//这里添加了onClicked信号处理器
        target:mouseArea
        onClicked:rect.color="red"
    }
}
```
