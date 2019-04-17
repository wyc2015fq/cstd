# 设计模式 —— 装饰模式（Decorator） - Alex_McAvoy的博客 - CSDN博客





2019年03月24日 21:42:56[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：13
个人分类：[设计模式](https://blog.csdn.net/u011815404/article/category/8856120)

所属专栏：[设计模式学习笔记](https://blog.csdn.net/column/details/35125.html)









# 【概述】

装饰模式是**动态地以对客户端透明的方式扩展对象的功能**，单就增加功能来说，其比生成子类更加灵活，可以当作继承关系的替代方案，其本质是**动态组合**，动态是手段，组合是目的。

其与策略模式十分相似，但两者有所不同，而且可以有机组合：
- 策略模式改变的是对象的内核，装饰模式改变的是对象的外壳
- 策略模式是一层调用，装饰模式是递归调用

# 【UML】

装饰模式涉及到了四个角色：
- 抽象构件角色：Component，其给出一个抽象接口，以规范**准备接收附加责任的对象**。
- 具体构件角色：Concrete Component，其实现了组件对象接口，通常就是**被装饰器装饰的对象**。 
- 装饰角色：Decorator，其持有一个抽象构件对象的实例，并定义一个与抽象构件接口一致的接口。
- 具体装饰角色：Concrete Decorator，其负责给构件对象 “加上” 附加的责任。

简单来说，抽象构件角色定义了一个对象接口，可以给这些对象动态地添加职责；具体构件角色定义了一个具体的对象，其同样可以对这个对象添加一些职责；装饰角色继承了抽象构件角色，从外类来扩展抽象构件角色的功能，但对于抽象构件角色本身来说，其是不需知道装饰角色的；具体装饰角色是具体的装饰对象，起到给抽象构件角色添加职责的功能。

![](https://img-blog.csdnimg.cn/20190324212004568.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

# 【基本思想】

装饰模式中，**所有组件都是抽象构建角色 Component 的子类**，由 Component 规划对象的行为，装饰类 Decorator 与被装饰类 Concrete Component 都是 Component 的子类，那么他们具有相同的行为。

而装饰类 Decorator 以 Component 为成员变量，那他就有机会操作 Component 的所有子类，而 Decorator 的子类都重写了 Decorator 定义的接口（确切的说是 Component 定义的接口），这样各装饰类有了机会重写自己关心的装饰工作。 

其效果可以让我们创建**以装饰对象开始的一条对象链（负责新的功能的对象），并结束于最初的对象**，与递归十分相似。

通过装饰类  Decorator 构建对象链，从而能够实现动态地为对象添加功能，即对于继承同一个接口的两个对象，通过对一个对象的外部来给另一个对象来增加功能

![](https://img-blog.csdnimg.cn/20190324213143979.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

#  【优缺点】

优点：
- 装饰模式与继承关系的目的都是要扩展对象的功能，但是装饰模式可以提供比继承灵活性更高。 
- 通过使用不同的具体装饰类以及这些装饰类的排列组合，可以创造出很多不同行为的组合。 

缺点：
- 会产生很多细粒度对象

# 【经典案例】

利用装饰模式最经典的案例就是 Java 中的 I/O 流，当想要添加新的功能，只需要实现新的装饰器，然后再使用时，组合进去就可以。

```java
public class ioTest {
public static void main(String[] args) {
    //装饰模式递归调用
    DataOutputStream dout=new DataOutputStream(
        new BufferedOutputStream(
            new EncryptOutputStream(
                new FileOutputStream("IOTest.txt"))));
        
        dout.write("abcdxyz".getBytes());
		dout.close();
	}
}
```





