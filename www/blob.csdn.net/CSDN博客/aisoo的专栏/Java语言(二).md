# Java语言(二) - aisoo的专栏 - CSDN博客
2009年11月10日 11:14:00[aisoo](https://me.csdn.net/aisoo)阅读数：409标签：[java																[语言																[class																[编译器																[import																[string](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=import&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
 二  **对象**
    类实例化可生成对象，对象通过消息传递来进行交互。消息传递即激活指定的某个对象的方法以改变其状态或让它产生一定的行为。一个对象的生命周期包括三个阶段：生成、使用和消除。
   对象的清除
　　 当不存在对一个对象的引用时，该对象成为一个无用对象。Java的垃圾收集器自动扫描对象的动态内存区，把没有引用的对象作为垃圾收集起来并释放。
　　System.gc( 
);　　System.exit();//terminate the current JVM
　　当系统内存用尽或调用System.gc( 
)要求垃圾回收时，垃圾回收线程与系统同步运行。
**java语言中有三个典型的面向对象的特性：封装性、继承性和多态性。**
1． 封装性
        java语言中，对象就是对一组变量和相关方法的封装，其中变量表明了对象的状态，方法表明了对象具有的行为。通过对象的封装，实现了模块化和信息隐藏。通过对类的成员施以一定的访问权限，实现了类中成员的信息隐藏。
    ◇ java类中的限定词
　　java语言中有四种不同的限定词，提供了四种不同的访问权限。
　　1） private 
　　    类中限定为private的成员，只能被这个类本身访问。
　　    如果一个类的构造方法声明为private,则其它类不能生成该类的一个实例。
　　2） 
default
　　    类中不加任何访问权限限定的成员属于缺省的（default）访问状态：friend，可以被这个类本身和同一个包中的类所访问。
    3） 
protected
　　    类中限定为protected的成员，可以被这个类本身、它的子类（包括同一个包中以及不同包中的子类）和同一个包中的所有其他的类访问。
   4） 
public
　　   类中限定为public的成员，可以被所有的类访问。
   2． 继承性
        通过继承实现代码复用。Java中所有的类都是通过直接或间接地继承java.lang.Object类得到的。继承而得到的类称为子类，被继承的类称为父类。子类不能继承父类中访问权限为private的成员变量和方法。子类可以重写父类的方法，及命名与父类同名的成员变量。但Java不支持多重继承，即一个类从多个超类派生的能力。
      ◇ 
成员变量的隐藏和方法的重写
　　子类通过隐藏父类的成员变量和重写父类的方法，可以把父类的状态和行为改变为自身的状态和行为。
例如：
　　class 
SuperClass{
　　　　int x; …
　　　　void setX( ){ x=0; } …
　　}
　　class 
SubClass extends SuperClass{
　　　　int x; 　　//隐藏了父类的变量x
　　　　…
　　　　void 
setX( ) { //重写了父类的方法 setX()
　　　　x=5; } 
….
　　}
　　注意：子类中重写的方法和父类中被重写的方法要具有相同的名字，相同的参数表和相同的返回类型，只是函数体不同。
　　◇ 
super
　　java中通过super来实现对父类成员的访问，super用来引用当前对象的父类。Super 
的使用有三种情况：
　　1）访问父类被隐藏的成员变量，如：
　　　　super.variable; 
　　2）调用父类中被重写的方法，如：
　　　　super.Method([paramlist]);
　　3）调用父类的构造函数，如：
　　　　super([paramlist]);
   3． 多态性
      在java语言中，多态性体现在两个方面：由方法重载实现的静态多态性（编译时多态）和方法重写实现的动态多态性（运行时多态）。
　　 1） 
编译时多态
　　     在编译阶段，具体调用哪个被重载的方法，编译器会根据参数的不同来静态确定调用相应的方法。
　　 2） 
运行时多态
　　   由于子类继承了父类所有的属性（私有的除外），所以子类对象可以作为父类对象使用。程序中凡是使用父类对象的地方，都可以用子类对象来代替。一个对象可以通过引用子类的实例来调用子类的方法。
　　◇ 
重写方法的调用原则：java运行时系统根据调用该方法的实例，来决定调用哪个方法。对子类的一个实例，如果子类重写了父类的方法，则运行时系统调用子类的方法；如果子类继承了父类的方法（未重写），则运行时系统调用父类的方法。
在例3-1中，父类对象a引用的是子类的实例，所以，java运行时调用子类B的callme方法。
【例3-1】
　　import 
java.io.*;
　　class A{
　　　　　void callme( ) 
{
　　　　　　System.out.println("Inside A's callme()method"); 
　　　　　} 
　　}
　　class B extends A{
　　　　　void callme( ) 
{
　　　　　　System.out.println("Inside B's callme() Method"); 
　　　　　}
　　}
　　public class Dispatch{
　　　　　public static void 
main(String args[]) {
　　　　　　A a=new B();
　　　　　　a.callme( 
);
　　　　　}
　　}
◇ 
方法重写时应遵循的原则：
　　1）改写后的方法不能比被重写的方法有更严格的访问权限（可以相同）。
　　2）改写后的方法不能比重写的方法产生更多的例外。
