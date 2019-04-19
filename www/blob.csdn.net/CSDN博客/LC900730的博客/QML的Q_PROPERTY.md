# QML的Q_PROPERTY - LC900730的博客 - CSDN博客
2017年09月13日 22:38:44[lc900730](https://me.csdn.net/LC900730)阅读数：449
### QObject
QObject子类使用了Q_PROPERTY宏定义了一个属性，该宏的作用是向Qt元对象系统注册类的属性。一个类的属性是类的数据成员，通常会有一个读取的READ函数和一个可选的WRITE函数。
```
Q_PROPERTY(QString author READ author WRITE setAuthor
NOTIFY authorChanged)
//有一个信号authorChanged()。当setAuthor()函数修改了author()属性的值。
public :
void setAuthor(const QString &a){
    if(a！=m_author){
        m_author=a;
        emit authorChanged();
    }
}
```
在使用时候将Message类的一个实例作为加载QML文档myItem.qml的上下文属性：
```
int main(int argc,char *argv[]){
    QCoreApplication app(argc,argv);
    QQmlEngine engine;
    Message msg;
    engine.rootContext()->setContextProperty("msg",&msg);
    QQmlComponent component(&engine,QUrl::fromLocalFile("myItem,qml"));
    component.create();
    return app.exec();
}
```
然后在可以在myItem.qml中直接操作author属性
```
import QtQuick 2.2
Text{
    width:100;
    height:100;
    text:msg.author;
    Component.onCompleted:{
    //调用setAuthor方法
        msg.author="Jonah";
    }
}
```
为了尽可能增强与QML的可交互性，任何可写属性都应该关联一个NOTIFY信号。当属性值发生改变时候，发出该信号。这种机制可以使得该属性能够应用于属性绑定，而属性绑定正是QML最强大的功能之一。
CONSTRANT特性：仅用于那些在构造函数中设置值，并且之后不会被修改的属性，而所有可能会被绑定 的属性都应该使用NOTIFY信号。
