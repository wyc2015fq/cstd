# 初始化QML运行环境 - LC900730的博客 - CSDN博客
2017年08月31日 18:33:36[lc900730](https://me.csdn.net/LC900730)阅读数：482
### QML运行环境
要运行一个使用QML的应用程序，QML运行环境必须由应用程序调用。两种方式 
- 通过QQuickView加装QML文件
- 创建一个QQmlEngine，并且使用QQmlComponent加装QML文件
## 用QQuickView初始化
QQuickView是一个基于QWindow的类，能够加装QML文件。
```
int main(int argc,char *argv[]){
    QGuiApplication app(argc,argv);
    QQuickView view;
    view.setSource(QUrl(QStringLiteral("qrc:/main.qml")));
    view.show();
return app.exec();
}
```
这将创建一个基于QWindow的视图，来显示main.qml的内容。
即在main.qml中，不在需要Window根对象。 
如果有Window根对象，那么
## 直接创建QQmlEngine
如果main.qml中没有任何图形组件，或者希望避免使用QQuickView，可以直接使用QQmlEngine。这种情况下，main.qml将作为一个QQmlComponent实例被加载，而不是放入view中。
```
QGuiApplication app(argc,argv);
QQmlEngine engine;
QQmlContext *objContext;
objContext=new QQmlContext(engine.rootContext());
QQmlComponent componnet(&engine,"qrc:///main.qml");
QObject *object=component.create(objContext);
//在必要时候删除object和objectContext
return app.exec();
```
