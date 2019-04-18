# qml中调用c++ - 深之JohnChen的专栏 - CSDN博客

2019年04月14日 23:20:49[byxdaz](https://me.csdn.net/byxdaz)阅读数：21


转载[https://blog.csdn.net/iceboy314159/article/details/79189514](https://blog.csdn.net/iceboy314159/article/details/79189514)

在qml中调用c++ (类成员，成员函数)，在官方文档中有详细描述，地址如下：

http://doc.qt.io/qt-5/qtqml-cppintegration-exposecppattributes.html

这里尽可能简单的总结一下，只说方法，不谈理论。

先说c++部分

代码头文件如下：

```
#ifndef QDATAMGR_H
#define QDATAMGR_H
 
#include <QObject>
 
class QDataMgr : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString _txt READ rtxt WRITE settxt NOTIFY txtchanged)
public:
    explicit QDataMgr(QObject *parent = nullptr);
    QString rtxt() const{return _txt;}
    void settxt(QString s){_txt = s; emit txtchanged();}
 
signals:
    void txtchanged();
 
public slots:
public:
    Q_INVOKABLE void info();
private:
    QString _txt;
};
 
#endif // QDATAMGR_H
```

实现文件

```
#include "qdatamgr.h"
#include <QDebug>
QDataMgr::QDataMgr(QObject *parent) : QObject(parent)
{
    _txt = "top color is red";
}
 
void QDataMgr::info(){
    qDebug()<<"_txt: "<<_txt;
}
```

QDataMgr类中 成员_txt和成员函数info是要交给qml使用的，但是c++类中的变量和函数qml是无法直接使用的，要使用需要满足以下条件：

1. 对成员变量来数，需要使用属性的概念，具体实现来说就是使用Q_PROPERTY,如_txt

2. qml 中可以使用c++中的singnal 

3. qml 中可以使用c++ 中前边加了Q_INVOKABLE宏的成员函数， 如QDataMgr::info

要达到以上任何一点要求，类中一定要使用Q_OBJECT宏

第一个方法

可以使用setContextProperty函数，main函数代码如下：

```
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "datamgr.h"
#include "qdatamgr.h"
 
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
 
    QQmlApplicationEngine engine;
    QDataMgr data_mgr;
    engine.rootContext()->setContextProperty("DATAMGR",&data_mgr);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
 
    if (engine.rootObjects().isEmpty())
        return -1;
 
 
    return app.exec();
}
    engine.rootContext()->setContextProperty("DATAMGR",&data_mgr);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
```

注意setContextProperty函数的调用要在load qml之前。完成这一步之后，就可以在qml文件中使用DATAMGR这个名字调用对象data_mgr的成员了。

qml文件如下：

```
import QtQuick 2.6
import QtQuick.Window 2.2
 
Window {
    visible: true
    width: 160
    height: 480
    title: qsTr("Hello World")
    color: "black"
    Column{
        width: parent.width
        height: parent.height
        spacing: 0
            Rectangle {
                id:top1
                color: "red";
                width:parent.height/3; height: parent.height/3;
                radius:  parent.height/6;
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        DATAMGR._txt = "top area is clicked";
                        DATAMGR.info();
                    }
                }
                Text {
                    id: top1txt
                    text: DATAMGR._txt;
                    anchors.centerIn: parent;
                }
            }
 
            Rectangle {
                id:top2
                color: "green";
                width:parent.height/3; height: parent.height/3;
                radius:  parent.height/6;
            }
 
            Rectangle {
                color: "blue";
                width:parent.height/3; height: parent.height/3;
                radius:  parent.height/6;
            }
    }
}
                        DATAMGR._txt = "top area is clicked";
                        DATAMGR.info();
                    }
                }
                Text {
                    id: top1txt
                    text: DATAMGR._txt;
                    anchors.centerIn: parent;
                }
            }

            Rectangle {
                id:top2
                color: "green";
                width:parent.height/3; height: parent.height/3;
                radius:  parent.height/6;
            }

            Rectangle {
                color: "blue";
                width:parent.height/3; height: parent.height/3;
                radius:  parent.height/6;
            }
    }
}
```

## 第二个方法

使用qmlRegisterSingletonType,main.cpp代码如下

```
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "datamgr.h"
#include "qdatamgr.h"
 
 
 
static QObject* qDataMgrSingletonFactory(QQmlEngine*, QJSEngine*)
{
    QDataMgr* dmgrGlobal = new QDataMgr();
 
    return dmgrGlobal;
}
 
 
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
 
    QQmlApplicationEngine engine;
    qmlRegisterSingletonType<QDataMgr>("Qt.xq.qdmgrSingleton", 1, 0, "DATAMGR", qDataMgrSingletonFactory);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
 
    if (engine.rootObjects().isEmpty())
        return -1;
 
 
    return app.exec();
}
```

qml代码中只需要加入下边这一行，其他无需改动
`import Qt.xq.qdmgrSingleton 1.0`

