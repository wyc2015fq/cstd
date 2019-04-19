# Qt中注册QML类型 - LC900730的博客 - CSDN博客
2017年08月16日 11:05:15[lc900730](https://me.csdn.net/LC900730)阅读数：984
# 注册QML类型
使用C++代码扩展QML，可以将一个C++类注册到QML类型系统，以便在QML代码中将其作为一个数据类型。是一个可以将C++实现的类在QML中调用的，连接C++和QML的一个工具。
## qmlRegisterType：注册可实例化的对象类型
QObject子类都可以注册为QML对象类型。注册成功之后，这个类就可以在QML代码中像其他对象类型一样声明和初始化。一旦创建完成，就可以在QML中使用属性值、函数和信号等特性。
### 作用：
QML和C++为什么要混合编程，简单来说，就是使用QML高效便捷地构建UI，而C++则用来实现业务逻辑和复杂算法。
qmlRegisterType里共4个参数，第一个参数*url指的是QML中import后的内容，相当于头文件名，第二个第三个参数分别是主次版本号，第四个指的是QML中类的名字。
```
class Message：public QObject{
    Q_OBJECT
    Q_PROPERTY(QString author READ author WRITE setAuthor NOTIFY authorChanged)
    Q_PROPERTY(QDateTime creationDate READ creationDate WRITE setCreationDate NOTIFY creationDateChanged)
    public:
    //...
};
```
使用qmlRegisterType可以将其注册到QML类型系统。qmlRegisterType函数需要一个合适的命名空间和一个版本号。 
如下所示： 
qmlRegisterType(“com.mycompany.messageing”,1,0,”Message”); 
注册成功之后，就可以在QML中声明和创建这个类型的对象，并使用其属性：
```
import com.mycompany.messaging 1.0
Message{
    author:"Amelie"
    creationDate:new Date()
}
```
### 注册不可实例化对象类型
如一个C++类
- 是一个接口类型，不应该被实例化
- 是一个基类，不需要通过QML代码访问
- 仅 提供了一些有用的枚举，其余部分不应该被QML使用
- 是一个单例，只能使用其唯一的实例，不应该从QML进行实例化
为了应对这些情况，QML提供了一些用于注册不可实例化对象类型的方法。 
- 
可以采用无参数的qmlRegisterType()函数。由于qmlRegisterType()没有提供命名空间和版本号，也就不能在QML中引用，限制了QML引擎对该类型实例化的能力。 
- 使用qmlRegisterInterface()注册指定QML类型名称的Qt接口类型。这种类型不能实例化，但是可以使用其类型名称进行引用。
- 使用qmlRegisterUncreatableType()注册不可被实例化的具有名称的C++类型，但是这样的类型依然可以作为QML类型系统可识别的一种类型。如果该类型的枚举或者附加特性需要在QML中使用，但是类型本身不应被实例化，就需要使用这种方式进行注册。
- 使用qmlRegisterSingletonType()注册不能从QML导入的单例类型
### 注册单例类型
