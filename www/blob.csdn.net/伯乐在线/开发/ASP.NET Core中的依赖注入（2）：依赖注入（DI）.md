# ASP.NET Core中的依赖注入（2）：依赖注入（DI） - 文章 - 伯乐在线
原文出处： [蒋金楠](http://www.cnblogs.com/artech/p/asp-net-core-di-di.html)
IoC主要体现了这样一种设计思想：通过将一组通用流程的控制从应用转移到框架之中以实现对流程的复用，同时采用“好莱坞原则”是应用程序以被动的方式实现对流程的定制。我们可以采用若干设计模式以不同的方式实现IoC，比如我们在[上面](http://blog.jobbole.com/100389/)介绍的模板方法、工厂方法和抽象工厂，接下来我们介绍一种更为有价值的IoC模式，即依赖注入（DI：Dependency Injection，以下简称DI）。
> 
目录
一、由外部容器提供服务对象
二、三种依赖注入方式
构造器注入
属性注入
方法注入
三、实例演示：创建一个简易版的DI框架
# 一、由外部容器提供服务对象
和上面介绍的工厂方法和抽象工厂模式一样，DI旨在实现针对服务对象的动态提供。具体来说，服务的消费者利用一个独立的容器（Container）来获取所需的服务对象，容器自身在提供服务对象的过程中会自动完成依赖的解析与注入。话句话说，由DI容器提供的这个服务对象是一个” 开箱即用”的对象，这个对象自身直接或者间接依赖的对象已经在初始化的工程中被自动注入其中了。
举个简单的例子，我们创建一个名为Cat的DI容器类，那么我们可以通过调用具有如下定义的扩展方法GetService从某个Cat对象获取指定类型的服务对象。我之所以将其命名为Cat，源于我们大家都非常熟悉的一个卡通形象“机器猫（哆啦A梦）”。它的那个四次元口袋就是一个理想的DI容器，大熊只需要告诉哆啦A梦相应的需求，它就能从这个口袋中得到相应的法宝。DI容器亦是如此，服务消费者只需要告诉容器所需服务的类型（一般是一个服务接口或者抽象服务类），就能得到与之匹配的服务对象。


```
public class Foo
 {
     public IBar Bar{get; private set;}
     public Foo(IBar bar)
     {
         this.Bar = bar;
     }
 }
```

对于我们在[上一篇](http://blog.jobbole.com/100389/)演示的MVC框架，我们在前面分别采用不同的设计模式对框架的核心类型MvcEngine进行了改造，现在我们采用DI的方式并利用上述的这个Cat容器按照如下的方式对其进行重新实现，我们会发现MvcEngine变得异常简洁而清晰。


```
public class MvcEngine
 {
     public Cat Cat { get; private set; }
  
     public MvcEngine(Cat cat)
     {
         this.Cat = cat;
     }
  
     public void Start(Uri address)
     {
         while (true)
         {
             Request request = this.Cat.GetService<Listener>().Listen(address);
             Task.Run(() =>
             {
                 Controller controller = this.Cat.GetService<ControllerActivator>().ActivateController(request);
                 View view = this.Cat.GetService<ControllerExecutor>().ExecuteController(controller);
                 this.Cat.GetService<ViewRenderer>().RenderView(view);
             });
         }
     }
 }
```

DI体现了一种最为直接的服务消费方式，消费者只需要告诉生产者（DI容器）关于所需服务的抽象描述，后者根据预先注册的规则提供一个匹配的服务对象。这里所谓的服务描述主要体现为服务接口或者抽象服务类的类型，当然也可以是包含实现代码的具体类型。至于应用程序对由框架控制的流程的定制，则可以通过对DI容器的定制来完成。如果具体的应用程序需要采用上面定义的SingletonControllerActivator以单例的模式来激活目标Controller，那么它可以在启动MvcEngine之前按照如下的形式将SingletonControllerActivator注册到后者使用的DI容器上。


```
public class App
 {
     static void Main(string[] args)
     {
         Cat cat = new Cat().Register<ControllerActivator, SingletonControllerActivator>();
         MvcEngine engine     = new MvcEngine(cat);
         Uri address          = new Uri("http://localhost/mvcapp");
         Engine.Start(address);
     }
 }
```

# 二、三种依赖注入方式
一项确定的任务往往需要多个对象相互协作共同完成，或者某个对象在完成某项任务的时候需要直接或者间接地依赖其他的对象来完成某些必要的步骤，所以运行时对象之间的依赖关系是由目标任务来决定的，是“恒定不变的”，自然也无所谓“解耦”的说法。但是运行时的对象通过设计时的类来定义，类与类之间耦合则可以通过依赖进行抽象的方式来解除。
从服务使用的角度来讲，我们借助于一个服务接口对消费的服务进行抽象，那么服务消费程序针对具体服务类型的依赖可以转移到对服务接口的依赖上。但是在运行时提供给消费者总是一个针对某个具体服务类型的对象。不仅如此，要完成定义在服务接口的操作，这个对象可能需要其他相关对象的参与，换句话说提供的这个服务对象可能具有针对其他对象的依赖。作为服务对象提供者的DI容器，在它向消费者提供服务对象之前会自动将这些依赖的对象注入到该对象之中，这就是DI命名的由来。
![3-8](http://jbcdn2.b0.upaiyun.com/2016/05/781e48dfa2565272ee9ae1429ee3bd93.png)如右图所示，服务消费程序调用GetService()方法向DI容器索取一个实现了IFoo接口的某个类型的对象，DI容器会根据预先注册的类型匹配关系创建一个类型为Foo的对象。此外，Foo对象依赖Bar和Baz对象的参与才能实现定义在服务接口IFoo之中的操作，所以Foo具有了针对Bar和Baz的直接依赖。至于Baz，它又依赖Qux，那么后者成为了Foo的间接依赖。对于DI容器最终提供的Foo对象，它所直接或者间接依赖的对象Bar、Baz和Qux都会预先被初始化并自动注入到该对象之中。
从编程的角度来讲，类型中的字段或者属性是依赖的一种主要体现形式，如果类型A中具有一个B类型的字段或者属性，那么A就对B产生了依赖。所谓依赖注入，我们可以简单地理解为一种针对依赖字段或者属性的自动化初始化方式。具体来说，我们可以通过三种主要的方式达到这个目的，这就是接下来着重介绍的三种依赖注入方式。
## 构造器注入
构造器注入就在在构造函数中借助参数将依赖的对象注入到创建的对象之中。如下面的代码片段所示，Foo针对Bar的依赖体现在只读属性Bar上，针对该属性的初始化实现在构造函数中，具体的属性值由构造函数的传入的参数提供。当DI容器通过调用构造函数创建一个Foo对象之前，需要根据当前注册的类型匹配关系以及其他相关的注入信息创建并初始化参数对象。


```
public class Foo
 {
     public IBar Bar{get; private set;}
     public Foo(IBar bar)
     {
         this.Bar = bar;
     }
 }
```

除此之外，构造器注入还体现在对构造函数的选择上面。如下面的代码片段所示，Foo类上面定义了两个构造函数，DI容器在创建Foo对象之前首选需要选择一个适合的构造函数。至于目标构造函数如何选择，不同的DI容器可能有不同的策略，比如可以选择参数做多或者最少的，或者可以按照如下所示的方式在目标构造函数上标注一个相关的特性（我们在第一个构造函数上标注了一个InjectionAttribute特性）。


```
public class Foo
 {
     public IBar Bar{get; private set;}
     public IBaz Baz {get; private set;}
  
     [Injection]
     public Foo(IBar bar)
     {
         this.Bar = bar;
     }
  
     public Foo(IBar bar, IBaz):this(bar)
     {
         this.Baz = baz;
     }
 }
```

## 属性注入
如果依赖直接体现为类的某个属性，并且该属性不是只读的，我们可以让DI容器在对象创建之后自动对其进行赋值进而达到依赖自动注入的目的。一般来说，我们在定义这种类型的时候，需要显式将这样的属性标识为需要自动注入的依赖属性，以区别于该类型的其他普通的属性。如下面的代码片段所示，Foo类中定义了两个可读写的公共属性Bar和Baz，我们通过标注InjectionAttribute特性的方式将属性Baz设置为自动注入的依赖属性。对于由DI容器提供的Foo对象，它的Baz属性将会自动被初始化。


```
public class Foo
 {
     public IBar Bar{get; set;}
  
     [Injection]
     public IBaz Baz {get; set;}
 }
```

## 方法注入
体现依赖关系的字段或者属性可以通过方法的形式初始化。如下面的代码片段所示，Foo针对Bar的依赖体现在只读属性上，针对该属性的初始化实现在Initialize方法中，具体的属性值由构造函数的传入的参数提供。我们同样通过标注特性（InjectionAttribute）的方式将该方法标识为注入方法。DI容器在调用构造函数创建一个Foo对象之后，它会自动调用这个Initialize方法对只读属性Bar进行赋值。在调用该方法之前，DI容器会根据预先注册的类型映射和其他相关的注入信息初始化该方法的参数。


```
public interface IFoo {}
 public interface IBar {}
 public interface IBaz {}
 public interface IQux {}
  
 public class Foo : IFoo
 {
     public IBar Bar { get; private set; }
  
     [Injection]
     public IBaz Baz { get; set; }
  
     public Foo() {}
  
     [Injection]
     public Foo(IBar bar)
     {
         this.Bar = bar;
     }
 }
  
  22: public class Bar : IBar {}
  23:  
  24: public class Baz : IBaz
  25: {
  26:     public IQux Qux { get; private set; }
  27:  
  28:     [Injection]
  29:     public void Initialize(IQux qux)
  30:     {
  31:         this.Qux = qux;
  32:     }
  33: }
  34:  
  35: public class Qux : IQux {}
```

我们在一个控制台应用中按照如上的形式定义了四个服务类型（Foo、Bar、Baz和Qux），它们分别实现了各自的服务接口（IFoo、IBar、IBaz和IQux）。定义在Foo中的属性Bar和Baz，以及定义在Baz中的属性Qux是三个需要自动注入的依赖属性，我们采用的注入方式分别是构造器注入、属性注入和方法注入。
我们在作为应用入口的Main方法中编写了如下一段程序。如下面的代码片段所示，在创建了作为DI容器的Cat对象之后，我们调用它的Register()方法注册了服务类型和对应接口之间的匹配关系。然后我们调用Cat对象的GetService()方法通过指定的服务接口类型IFoo得到对应的服务对象，为了确保相应的依赖属性均按照我们希望的方式被成功注入，我们将它们显式在控制台上。


```
class Program
 {
     static void Main(string[] args)
     {
         Cat cat = new Cat();
         cat.Register<IFoo, Foo>();
         cat.Register<IBar, Bar>();
         cat.Register<IBaz, Baz>();
         cat.Register<IQux, Qux>();
  
         IFoo service = cat.GetService<IFoo>();
         Foo foo = (Foo)service;
         Baz baz = (Baz)foo.Baz;
  
         Console.WriteLine("cat.GetService<IFoo>(): {0}", service);
         Console.WriteLine("cat.GetService<IFoo>().Bar: {0}", foo.Bar);
         Console.WriteLine("cat.GetService<IFoo>().Baz: {0}", foo.Baz);
         Console.WriteLine("cat.GetService<IFoo>().Baz.Qux: {0}", baz.Qux);
    }
 }
```

这段程序被成功执行之后会在控制台上产生如下所示的输出结果，这充分证明了作为DI容器的Cat对象不仅仅根据指定的服务接口IFoo创建了对应类型（Foo）的服务对象，而且直接依赖的两个属性（Bar和Baz）分别以构造器注入和属性注入的方式被成功初始化，间接依赖的属性（Baz的属性Qux）也以方法注入的形式被成功初始化。


```
cat.GetService<IFoo>(): Foo
 cat.GetService<IFoo>().Bar: Bar
 cat.GetService<IFoo>().Baz: Baz
 cat.GetService<IFoo>().Baz.Qux: Qux
```

在对Cat容器的用法有了基本了解之后，我们来正式讨论它的总体设计和具体实现。我们首先来看看用来标识注入构造函数、注入属性和注入方法的InjectionAttribute特性的定义，如下面的代码片段所示，InjectionAttribute仅仅是一个单纯的标识特性，它的用途决定了应用该特性的目标元素的类型（构造函数、属性和方法）。


```
[AttributeUsage( AttributeTargets.Constructor| 
                  AttributeTargets.Property| 
                  AttributeTargets.Method, 
                  AllowMultiple = = false)]
 public class InjectionAttribute: Attribute {}
```

如下所示的是Cat类的完整定义。我们采用一个ConcurrentDictionary类型的字段来存放服务接口和具体服务类型之间的映射关系，这样的映射关系通过调用Register方法实现。针对服务类型（服务接口类型或者具体服务类型均可）的服务对象提供机制实现在GetService方法中。


```
public class Cat
 {
     private ConcurrentDictionary<Type, Type> typeMapping = new ConcurrentDictionary<Type, Type>();
  
     public void Register(Type from, Type to)
     {
         typeMapping[from] = to;
     }
  
     public object GetService(Type serviceType)
     {
         Type type;
         if (!typeMapping.TryGetValue(serviceType, out type))
         {
             type = serviceType;
         }
         if (type.IsInterface || type.IsAbstract)
         {
             return null;
         }
 
         ConstructorInfo constructor = this.GetConstructor(type);
         if (null == constructor)
         {
             return null;
         }
  
         object[] arguments = constructor.GetParameters().Select(p => this.GetService(p.ParameterType)).ToArray();
         object service = constructor.Invoke(arguments);
         this.InitializeInjectedProperties(service);
         this.InvokeInjectedMethods(service);
         return service;
     }
  
     protected virtual ConstructorInfo GetConstructor(Type type)
     {
         ConstructorInfo[] constructors = type.GetConstructors();
         return constructors.FirstOrDefault(c => c.GetCustomAttribute<InjectionAttribute>() != null)
             ?? constructors.FirstOrDefault();
     }
  
     protected virtual void InitializeInjectedProperties(object service)
    {
         PropertyInfo[] properties = service.GetType().GetProperties()
             .Where(p => p.CanWrite && p.GetCustomAttribute<InjectionAttribute>() != null)
             .ToArray();
         Array.ForEach(properties, p =>p.SetValue(service, this.GetService(p.PropertyType)));
     }
  
     protected virtual void InvokeInjectedMethods(object service)
     {
         MethodInfo[] methods = service.GetType().GetMethods()
             .Where(m => m.GetCustomAttribute<InjectionAttribute>() != null)
             .ToArray();
         Array.ForEach(methods, m=> 
         {
             object[] arguments = m.GetParameters().Select(p => this.GetService(p.ParameterType)).ToArray();
             m.Invoke(service, arguments);
         });
     }        
 }
```

如上面的代码片段所示，GetService方法利用GetConstructor方法返回的构造函数创建服务对象。GetConstructor方法体现了我们采用的注入构造函数的选择策略：优先选择标注有InjectionAttribute特性的构造函数，如果不存在则选择第一个公有的构造函数。执行构造函数传入的参数是递归地调用GetService方法根据参数类型获得的。
服务对象被成功创建之后，我们分别调用InitializeInjectedProperties和InvokeInjectedMethods方法针对服务对象实施属性注入和方法注入。对于前者（属性注入），我们在以反射的方式得到所有标注了InjectionAttribute特性的依赖属性并对它们进行赋值，具体的属性值同样是以递归的形式调用GetService方法针对属性类型获得。至于后者（方法注入），我们同样以反射的方式得到所有标注有InjectionAttribute特性的注入方法后自动调用它们，传入的参数值依然是递归地调用GetService方法针对参数类型的返回值。
