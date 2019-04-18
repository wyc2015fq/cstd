# Effective C# 原则20：明辨接口实现和虚函数重载的区别(译) - weixin_33985507的博客 - CSDN博客
2007年03月10日 19:54:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
Effective C# 原则20：明辨接口实现和虚函数重载的区别
Item 20: Distinguish Between Implementing Interfaces and Overriding Virtual Functions 
粗略的看一下，感觉实现接口和虚函数重载是一样的。你定义了一些对象，但是这些对象是在另一个类型里申明的。你被第一感觉骗了，实现接口与虚函数重载是完全不同的。在接口里定义的成员默认情况下，是根本不存在实际内容的。
派生类不能重载基类中的接口成员。接口可以隐式的实现，就是把它们从类的公共接口中隐藏。它们的概念是不同的而且使用也是不同的。
但你可以这样的实现接口：让你的派生类可以修改你的实现。你只用对派生类做一个Hook就行了。(译注：相信写过C++程序的人就知道hook是什么意思，而且我也实在想不到把hook译成什么比较好，所以就直接用hook这个原词了，就像bug一样。)
为了展示它们的不同之处，试着做一个简单的接口以及在一个类中实现它：
interface IMsg
{
  void Message();
}
public class MyClass : IMsg
{
  public void Message()
  {
    Console.WriteLine( "MyClass" );
  }
}
Message()方法是MyClass的公共接口，Message同样可以用一个接口指针IMsg来访问。现在让我们来一点繁杂的，添加一个派生类：
public class MyDerivedClass : MyClass
{
  public new void Message()
  {
    Console.WriteLine( "MyDerivedClass" );
  }
}
注意到，我添加了一个关键字new在Message方法上，用于区别前面的一个Message(参见原则29)。MyClass.Message()不是虚函数，派生类可以不提供重载版本。MyDerived类创建了一个新的Message方法，但这个方法并不是重载MyClass.Message:它隐藏了原来的方法。而且，MyClass.Message还是可以通过IMsg的引用来访问：
MyDerivedClass d = new MyDerivedClass( );
d.Message( ); // prints "MyDerivedClass".
IMsg m = d as IMsg;
m.Message( ); // prints "MyClass"
接口方法不是虚的，当你实现一个接口时，你就要在详细的相关类型中申明具体的实现内容。
但你可能想要创建接口，在基类中实现这些接口而且在派生类中修改它们的行为。这是可以办法到的。你有两个选择，如果不访问基类，你可以在派生类中重新实现这个接口：
public class MyDerivedClass : MyClass, IMsg
{
  public new void Message()
  {
    Console.WriteLine( "MyDerivedClass" );
  }
}
添加的IMsg让你的派生类的行为发生了改变，以至IMsg.Message现在是在派生类上使用的：
MyDerivedClass d = new MyDerivedClass( );
d.Message( ); // prints "MyDerivedClass".
IMsg m = d as IMsg;
m.Message( ); // prints "MyDerivedClass"
派生类上还是须要在MyDerivedClass.Message()方法上添加关键字new，这还是有一点隐患(参见原则29)。基类还是可以通过接口引用来访问：
MyDerivedClass d = new MyDerivedClass( );
d.Message( ); // prints "MyDerivedClass".
IMsg m = d as IMsg;
m.Message( ); // prints "MyDerivedClass"
MyClass b = d;
b.Message( ); // prints "MyClass"
唯一可以修正这个问题的方法是修改基类，把接口的申明修改为虚函数：
public class MyClass : IMsg
{
  public virtual void Message()
  {
    Console.WriteLine( "MyClass" );
  }
}
public class MyDerivedClass : MyClass
{
  public override void Message()
  {
    Console.WriteLine( "MyDerivedClass" );
  }
}
MyDerivedClass以及其它所有从MyClass派生的类可以申明它们自己的Message()方法。这个重载的版本每次都会调用：通过MyDerivedClass的引用，通过IMsg接口的引用，或者直接通过MyClass的引用。
如果你不喜欢混杂的虚函数概念，那就对MyClass的定义做一个小的修改：
public abstract class MyClass, IMsg
{
  public abstract void Message();
}
是的，你可以用一个抽象方法来实现一个接口。通过申明一个接口内的抽象的方法，你可以让你的所有派生都必须实现这个接口。现在，IMsg接口成为了MyClass的一个组成部份，你的每一个派生类都必须实现它。
隐式接口实现，可以让你在一个类上隐藏公共的接口成员方法，而且也实现了这个接口。它在实现接口和虚函数重载上绕了几个圈。当有多个合适的函数版本时，你可以利用隐式接口的实现来限制用户的编码。在原则26讲到的IComparable习惯会详细的讨论这一点。
实现接口让我们有更多的选择，用于创建和重载虚函数。你可以创建隐秘的实现，虚的实现，或者抽象关联到派生类。你可以精确的决定，你的派生类如何以及何时，修改接口的默认实现。接口方法不是虚方法，而是一个独立的约定!
===============================
Item 20: Distinguish Between Implementing Interfaces and Overriding Virtual Functions
At first glance, implementing an interface seems to be the same as overriding avirtual function. You provide a definition for a member that has been declared in another type. That first glance is very deceiving. Implementing an interface is very different from overriding a virtual function. Members declared in interfaces are not virtualat least, not by default.
Derived classes cannot override an interface member implemented in a base class. Interfaces can be explicitly implemented, which hides them from a class's public interface. They are different concepts with different uses.
But you can implement interfaces in such a manner that derived classes can modify your implementation. You just have to create hooks for derived classes.
To illustrate the differences, examine a simple interface and implementation of it in one class:
interface IMsg
{
  void Message();
}
public class MyClass : IMsg
{
  public void Message()
  {
    Console.WriteLine( "MyClass" );
  }
}
The Message() method is part of MyClass's public interface. Message can also be accessed through the IMsg point that is part of the MyClass type. Now let's complicate the situation a little by adding a derived class:
public class MyDerivedClass : MyClass
{
  public new void Message()
  {
    Console.WriteLine( "MyDerivedClass" );
  }
}
Notice that I had to add the new keyword to the definition of the previous Message method (see Item 29). MyClass.Message() is not virtual. Derived classes cannot provide an overridden version of Message. The MyDerived class creates anew Message method, but that method does not override MyClass.Message: It hides it. Furthermore, MyClass. Message is still available through the IMsg reference:
MyDerivedClass d = new MyDerivedClass( );
d.Message( ); // prints "MyDerivedClass".
IMsg m = d as IMsg;
m.Message( ); // prints "MyClass"
Interface methods are not virtual. When you implement an interface, you are declaring a concrete implementation of a particular contract in that type.
But you often want to create interfaces, implement them in base classes, and modify the behavior in derived classes. You can. You've got two options. If you do not have access to the base class, you can reimplement the interface in the derived class:
public class MyDerivedClass : MyClass, IMsg
{
  public new void Message()
  {
    Console.WriteLine( "MyDerivedClass" );
  }
}
The addition of the IMsg keyword changes the behavior of your derived class so that IMsg.Message() now uses the derived class version:
MyDerivedClass d = new MyDerivedClass( );
d.Message( ); // prints "MyDerivedClass".
IMsg m = d as IMsg;
m.Message( ); // prints "MyDerivedClass"
You still need the new keyword on the MyDerivedClass.Message() method. That's your clue that there are still problems (see Item 29). The base class version is still accessible through a reference to the base class:
MyDerivedClass d = new MyDerivedClass( );
d.Message( ); // prints "MyDerivedClass".
IMsg m = d as IMsg;
m.Message( ); // prints "MyDerivedClass"
MyClass b = d;
b.Message( ); // prints "MyClass"
The only way to fix this problem is to modify the base class, declaring that the interface methods should be virtual:
public class MyClass : IMsg
{
  public virtual void Message()
  {
    Console.WriteLine( "MyClass" );
  }
}
public class MyDerivedClass : MyClass
{
  public override void Message()
  {
    Console.WriteLine( "MyDerivedClass" );
  }
}
MyDerivedClassand all classes derived from MyClasscan declare their own methods for Message(). The overridden version will be called every time: through the MyDerivedClass reference, through the IMsg reference, and through the MyClass reference.
If you dislike the concept of impure virtual functions, just make one small change to the definition of MyClass:
public abstract class MyClass, IMsg
{
  public abstract void Message();
}
Yes, you can implement an interface without actually implementing the methods in that interface. By declaring abstract versions of the methods in the interface, you declare that all types derived from your type must implement that interface. The IMsg interface is part of the declaration of MyClass, but defining the methods is deferred to each derived class.
Explicit interface implementation enables you to implement an interface, yet hide its members from the public interface of your type. Its use throws a few other twists into the relationships between implementing interfaces and overriding virtual functions. You use explicit interface implementation to limit client code from using the interface methods when a more appropriate version is available. The IComparable idiom in Item 26 shows this in detail.
Implementing interfaces allows more options than creating and overriding virtual functions. You can create sealed implementations, virtual implementations, or abstract contracts for class hierarchies. You can decide exactly how and when derived classes can modify the default behavior for members of any interface your class implements. Interface methods are not virtual methods, but a separate contract.
