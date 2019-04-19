# C# 单例模式的五种写法 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [寻找"四叶草"](http://www.jobbole.com/members/xunzhaosiyechao) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
保证一个类仅有一个实例，并提供一个该实例的全局访问点。
——《设计模式》
单例模式的概念很简单，下面以C#语言为例子，列出常见单例写法的优缺点。
## 1、简单实现

C#
```
public sealed class Singleton
    {
        static Singleton instance = null;
        public void Show()
        {
            Console.WriteLine(  "instance function");
        }
        private Singleton()
        {
        }
        public static Singleton Instance
        {
            get
            {
                if (instance == null)
                {
                    instance = new Singleton();
                }
                return instance;
            }
        }
    }
```
评注：
对于线程来说不安全
单线程中已满足要求
优点:
由于实例是在 Instance 属性方法内部创建的，因此类可以使用附加功能
直到对象要求产生一个实例才执行实例化；这种方法称为“**惰性实例化**”。惰性实例化避免了在应用程序启动时实例化不必要的 singleton。
## 2、线程的安全

C#
```
public sealed class Singleton
    {
        static Singleton instance = null;
        private static readonly object padlock = new object();
        private Singleton()
        {
        }
        public static Singleton Instance
        {
            get
            {
                lock (padlock)
                {
                    if (instance == null)
                    {
                        instance = new Singleton();
                    }
                }
                return instance;
            }
        }
    }
```
评注：
同一个时刻加了锁的那部分程序只有一个线程可以进入
对象实例由最先进入的那个线程创建
后来的线程在进入时（instence == null）为假，不会再去创建对象实例
增加了额外的开销，损失了性能
## 3、双重锁定

C#
```
public sealed class Singleton
    {
        static Singleton instance = null;
        private static readonly object padlock = new object();
        private Singleton()
        {
        }
        public static Singleton Instance
        {
            get
            {
                if (instance == null)
                {
                    lock (padlock)
                    {
                        if (instance == null)
                        {
                            instance = new Singleton();
                        }
                    }
                }
                return instance;
            }
        }
    }
```
评注：
多线程安全
线程不是每次都加锁
允许实例化延迟到第一次访问对象时发生
## 4、静态初始化

C#
```
public sealed class Singleton
    {
        private static readonly Singleton instance = null;
        static Singleton()
        {
            instance = new Singleton();
        }
        private Singleton()
        {
        }
        public static Singleton Instance
        {
            get
            {
                return instance;
            }
        }
    }
```
评注：
依赖公共语言运行库负责处理变量初始化
公共静态属性为访问实例提供了一个全局访问点
对实例化机制的控制权较少(.NET代为实现)
静态初始化是在 .NET 中实现 Singleton 的首选方法
小注：
静态构造函数既没有访问修饰符，C#会自动把他们标记为private，之所以必须标记为private，是为了阻止开发人员写的代码调用它，对它的调用总是由CLR负责的。
![](http://jbcdn2.b0.upaiyun.com/2016/05/87033e3aa83cfa356c82f181b5b173f4.png)
![](http://jbcdn2.b0.upaiyun.com/2016/05/a9ea07e78af88dfe983be174151a54e5.png)
![](http://jbcdn2.b0.upaiyun.com/2016/05/dff2281b62a90eca4b3feef9e9e0bfea.png)
## 5、延迟初始化

C#
```
public sealed class Singleton
    {
        private Singleton()
        {
        }
        public static Singleton Instance
        {
            get
            {
                return Nested.instance;
            }
        }
        public static void Hello()
        {
        }
        private class Nested
        {
            internal static readonly Singleton instance = null;
            static Nested()
            {
                instance = new Singleton();
            }
        }
    }
```
评注：
初始化工作由Nested类的一个静态成员来完成，这样就实现了延迟初始化。
由于静态函数的调用时机，是在类被实例化或者静态成员被调用的时候进行调用，并且是由.net框架来调用静态构造函数来初始化静态成员变量，所以，如果按照写法四来写，再调用Hello方法的时候，就会实例化出来Singleton实例，这不是我们想看到的，因为我们有可能只是想用Hello方法，而不是别的。
注意事项：
1、Singleton模式中的实例构造器可以设置为protected以允许子类派生。
2、Singleton模式一般不要支持ICloneable接口，因为这可能会导致多个对象实例，与Singleton模式的初衷违背。
3、Singleton模式一般不要支持序列化，因为这也有可能导致多个对象实例，同样与Singleton模式的初衷违背。
4、Singletom模式只考虑到了对象创建的管理，没有考虑对象销毁的管理。就支持垃圾回收的平台和对象的开销来讲，我们一般没有必要对其销毁进行特殊的管理。
总结：
1、Singleton模式是限制而不是改进类的创建。
2、理解和扩展Singleton模式的核心是“如何控制用户使用new对一个类的构造器的任意调用”。
3、可以很简单的修改一个Singleton，使它有少数几个实例，这样做是允许的而且是有意义的。
