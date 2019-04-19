# Java类、对象、封装、继承、多态、打包器详解--(五) - fanyun的博客 - CSDN博客
2016年03月26日 09:22:37[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1016
所属专栏：[Java框架精要](https://blog.csdn.net/column/details/java-01.html)
一、面向对象基本概念：类、对象、封装、继承、多态、打包器
（1）   封装：就是把数据和行为结合在一起形成统一的整体，并对对象使用者隐藏数据的实现过程。
（2）   继承：Java继承是使用已存在的类的定义作为基础建立新类的技术，继承避免了对一般类和特殊类之间共同特征进行的重复描述。
多态：多态指同一个实体同时具有多种形式。
类与对象关系：类是描述具有相同特征的一类事物的基本原型，定义了这类事物所拥有的数据特征以及可以执行的操作；对象是类的实例，是类的具体化
java类是面向对象封装概念的基本体现，java类封装了抽象概念的数据（属性）与行为（方法）。
java类的基本构成：属性、方法、构造方法
属性是是类对抽象概念数据特征的描述，方法是类对概念行为的描述，构造方法在类的对象实例化时对类对象进行初始化。
（3）   重载与覆写（重写）
重载：方法的重载是多态性的体现，重载方法具有相同的方法名称，但方法参数列表不同（参数类型或数目不同），重载为相似功能提供了不同的实现
重写：重写是指在子类中覆盖父类方法的实现，对父类方法进行重新定义，当父类引用指向子类对象并调用重写方法时，将调用子类方法的实现。子类函数的访问修饰权限不能低于父类的。
（4）   super与this
super代表当前类的父类（超类），子类的构造函数如果要引用super的话，必须把super放在函数的首位；当子类变量与父类变量重名时，使用super调用父类变量
this代表当前对象，this使用：this.属性，this.方法，this（）；使用this来区别重名的局部变量与成员变量；使用this在一个构造函数中调用其他的重载构造函数
java修饰符：访问权限修饰符、final、static、abstract
访问权限修饰符：public、protected、private、default
public 成员对所有类可见
private 成员仅类内部可见
protected相同包中的类可以访问（包访问权限）；基类通过protected把基类中的成员的访问权限赋予派生类不是所有类（派生类访问权限）。
default如果一个类的成员没有任何权限修饰，那么它门就是缺省包访问权限
类仅能用public或默认权限修饰
（5）   final修饰符：
       final修饰成员变量：该变量为常量；修饰方法：该方法不能够在子类中被重写；修饰类：该类不能被继承
（6）   static修饰符：
       修饰成员变量：该变量为静态变量（类变量），属于类本身，所有该类对象公用该变量；
       修饰方法：该方法为静态方法，在静态方法中不能使用非静态成员变量或方法，因为在静态方法调用时可能还没有对象被创建，没有对象也就无法获取其成员。静态成员函数中也不能使用this或者super，因为它们是和类的对象相关联的
       静态内部类：静态内部类可以对照静态成员变量来理解
abstract修饰符：
      定义抽象类、方法
     抽象方法 没有方法体｛｝，仅有方法声明
抽象类中可以没有抽象方法，有抽象方法的类一定是抽象类
（7）   Java中定义类：
class 类名
用Java语法定义人类：
public class Person
{
}
（8）  定义对象：
对象声明：类名对象名；
对象创建对象名 =  new类名（）； new作用：分配内存空间。也可以合写为：类名对象名
 = new类名();
（9）   Java基本类型打包器
每一个基本类型 都有一个与之对应的类，这个类就称为对象包装器。 
例如，int 的对象包装器就是 Integer。 
其余的还有： 
   Long、Double、Float、Short、Byte、Character... 
   它能提供丰富的方法！ 
   注意对象包装器是不可以变的，一旦构造了包装器，就不容许更改其中的值了！ 
自动打包 解包 
申明了一个list，List的add方法： 
boolean add(Object obj); 
如果我们只是要在list中加入一些简单的int数据，还得 
list.add(new Integer(10)); 
麻烦吗？ 
要是可以list.add(10);就好了！ 
java se 5.0 可以了，他能将list.add(10);自动转成list.add(new Integer(10)); 
这个过程就是自动打包的过程！ 
取数据的时候，以前得： 
int a = list.get(i).intValue(); 
现在： 
int a = list.get(i); 
自动完成的这个过程就是自动解包。 
API【Integer】 
int intValue(); 
static String toString(int i ); 
static int parseInt(String s); 
static Integer valueOf(String s); 
    基本数据(Primitive)类型的自动装箱(autoboxing)、拆箱(unboxing)是自J2SE 5.0开始提供的功能。虽然为您打包基本数据类型提供了方便，但提供方便的同时表示隐藏了细节，建议在能够区分基本数据类型与对象的差别时再使用。 
     autoboxing和unboxing 
     在Java中，所有要处理的东西几乎都是对象 (Object)，例如之前所使用的Scanner是对象，字符串(String)也是对象，之后还会看到更多的对象。然而基本(Primitive)数据类型不是对象，也就是您使用int、double、boolean等定义的变量，以及您在程序中直接写下的字面常量。 
     在前一个小节中已经大致看到了操作对象的方便性，而使用Java有一段时间的人都知道，有时需要将基本数据类型转换为对象。例如使用Map对象要操作put()方法时，需要传入的参数是对象而不是基本数据类型。 
     要使用打包类型(Wrapper Types)才能将基本数据类型包装为对象，前一个小节中您已经知道在J2SE 5.0之前，要使用以下语句才能将int包装为一个Integer对象：Integer integer = new Integer(10); 
     在 J2SE 5.0之后提供了自动装箱的功能，您可以直接使用以下语句来打包基本数据类型：Integer integer = 10; 
     在进行编译时，编译器再自动根据您写下的语句，判断是否进行自动装箱动作。在上例中integer参考的会是Integer类的实例。同样的动作可以适用于 boolean、byte、short、char、long、float、double等基本数据类型，分别会使用对应的打包类型(Wrapper Types)Boolean、Byte、Short、Character、Long、Float或Double。下面直接使用自动装箱功能来改写范例 4.4。 
     范例4.5   AutoBoxDemo.java 
     public class AutoBoxDemo { 
         public static voidmain(String[] args) { 
            Integer data1 = 10; 
            Integer data2 =20;            
             // 转为double值再除以3 
            System.out.println(data1.doubleValue() / 3); 
             // 进行两个值的比较 
            System.out.println(data1.compareTo(data2)); 
         } 
     } 
     程序看来简洁了许多，data1与data2在运行时就是Integer的实例，可以直接进行对象操作。执行的结果如下： 
     3.3333333333333335 
     –1 
     自动装箱运用的方法还可以如下： 
     int i = 10; 
     Integer integer = i; 
     也可以使用更一般化的java.lang.Number类来自动装箱。例如： 
     Number number = 3.14f; 
     3.14f会先被自动装箱为Float，然后指定给number。 
     从J2SE 5.0开始可以自动装箱，也可以自动拆箱(unboxing)，也就是将对象中的基本数据形态信息从对象中自动取出。例如下面这样写是可以的： 
     Integer fooInteger = 10; 
     int fooPrimitive = fooInteger; 
     fooInteger引用至自动装箱为Integer的实例后，如果被指定给一个int类型的变量fooPrimitive，则会自动变为int类型再指定给fooPrimitive。在运算时，也可以进行自动装箱与拆箱。例如： 
     Integer i = 10; 
     System.out.println(i + 10); 
     System.out.println(i++); 
     上例中会显示20与10，编译器会自动进行自动装箱与拆箱，也就是10会先被装箱，然后在i + 10时会先拆箱，进行加法运算；i++该行也是先拆箱再进行递增运算。再来看一个例子： 
     Boolean boo = true; 
     System.out.println(boo && false); 
     同样的boo原来是Boolean的实例，在进行AND运算时，会先将boo拆箱，再与false进行AND运算，
