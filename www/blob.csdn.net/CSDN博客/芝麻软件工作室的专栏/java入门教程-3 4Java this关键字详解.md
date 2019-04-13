
# java入门教程-3.4Java this关键字详解 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月25日 08:42:37[seven-soft](https://me.csdn.net/softn)阅读数：135个人分类：[Java																](https://blog.csdn.net/softn/article/category/6242590)



this 关键字用来表示当前对象本身，或当前类的一个实例，通过 this 可以调用本对象的所有方法和属性。例如：publicclassDemo{
publicintx=10;
publicinty=15;
publicvoidsum(){
// 通过 this 点取成员变量
intz=this.x+this.y;
System.out.println("x + y = "+z);
}
publicstaticvoidmain(String[]args){
Demoobj=newDemo();
obj.sum();
}
}

运行结果：
x + y = 25
上面的程序中，obj 是 Demo 类的一个实例，this 与 obj 等价，执行 int z = this.x + this.y;，就相当于执行 int z = obj.x + obj.y;。
注意：this 只有在类实例化后才有意义。
## 使用this区分同名变量
成员变量与方法内部的变量重名时，希望在方法内部调用成员变量，怎么办呢？这时候只能使用this，例如：
publicclassDemo{
publicStringname;
publicintage;
publicDemo(Stringname,intage){
this.name=name;
this.age=age;
}
publicvoidsay(){
System.out.println("网站的名字是"+name+"，已经成立了"+age+"年");
}
publicstaticvoidmain(String[]args){
Demoobj=newDemo("微学苑",3);
obj.say();
}
}

运行结果：
网站的名字是微学苑，已经成立了3年
形参的作用域是整个方法体，是局部变量。在Demo()中，形参和成员变量重名，如果不使用this，访问到的就是局部变量name和age，而不是成员变量。在 say() 中，我们没有使用 this，因为成员变量的作用域是整个实例，当然也可以加上
 this：publicvoidsay(){
System.out.println("网站的名字是"+this.name+"，已经成立了"+this.age+"年");
}

Java 默认将所有成员变量和成员方法与 this 关联在一起，因此使用 this 在某些情况下是多余的。
## 作为方法名来初始化对象
也就是相当于调用本类的其它构造方法，它必须作为构造方法的第一句。示例如下：publicclassDemo{
publicStringname;
publicintage;
publicDemo(){
this("微学苑",3);
}
publicDemo(Stringname,intage){
this.name=name;
this.age=age;
}
publicvoidsay(){
System.out.println("网站的名字是"+name+"，已经成立了"+age+"年");
}
publicstaticvoidmain(String[]args){
Demoobj=newDemo();
obj.say();
}
}

运行结果：
网站的名字是微学苑，已经成立了3年
值得注意的是：在构造方法中调用另一个构造方法，调用动作必须置于最起始的位置。
不能在构造方法以外的任何方法内调用构造方法。
在一个构造方法内只能调用一个构造方法。
上述代码涉及到方法重载，即Java允许出现多个同名方法，只要参数不同就可以。后续章节会讲解。
## 作为参数传递
需要在某些完全分离的类中调用一个方法，并将当前对象的一个引用作为参数传递时。例如：publicclassDemo{
publicstaticvoidmain(String[]args){
Bb=newB(newA());
}
}
classA{
publicA(){
newB(this).print();// 匿名对象
}
publicvoidprint(){
System.out.println("Hello from A!");
}
}
classB{
Aa;
publicB(Aa){
this.a=a;
}
publicvoidprint(){
a.print();
System.out.println("Hello from B!");
}
}

运行结果：
Hello from A!
Hello from B!
匿名对象就是没有名字的对象。如果对象只使用一次，就可以作为匿名对象，代码中 new B(this).print(); 等价于 ( new B(this) ).print();，先通过 new B(this) 创建一个没有名字的对象，再调用它的方法。

