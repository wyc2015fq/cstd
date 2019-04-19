# Autofac - 左直拳的马桶_日用桶 - CSDN博客
2015年10月19日 20:00:31[左直拳](https://me.csdn.net/leftfist)阅读数：3031
Autofac、Autofac、Autofac！好的东西要说三遍。
这东西干嘛用？
用于自动注入。
啥注入？注水猪肉么？
这要扯到控制反转（Inversion of Control，IoC），依赖注入（DI）等。其实控制反转、依赖注入都是面向接口编程的同一类概念。
何谓控制反转？
以前我们要使用一个对象，是这样的：
`Class1 obj = new Class1();`
面向接口编程后，是这样的：
```
//class Class1 : IClass1
IClass1 obj = new Class1();
```
应用控制反转、依赖注入，是类似这样的：
```
public class Test
{
    IClass1 obj;//类内只有接口
    public Test(IClass obj)
    {
        this.obj = obj;//你会给我一个啥实例？给点提示好不好？呵呵，给啥用啥，没得挑！
    }
    ...
}
//对象实例要靠类外面创建，再传递进来（注入）
Test test = new Test(new Class1());
```
这种使用对象实例方式，不经过类自身构造，无法控制，而只能听由外部或调用方安排，即所谓控制反转了。反了反了的意思。
那么Autofac呢？
呵呵，用了Autofac之后，连构造实例的语句：
`Test test = new Test(new Class1());`
都没有了。看代码的时候，只看见一堆接口，唯独没有实例声明，很容易让人大感疑惑！
其实，实例的声明，就由Autofac自动完成了。Autofac、Autofac,我猜想就是 Auto factory的缩写吧，自动产生工厂实例，呵呵。
**Autofac原理**
寻找到相关实例，将其赋给接口对象。比如说，我们在调用方使用了一堆接口，然后引用了一个DLL，这个DLL包含了实现这些接口的实例，那么Autofac就自动从这个DLL中提取出这些实例，赋给接口对象。这一切都是Autofac自动完成的。
Asp.net MVC 中的应用
```
//global.asax.cs
 protected void Application_Start()
        {
            ……
            //激发autofac
            AutofacConfig.BuildContainer();
        }
……
//App_Start/AutofacConfig
    public class AutofacConfig
    {
        public static void BuildContainer()
        {
            var builder = new ContainerBuilder();
            //Infrastructure objects
builder.RegisterControllers(typeof(MvcApplication).Assembly);
           builder.RegisterAssemblyTypes(typeof(MvcApplication).Assembly).AsImplementedInterfaces(); 
            //自动用Services里的类来注册相应实例，无须一个个注册
builder.RegisterAssemblyTypes(typeof(ISysManagerService).Assembly)
                .Where(t => t.IsClass && t.Name.EndsWith("Service"))
                .As(t => t.GetInterfaces().Single(i => i.Name.EndsWith(t.Name)))
                .InstancePerRequest();
……
            IContainer container = builder.Build();
            DependencyResolver.SetResolver(new AutofacDependencyResolver(container));
        }
```
适合场景 
比如说，甲乙两个人一起开发项目，甲负责前端及逻辑，乙负责数据库。开发过程中，甲需要一些数据进行测试，按道理，他是调用乙的方法来获取，问题是，乙还没弄好，这个时候该怎么办？难道甲只好一直等到乙弄好？
这个时候，面向接口编程的好处就体现出来了。甲制定了一些接口，前端使用了这些接口，然后自己又写了一些类实现这些接口，实现很简单，只提供一些测试数据。结果甲开发得很顺利。
后来，乙终于也将方法写好了，当然也是实现同样接口的。那么怎么应用这些真正的类呢？
这时Autofac的优势也体现出来了，只需替换相关的DLL就可以了，剩下的，Autofac代劳。否则的话，还要自己将每个对接口的赋值修改一下。数量不多还好，数量多，分布又多的话，还是要花一点时间。
