# QGC的MainWindow的menuWidget - LC900730的博客 - CSDN博客
2017年09月16日 17:57:16[lc900730](https://me.csdn.net/LC900730)阅读数：201
![这里写图片描述](https://img-blog.csdn.net/20170916175318615?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
MainWindow::_buildCommonWidgets(void)
## QGC主界面的菜单栏
### 第二个菜单栏
#### 第一个action
```
//ui/MainWindow.cc
#ifdef UNITTEST_BUILD
    QAction * qmlTestAction=new QAction("Test QML palette and controls",NULL);
    connect(qmlTestAction,QAction::triggered,this,&MainWindow::_showQmlTestWidget);
    _ui.menuWidgets->addAction(qmlTestAction);
```
#### 剩下6个action
```
//ui/MainWindow.cc
static const char *rgDockWidgetNames[] = {
    "MAVLink Inspector",
    "Custom Command",
    "Onboard Files",
    "Deprecated Widget",
    "HIL Config",
    "Analyze"
};
void MainWindow::_buildCommonWidgets(void){
    logPlayer=new QGCMAVLinkLogPlayer(statusBar());
    statusBar()->addPermanentWidget(logPlayer);
    for(int i=0,end=ARRAY_SIZE(rgDockWidgetNames);i<end;i++){
    const char *pDockWidgetName=rgDockWidgetNames[i];
    //Add to menu
    QAction *action=new QAction(pDockWidgetName,this);
    action->setCheckable(true);
    action->setData(i);
    connect(action,QAction::triggered,this,&MainWindow::_showDockWidgetAction);
    _ui.menuWidgets->addAction(action);
    _mapName2Action[pDockWidgetName]=action;
}
。
QGCApplication *app=new QGCApplication();
...
在构造函数中，app->init
主要是注册了一对qmlRegiasterType
app->_initNormalAppBoot()
调用了MainWindow->create()创建窗口
```
