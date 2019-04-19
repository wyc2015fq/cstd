# Qt Quick学习笔记(四) - LC900730的博客 - CSDN博客
2017年08月20日 19:03:57[lc900730](https://me.csdn.net/LC900730)阅读数：136
## 第六章
Qt Quick最大的特定就是与Qt元对象系统的结合。
#### Connections对象
一个Connections对象创建一个到QML信号的连接
```
Connections{
    target:area;
    ``on<Signal>``:function or code block;
}
```
```
Button{
        id:changeButton;
        anchors.horizontalCenter:parent.horizontalCenter;
        ...
        ...
    }
    Connections{
        target:changeButton;
        onClicked:{
            text1.color="xxx";
            text2.color="xxx";
        }
    }
```
