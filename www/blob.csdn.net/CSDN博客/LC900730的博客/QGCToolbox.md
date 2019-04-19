# QGCToolbox - LC900730的博客 - CSDN博客
2017年08月16日 23:22:47[lc900730](https://me.csdn.net/LC900730)阅读数：281
## QGCTool
`    QGCTool::QGCTool(QGCApplication *app,QGCToolbox *toolbox):QObject(toolbox),_app(app),_toolbox(NULL)`
### QGCToolbox头文件
```
class FactSystem;
    ...
    class SettingsManager;
//管理我们顶层所有的service/tools
    class QGCToolbox: public QObject{
        Q_OBJECT
    public:
        QGCToolbox(QGCApplication *app);
        FirmwarePluginManager * firmwarePluginManager(void)
    {
        return _firmwarePluginManager;
    }       
        GAudioOutput * audioOutput(void)
    {
        return _audioOutput;
    }       
...
private:
    void setChildToolboxes(void);
    void _scanAndLoadPlugins(QGCApplication *app);
    ...
    GAudioOutput * _audioOutput;
    ...
    SettingsManager * _settingsManager;
    friend  class QGCApplication;
};
```
### QGCToolbox源文件
```
QGCToolbox::QGCToolbox(QGCApplication *app)
    : _audioOutput(NULL)
    , _factSystem(NULL)
    , _firmwarePluginManager(NULL)
#ifndef __mobile__
    , _gpsManager(NULL)
#endif
    , _imageProvider(NULL)
    , _joystickManager(NULL)
    , _linkManagr(NULL)
    , _mavlinkProtocol(NULL)
    , _missionCommandtree(NULL)
    , _multiVehicleManager(NULL)
    , _mapEngineManager(NULL)
    , _uasMessageHandler(NULL)
    , _followMe(NULL)
    , _qgcPositionManager(NULL)
    , _videoManager(NULL)
    , _mavlinkLogManager(NULL)
    , _corePlugin(NULL)
    , _settingsManager(NULL)
{
    //settingsManager必须是第一步，因此设置能够调用其他工具
    _settingsManager=new SettingsManager(app,this);
    //扫描加载插件
    _scanAndLoadPlugins(app);
    //上述变量初始化
    _audioOutput =new GAudioOutput(app,this);
}
```
### QGCToolbox::setChildToolboxes(void)
```
//SettingsManager必须是第一步设置的
    _settingsManager->setToolbox(this);
...
    _corePlugin->setToolbox(this);
...
    _mavlinkLogManager->setToolbox(this);
```
### 加载插件
```
QGCToolbox::_scanAndLoadPlugins(QGCApplication* app){
#if defined(QGC_CUSTOM_BUILD)
    //使用自定义的插件
    _corePlugin=(QGCCorePlugin *) new CUSTOMCLASS(app,app->toolbox());
    if(_corePlugin){
        retutn ;
    }
#endif
    _corePlugin=new QGCCorePlugin(app,app->toolbox());
}
}
```
