# QML之QQmlApplicationEngine - LC900730的博客 - CSDN博客
2017年08月31日 19:04:21[lc900730](https://me.csdn.net/LC900730)阅读数：3640
## QQmlApplication
继承自QQmlEngine，提供了一种方便的方式：从单个qml文件中加载应用程序。
```xml
QList<QObject *>rootObjects()
```
与QQuickView不同的是，QQmlApplicationEngine不会自动创建一个根窗口。如果使用可视化项目，需要将他们放入Window里。
```
QGuiApplication app(argc, argv);
    QQuickView view;
    view.setSource(QUrl("qrc:/main.qml"));
//    view.show();
engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return app.exec();
```
//main.qml
```
Window{
title:"hello"
color:"red"
Rectangle{
    width:100;
    height:100;
    color:"black"   
}
}
```
### 返回根对象
```xml
QList<QObject *>QQmlApplicationEngine::rootObjects()
```
//1.实例化 
QQmlApplicationEngine engine； 
//2.加载qml文件 
engine.load(QUrl(QStringLiteral(“qrc:/main.qml”))); 
//3.调用rootObjects 
qDebug()<
