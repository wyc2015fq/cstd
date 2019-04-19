# QGC的ScreenToolsController - LC900730的博客 - CSDN博客
2017年09月18日 09:57:53[lc900730](https://me.csdn.net/LC900730)阅读数：387
## SCreentToolsController.cc
在QApplication.cpp中注册单例
```bash
qmlRegisterSingletonType<ScreenToolsController>     ("QGroundControl.ScreenToolsController",    1, 0, "ScreenToolsController",  screenToolsControllerSingletonFactory);
```
在ScreenTools.qml中
```
Item{
    id:_screenTools
    property real defaultPointsize : 10
    property real defaultFontPixelHeight : 10
    property bool isAndroid  : ScreenToolsController.isAndroid
    property bool isiOS : ScreenToolsController.isiOS
}
```
