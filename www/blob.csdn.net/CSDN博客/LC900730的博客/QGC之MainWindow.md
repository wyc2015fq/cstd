# QGC之MainWindow - LC900730的博客 - CSDN博客
2017年08月30日 11:00:14[lc900730](https://me.csdn.net/LC900730)阅读数：263
### ui\MainWindow.cc
在QGCApplication中调用了MainWindow::_create()
```
MainWindow * MainWindow::_create(){
    new MainWindow();
    return _instance;
}
```
在MainWindow的构造方法，构造方法是私有的，为了得到一个单例的MainWindow，只有在MainWindow::_create()方法中调用构造方法，其余代码都应该无权限调用此构造方法。
```
MainWindow::MainWindow()
        :_mavlinkDecoder        (NULL)
        ,_lowPowerMode          (false)
        ,_showStatusBar         (false)
        ,_mainQmlWidgetHolder   (null)
        ,_forceClose          (false)
{
    _instance=this;
    //设置用户界面
    loadSettings();
    emit initStatusChanged(tr("Setting up user interface"),Qt::AlignLeft|Qt::AlignBottom,QColor(62,93,141));
    _ui.setupUi(this);
    setMinimunWidth(1008);
    configureWindowName();
    //setup central widget with a layout to hold the views
    _centralLayout=new QVBoxLayout();
    _centralLayout->setContentsMargins(0,0,0,0);
    centralWidget()->setLayout(_centralLayout);
    //主要qml相关
    _mainQmlWidgetHolder=new QGCQmlWidgetHolder(QString(),NULL,this);
    _centralLayout->addwidget(_mainQmlWidgetHolder);
    _mainQmlWidgetHolder->setVisible(true);
    //QQmlEngine相关
    QQmlEngine::setObjectOwnership(this,QQmlEngine::CPPOwnership);
    _mainQmlWidgetHolder->setContextPropertyObject("controller",this);
    _mainQmlWidgetHolder->setContextPropertyObject("debugMessageModel",APPMessage::getModel());
    _mainQmlWidgetHolder->setSource(QUrl::fromUserInput("qrc:qml/MainWindowHybrid.qml"));
    //image provider
    QQuickImageProvider *pImgProvider=dynamic_cast<QQuickImageProvider *>(
    qgcApp()->toolbox()->imageProvider());
    _mainQmlWidgetHolder->getEngine()->addImageProvider(QLatin1String("QGCImages"),pImgProvider);
    setDockOptions(0);
    setCorner(Qt::BottomRightCorner,Qt::BottomDockWidgetArea);
    )
}
```
