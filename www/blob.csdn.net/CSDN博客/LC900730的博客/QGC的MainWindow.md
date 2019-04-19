# QGC的MainWindow - LC900730的博客 - CSDN博客
2017年08月19日 22:13:34[lc900730](https://me.csdn.net/LC900730)阅读数：221
## MainWindow.cc
```
class MainWindows:public QMainWindow{
    Q_OBJECT
public:
    //返回mainwindow单例
    static MainWindow * instance(void);
    //删除MainWindow单例
    void deleteInstance(void);
    //
    static MainWindow* _create();
//构造函数是私有的，创建mainwindow必须通过MainWindow::_create() 
//头文件中定义了
QMap<QString,QGCDockWidget *> _mapName2DockWidget;
QMap<QString,QAction *>  _mapName2Action;
}
```
在Resources/qgroundcontrol.qrc/qml下面
GeneralSettings.qml
```
QGCView{
property Fact _percentRemainingAnnounce: 
QGroundControl.settingsManager.appSettings.batteryPercentRemainingAnnounce
//地图的提供者
property Fact _mapProvider 
QGroundControl.settingsManager.flightMapSettings.mapProvider
//地图的类型
property Fact _mapType QGroundControl.settingsManager.flightMapSettings.mapType
}
```
在Resources/qgroundcontrol.qrc/qml/QGroundControl下面的GeneralSettings，是点击最左上角出现的视图 
里面有各种参数的设置
Comm   Links在Resources/qgroundcontrol.qrc/qml/QGroundControl下面的LinkSettings.qml
在点击Offline Maps时候，Add New Set时候，出现配置里面的所有地图类型
