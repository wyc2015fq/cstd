# Java 类/抽象类/接口 - Simple 专栏 - CSDN博客
2012年01月06日 20:47:59[Simple_Zz](https://me.csdn.net/love284969214)阅读数：307标签：[java																[Java基础																[Java面试																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=Java面试&t=blog)](https://so.csdn.net/so/search/s.do?q=Java基础&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Java](https://blog.csdn.net/love284969214/article/category/1235868)
所属专栏：[Java 浅谈](https://blog.csdn.net/column/details/27397.html)
Java的类可以分为三类：
- 类：使用class定义，没有abstract方法。
- 抽象类：使用abstract class定义，可以有也可以没有abstract方法。
- 接口：使用inerface定义，只能有abstract方法。
在这三个类型之间存在如下关系：
- 类可以继承：类、抽象类（必须实现所有抽象方法），可以实现多个接口（必须实现所有接口方法）。
- 抽象类可以继承：类，抽象类（可随意实现抽象方法），可以实现多个接口（可随意实现接口方法）。
- 接口只能继承一个接口。
在使用抽象类时需要注意几点：
- 抽象类不能被实例化，实例化的工作应该交由它的子类来完成，它只需要有一个引用即可。
- 抽象方法必须由子类来进行重写。
- 只要包含一个抽象方法的抽象类，该方法必须要定义成抽象类，不管是否还包含有其他方法。
- 抽象类中可以包含具体的方法，当然也可以不包含抽象方法。
- 子类中的抽象方法不能与父类的抽象方法同名。
- abstract不能与final并列修饰同一个类。
- abstract 不能与private、static、final或native并列修饰同一个方法。
在使用接口过程中需要注意如下几个问题：
- interface的方所有法访问权限自动被声明为public。确切的说只能为public，当然你可以显示的声明为protected、private，但是编译会出错！
- 接口中可以定义“成员变量”，或者说是不可变的常量，因为接口中的“成员变量”会自动变为为public static final。可以通过类命名直接访问：ImplementClass.name。
- 接口中不存在实现的方法。
- 实现接口的非抽象类必须要实现该接口的所有方法。抽象类可以不用实现。
- 不能使用new操作符实例化一个接口，但可以声明一个接口变量，该变量必须引用（refer to)一个实现该接口的类的对象。可以使用 instanceof 检查一个对象是否实现了某个特定的接口。例如：if(anObject instanceof Comparable){}。
- 在实现多接口的时候一定要避免方法名的重复。
抽象类与接口的区别：
- 一个类 （继承 --> 抽象类） 或 （实现 --> 接口） 必须实现全部抽象方法，否则该类仍然需要被声明为抽象类。
- 抽象类和接口都不能够实例化。
- 抽象类和接口都可以被引用。
- 抽象类中可以定义构造器，而接口中不能定义构造器。
- 抽象类中可以有抽象方法和具体方法，而且其中的方法全部都是抽象方法。
- 抽象类中的成员可以是private、default、protected、public的，而接口中的成员全都是public的。
- 抽象类中可以定义成员变量，而接口中定义的成员变量实际上都是常量。
- 有抽象方法的类必须被声明为抽象类，而抽象类未必要有抽象方法。
