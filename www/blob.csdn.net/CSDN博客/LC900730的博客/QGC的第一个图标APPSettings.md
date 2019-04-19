# QGC的第一个图标APPSettings - LC900730的博客 - CSDN博客
2017年09月14日 18:11:11[lc900730](https://me.csdn.net/LC900730)阅读数：268
## Application Settings
```
Rectangle{
    id:settingsView
    ...
    Component.onCompleted:{
    //组件显示默认的一个
    //defaultSettings
    __rightPanel.source=QGroundControl.corePlugin.settingsPages[QGroundControl.corePlugin.defaultSettings].url
    }
QGCFlickable{
    id:
    width:
    ...
    flickableDirection:Flickable.VerticalFlick
    clip:
}
QGCLabel{
    ...
    ...
}
...
```
按钮的个数由corePlugin决定，体现在QGCCorePlugin.cc中
```
Repeater{
    model:QGroundControl.corePlugin.settingsPages;
    QGCButton{
            ...
            ...
        onClicked:{
            if(__rightPael.source!==modelData.url){
                __rightPanel.source=modelData.url
            }
        }
    }
}
```
![这里写图片描述](https://img-blog.csdn.net/20170919182102090?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
决定数目的具体体现在：
```
QVariantList &QGCCorePlugin::settingPages(){
    //如果没有被重写，创建默认设置
    if(_p->pGeneral){
    //第一个菜单项General
        _p->pGeneral=new QGCSettings(tr("General"),
         QUrl::fromUserInput("qrc:/qml/GeneralSettings.qml"),       QUrl::fromUserInput("qrc:/res/gear-white.svg"))
         //加入到settingsList
_p->settingsList.append(QVariant::fromValue((QGCSettings*)
_p->pGeneral);
    //第二个Commons Links
    ...
    ...
    //最后一个Debug
    ...
    return _p->settingsList
}
QGCSettings.h文件中：
class QGCSettings(QString title,QUrl url,QURL icon=QUrl());
```
在QGCCorePlugin.h头文件中 
定义了Q_PROPERTY(QVariantList settingsPages READ settingsPages NOTIFY settingsPagesChanged)
因此在AppSettings.qml
```
//初始化的时候
Component.onCompleted{
    __rightPanel.source=QGroundControl.corePlugin.settingPages[QGroundControl.corePlugin.defaultSettings].url
}
Repeater{
    //数据模型就来自上文
    model:QGroundControl.corePlugin.settingsPages;
    QGCButton{
        height:_buttonHeight
        text:modelData.title
        exclusiveGroup:panelActionGroup
        Layout.fillwidth:true
        onClicked{
            //当点击左侧面板
            //右侧面板的source随之改变
            if(__rightPanel.source!==modelData.url){
                __rightPanel.source=modelData.url   
            }
            checked=true;
        }
    }
}
```
总结：数据模型都是在QGCCorePlugin.cc中完成的； 
在APPSettings.qml中完成展示
