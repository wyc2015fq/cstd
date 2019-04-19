# QGC - LC900730的博客 - CSDN博客
2017年08月16日 00:20:15[lc900730](https://me.csdn.net/LC900730)阅读数：463
## 记录学习知识点
### QGCApplication头文件
```
QGCApplication：public 
#ifdef __mobile__
    QGuiApplication
#else
    QApplication
#endif
{
    Q_OBJECT
public:
    QGCApplication(int &argc,char *argv[],bool unitTesting);
    ~QGCApplication();
    //设置标志位，在下次启动QGC时候删除所有设置
    void deleteAllSettingsNextBoot(void);
    //清除上述设置的标志位
    void clearDeleteAllSettingsNextBoot(void);
    //如果unittest运行，那么返回true
    bool runningUnitTests(void){
        return _runningUnitTests;
    }
    //缺少参数提醒（也许会多次调用）
    void reportMissingParameter(int componentId,const QString &name);
    //展示非模态消息
    void showMessage(const QString &mesage);
    //fake ui into showing mobile interface
    bool fakeMobile(void ){
        return _fakeMobile;
    }
...
    QGCToolbox * toolbox(void){
        return _toolbox;
    }
    //是否有蓝牙支持
    bool isBluetoothAvailable(){
        return _bluetoothAvailable;
    }
    bool isInternetAvailable();
//slots相关
public slots:
    //可以从其他线程connect to这个槽展示information
    void informationMessageBoxOnMainThread(const QString &title,const QString &msg);
    //可以从其他线程connect to这个槽展示warning
    void warningMessageBoxOnMainThread(const QString &title,const QString &msg);
    //可以从其他线程connect to这个槽展示critical
    void criticalMessageBoxOnMainThread(const QString &title,const QString &msg);
    void showSetupView(void);
    void qmlAttemptWindow(void);
    //保存指定的遥感记录
    void saveTelemetryLogOnMainThread(QString tempLogfile);
    //查看遥感日志保存的路径是否正确
    void checkTelemetrySavePathOnMainThread(void);
signals：
    //与mavlinkprotocol::checkForLostLogFiles,signal this to ourselves 调用mavlinkprotocol线程的槽函数
    void checkForLostLogFiles(void);
public:
    //尽管声明public，但是这些方法内部使用，只应该由UnitTest代码调用
    //执行常规应用程序的初始化操作和unit test
    //尽管public，但是只应该由main函数调用
    void _initCommon(void);
    //执行常规应用程序的初始化操作，但是不初始化unit test
    //main调用
    void _initForNormalAppBoot(void);
    //执行unit test初始化，但是不执行常规应用程序的初始化操作(代码中原文档注释有误)
    //main调用
    void _initForUnitTests(void);
    void _loadCurrentStyleSheet(void)
    static QGCApplication * _app;  //单例，应该由gqcApp  directly 引用
public:
    //应该只由UnitTest调用
    //Shutdown application对象
    void _shutdown();
    bool _checkTelemetrySavePath(bool useMessageBox);
private slots:
    void _missingParamDisplay(void);
private:
    QObject * _rootQmlObject(void);
#ifdef __mobile__
    QQmlApplicationEngine * _qmlAppEngine;
#endif
    bool _runningUnitTests;
    static const char * _darkStyleFile;
    static const char * _darkStyleFile;
    static const int _missingParamsDelayedDisplayTimerTimeout=1000;
    //用来delay missing fact display
    QTimer _missingParamsDelayedDisplayTimerTimeout;
    QStringList _missingParams;
    //true:fake ui into displaying mobile interface
    bool _fakeMobile;
    //设置已升级的新版本的格式
    bool _settingsUpgraded;
#ifdef QT_DEBUG
    //双倍字体大小来模拟高dpi设备
    bool __testHighDPI;
#endif
    QGCToolbox * _toolbox;
    bool _bluetoothAvailable;
    //
    static const char * _settingVersionKey;
    //如果启动时候设置了setting key，那么所有的setting key会被删除
    static const char * _deleteAllSettingsKey;
    //友元类
    friend class UnitTest;
};
    //返回QGCApplication单例
    QGCApplication * qgcApp(void);
#endif;
```
### QGCApplication源文件
包含了”QGCMapEngine.h”
```
QGCApplication *QGCApplication::_app=null;
    const char* QGCApplication::_deleteAllSettingsKey="DeleteAllSettingsKeyNextBoot";
    const char* QGCApplication::_settingsVersionKey="SettingsVersion";
    const char* QGCApplication::_darkStyleFile=":/res/styles/style-dark.css";
    const char* QGCApplication::_lightStyleFile=":/res/styles/style-light.css";
    //整个应用中mavlink状态结构
    mavlink_status_t m_mavlink_status[MAVLINK_COMM_NUM_BUFFERS];
    //qml单例工厂
    static QObject* screenToolsControllerSingletonFactory(QQmlEngine *,QJSEngine*){
        ScreenToolsController *screenToolsController=new ScreenToolsController;
        return screenToolsController;
    }
    static QObject* mavlinkQmlSingletonFactory(QQmlEngine *,QJSEngine*){
        return new MavlinkQmlSingleton;
    }
    static QObject* qgroundcontrolQmlGlobalSingletonFactory(QQmlEngine *,QJSEngine*){
        //创建一个QGCTool即使它不在toolbox里
        QGroundControlQmlGlobal * qmlGlobal=new QGroundControlQmlGlobal(qgcapp(),qgcApp()->toolbox());
        qmlGlobal->setToolbox(qgcApp()->toolbox());
        return qmlGlobal;
    }
//QGCApplication的构造函数
QGCApplication::QGCApplication(int &argc,char * argv[],bool unitTesting)
#ifdef __mobile__
    :QGuiApplication(argc,argv)
    ,_qmlAppEngine(NULL)
    #else
    :QApplication(argc,argv)
    #endif
    , _runningUnitTests(unitTesting)
    , _fakeMoile(false)
    ,_settingsUpgraded(false)
    #ifdef QT_DEBUG
    ,_testHighDPI(false)
    #endif
    ,_toolbox(NULL)
    ,_bluetoothAvailable(false)
{
    _app=this;
#ifndef __android__
    setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
#endif
    QNetworkProxyFactory::SetUserSystemConfiguration(true);
```
### QGCApplication中的__initCommon
```
QSettings settings;
    qmlRegisterType<QGCPalette> ("QGroundControl.Palette",1,0,"QGCPalette"); 
qmlRegisterType<QGCMapPalette> ("QGroundControl.Palette",1,0,"QGCMapPalette"); 
xxx
//执行大量的qml，最后注册了qmlsingletons
```
#### QGCApplication中的__initForNormalAppBoot
```
//加载当前Stylesheet
    connect(this,&QGCApplication::lastWindowClosed,this,QGCApplication::quit);
    MainWindow * mainwindow=MainWindow::_create();
    Q_CHECK_PTR(mainWindow);
//main window检查丢失的log file
connect(this,&QGCApplication::checkForLostLogFiles,toolbox()->mavlinkProtocol(),&MAVLINKProtocol::checkForLostLogFiles);
emit checkForLostLogFiles();
//加载已知的连接配置
toolbox()->linkManager()->loadLinkConfiguratiinList();
//检测joysticks
toolbox()->joystickManager()->init();
//if(_settingsUpgraded){
    showMmessage("保存的设置已经更改，保存 的设置重置到默认状态")。
}
//连接links with flag AutoconnectLink
toolbox()->linkManager()->startAutoConnectedLinks();
//如果getQGCMapEngine()->wasCacheReset()
    showmessage(离线数据库已经更新，缓存的地图被重置);
settings.sync();
return true;
```
#### QGCApplication *gqcApp(void)
```
return QGCApplication::_app;
```
#### 主线程中保存遥感日志
saveTelemetryLgOnMainThread()
```
if(_checkTelemetrySavePath(true)){
        //定义日志文件路径、格式等
    //从临时文件保存到文件中，保存失败则报错
    }
```
#### QGCApplication::_rootQmlObject()
MainWindow * mainWindow=MainWindow::instance();
#### QGCApplication::showSetupView(void)
QMetaObject::invokeMethod(__rootQmlObject(),”showSetupView”);
