# QML与C++中信号和槽的使用 - LC900730的博客 - CSDN博客
2017年08月30日 23:36:04[lc900730](https://me.csdn.net/LC900730)阅读数：1080
### 信号与槽
```
//Gemini.h
class Gemini:public Q_Object{
    Q_OBJECT
    signals:
        void begin()
    public slots:
        void doSomething(){
            qDebug()<<"Gemini ::doSomething() called";
        }
}
```
Gemini类中的信号begin()和槽doSomething()都可被QML访问。槽必须声明为public或protected。 
在C++中使用时候需要emit关键字，但是在QML中就是普通函数，用法跟函数一样，信号处理器形式为onSignal。信号不支持重载。
```
//main.cpp
#include<QGuiApplication>
#include<QQmlApplicationEngine>
#include<QtQml>
#include "Gemini.h"
int main(int argc,char *argv){
    QGuiAppliction app(argc,argv);
    qmlRegisterType<Gemini>("Union.Lotto.Gemini",1,0,"Gemini");
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:///main.qml)));
    return app.exec();
}
```
//main.qml
```
import Union.Lotto.Gemini 1.0
Window {
    visible:true
    width:360;height:360;
    title:"Union Lotto Game"
    color:'white'
    MouseArea{
        anchors.fill:parent;
        onclicked:{
            gemini.begin()
        }
    }
    Gemini {
        id:gemini;
        //begin就是Gemini类中的信号，doSomething就是Gemini的槽
        onBegin:doSomething();
    }
}
```
    
在qmlRegisterType()中，
- 其中尖括号放的是类名称
- @param1:在qml中唯一标识一个类，使用import的url
- @param2:主版本
- @param3:次版本
- @param4:在qml中使用的模块代表这个类
