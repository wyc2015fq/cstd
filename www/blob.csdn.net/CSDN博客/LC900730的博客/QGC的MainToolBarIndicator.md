# QGC的MainToolBarIndicator - LC900730的博客 - CSDN博客
2017年08月22日 12:21:46[lc900730](https://me.csdn.net/LC900730)阅读数：458
## QGC的MainToolBar
一直在疑惑，打开QGroundControl地面站软件，没有连接飞控的时候，会显示“Waiting For Vehicle Connection ”找不到来源 
最后是在Resources/qgroundcontrol.qrc/qml/QGroundControl/Controls/MainToolBarIndicators.qml中。 
MainToolBar.qml中定义了软件ToolBarButton，默认是5个
```
Row{
        id:viewRow;
        anchors.top:parent.top
        anchors.bottom:parent.bottom
        ExclusiveGroup{
            id:mainActionGroup
        }
        QGCToolButton{
            id:settingsButton
            anchors.top: parent.top
            exclusiveGroup:mainActionGroup
            source:"/res/QGCLogoWhite"
            logo:true
            onClicked:toolBar.showSettingsView()
            visible:!QGroundControl.corePlugin.options.combineSettingsAndSetup
        }
    }
```
在QGCApplication.cc中的
```
#include "PositionManager.h"
#include "FollowMe.h"
#include "MissionCommandTree.h"
...
...
...
qmlRegisterType<QGCPalette>("QGroundControl.Palette",1,0,"QGCPaletette");
qmlRegisterType<QGCMapPalette>("QGroundControl",1,0,"QGCMapPalette");
...
...
qmlRegisterUncreatableType<CoordinateVector>("QGroundControl",1,0,"CoordinateVector","Reference only");
qmlRegisterUncreatableType<RallyPointController>("QGroundControl.Controllers",1,0,"RallyPointController","Reference only");
...
...
```
### Loader
Loader用来动态加载QML组件，我们可以把Loader作为占位符号使用，在需要显示某个元素时，才使用Loader把它加载进来。
#### Loader详细介绍
Loader可以使用其source属性加载一个QML文档，也可以通过其sourceComponent属性加载一个Component对象。当需要延迟一些对象直到需要才创建时候，Loader非常有用。当Loader的source或者sourceComponent属性发生变化时候，它之前加载的Component会自动销毁，新对象会被加载。将source设置为一个空字符串或者将sourceCompoennt设置为undefined，将会销毁当前加载的对象，相关资源也会被释放，而Loader对象则变成一个空对象。 
- Loader的item属性
item属性指向它加载的组件顶层Item，如Loader加载了颜色选择组件，其item属性就指向颜色选择组件的Rectangle对象。对于Loader加载的Item，它暴露出来的接口，如属性、信号灯，都可以通过Loader的item属性来访问。
```
Loader{
    id:redLoader;
    anchors.left:parent.left;
    anchors.leftMargin:4;
    anchors.bottom:parent.bottom;
    anchors.bottomMargin:4;
    //sourceComponent属性来加载id为colorComponent的组件对象
    sourceComponent:colorComponent;
    //onLoad信号处理器使用item属性来设置颜色选择组件的颜色
    onLoaded:{
        item.color="red";
    }
}
```
