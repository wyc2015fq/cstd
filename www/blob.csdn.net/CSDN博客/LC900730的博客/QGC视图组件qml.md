# QGC视图组件qml - LC900730的博客 - CSDN博客
2017年09月13日 18:14:22[lc900730](https://me.csdn.net/LC900730)阅读数：311
### 主要视图qml
MainWindowHybrid.qml中主要是加载MainWindowInner.qml. 
定义了一个Connection
```
Loader{
    id:mainWindowInner
    anchors.fill:parent
    source:"MainWindowInner.qml"
    Connections{
        target :mainWindowInner.item
        onReallyClose:controller.reallyClose()  
    }
}
```
```
//AGCApplication.cc中
qmlRegisterSingletonType<"QGroundControl",1,0,"QGroundControl",qgroundcontrolQmlGlobalSingletonFactory>
```
```
qmlRegisterSingletonType<MavlinkQmlSingleton>("QGroundControl.Mavlink",1,0,"Mavlink",mavlinkQmlSingletonFactory);
//将MavlinkQmlSingleton这个类注册，唯一标识的url是QGroundControl.Mavlink。在QML中通过Mavlink可以引用。
```
```bash
qmlRegisterType<MavlinkQmlSingleton>("QGroundControl",1,0,"QGroundControl",mavlinkQmlSingletonFactory);
```
```
//QGroundControlQmlGlobal.cc
QGroundControlQmlGlobal::QGroundControlQmlGlobal(
QGCApplication *app,QGCToolbox * toolbox):QGCTool(app,toolbox),
...
_multiVehicleManager(NULL)
```
