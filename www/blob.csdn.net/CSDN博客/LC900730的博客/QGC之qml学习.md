# QGC之qml学习 - LC900730的博客 - CSDN博客
2017年08月22日 23:21:25[lc900730](https://me.csdn.net/LC900730)阅读数：235
## QGCToolBarButton
这个控件是QGCToolBarButton.qml中。 
在qgroundcontrol.qrc/qml/QGroundControl/Controls/QGCToolBarButton.qml中。 
在MainToolBar.qml程序的主界面中：引入了这5个QGCToolBarButton 
在一个qrc文件维护的qml中，如果qresource prefix相同的情况下
```xml
<qresource prefix="/">
    <file>main.qml</file>
    <file>MyButton.qml</file>
</qresource>
```
每个目录下有一个qmldir的文件，格式如下： 
Module QGroundControl.Controls
AnalyzePage  1.0  AnalyzePage.qml 
AppMessage  1.0  AppMessage.qml
