# autofac如何注册静态方法里的接口对象 - 左直拳的马桶_日用桶 - CSDN博客
2016年03月14日 19:44:31[左直拳](https://me.csdn.net/leftfist)阅读数：3221
*标题可能是不准确的，因为我不知道如何描述。不知道的原因，是对依赖注入一知半解。*
Autofac可以自动注册对象实例到接口，人所尽知。而在asp.net mvc中，这个实例化的工作，通常在每个控制器的构造函数里完成。
假如，我有些静态方法里，非要用到这些接口呢？怎么实例化它们？例如：
```
public static bool SetData()
{
    //这里需要用到实例化的某个对象，例如ILog
}
```
你当然可以将这个对象作为参数传递进来，如
`public static bool SetData(Ilog log)`
问题是，有时候并不方便将这个实例化对象传进来，原因可能是这个静态方法很多地方都在使用，要传这个实例对象，需要改好多地方；并且每个调用方，都要在自己的构造函数里加入这个接口作为参数（不然咋注册？）
最好的办法就是在这个静态方法里注册或获得实例对象
```
public static bool SetData()
{
    //在这里获得实例化对象，如
    ILog log = ....
}
```
但如何获取这个实例对象呢？new一个的话，autofac的优势就荡然无存了。之所以使用autofac，使用IOC容器，就是为了面向接口编程。如果从设计阶段就写死对象的实例化类型，就不叫面向接口编程了。
那么，如何实例化？
可以酱紫：
```
public static bool SetData()
{
    //DependencyResolver是System.MVC里的对象
    //下面这个表达式大意是，获得现行注册的对象类型，来实例化接口对象
    //换言之，这是autofac的手动实例化方式
    ILog log = DependencyResolver.Current.GetService<ILog>();
}
```
前提条件，是该实例化对象类型要在项目的初始化阶段注册，比如在ASP.NET MVC项目里的app_start事件里注册：
`builder.RegisterType<MyLog>().As<ILog>().InstancePerLifetimeScope();`
参考文章： 
[Autofac怎么依赖注入ASP.NET MVC5类的静态方法] 
([http://www.lanhusoft.com/Article/366.html](http://www.lanhusoft.com/Article/366.html))
