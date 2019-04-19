# Java 继承/实现 extends 与 implements区别 - Simple 专栏 - CSDN博客
2012年01月09日 19:37:24[Simple_Zz](https://me.csdn.net/love284969214)阅读数：301
所属专栏：[Java 浅谈](https://blog.csdn.net/column/details/27397.html)
extends 是继承父类，只要那个类不是声明final或者定义为abstract就能继承,Java中不支持多重继承，继承只能继承一个类，但implements可以实现多个接口，用逗号分开就行了。
比如:
```
class A extends B implements C,D,E(){ //class子类名extends父类名implements接口名
    
}
```
父类与子类继承关系上的不同：
```
class A{
    int i;
    void f(){
        
    }
}
class B extends A{
    int j;
    void f(){}  //重写方法
    void g(){}
}
```
B b=new B();
b就是子类对象的实例，不仅能够方位自己的属性和方法，也能访问父类的属性和方法。诸如b.i,b.j.b.fn()都是合法的。此时b.f()是B中的f()
A a=new B();
a虽然是用的B的构造函数，但经过upcast，成为父类对象的实例，不能访问子类的属性和方法。a.i,a.f()是合法的，而a.j,a.g()是非法的。此时访问a.f()是访问B中的f();
**实现过程**
A a = new B(); 这条语句，实际上有三个过程：
(1) A a;
将a声明为父类对象，只是一个引用，未分配空间
(2) B temp = new B();
通过B类的构造函数建立了一个B类对象的实例，也就是初始化
(3) a = (A)temp;
将子类对象temp转换未父类对象并赋给a，这就是上传(upcast)，是安全的。
经过以上3个过程，a就彻底成为了一个A类的实例。
子类往往比父类有更多的属性和方法，上传只是舍弃，是安全的；而下传(downcast)有时会增加，通常是不安全的。
**多态**
a.f()对应的应该是B类的方法f()
调用构造函数建立实例后，对应方法的入口已经确定了。
如此一来，a虽被上传为A类，但其中重写的方法f()仍然是B的方法()。也就是说，每个对象知道自己应该调用哪个方法。
A a1 = new B();
A a2 = new C();
a1,a2两个虽然都是A类对象，但各自的f()不同。这正是多态性的体现。
