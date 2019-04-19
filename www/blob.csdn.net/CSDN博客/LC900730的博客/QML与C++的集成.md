# QML与C++的集成 - LC900730的博客 - CSDN博客
2017年08月29日 18:07:10[lc900730](https://me.csdn.net/LC900730)阅读数：256
### QML与C++
QML引擎与Qt的元对象系统的集成，使得QML中可以直接调用C++的功能。 
只有QObject子类才能够将数据或者函数提供给QML使用。由于QML引擎集成了Qt元对象系统，由QObject派生的所有子类的属性、方法和信号灯都可以在QML中访问。
- C++类可以被注册为一个可实例化的QML类型，这样他就可以像其它普通QML对象类型一样在QML代码中被实例化使用
- C++类可以被注册为一个单例类型，这样可以在QML代码中导入这个单例对象实例
- C++类的实例可以作为上下文属性或者上下文对象嵌入到QML代码中。
除了可以从QML中访问C++的功能，在Qt QML模块中也提供了多种方式从C++代码中操作QML对象。
### QML运行时的C++类
Qt QML模块提供的一些实现了QML框架的C++类。客户端可以使用这些类与QML进行时进行交互(如向对象注入数据，或者调用对象方法)，并且从QML文档实例化一个对象树状结构。
QML应用程序构成一个QML客户端，在启动时候，客户端会初始化一个QQmlEngine类作为QML引擎，然后使用QQmlComponent对象加载QML文档。 
QML引擎会提供一个默认的QQmlContext对象作为顶层执行上下文，用来执行QML文档中定义的函数和表达式。这个上下文可以通过QQmlEngine::rootContext()函数获取，利用QML引擎可以对其进行修改等操作。
### QQmlEngine
QQmlEngine允许将全局设置应用到其管理的所有对象。如用于网络通信的QNetworkAccessManager以及用于永久存储的文件路径。 
在QQmlApplicationEngine类，它是QQmlEngine的子类。QQmlApplicationEngine结合了QQmlEngine和QQmlComponent的功能，提供了一种简便的方式实现了从一个单一的QML文件加载一个应用程序。
```
QQmlApplicationEngine engine;
engine.load(QUrl(QStringLiteral("qrc:///main.qml")));
//load()函数会自动加载给定的文件并立即创建文件中定义的对象树。
```
### QQmlContext
QQmlContext提供了对象实例化和表达式执行所需要的运行时上下文。所有对象都需要在一个特定的上下文中实例化，所有表达式都要在一个特定的上下文中执行。QQmlContext类在QML引擎中定义了这样一个上下文，允许数据暴露给由QML引擎实例化的QML组件。 
QQml组件Context包含了一系列属性，能够通过名字将数据显式绑定到上下文。可以使用QQmlContext::setContextProperty()函数来定义、更新上下文中的属性。 
如：
```
QQmlEngine engine;
    QStringListMOdel modelData;
    QQmlContext * context=engine.rootContext();
    context->setContextProperty ("stringModel",&modelData); 
//QQmlContext对象作为顶层执行上下文,用来执行QML文档中定义的函数或者表达式。
```
//根上下文创建了一个新的上下文对象 
//这里是由根上下文创建了一个新的上下文对象。modelData被添加到新的上下文中，并且该上下文作为动态创建的组件上下文。以
```
QQmlEngine engine;
    QStringListMOdel modelData;
    QQmlContext * context=new QQmlContext(engine.rootContext());
    context->setContextProperty("stringModel",&modelData);
QQmlComponent component(&engine);
component.setData("import QtQuick 2.2\nListView"{
model:stringModel}",QUrl());
    QObject *window=component.create(context);
```
### QQmlExpression
动态执行表达式。QQmlExpression允许客户端在C++中利用一个 特定的QML上下文执行Js表达式。表达式的执行结果以QVariant形式返回，且遵守QML引擎确定的转换规则。 
如： 
//main.qml
```
import QtQuick 2.2
Item{
    width:200;
    height:200
}
```
//可以使用下面的C++代码，在上下文中执行Js表达式
```
QQmlEngine *engine=new QQmlEngine;
QQmlComponent component (engine,QUrl::fromLocalFile("main.qml"));
QObject * object=component,create();
QQmlExpression *expr=new QQmlExpression(
engine->rootContext(),object,"width *2");
int result=expr->evaluate().toInt();
```
### 在QML中使用C++特性
QML引擎和元对象系统的紧密集成，QML可以非常方便地通过C++进行扩展。QML可以直接访问QObject子类的属性、方法和信号：属性可以在QML中读取和修改；方法可以直接通过Js调用；信号处理函数则根据信号自动创建。
#### 使用枚举类型
使用Q_ENUMS宏将枚举注册到Qt元对象系统。
#### 使用C++属性
QObject子类的所有属性都能够被QML访问。 
QObject子类使用Q_PROPERTY宏定义一个属性，该宏的作用是向Qt元对象系统注册类的属性。一个类的属性就是类的数据成员，通常会有一个用于读取的READ函数和一个可选的用于修改的WRITE函数。 
如
```
class Mesage :public QObject{
    Q_OBJECT
    Q_PROPERTY(QString author READ author WRITE setAuthor NOTIFY authorChanged)
    public :
        void setAuthor(const QString &a){
            if(a!=m_author){
                m_author=a;
                emit authorChanged();
            }
        } 
    QString author() const{
        return m_author;
    }
    signals:
        void authorChanged();
    private:
        QString m_author;
} ;
```
在使用时，可以将Message类的一个实例作为加载QML文档myItem.qml的上下文属性：
```
int main(int argc,char *argv[]){
    QCoreApplication app(argc,argv);
    QQmlEngine engine;
    Message msg;
    engine.rootContext()->setContextProperty("msg",&msg);
    QQmlComponent component(&engine,QUrl::fromLocalFile("myItem.qml"));
    component.create();
    return app.exec()
}
//然后可以在myItem.qml中直接操作author属性
import QtQuck 2.2
Text{
    width:100 ; height:100
    text:msg..author  //会调用Message::author()
    Component.onCompleted:{
        msg.author="Jonah"; //调用MEssage::setAuthor()
    }
}
```
为了尽可能增强与QML的可交互性，任何可写属性都应该关联一个NOTIFY信号。 
如上面属性author有一个NOTIFY信号authorChanged()。当setAuthor()函数修改了author()属性时候，该信号便会发出。
NOTIFY信号的确会产生不小开销。有些属性值仅在对象构造时候设置，之后就不会被修改。此时应该使用CONSTRANT特性而不是NOTIFY。
