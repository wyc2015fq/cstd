# QML与C++交换数据 - LC900730的博客 - CSDN博客
2017年08月30日 16:12:48[lc900730](https://me.csdn.net/LC900730)阅读数：279
### C++调用QML方法
QML和C++对象之间可以通过信号、槽、属性修改等机制进行通信。对于一个C++对象，任何一个暴露在Qt的元对象系统中的数据–属性–信号–槽和使用Q_INVOKEABLE标记的方法都可以在QML中访问。在QML端，所有QML对象的数据都可以在Qt元对象系统和C++中访问。
```java
//MyItem.qml
import QtQuick 1.0
Item {
    function myQmlFunction(msg){
        console.log("Got message:",msg)
        return "some return value"
    }
}
```
对应的cpp
```
QDeclarativeEngine engine;
QDeclarativeComponent component(&engine,"MyItem.qml");
QObject *object=component.create();
QVariant returnedValue;
QVariant msg="Hello from c++";
QMetaObject::invokeMethod(object,"myQmlFunction",Q_RETURN_ARG(QVariant),returnedValue),Q_ARG(QVariant,msg));
```
### QML调用C++中类对象
```
//MyItem.qml
import QtQuick 1.0
Item {
    width:100;
    height:100;
    MouseArea{
        anchor.fill:parent
        onClicked:{
            myObject.cppMethod("hello from QML")；
            myObject.cppSlot(12345)
        }
    }
}
```
对应的cpp
```
class MyClass: public Object{
    Q_OBJECT
    public:
    Q_INVOKABLE void cppMethod(const QString &msg){
        qDenug()<<"Called c++ method with"<<msg;
    }
public slots:
    void sppSlot(int number){
        qDebug()<<"Called with C++ slot with"<<number;
    }
};
int main(int argc,char *argv[]){
    QApplication app(argc,argv);
    MyClass myClass;
    view.rootContext()->setContextProperty("myObject",&myClass);
    view.setSource(QUrl::fromLocalFile("MyItem.qml"));
    view.show();
    return app.exec();  
}
```
//可以将对象转换成实际类型，这样做的好处是可以在调用函数的时候获得编译器检查。
```
import QtQuick 2.2
Item{
    width:100;height:200;
}
```
//在main.cpp中
```
int main(int argc,char *argc[]){
    QGuiApplication app(argc,argv);
    QQmlEngine engine;
    QQmlComponent component(&engine,QUrl(QStringLiteral("qrc:///main.qml")));
    QObject *object=component.create();
    ...
    ...
}
```
如下方式，可以将对象转换成实际类型，这样做的好处是可以在调用函数的时候获得编译器检查。 
普通写法： 
object->setProperty(‘width’,500); 
QQmlProperty(object,’width’).write(500);
//QQuickItem *item=qobject_cast
```
QGCQmlWidgetHolder::setContextPropertyObject(const QString &name,QObject * object){
_ui.qmlWidget->rootContext()->setContextProperty(name,object);
}
```
