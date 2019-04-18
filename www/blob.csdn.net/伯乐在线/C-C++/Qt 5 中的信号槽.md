# Qt 5 中的信号槽 - 文章 - 伯乐在线
原文出处： [zi-xing，2016-09-04](http://www.cnblogs.com/zi-xing/p/5838948.html)
#### Qt4中的信号槽
Qt4中的信号槽是通过`SIGNAL,SLOT`两个宏,将参数转换成字符串.**Qt编译前**,会从源码的头文件中提取由`signal`和`slot`声明的信号和槽的函数,
将其组成一张信号和槽对应的字符串表.`connect`函数的作用是,将信号关联的槽字符串,同这张表的信息进行对比.这样信号发出的时候,就可以知道调用哪一个槽函数了.
#### Qt4信号槽的不足
- **没有编译期的检查**:Qt4中的信号槽会被宏转化成字符串处理,而字符串的比较机制是在程序运行的时候检测的.而且,转换成字符串后,信号槽的参数数据类型就会丢失.这就导致,有的时候,信号槽在编译的时候没有问题,在运行的时候,反而出错.
- **无法使用相容类型的参数**:因为信号槽的机制使用的是字符串的匹配的方法,所以,槽函数的参数类型的名字,必须和信号参数类型的名字一致,同时,还必须和头文件中声明的类型名字一致,也就是字符串意义上的严格相同.如果使用了`typdef`或者`namespace`这样的类型,虽然实际的类型是一样的,但是由于字符串的名字不一样,所以Qt4中是会有错误的.如下伪代码示例(实际类型都是int,但因为按照字符串处理,所以Qt4中,编译前不能通


```
//head.h file
typedef int MyInt;
typedef int BigInt;
//head.cpp file
connect(Sender,SIGNAL(sigFun(MyInt)),Receiver,SLOT(sltFun(BigInt)));
```
### Qt5中的信号槽
Qt5中不仅解决了上述Qt4中的问题,而且还有一些扩充.
- **支持编译期的检查**:拼写错误,槽函数参数个数大于信号参数的个数等;
- **支持相容类型的自动转换**;
- **槽允许连接到任意的函数**:Qt5中,因为槽使用的是函数指针,所以槽的调用,可以是任意的成员函数,静态函数,还可以是C++11 的lambda表达式;Qt4中槽的声明一般是`private slots`,`private`是私有限制,只有把槽函数当作普通函数使用的时候,才会体现私有的性质.而`SLOT`,把槽函数转化成了字符串,此时`private`是不起作用的.Qt5中,因为使用的是函数指针,所以在类的外部,`connect`是无法关联一个类的私有槽的,否则,编译的时候就会报错.
总之,Qt5中,增加了信号槽的灵活性,加强了信号槽的检测性.
#### Qt5信号槽的语法例子
##### 常用用法


```
//ClassA.h
signal:
  void sigClassA(int num);
  void sigStringChanged(QString str);
//ClassB.h
  void sltClassB(int num);//任意的成员函数,静态函数都阔以
  void sltStringChanged(QVariant str);
//ClassB.cpp
connect(Sender, &ClassA::sigClassA, this, &ClassB::sltClassB);//函数指针关联的时候,不需要指明参数,而且this可以省略[**Update:2016_11_20,注意在这种情况下,可以省略, 在其他的情况就一定了.省略会有错误危险的.**]
connect(Sender, &ClassA::sigClassA, &ClassB::quit);//省略了this,同时静态函数quit作为槽
//QString 可以转化成 QVariant
connect(Sender, &ClassA::sigStringChanged, &ClassB::sltStringChanged);//信号槽的参数类型可以发生隐士类型转化即可
```
##### 信号槽的重载
解决方法:
- 使用Qt4的方法(不再介绍)
- Qt5显示转换函数指针


```
//信号的重载和槽的重载都是一样的解决机制
//ClassA.h
signal:
  void sigClassA();
  void sigClassA(int num);
//ClassB.h
    void sltClassB();
    void sltClassB(int num);
//ClassB.cpp
connect(Sender, static_cast<void(ClassA::\*)()>(&ClassA::sigClassA),//注意\*为markdown转义
this,static_cast<void(ClassA::\*)()>(&ClassB::sltClassB) );
connect(Sender, static_cast<void(ClassA::\*)(int)>(&ClassA::sigClassA),
this,static_cast<void(ClassA::\*)()>(&ClassB::sltClassB) );  
connect(Sender, static_cast<void(ClassA::\*)(int)>(&ClassA::sigClassA),
this,static_cast<void(ClassA::\*)(int)>(&ClassB::sltClassB) );
```
##### 带默认数值的槽函数
解决方法:
- 进一步的封装函数(不做介绍)
- 采用Qt5的C++11 lambda表达式(表达式规则暂且不做详细介绍)


```
//ClassA.h
signal:
  void sigClassA();
  void sigClassA(int num);
//ClassB.h
    void sltClassB(int num = 10);
//ClassB.cpp
connect(Sender, static_cast<void(ClassA::\*)(int)>(&ClassA::sigClassA),//注意\*为markdown转义
this,static_cast<void(ClassA::\*)(int)>(&ClassB::sltClassB) );//信号和槽的参数个数对应,是可以的
connect(Sender, static_cast<void(ClassA::\*)()>(&ClassA::sigClassA),
this,static_cast<void(ClassA::\*)()>(&ClassB::sltClassB) );//槽的参数,比信号多,这个会报错误的
//函数参数的默认数值,只有在函数调用的时候,才会有效,取函数地址的时候,是看不到参数的默认数值的.函数指针并不包含默认数值.又因为槽包含默认数值,所以信号可以不提供参数.那么,这就和信号的参数个必须大于槽的参数的个数产生了矛盾.
connect(Sender, static_cast<void(ClassA::\*)()>(&ClassA::sigClassA),
this,[=](int num = 10){//使用了lambda表达式
  //...函数体
}] );
```
[update:2016_11_20]
##### 思考this的省略?
前面提到过connect函数的第三个参数this指针是可以省略的.但是在某些情况下this是绝不可以省略的.甚至我建议大家为了避免不必要的错误, this指针最好不要省略, 还是带上比较好.connect函数基本是如下的原型:


```
connect( 发送者, 发送者信号, 接收者, 接收者处理方法 );    ///< 一般的四个参数
connect( 发送者, 发送者信号, **发送者**处理方法 ); ///< 如果省略this, 是三个参数, 那么最后一个参数的意义就发生了变化. 此时调用的方法则是发送者自己的方法. 
///< 试想一下, 如果此时发送者和接收者又刚好拥有同样的函数名字, 但是内部的方法不同, 那么最后的结果就会让人莫名奇妙的诡异起来.
```
所以,一定要明确的区分每个参数的具体意义, 马马虎虎最终还是自己填坑.
你也看到connect是可以使用C++的匿名函数的, 也是可以省略this的,但是, 这一步一定要小心了. 尤其是当你在使用线程的时候, 在接收线程信号的时候, 一万个小心.比如:


```
connect(pThread, &QThread::finished, [=]() { myFun(); } );  ///< 当线程执行完后, 你会惊奇的接收到应用程序的崩溃. 基本的提示内容, 就是, 你的某个线程出了问题. 
///< 就上面的问题,你可以在多个地方, 把线程的ID打印出来就知道了.
qDebug() << "ThreadId1:";
connect(pThread, &QThread::finished, [=]() 
{
    qDebug() << "ThreadId2:";
    myFun();
} );
///< 打印出来以后, 你会发现lambda表达式函数里面ID和线程run函数里面的id是一样的. 虽然说, 代码在不同的类里面, 不同的文件里面. 可是运行环境, 运行的线程却是可以在一起的. 解决方法, 加个this, 就可以了. 你懂的.
```
#### 参考
> 
[《Qt 5编程入门》](http://www.qter.org/?page_id=91)
