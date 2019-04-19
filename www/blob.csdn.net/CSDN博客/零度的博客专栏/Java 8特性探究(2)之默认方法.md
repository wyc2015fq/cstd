# Java 8特性探究(2)之默认方法 - 零度的博客专栏 - CSDN博客
2016年11月01日 10:20:08[零度anngle](https://me.csdn.net/zmx729618)阅读数：208标签：[java8](https://so.csdn.net/so/search/s.do?q=java8&t=blog)
个人分类：[Java](https://blog.csdn.net/zmx729618/article/category/3108443)
                
        上篇讲了[lambda表达式](http://www.dexcoder.com/selfly/article/3995)的语法，但只是 JEP126 特性的一部分，另一部分就是默认方法（也称为虚拟扩展方法或防护方法）
什么是默认方法，为什么要有默认方法
       简单说，就是接口可以有实现方法，而且不需要实现类去实现其方法。只需在方法名前面加个default关键字即可。
        为什么要有这个特性？首先，之前的接口是个双刃剑，好处是面向抽象而不是面向具体编程，缺陷是，当需要修改接口时候，需要修改全部实现该接口的类，目前的 java 8之前的集合框架没有foreach方法，通常能想到的解决办法是在JDK里给相关的接口添加新的方法及实现。然而，对于已经发布的版本，是没法在给接口 添加新方法的同时不影响已有的实现。所以引进的默认方法。他们的目的是为了使接口没有引入与现有的实现不兼容发展。
简单的例子
      一个接口A，Clazz类实现了接口A。
```java
public interface A { 
    default void foo(){ 
       System.out.println("Calling A.foo()"); 
    } 
} 
public class Clazz implements A { 
    public static void main(String[] args){ 
       Clazz clazz = new Clazz(); 
       clazz.foo();//调用A.foo() 
    } 
}
```
        代码是可以编译的，即使Clazz类并没有实现foo()方法。在接口A中提供了foo()方法的默认实现。
java 8抽象类与接口对比
         这一个功能特性出来后，很多同学都反应了，java 8的接口都有实现方法了，跟抽象类还有什么区别？其实还是有的，请看下表对比。
|相同点|不同点|
|----|----|
|1.都是抽象类型；2.都可以有实现方法（以前接口不行）；3.都可以不需要实现类或者继承者去实现所有方法，（以前不行，现在接口中默认方法不需要实现者实现）|1.抽象类不可以多重继承，接口可以（无论是多重类型继承还是多重行为继承）；2.抽象类和接口所反映出的设计理念不同。其实抽象类表示的是"is-a"关系，接口表示的是"like-a"关系；3.接口中定义的变量默认是public static final 型，且必须给其初值，所以实现类中不能重新定义，也不能改变其值；抽象类中的变量默认是 friendly 型，其值可以在子类中重新定义，也可以重新赋值。|
多重继承的冲突说明
        由于同一个方法可以从不同接口引入，自然而然的会有冲突的现象，默认方法判断冲突的规则如下：
        1.一个声明在类里面的方法优先于任何默认方法（classes always win）
        2.否则，则会优先选取最具体的实现，比如下面的例子 B重写了A的hello方法。
```java
package zmx.test;
public interface A {
   default void hello(){
	   System.out.println("Hello World from A");
   }
}
public interface B extends A{
	default void hello(){
		   System.out.println("Hello World from B");
	}
}
public class C implements B,A{
	@Override
	public void hello() {
		new C().hello();
		
	}
	
}
```
        输出结果是：Hello World from B
        如果想调用A的默认函数，则用到新语法X.super.m(...),下面修改C类，重写一个hello方法，如下所示：
```java
public class C implements B,A{
	@Override
	public void hello() {
		A.super.hello();	
	}
}
```
        输出结果是：Hello World from A
总结
       默认方法给予我们修改接口而不破坏原来的实现类的结构提供了便利，目前java 8的集合框架已经大量使用了默认方法来改进了，当我们最终开始使用Java 8的lambdas表达式时，提供给我们一个平滑的过渡体验。也许将来我们会在API设计中看到更多的默认方法的应用。 
