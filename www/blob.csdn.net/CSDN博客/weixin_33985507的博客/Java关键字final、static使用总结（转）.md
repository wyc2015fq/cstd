# Java关键字final、static使用总结（转） - weixin_33985507的博客 - CSDN博客
2016年03月09日 14:57:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
### 原文链接：[Java关键字final, static一些要点](http://www.cnblogs.com/love-lie/archive/2012/03/31/2427298.html)
1. final 
final类不能被继承，没有子类，final类中的方法默认是final的
final方法不能被子类的方法复盖，但可以被继承
final成员变量表示常量，只能被赋值一次，赋值后不能再被改变
final不能用于修饰构造方法
private不能被子类方法覆盖，private类型的方法默认是final类型的
final修饰的变量有三种：静态变量、实例变量和局部变量，分别表示三种类型的常量。
注意：final变量定义的时候，可以先声明，而不给初值，这中变量也称为final空白，无论什么情况，编译器都确保空白final在使用之前必须被初始化。
2. static
static表示“全局”或者“静态”的意思，用来修饰成员变量和成员方法，也可以形成静态static代码块，但是Java语言中没有全局变量的概念。
被static修饰的成员变量和成员方法独立于该类的任何对象。也就是说，它不依赖类特定的实例，被类的所有实例共享。只要这个类被加载，Java虚拟机就能根据类名在运行时数据区的方法区内定找到他们。因此，static对象可以在它的任何对象创建之前访问，无需引用任何对象。
用public修饰的static成员变量和成员方法本质是全局变量和全局方法，当声明它类的对象市，不生成static变量的副本，而是类的所有实例共享同一个static变量。
- 类成员变量
  - 静态变量（类变量）: static修饰
  - 实例变量      : 无static修饰
- 局部变量
3. static和final一起使用
static final用来修饰成员变量和成员方法，可以理解为“全局变量”
对于变量，表示一旦给值就不可修改，并且通过类名可以访问。
对于方法，表示不可覆盖，并且可以通过类名直接访问。
注意：
对于被static和final修饰过的实例常量，实例本身不能再改变了，但对于一些容器类型（比如，ArrayList、HashMap）的实例变量，不可以改变容器变量本身，但可以修改容器中存放的对象。
### 其它参考链接：
### [**Java关键字final、static使用总结**](http://lavasoft.blog.51cto.com/62575/18771/)
### [static和final关键字](http://blog.163.com/neu_lxb/blog/static/179417010201132332429436/)
