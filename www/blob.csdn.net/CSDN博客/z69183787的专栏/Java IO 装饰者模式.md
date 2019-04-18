# Java IO 装饰者模式 - z69183787的专栏 - CSDN博客
2015年01月20日 11:04:52[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1059
## 装饰模式（Decorator）
　　装饰模式又名**包装（Wrapper）模式**。
　　装饰模式以对客户端透明的方式扩展**对象**的功能，是继承关系的一个替代方案。
　　装饰模式通过创建一个包装对象，也就是装饰，来包裹真实的对象。
　　装饰模式以对客户端透明的方式**动态**地给一个对象附加上更多的责任。换言之，客户端并不会觉得对象在装饰前和装饰后有什么不同。
　　装饰模式可以在不创造更多子类的情况下，将对象的功能加以扩展。
　　装饰模式把客户端的调用委派到被装饰类。装饰模式的关键在于这种扩展是完全透明的。
## 装饰模式的角色
**抽象构件角色（Component）：**给出一个抽象接口，以规范准备接收附加责任的对象。
**具体构件角色（Concrete Component）：**定义将要接收附加责任的类。
**装饰角色（Decorator）：**持有一个构件（Component）对象的引用，并定义一个与抽象构件接口一致的接口。
**具体装饰角色（Concrete Decorator）：**负责给构件对象“贴上”附加的责任。
## Java IO中的装饰模式
　　在IO中，具体构件角色是**节点流**，装饰角色是**过滤流**。
　　FilterInputStream和FilterOutputStream是装饰角色，而其他派生自它们的类则是具体装饰角色。
## 装饰模式的特点
　　装饰对象和真实对象有相同的接口。这样客户端对象就可以以和真实对象相同的方式和装饰对象交互。
　　装饰对象包含一个真实对象的引用（reference）。
　　装饰对象接收所有来自客户端的请求，它把这些请求转发给真实的对象。
　　装饰对象可以在转发这些请求之前或之后附加一些功能。
　　这样就确保了在运行时，不用修改给定对象的结构就可以在外部增加附加的功能。
## 程序实例
```
public interface Component
{
    public void doSomething();
}
```
　　这是抽象构件角色，是一个接口。具体构件角色实现这个接口：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
public class ConcreteComponent implements Component
{
    @Override
    public void doSomething()
    {
        System.out.println("功能A");
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　装饰角色：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
public class Decorator implements Component
{
    private Component component;
    public Decorator(Component component)
    {
        this.component = component;
    }
    @Override
    public void doSomething()
    {
        component.doSomething();
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　其中包含了构件角色的引用，方法调用中利用构件角色的方法。
　　具体装饰角色（两个）：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
public class ConcreteDecorator1 extends Decorator
{
    public ConcreteDecorator1(Component component)
    {
        super(component);
    }
    
    @Override
    public void doSomething()
    {
        super.doSomething();
        
        this.doAnotherThing();
    }
    
    private void doAnotherThing()
    {
        System.out.println("功能B");
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
public class ConcreteDecorator2 extends Decorator
{
    public ConcreteDecorator2(Component component)
    {
        super(component);
    }
    @Override
    public void doSomething()
    {
        super.doSomething();
        
        this.doAnotherThing();
    }
    
    private void doAnotherThing()
    {
        System.out.println("功能C");
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　使用测试：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
public class Client
{
    public static void main(String[] args)
    {
        Component component = new ConcreteComponent();
        
        Component component1 = new ConcreteDecorator1(component);
        
        component1.doSomething();
        System.out.println("-----------");
        
        Component component2 = new ConcreteDecorator2(component1);
        
        component2.doSomething();
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
输出：
功能A
功能B
-----------
功能A
功能B
功能C
