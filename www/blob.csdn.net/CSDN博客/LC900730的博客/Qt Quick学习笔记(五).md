# Qt Quick学习笔记(五) - LC900730的博客 - CSDN博客
2017年08月20日 21:04:43[lc900730](https://me.csdn.net/LC900730)阅读数：177标签：[qt](https://so.csdn.net/so/search/s.do?q=qt&t=blog)
个人分类：[qt](https://blog.csdn.net/LC900730/article/category/6981788)
## QML与C++混合编程
Qt提供了两种在QML环境中使用C++对象的方式： 
1.在C++中实现一个类，注册为QML环境的一个类型，在QML环境中使用该类型创建对象 
2.在C++中构建一个对象，将这个对象的属性设置为QML的上下文属性，在QML环境中直接使用该属性。
### 定义可以导出的类
前提条件： 
从QObject或派生类继承 
使用Q_OBJECT宏 
是为了让一个类进入Qt强大的元对象系统，只有使用元对象系统，一个类的某些方法或属性才能通过字符串形式的名字来调用，才具有了在QML中访问的基础条件。
#### 信号槽
只要是信号、槽，都可以在QML中访问。可以把C++对象的信号连接到QML中定义的方法上，也可以把QML对象的信号连接到C++对象的槽上，还可以直接调用C++对象 的槽或信号。
```
class ColorMarker:piublic QObject{
        Q_OBJECT
    public:
        ColorMarker(QObject *parent=0);
        ~ColorMarker();
    signals:
        void colorChanged(const QColor & color);
        void currentTime(const QString &strTime);
    public slot:
        vod start();
        void stop();
    }
    //定义的startt()、stop()两个槽，colorChanged()、currentTime()两个信号，都可以QML中使用。
```
### Q_INVOKABLE宏
在定义一个类的成员函数的时候使用Q_INVOKABLE宏来修饰，就可以让该方法被元对象系统调用。这个宏必须放在返回类型前面。
Q_INVOKABLE void setAlgorithm(GenerateAlgorithm alo) 
一旦使用了Q_INVOKABLE将某个方法注册到元对象系统中，那么在QML中就可以通过Object.method来访问。
### _ENUMS宏
如果要导出的类定义了想在QML中使用的枚举类型，可以使用Q_ENUMS宏将该类枚举类型注册到元对象系统中。 
如
```
class ColorMarker:public QObject{
    Q_OBJECT
    Q_ENUMS(GenerateAlgorithm)
public:
    ColorMaker(QObject *parent=0);
    ~ColorMaker();
enum GenerateAlgorithm{
    RandomRGB,
    RandomRed,
    RandomGreen,
    RandomBlue,
    LinearIncrease,
};
Q_INVOKABLE GenerateAlgorithm algorithm() const;
}
//在QML中可以用CLASS_NAME.ENUM_VALUE形式访问
```
### Q_PROPERTY
用来定义可通过元对象系统访问的属性，通过它定义的属性，可以在QML中访问、修改，也可以在属性发生变化的时候发射特定的信号。必须在类中首先使用Q_OBJECT宏。
## 注册一个QML可以的类型
上面是怎么样实现一个可供QML访问的类，下面是学习怎样将一个C++类型注册为QML类型，以及在QML中如何使用这个类型。
- 实现C++类
- 注册QML类型
- 在QML中导入
- 在QML中创建由C++导出的类型的实例并使用
#### 注册QML类型
qmlRegisterSingletonType():注册一个单例类型； 
qmlRegisterType():注册一个非单例类型； 
qmlRegisteTypeNotAvailable():注册一个类型用来占位； 
qmlRegisterUncreatableType():注册一个具有附加属性的附加类型；
qmlRegisterType()是个模板 函数，有2个原型. 
其中第一个原型是注册一个新类型，后一个可以为特定版本注册类型。
```cpp
template<typename T>{
        int qmlRegister(const char *uri,int versionMajor,int versionMinor,const char *amlName);
    template<typename T,int metaObjectRevision>{
        int qmlRegister(const char *uri,int versionMajor,int versionMinor,const char *amlName);
    }
```
要使用qmlRegisterType，需要包含QQmlEngine或QtQml头文件。 
模板参数typename，就是实现的C++类的类名
qmlRegisterType()的第一个参数uri，就是让你指定一个唯一的包名。一来避免名字冲突，二来可以把多个相关的类聚合到一个包中方便引用。如import QtQuick.Controls 1.2 ,其中QtQuick.Controls就是包名uri，而1.1则是版本，是versionMajor和versionMinor的组合，qmlName是QML中可以使用的类名。
#### 在QML中导入C++注册的类型
一旦在C++中注册号了QML的类型，就可以在QML文档中引入注册的包，然后使用注册的类型； 
如import an.qt.ColorMaker 1.0
#### 在QML中创建C++导入类型的实例
引入包后，就可以在QML中创建C++导入类型的对象，与QML内建类型的使用完全一样。如下是创建ColorMaker实例的代码
```
Rectangle{
    width:360;
    height:360;
    ColorMaker{
        id:colorMaker;
        color:Qt.green;
    }   
}
```
