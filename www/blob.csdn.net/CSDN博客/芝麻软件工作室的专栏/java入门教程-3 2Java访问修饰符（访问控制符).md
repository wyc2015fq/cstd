
# java入门教程-3.2Java访问修饰符（访问控制符) -  芝麻软件工作室的专栏 - CSDN博客


2016年05月24日 11:17:39[seven-soft](https://me.csdn.net/softn)阅读数：185


Java 通过修饰符来控制类、属性和方法的访问权限和其他功能，通常放在语句的最前端。例如：publicclassclassName{
// body of class
}
privatebooleanmyFlag;
staticfinaldoubleweeks=9.5;
protectedstaticfinalintBOXWIDTH=42;
publicstaticvoidmain(String[]arguments){
// body of method
}

Java 的修饰符很多，分为访问修饰符和非访问修饰符。本节仅介绍访问修饰符，非访问修饰符会在后续介绍。
访问修饰符也叫访问控制符，是指能够控制类、成员变量、方法的使用权限的关键字。
在面向对象编程中，访问控制符是一个很重要的概念，可以使用它来保护对类、变量、方法和构造方法的访问。
Java支持四种不同的访问权限：
|修饰符|说明|
|public|共有的，对所有类可见。|
|protected|受保护的，对同一包内的类和所有子类可见。|
|private|私有的，在同一类内可见。|
|默认的|在同一包内可见。默认不使用任何修饰符。|
## public：公有的
被声明为public的类、方法、构造方法和接口能够被任何其他类访问。
如果几个相互访问的public类分布在不用的包中，则需要导入相应public类所在的包。由于类的继承性，类所有的公有方法和变量都能被其子类继承。
下面的方法使用了公有访问控制：publicstaticvoidmain(String[]arguments){
// body of method
}

Java程序的main() 方法必须设置成公有的，否则，Java解释器将不能运行该类。
## protected：受保护的
被声明为protected的变量、方法和构造方法能被同一个包中的任何其他类访问，也能够被不同包中的子类访问。
protected访问修饰符不能修饰类和接口，方法和成员变量能够声明为protected，但是接口的成员变量和成员方法不能声明为protected。
子类能访问protected修饰符声明的方法和变量，这样就能保护不相关的类使用这些方法和变量。
下面的父类使用了protected访问修饰符，子类重载了父类的bark()方法。publicclassDog{
protectedvoidbark(){
System.out.println("汪汪，不要过来");
}
}
classTeddyextendsDog{// 泰迪
voidbark(){
System.out.println("汪汪，我好怕，不要跟着我");
}
}

如果把bark()方法声明为private，那么除了Dog之外的类将不能访问该方法。如果把bark()声明为public，那么所有的类都能够访问该方法。如果我们只想让该方法对其所在类的子类可见，则将该方法声明为protected。
## private：私有的
私有访问修饰符是最严格的访问级别，所以被声明为private的方法、变量和构造方法只能被所属类访问，并且类和接口不能声明为private。
声明为私有访问类型的变量只能通过类中公共的Getter/Setter方法被外部类访问。
private访问修饰符的使用主要用来隐藏类的实现细节和保护类的数据。
下面的类使用了私有访问修饰符：publicclassDog{
privateStringname;
privateintage;
publicStringgetName(){
returnname;
}
publicvoidsetName(Stringname){
this.name=name;
}
publicintgetAge(){
returnage;
}
publicvoidsetAge(intage){
this.age=age;
}
}

例子中，Dog类中的name、age变量为私有变量，所以其他类不能直接得到和设置该变量的值。为了使其他类能够操作该变量，定义了两对public方法，getName()/setName() 和 getAge()/setAge()，它们用来获取和设置私有变量的值。
this 是Java中的一个关键字，本章会讲到，你可以点击[Java
 this关键字详解](http://www.weixueyuan.net/view/5988.html)预览。
在类中定义访问私有变量的方法，习惯上是这样命名的：在变量名称前面加“get”或“set”，并将变量的首字母大写。例如，获取私有变量 name 的方法为 getName()，设置 name 的方法为 setName()。这些方法经常使用，也有了特定的称呼，称为
 Getter 和 Setter 方法。
## 默认的：不使用任何关键字
不使用任何修饰符声明的属性和方法，对同一个包内的类是可见的。接口里的变量都隐式声明为public static final，而接口里的方法默认情况下访问权限为public。
如下例所示，类、变量和方法的定义没有使用任何修饰符：
classDog{
Stringname;
intage;
voidbark(){// 汪汪叫
System.out.println("汪汪，不要过来");
}
voidhungry(){// 饥饿
System.out.println("主人，我饿了");
}
}

## 访问控制和继承
请注意以下方法继承（不了解继承概念的读者可以跳过这里，或者点击[Java继承和多态](http://www.weixueyuan.net/java/rumen_5/)预览）的规则：
父类中声明为public的方法在子类中也必须为public。
父类中声明为protected的方法在子类中要么声明为protected，要么声明为public。不能声明为private。
父类中默认修饰符声明的方法，能够在子类中声明为private。
父类中声明为private的方法，不能够被继承。

## 如何使用访问控制符
访问控制符可以让我们很方便的控制代码的权限：
当需要让自己编写的类被所有的其他类访问时，就可以将类的访问控制符声明为 public。
当需要让自己的类只能被自己的包中的类访问时，就可以省略访问控制符。
当需要控制一个类中的成员数据时，可以将这个类中的成员数据访问控制符设置为 public、protected，或者省略。

