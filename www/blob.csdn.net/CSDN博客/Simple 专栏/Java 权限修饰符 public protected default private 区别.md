# Java 权限修饰符 public /protected /default /private 区别 - Simple 专栏 - CSDN博客
2012年01月08日 14:15:54[Simple_Zz](https://me.csdn.net/love284969214)阅读数：236标签：[java																[java基础](https://so.csdn.net/so/search/s.do?q=java基础&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Java](https://blog.csdn.net/love284969214/article/category/1235868)
所属专栏：[Java 浅谈](https://blog.csdn.net/column/details/27397.html)
### public
具有最大的访问权限，可以访问任何一个在classpath下的类、接口、异常等。它往往用于对外的情况，也就是对象或类对外的一种接口的形式。
### protected
主要的作用就是用来保护子类的。它的含义在于子类可以用它修饰的成员，其他的不可以，它相当于传递给子类的一种继承的东西。
### default
有时候也称为friendly，它是针对本包访问而设计的，任何处于本包下的类、接口、异常等，都可以相互访问，即使是父类没有用protected修饰的成员也可以。
### private
访问权限仅限于类的内部，是一种封装的体现，例如，大多数成员变量都是修饰符为private的，它们不希望被其他任何外部的类访问。
||本类|本包|子类|外部包|
|----|----|----|----|----|
|public|可以|可以|可以|可以|
|protected|可以|可以|可以|不可以|
|default|可以|可以|不可以|不可以|
|private|可以|不可以|不可以|不可以|
注意：java的访问控制是停留在编译层的，也就是它不会在.class文件中留下任何的痕迹，只在编译的时候进行访问控制的检查。其实，通过反射的手段，是可以访问任何包下任何类中的成员，例如，访问类的私有成员也是可能的。
### 区别
public：可以被所有其他类所访问
private：只能被自己访问和修改
protected：自身、子类及同一个包中类可以访问
default：同一包中的类可以访问，声明时没有加修饰符，认为是friendly。
