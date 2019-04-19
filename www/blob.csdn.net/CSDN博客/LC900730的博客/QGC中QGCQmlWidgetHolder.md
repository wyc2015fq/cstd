# QGC中QGCQmlWidgetHolder - LC900730的博客 - CSDN博客
2017年09月08日 09:58:05[lc900730](https://me.csdn.net/LC900730)阅读数：224
主要作用是通过一个qml文件产生widgets
```
//QGCQmlWidgetHolder.h
namespace Ui{
class QGCQmlWidgetHolder;
}
class QGCQmlWidgetHolder:public QGCDockWidget{
    Q_OBJECT
public:
    explicit QGCQmlWidgetHolder(const QString &title,
    QAction *action,QWidget *parent=0);
    ~QGCQmlWidgetHolder();
    QQmlContext *getRootContext(void);
    QQuickItem * getRootObject(void);
    QQmlEngine * getEngine();
private:
    Ui::QGCQmlWidgetHolder _ui;
}
```
在cpp文件中：
```
//QGCQmlWidgetHolder.cpp
QQuicItem * QGCQmlWidgetHolder::getRootObject(void)
{
return _ui.qmlWidget->engine();
```
在MainWindow.cc中
```
_mainQmlWidgetHolder=new 
QGCQmlWidgetHolder(QString(),NULL,this);
//添加挂件
_centralLayout->addWidget(_mainQmlWidgetHolder);
_mainQmlWidgetHolder->setVisible(true);
```
在QGC中QGCQmlWidgetHolder.ui中有个对象qmlWidget，因此可以_ui->qmlWidget。 
在其他地方使用
```bash
QGCQmlWidgetHolder::setSource(const QUrl &qmlUrl){
    return _ui.qmlWidget->setSource(qmlUrl);
}
```
