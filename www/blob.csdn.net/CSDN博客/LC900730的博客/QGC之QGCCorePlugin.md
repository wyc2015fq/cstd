# QGC之QGCCorePlugin - LC900730的博客 - CSDN博客
2017年08月19日 21:02:28[lc900730](https://me.csdn.net/LC900730)阅读数：200
## QGC核心控件的加载以及初始化
QGCCorePlugin
```
class QGCCorePlugin :public QGCTool{
    Q_OBJECT
public:
    QGCCorePlugin(QGCApplication *app,QGCToolbox,toolbox);
    ~QGCCorePlugin();
    ...
    Q_PROPERTY(QVariantList settingsPages READ settingsPages NOTIFY settingsPagesChanged)
    ...
    ...
    ...
    //定义一堆虚函数
    virtual QVariantList &settingsPages(void);
    ...
    ...
    ...
    virtual bool mavlinkMessage(Vehicle * vehicle,LinkInterface *link,mavlink_message_t message);
//重写父类的setToolbox方法
void setToolbox*(QGCToolbox *toolbox);
}
```
## QGroundControlQmlGlobal
继承自public QGCTool
```
//飞行图的缩放级别
double QGroundControlQmlGlobal::flightMapZoom(void){
    QSettings settings;
    settings.beginGroup(_flightMapPositionSettingsGroup);
    return settings.value(_flightMapZoomSettingsKey,2).toDouble();
}
//设置飞行图的位置
QGroundControlQmlGlobal::setFlightMapPosition(QGeoCoordinate& coordinate){
    if(coordinate!=flightMapPosition()){
        //如果位置不相同
        QSettings settings;
        //beginGroup的目的
        settings.beginGroup(_flightMapPositionSettingsGroup);
//设置经度  
    settings.setValue(_flightMapPositionLatitudeSettingsKey,coordinate.latitude());
//纬度    
    settings.setValue(_flightMapPositionLongitudeSettingsKey,coordinate.longitude());
    emit flightMapPositionChanged(coordinate);
    }
}
//设置飞行图缩放级别
QGroundControlQmlGlobal::setFlightMapZoom(double Zoom){
    if(zoom!=flightMapZoom()){
        //如果位置不相同
        QSettings settings;
        //beginGroup的目的
        settings.beginGroup(_flightMapPositionSettingsGroup);
        //设置缩放
        settings.setValue(_flightMapZoomSettingsKey,zoom);
        //发射信号，缩放级别发生变化
        emit flightMapZoomChanged(zoom);
    }
}
```
