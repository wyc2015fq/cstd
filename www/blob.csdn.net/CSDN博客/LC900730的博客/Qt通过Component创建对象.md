# Qt通过Component创建对象 - LC900730的博客 - CSDN博客
2017年09月15日 18:01:50[lc900730](https://me.csdn.net/LC900730)阅读数：215
在QtQuick.Controls 1.4中用的是ApplicationWindow 
在QtQuick.Window  2.2中用的是Window
```
int main(int argc,char *argv[]){
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;     engine.load(QUrl(QStringLiteral("qrc:///main.qml")));
    return app.exec();
}
```
```java
//main.qml
import QtQuick 2.6
import QtQuick.Controls 1.4
//import QtQuick.Window 2.2
//ApplicationWindow组件在QtQuick.Controls 1.4
//Window组件在QtQuick.Controls 1.4
ApplicationWindow {
    id: name
    width: 200
    height: 200
    //默认不显示
    visible: true
    Text {
        width: 100;
        height: 100;
        text:msg.author
    }
    Button_quit{
    }
}
```
