
# java入门教程-5.4Java接口(interface)的概念及使用 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月25日 09:03:27[seven-soft](https://me.csdn.net/softn)阅读数：199


在抽象类中，可以包含一个或多个抽象方法；但在接口(interface)中，所有的方法必须都是抽象的，不能有方法体，它比抽象类更加“抽象”。
接口使用 interface 关键字来声明，可以看做是一种特殊的抽象类，可以指定一个类必须做什么，而不是规定它如何去做。
现实中也有很多接口的实例，比如说串口电脑硬盘，Serial ATA委员会指定了Serial ATA 2.0规范，这种规范就是接口。Serial ATA委员会不负责生产硬盘，只是指定通用的规范。
希捷、日立、三星等生产厂家会按照规范生产符合接口的硬盘，这些硬盘就可以实现通用化，如果正在用一块160G日立的串口硬盘，现在要升级了，可以购买一块320G的希捷串口硬盘，安装上去就可以继续使用了。
下面的代码可以模拟Serial ATA委员会定义以下串口硬盘接口：//串行硬盘接口
publicinterfaceSataHdd{
//连接线的数量
publicstaticfinalintCONNECT_LINE=4;
//写数据
publicvoidwriteData(Stringdata);
//读数据
publicStringreadData();
}

注意：接口中声明的成员变量默认都是 public static final 的，必须显示的初始化。因而在常量声明时可以省略这些修饰符。
接口是若干常量和抽象方法的集合，目前看来和抽象类差不多。确实如此，接口本就是从抽象类中演化而来的，因而除特别规定，接口享有和类同样的“待遇”。比如，源程序中可以定义多个类或接口，但最多只能有一个public 的类或接口，如果有则源文件必须取和public的类和接口相同的名字。和类的继承格式一样，接口之间也可以继承，子接口可以继承父接口中的常量和抽象方法并添加新的抽象方法等。
但接口有其自身的一些特性，归纳如下。
1) 接口中只能定义抽象方法，这些方法默认为 public abstract 的，因而在声明方法时可以省略这些修饰符。试图在接口中定义实例变量、非抽象的实例方法及静态方法，都是非法的。例如：publicinterfaceSataHdd{
//连接线的数量
publicintconnectLine;//编译出错，connectLine被看做静态常量，必须显式初始化
//写数据
protectedvoidwriteData(Stringdata);//编译出错，必须是public类型
//读数据
publicstaticStringreadData(){//编译出错，接口中不能包含静态方法
return"数据";//编译出错，接口中只能包含抽象方法，
}
}

3) 接口中没有构造方法，不能被实例化。
4) 一个接口不实现另一个接口，但可以继承多个其他接口。接口的多继承特点弥补了类的单继承。例如：//串行硬盘接口
publicinterfaceSataHddextendsA,B{
// 连接线的数量
publicstaticfinalintCONNECT_LINE=4;
// 写数据
publicvoidwriteData(Stringdata);
// 读数据
publicStringreadData();
}
interfaceA{
publicvoida();
}
interfaceB{
publicvoidb();
}

## 为什么使用接口
大型项目开发中，可能需要从继承链的中间插入一个类，让它的子类具备某些功能而不影响它们的父类。例如 A -> B -> C -> D -> E，A 是祖先类，如果需要为C、D、E类添加某些通用的功能，最简单的方法是让C类再继承另外一个类。但是问题来了，Java
 是一种单继承的语言，不能再让C继承另外一个父类了，只到移动到继承链的最顶端，让A再继承一个父类。这样一来，对C、D、E类的修改，影响到了整个继承链，不具备可插入性的设计。
接口是可插入性的保证。在一个继承链中的任何一个类都可以实现一个接口，这个接口会影响到此类的所有子类，但不会影响到此类的任何父类。此类将不得不实现这个接口所规定的方法，而子类可以从此类自动继承这些方法，这时候，这些子类具有了可插入性。
我们关心的不是哪一个具体的类，而是这个类是否实现了我们需要的接口。
接口提供了关联以及方法调用上的可插入性，软件系统的规模越大，生命周期越长，接口使得软件系统的灵活性和可扩展性，可插入性方面得到保证。
接口在面向对象的 Java 程序设计中占有举足轻重的地位。事实上在设计阶段最重要的任务之一就是设计出各部分的接口，然后通过接口的组合，形成程序的基本框架结构。
## 接口的使用
接口的使用与类的使用有些不同。在需要使用类的地方，会直接使用new关键字来构建一个类的实例，但接口不可以这样使用，因为接口不能直接使用 new 关键字来构建实例。
接口必须通过类来实现(implements)它的抽象方法，然后再实例化类。类实现接口的关键字为implements。
如果一个类不能实现该接口的所有抽象方法，那么这个类必须被定义为抽象方法。
不允许创建接口的实例，但允许定义接口类型的引用变量，该变量指向了实现接口的类的实例。
一个类只能继承一个父类，但却可以实现多个接口。
实现接口的格式如下：
修饰符 class 类名 extends 父类 implements 多个接口 {
实现方法
}
请看下面的例子：importstaticjava.lang.System.*;
publicclassDemo{
publicstaticvoidmain(String[]args){
SataHddsh1=newSeagateHdd();//初始化希捷硬盘
SataHddsh2=newSamsungHdd();//初始化三星硬盘
}
}
//串行硬盘接口
interfaceSataHdd{
//连接线的数量
publicstaticfinalintCONNECT_LINE=4;
//写数据
publicvoidwriteData(Stringdata);
//读数据
publicStringreadData();
}
// 维修硬盘接口
interfacefixHdd{
// 维修地址
Stringaddress="北京市海淀区";
// 开始维修
booleandoFix();
}
//希捷硬盘
classSeagateHddimplementsSataHdd,fixHdd{
//希捷硬盘读取数据
publicStringreadData(){
return"数据";
}
//希捷硬盘写入数据
publicvoidwriteData(Stringdata){
out.println("写入成功");
}
// 维修希捷硬盘
publicbooleandoFix(){
returntrue;
}
}
//三星硬盘
classSamsungHddimplementsSataHdd{
//三星硬盘读取数据
publicStringreadData(){
return"数据";
}
//三星硬盘写入数据
publicvoidwriteData(Stringdata){
out.println("写入成功");
}
}
//某劣质硬盘，不能写数据
abstractclassXXHddimplementsSataHdd{
//硬盘读取数据
publicStringreadData(){
return"数据";
}
}

## 接口作为类型使用
接口作为引用类型来使用，任何实现该接口的类的实例都可以存储在该接口类型的变量中，通过这些变量可以访问类中所实现的接口中的方法，Java 运行时系统会动态地确定应该使用哪个类中的方法，实际上是调用相应的实现类的方法。
示例如下：[复制](http://www.weixueyuan.net/view/6009.html#)[纯文本](http://www.weixueyuan.net/view/6009.html#)[新窗口](http://www.weixueyuan.net/view/6009.html#)
publicclassDemo{
publicvoidtest1(Aa){
a.doSth();
}
publicstaticvoidmain(String[]args){
Demod=newDemo();
Aa=newB();
d.test1(a);
}
}
interfaceA{
publicintdoSth();
}
classBimplementsA{
publicintdoSth(){
System.out.println("now in B");
return123;
}
}

运行结果：
now in B
大家看到接口可以作为一个类型来使用，把接口作为方法的参数和返回类型。

