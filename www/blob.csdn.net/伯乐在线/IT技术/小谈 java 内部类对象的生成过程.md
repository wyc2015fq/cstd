# 小谈 java 内部类对象的生成过程 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [占小狼](http://www.jobbole.com/members/wx4097456919) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
内部类指在一个外部类的内部再定义一个类，有点废话…，分为静态内部类和成员内部类，平常使用较多的为静态内部类，使用方式和普通类一样，本文准备在字节码层面看看成员内部类对象的生成过程。

Java
```
public class Outer {  
  public class Inner { }  
  public static void main(String[] args) {  
    Outer outer = new Outer();  
    Outer.Inner inner = outer.new Inner();  
  }  
}
```
采用`javap -c Outer`命令查看字节码指令
![](http://jbcdn2.b0.upaiyun.com/2016/12/64e7d256baa07e29e959777f4d8e72bd.png)
 1、第18行的invokespecial指令，调用Inner类的构造方法，并传入outer实例作为参数，所以在inner实例中可以访问外部类实例；
2、在14行中调用了`outer.getClass()`方法，得到的结果又在17行的pop指令抛弃了，是不是很奇怪？
先看看Java语言规范
> 
15.9.4 Run-time Evaluation of Class Instance Creation Expressions
At run time, evaluation of a class instance creation expression is as follows. First, if the class instance creation expression is a qualified class instance creation expression, the qualifying primary expression is evaluated. If the qualifying expression evaluates to **null**, a **NullPointerException** is raised, and the class instance creation expression completes abruptly. If the qualifying expression completes abruptly, the class instance creation expression completes abruptly for the same reason.
该小节规定了在进行实例化操作时，必须对表达式进行空指针验证，如果表达式为null，则抛出NullPointerException，对应上述例子中，必须对outer对象进行验证。
规范中只是明确了“要抛出NullPointerException”的行为，至于如何实现null检查的则没规定，可以自由实现，比如用普通的if…else做验证，只不过生成的字节码就比调用getClass()的办法更长一些，getClass()方法在Object上声明的，所有对象上必然存在，由final修饰，保证了有确定的行为，而且运行开销比较低。
参考
[javac在编译创建内部类对象时生成的奇怪的getClass()调用是什么？](http://rednaxelafx.iteye.com/blog/1089554)
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
![](http://jbcdn2.b0.upaiyun.com/2016/08/c3fbbefbe3e302d0f6944c94db3cb1c4.jpg)
