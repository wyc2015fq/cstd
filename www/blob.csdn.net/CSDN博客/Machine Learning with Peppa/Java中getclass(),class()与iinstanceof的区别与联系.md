# Java中getclass(),class()与iinstanceof的区别与联系 - Machine Learning with Peppa - CSDN博客





2018年04月30日 23:15:56[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：108标签：[Java																[面向对象																[类型](https://so.csdn.net/so/search/s.do?q=类型&t=blog)
个人分类：[编程之美：Java](https://blog.csdn.net/qq_39521554/article/category/7599598)

所属专栏：[JavaSE](https://blog.csdn.net/column/details/21699.html)](https://so.csdn.net/so/search/s.do?q=面向对象&t=blog)






getclass()学过Python的同学肯定很熟悉，是不是有点像__dict__ method?其实这个自省，JAVA中叫做反射，可以理解为在运行时期获取对象类型信息的操作，两者几乎相同。传统的编程方法要求程序员在编译阶段决定使用的类型，但是在反射的帮助下，编程人员可以动态获取这些信息，从而编写更加具有可移植性的代码。严格地说，反射并非编程语言的特性，因为在任何一种语言都可以实现反射机制，但是如果编程语言本身支持反射，那么反射的实现就会方便很多。
我们知道在Java中一切都是对象，我们一般所使用的对象都直接或间接继承自Object类。Object类中包含一个方法名叫getClass，利用这个方法就可以获得一个实例的类型类。类型类指的是代表一个类型的类，因为一切皆是对象，类型也不例外，在Java使用类型类来表示一个类型。所有的类型类都是Class类的实例。例如，有如下一段代码：


```java
A a = new A();

if(a.getClass()==A.class)
    System.out.println("equal");

else System.out.println("unequal");
```
结果就是打印出 “equal”。可以看到，对象a是A的一个实例，A某一个类，在if语句中使用a.getClass()返回的结果正是A的类型类，在Java中表示一个特定类型的类型类可以用“类型.class”的方式获得，因为a.getClass()获得是A的类型类，也就是A.class，因此上面的代码执行的结果就是打印出 “equal”。特别注意的是，类型类是一一对应的，父类的类型类和子类的类型类是不同的，因此，假设A是B的子类，那么如下的代码将得到 “unequal”的输出：

```java
A a = new A();

if(a.getClass()==B.class)
    System.out.println("equal");

else System.out.println("unequal");
```

因此，如果你知道一个实例，那么你可以通过实例的“getClass()”方法获得该对象的类型类，如果你知道一个类型，那么你可以使用“.class”的方法获得该类型的类型类。





### 联系 
一般情况下，getclass（）方法和class（）方法是等价的，都可以获得一个类型名，例如下面的代码：

在比较一个类是否和另一个类属于同一个类实例的时候，我们通常可以采用instanceof和getClass两种方法通过两者是否相等来判断，但是两者在判断上面是有差别的，下面从代码中看看区别：

```java
public class Test  
{  
    public static void testInstanceof(Object x)  
    {  
        System.out.println("x instanceof Parent:  "+(x instanceof Parent));  
        System.out.println("x instanceof Child:  "+(x instanceof Child));  
        System.out.println("x getClass Parent:  "+(x.getClass() == Parent.class));  
        System.out.println("x getClass Child:  "+(x.getClass() == Child.class));  
    }  
    public static void main(String[] args) {  
        testInstanceof(new Parent());  
        System.out.println("---------------------------");  
        testInstanceof(new Child());  
    }  
}  
class Parent {  
  
}  
class Child extends Parent {  
  
}  
/* 
输出: 
x instanceof Parent:  true 
x instanceof Child:  false 
x getClass Parent:  true 
x getClass Child:  false 
--------------------------- 
x instanceof Parent:  true 
x instanceof Child:  true 
x getClass Parent:  false 
x getClass Child:  true 
*/
```

从程序输出可以看出,instanceof进行类型检查规则是:你属于该类吗？或者你属于该类的派生类吗？而通过getClass获得类型信息采用==来进行检查是否相等的操作是严格的判断。不会存在继承方面的考虑；








### 区别
两者最直接的区别就是，getClass（）是一个类的实例所具备的方法，而class（）方法是一个类的方法。 
另外getClass（）是在运行时才确定的，而class（）方法是在编译时就确定了。
例如下面的程序：

```java
class A{
    public void func(){
    }
}

class B extends A{

}
public class Test {

    public static void main(String[] args) {
        A a = new A();
        B b = new B();
        A ab = new B();
        System.out.println(a.getClass()+" "+A.class);
        System.out.println(b.getClass()+" "+B.class);
        System.out.println(ab.getClass());
        ab = a;
        System.out.println(ab.getClass());
    }
}
```

 输出结果为：

```java
class A class A 
class B class B 
class B 
class A
```](https://so.csdn.net/so/search/s.do?q=Java&t=blog)




