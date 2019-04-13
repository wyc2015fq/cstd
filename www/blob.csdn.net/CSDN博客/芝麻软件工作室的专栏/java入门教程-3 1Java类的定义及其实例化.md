
# java入门教程-3.1Java类的定义及其实例化 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月24日 11:15:49[seven-soft](https://me.csdn.net/softn)阅读数：328


如果你不了解类和对象的概念，请猛击这里：[Java类和对象的概念](http://www.weixueyuan.net/view/6311.html)
类必须先定义才能使用。类是创建对象的模板，创建对象也叫类的实例化。
下面通过一个简单的例子来理解Java中类的定义：publicclassDog{
Stringname;
intage;
voidbark(){// 汪汪叫
System.out.println("汪汪，不要过来");
}
voidhungry(){// 饥饿
System.out.println("主人，我饿了");
}
}

对示例的说明：public 是类的修饰符，表明该类是公共类，可以被其他类访问。修饰符将在下节讲解。
class 是定义类的关键字。
Dog 是类名称。
name、age 是类的成员变量，也叫属性；bark()、hungry() 是类中的函数，也叫方法。
一个类可以包含以下类型变量：局部变量：在方法或者语句块中定义的变量被称为局部变量。变量声明和初始化都是在方法中，方法结束后，变量就会自动销毁。
成员变量：成员变量是定义在类中、方法体之外的变量。这种变量在创建对象的时候实例化（分配内存）。成员变量可以被类中的方法和特定类的语句访问。
类变量：类变量也声明在类中，方法体之外，但必须声明为static类型。static 也是修饰符的一种，将在下节讲解。
## 构造方法
在类实例化的过程中自动执行的方法叫做构造方法，它不需要你手动调用。构造方法可以在类实例化的过程中做一些初始化的工作。
构造方法的名称必须与类的名称相同，并且没有返回值。
每个类都有构造方法。如果没有显式地为类定义构造方法，Java编译器将会为该类提供一个默认的构造方法。
下面是一个构造方法示例：publicclassDog{
Stringname;
intage;
// 构造方法，没有返回值
Dog(Stringname1,intage1){
name=name1;
age=age1;
System.out.println("感谢主人领养了我");
}
// 普通方法，必须有返回值
voidbark(){
System.out.println("汪汪，不要过来");
}
voidhungry(){
System.out.println("主人，我饿了");
}
publicstaticvoidmain(Stringarg[]){
// 创建对象时传递的参数要与构造方法参数列表对应
DogmyDog=newDog("花花",3);
}
}

运行结果：
感谢主人领养了我
说明：构造方法不能被显示调用。
构造方法不能有返回值，因为没有变量来接收返回值。
## 创建对象
对象是类的一个实例，创建对象的过程也叫类的实例化。对象是以类为模板来创建的。
在Java中，使用new关键字来创建对象，一般有以下三个步骤：声明：声明一个对象，包括对象名称和对象类型。
实例化：使用关键字new来创建一个对象。
初始化：使用new创建对象时，会调用构造方法初始化对象。
例如：DogmyDog;// 声明一个对象
myDog=newDog("花花",3);// 实例化

也可以在声明的同时进行初始化：DogmyDog=newDog("花花",3);

## 访问成员变量和方法
通过已创建的对象来访问成员变量和成员方法，例如：// 实例化
DogmyDog=newDog("花花",3);
// 通过点号访问成员变量
myDog.name;
// 通过点号访问成员方法
myDog.bark();

下面的例子演示了如何访问成员变量和方法：publicclassDog{
Stringname;
intage;
Dog(Stringname1,intage1){
name=name1;
age=age1;
System.out.println("感谢主人领养了我");
}
voidbark(){
System.out.println("汪汪，不要过来");
}
voidhungry(){
System.out.println("主人，我饿了");
}
publicstaticvoidmain(Stringarg[]){
DogmyDog=newDog("花花",3);
// 访问成员变量
Stringname=myDog.name;
intage=myDog.age;
System.out.println("我是一只小狗，我名字叫"+name+"，我"+age+"岁了");
// 访问方法
myDog.bark();
myDog.hungry();
}
}

运行结果：
感谢主人领养了我
我是一只小狗，我名字叫花花，我3岁了
汪汪，不要过来
主人，我饿了

