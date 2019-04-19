# QQmlComponent加载component - LC900730的博客 - CSDN博客
2017年09月13日 18:43:10[lc900730](https://me.csdn.net/LC900730)阅读数：1085
### QQmlComponent创建
```
//main.cpp
QQmlEngine *engine=ne QQmlEngine();
QQmlComponent component(engine);
component.loadUrl(QUrl("qrc:///main.qml"));
if(component.isReady()){
    QObject *myObject=component.create();
    QQuickItem *item=qobject_cast<QQuickItem *>(myObject);
    int width=item->width();
    qDebug()<<width;
}
```
对应的qml文件
```
//main.qml
Item{
    width:200
    height:200
}
```
### QQmlExpression
动态执行表达式也是QML的核心概念之一。 
QQmlExpression允许客户端在C++中，利用一个特定的QML上下文执行Js表达式。执行结果以QVariant形式返回，并且遵循QML引擎确定的转换规则。
```
//main.qml
import QtQuick 2.2
Item{
    width:200;
    height:200
}
```
```
QQmlEngine *engine=new QQmlEngine;
QQmlComponent component(engin);
component.loadUrl(QUrl("qrc:///main.qml"));
QQbject *myObject=component.create();
QQmlExpression *expr=new QQmlExpression(engine->rootContext(),
object,"width*2");
int result=expr->evaluate().toInt();
```
