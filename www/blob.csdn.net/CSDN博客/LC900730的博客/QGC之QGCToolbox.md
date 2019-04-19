# QGC之QGCToolbox - LC900730的博客 - CSDN博客
2017年09月19日 10:40:07[lc900730](https://me.csdn.net/LC900730)阅读数：320
## 负责加载各个工具组件，如GpsManager，MapEngineManager
将所有的都作为QGCToolbox的子Toolbox
在QGCApplication.cc的构造方法中，最后2行就是
```
//初始化见下面
_toolbox=new QGCToolbox(this);
_toolbox->setChildToolboxes();
```
## QGCToolbox的构造函数
```
QGCToolbox::QGCToolbox(QGCApplication*app)
    :_audioOutput(NULL)
    ,_factSystem(NULL)
    ,_firmwarePluginManager(NULL)
#ifndef __mobile__
    ,_gpsManager(NULL)
#endif  
    ...
    ...
    {
        //必须是第一步加载的
       _settingsManager=new SettingsManager(app,this);
        //扫描加载插件
        _scanAndLoadPlugins(app);
       _audioOutput=new GAudioOutput(app,this);
       _factSystem=new FactSystem(app,this);
    }
```
