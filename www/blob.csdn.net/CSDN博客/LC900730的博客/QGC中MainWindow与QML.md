# QGC中MainWindow与QML - LC900730的博客 - CSDN博客
2017年09月01日 16:59:50[lc900730](https://me.csdn.net/LC900730)阅读数：236
### 对QGC主窗口的学习
在ui\MainWindow.h中 
构造方法被定义成私有的。
在QGCApplication.cc中__initForNormalAppBoot()中 
创建窗口，不是new MainWindow，也是由于构造方法是私有的 
采用 
MainWindow *mainWindow=MainWindow::__create();方式创建
在_initForNormalAppBoot(void)执行时候：
```
QSettings settings;
_loadCurrentStyleSheet();
//上一个窗口关闭时候退出main application 
connect(this,&QGCApplication::lastWindowClosed,this,QGCApplication::quit);
```
在MainWindow的构造方法中，
```
_instance=this;
loadSettings();
emit initStatusChanged(tr('xxx'),Qt::AlignLeft|Qt::AlignBottom,QColor(62,93,141));
_ui.setupUi(this);
```
//确保在改变最小宽度之前，确保所有tool bar elements都fit
```
setMinimumWidth(1008);
configureWindowName();
//使用QVBoxLayout这个widget来装所有view
_centralLayout=new QVBoxLayout();
_centralLayout->setContentsMargins(0,0,0,0);
centralWidget()->setLayout(_centralLayout);
```
### MainWindowHybrid.qml
```java
Item{
    function showSetupView(){
        mainWindowInner.item.showSetupView()
    }
    function attemptWindowClose(){
        mainWindowInner.item.attemptWindowClose()
    }
    function showMessage(message){
        mainWindowInner.item.showMessage(message)
    }
}
```
在上述qml中的方法，在QGCApplication.cc中定义
```
void QGCApplication::showSetupView(void)
{
    QMetaObject::invokeMethod(_rootQmlObject(), "showSetupView");
}
void QGCApplication::qmlAttemptWindowClose(void)
{
    QMetaObject::invokeMethod(_rootQmlObject(), "attemptWindowClose");
}
void QGCApplication::showMessage(const QString& message)
{
    // Special case hack for ArduPilot prearm messages. These show up in the center of the map, so no need for popup.
    if (message.contains("PreArm:")) {
        return;
    }
    QObject* rootQmlObject = _rootQmlObject();
    if (rootQmlObject) {
        QVariant varReturn;
        QVariant varMessage = QVariant::fromValue(message);
        QMetaObject::invokeMethod(_rootQmlObject(), "showMessage", Q_RETURN_ARG(QVariant, varReturn), Q_ARG(QVariant, varMessage));
#ifndef __mobile__
    } else if (runningUnitTests()){
        // Unit test can run without a main window which will lead to no root qml object. Use QGCMessageBox instead
        QGCMessageBox::information("Unit Test", message);
#endif
    } else {
        qWarning() << "Internal error";
    }
}
```
在ui\MainWindow.h中
```
_mainQmlWidgetHolder->setContextPropertyObject("controller",this);
_mainQmlWidgetHolder-
>setContextPropertyObject("debugMessageModel",AppMEssage::getModel());
_mainQmlWidgetHolder->setSource(QUrl::fromUserInput("qrc:qml/MainWindowHybrid.qml"));
```
因此在qml中有个controller可以用
