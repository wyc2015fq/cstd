# 注册QML类型 - LC900730的博客 - CSDN博客
2017年09月16日 21:18:50[lc900730](https://me.csdn.net/LC900730)阅读数：325
使用qmlRegisterType可以将其注册到QML类型系统。 
qmlRegisterType函数需要一个合适的命名空间和一个版本号； 
如下面的代码将Message类注册到命名空间org.qter.messaging,版本号为1.0
```java
//Message有2个属性author、creationDate
qmlRegisterType<Message>("com.mycompany.messaging",1,0,"Message");
//注册成功之后，就可以在QML中声明和创建这个类型的对象，并使用其属性。
Message{
    author:"Amelie"
    creationDate:new Date()
}
```
