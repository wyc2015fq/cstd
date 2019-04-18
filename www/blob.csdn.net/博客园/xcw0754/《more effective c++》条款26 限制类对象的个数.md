# 《more effective c++》条款26 限制类对象的个数 - xcw0754 - 博客园
# [《more effective c++》条款26 限制类对象的个数](https://www.cnblogs.com/xcw0754/p/4965880.html)
问题：
　　如何限制类对象的个数？比如1个，10个等等。
方法（1）：
　　将类的构造函数定义为private，那么就无法实例化这个类了。但是如何创建1个对象出来？方法有2种：
　　1.声明一个友元函数，那么在友元函数中就可以调用构造函数了，创建对象时使用static限制，那么就保证只有一个对象了。类似的定义如下：
```
1 class Printer
 2 {
 3 public:
 4     friend Printer& thePrinter();
 5 private:
 6     Printer();
 7     Printer(const Printer& rhs);
 8 };
 9 Printer& thePrinter()
10 {
11     static Printer p;
12     return p;
13 }
```
　　2.定义一个静态成员函数来代替上面的友元函数也是可以达到同样效果的，就是调用时麻烦了些，需要加上类名才能调用函数，但是这也是可以解决的，将类定义在一个命名空间（namespace）中，再using该命名空间即可直接使用函数名来调用了（具体去看如何定义命名空间）。使用命名空间还可以防止出现名字冲突问题，而且将对象声明在函数中的好处是，只有当调用该函数时，对象才会被创建。注意：尽量不要在内联函数中创建静态对象，可能产生多个副本。
```
1 class Printer
 2 {
 3 public:
 4     static Printer& thePrinter();//注意这里
 5 private:
 6     Printer();
 7     Printer(const Printer& rhs);
 8 };
 9 Printer& thePrinter()
10 {
11     static Printer p;
12     return p;
13 }
```
方法（2）：
　　在类中定义一个静态计数器，通过计数器来限制实例对象的个数，一旦创建超过限制的个数就抛出异常。这貌似更加好用。
　　如果单例类是作为基类被继承，那么派生类对象中的基类部分也会被统计进去了，比如彩色打印机继承自打印机，而现在有1台打印机，2台彩色打印机，计数器就是3了。显然不符合我们的要求，我们可以通过private构造函数，那么单例类就无法被继承了。
**允许对象来去自由**
　　通常情况下，我们希望可以这样：
```
1 create Printer1;
2 use Printer1;
3 destroy Printer1;
4 
5 create Printer2;
6 use Printer2;
7 destroy Printer2;
```
　　但是方法（1）并不能支持。但是结合计数器就可以实现了。具体看书 P105中的代码例子。
**一个用于对象计数的基类**
　　如果需要多个具体类怎么办？难道一个个去写去实现？其实可以用模板类作为基类来完成这个功能，使用前将设置一下实例个数就可以使用了，具体看书 P107。　　

