# ASP.NET Core 源码阅读之 Microsoft.Extensions.DependencyInjection - 文章 - 伯乐在线
原文出处： [远方无风](http://www.cnblogs.com/bill-shooting/p/5540665.html)
这篇随笔主要记录一下ASP.NET Core团队实现默认的依赖注入容器的过程，我的理解可能并不是正确的。
DependencyInjection这个项目不大，但却是整个ASP.NET Core的基础，因为它提供了依赖注入（DI）容器的默认实现，而依赖注入贯穿整个ASP.NET Core。相关源码可以去[GitHub AspNet](https://github.com/aspnet/DependencyInjection) 上下载。
要实现是一个依赖注入容器，主要是实现它添加依赖、描述依赖、存储依赖和解析依赖的能力，可以分别用Add(A), Describe(D), Store(S), Resolve(R)表示。从功能的角度来讲，分别对应着ServiceCollection，ServiceDescriptor，Service，ServiceEntry，ServiceTable，ServiceProvider，以及CallSite相关的类。
对于框架使用者来说，注册一项服务最自然的方式就是提供服务的接口和实现这个接口的服务实例，比如IEmail是用户需求的服务，而Outlook类就是服务的实例类型，用这两种信息注册一项服务是最自然的。所以ASP.NET Core团队提供了ServiceDescriptor类型来提供对服务的描述功能。


C#
```
public class ServiceDescriptor
    {
        /// <inheritdoc />
        public ServiceLifetime Lifetime { get; }
        /// <inheritdoc />
        public Type ServiceType { get; }
        /// <inheritdoc />
        public Type ImplementationType { get; }
        /// <inheritdoc />
        public object ImplementationInstance { get; }
        /// <inheritdoc />
        public Func<IServiceProvider, object> ImplementationFactory { get; }
        internal Type GetImplementationType(){...}
        public static ServiceDescriptor Transient(){...}
        public static ServiceDescriptor Singleton(){...}
        public static ServiceDescriptor Scoped(){...}
    }
```
可以看到ServiceDescriptor已经存储了服务的类型信息以及生命周期，貌似已经可以凭借着Dictionary存储所有的服务关系了。但有个问题，如果同一个服务注册了多个服务实例类型怎么办？比如IEmail服务同时注册Outlook和GMail，该怎么存储，解析的时候又该用哪个？为了解决这个问题，ASP.NET Core团队提供了Service和ServiceEntry。不要以为Service是非常牛逼的类，其实它非常简单，Service就是一个存储ServiceDescriptor的单向链表节点，而ServiceEntry就是以Service为节点的单向链表。


C#
```
internal class ServiceEntry
    {
        private object _sync = new object();
        public ServiceEntry(IService service)
        {
            First = service;
            Last = service;
        }
        public IService First { get; private set; }
        public IService Last { get; private set; }
        public void Add(IService service)
        {
            lock (_sync)
            {
                Last.Next = service;
                Last = service;
            }
        }
    }
```


C#
```
internal class Service : IService
    {
        private readonly ServiceDescriptor _descriptor;
        public Service(ServiceDescriptor descriptor)
        {
            _descriptor = descriptor;
        }
        public IService Next { get; set; }
        public ServiceLifetime Lifetime
        {
            get { return _descriptor.Lifetime; }
        }
        public IServiceCallSite CreateCallSite(){...}
    }
```
从上面的源码可以看出Service类和ServiceEntry类就是一个典型的链表节点和链表的关系，Service类中还有一个很重要的方法是CreateCallSite()，这是每个实现了IService的接口都要实现的方法。至于什么是callsite，之后会说到。
用ServiceEntry解决了一个服务的存储问题，自然一堆服务的存储就是用ServiceTable来存储。ServiceTable使用哈希表作为底层容器，以ServiceType为Key，ServiceEntry为Value存储在Dictionary中。为了优化存储结构，缓存一些已经实现过的服务，ServiceTable还添加了关于RealizedService的字段和方法。主要源码见下面：


C#
```
internal class ServiceTable
    {
        private readonly object _sync = new object();
        private readonly Dictionary<Type, ServiceEntry> _services;
        private readonly Dictionary<Type, List<IGenericService>> _genericServices;
        private readonly ConcurrentDictionary<Type, Func<ServiceProvider, object>> _realizedServices = new ConcurrentDictionary<Type, Func<ServiceProvider, object>>();
     
        //注意ServiceTable只能被ServiceDescriptor的集合初始化
        public ServiceTable(IEnumerable<ServiceDescriptor> descriptors){...}
        //省略了有关容器添加获取的方法
    }
```
以上就是ASP.NET Core服务存储的相关过程，就实现来说，还是比较简单的，就是以K/V的形式，按照服务的类别存储实现了服务的相应类型(普通类，泛型类，委托等）。
仔细观察这些类型，你会发现它们都是internal级别的，那哪个才是公开类型呢？答案是ServiceCollection，这个类和Service一样，看着很重要，其实就是一个ServiceDescriptor的List，因为它实现的接口继承了IList。


C#
```
public interface IServiceCollection : IList
    {
    }
    public class ServiceCollection : IServiceCollection
    {
        //省略相关代码
    }
```
ServiceCollection本质上是一个ServiceDescriptor的List，回忆一下，ServiceTable的构造函数正需要这样的类型啊！那个这两个类又有什么关系，解开这个谜题的关键在于这整个解决方案真正的主角：ServiceProvider。我在这之前迟迟没有提到一个依赖注入最关键的功能：解析依赖。对于一个服务A来说，它可能并不是独立的，它还在依赖服务B和服务C，而服务B又依赖服务D和服务E。。。一个合格的容器得再我们需要服务A时，能够正确的解析这个依赖链，并按照正确的顺序实例化并返回服务A。ServiceProvider是ASP.NET Core团队提供的默认的依赖注入容器。


C#
```
internal class ServiceProvider : IServiceProvider, IDisposable
    {
        private readonly ServiceProvider _root;
        private readonly ServiceTable _table;
        private bool _disposeCalled;
        private readonly Dictionary<IService, object> _resolvedServices = new Dictionary<IService, object>();
        private List<IDisposable> _transientDisposables;
        private static readonly Func<Type, ServiceProvider, Func<ServiceProvider, object>> _createServiceAccessor = CreateServiceAccessor;
        public ServiceProvider(IEnumerable<ServiceDescriptor> serviceDescriptors)
        {
            _root = this;
            _table = new ServiceTable(serviceDescriptors);
            _table.Add(typeof(IServiceProvider), new ServiceProviderService());
            _table.Add(typeof(IServiceScopeFactory), new ServiceScopeService());
            _table.Add(typeof(IEnumerable<>), new OpenIEnumerableService(_table));
        }
        public object GetService(Type serviceType){...}
        internal static Func<ServiceProvider, object> RealizeService(ServiceTable table, Type serviceType, IServiceCallSite callSite){...}
        internal IServiceCallSite GetServiceCallSite(Type serviceType, ISet<Type> callSiteChain){...}
        //省略了一些有关服务生存周期管理的方法以及一些其他私有方法
    }
```
首先需要注意的是，它有一个ServiceTable类型的字段，所以一个ServiceProvider不仅是一个解析器，而且是一个容器，是一个依赖注入容器。第二点，仔细观察它的构造函数，你会发现它向table字段中添加了三个服务，而且这三个服务是自添加的，每个ServiceProvider都有。再研究一下这些服务的名字，更加有意思，ServiceProviderService！！也就是说ServiceProvider也是一种服务，解析服务也是一种服务，容器也是一种服务。这意味着我们可以使用其他依赖注入容器。第三点，也是最重要的一点，这个Service，RealizedService，ResolvedService以及我们一直避而不谈的callsite究竟是啥？
当我们以类型的方式描述一种服务时，它就是所谓的Service，这时它的信息全部以元数据的方式存储。
每一个Service都有一个CreateCallSite方法，所谓callsite，直接翻译是“调用点”，但更好的理解方式我觉得是元数据和服务实例之间的桥梁，而如果一种Service元数据变成了Func委托，我们就把它称为RealizedService，在Provider的table里面，有这么一个字段专门管理RealizedService。那Func委托又怎么理解呢？这种委托可以看作是服务的兑换券，它还不是解析的服务，但是离它很近了！因为只要把ServiceProvider传进去，我们就能得到解析过的Service。
如果把Func委托当成兑换券，那么ServiceProvider就是兑换人，把兑换券拿给兑换人，我们就能得到object类型的服务，这种服务称之为ResolvedService，在ServiceProvider中专门有一个字段缓存这些解析过的服务。callsite的Invoke(provider)方法得到一个服务实例(Resolved)，而callsite的Build().Complie()方式可以得到Func委托(Realized)。
—————————————————————————————————————–
总结一下整个流程：
- 当我们注册一个服务时，最自然是通过它的类型和它的实现类型来注册，比如IEmail类型和Outlook类型，所以要用到ServiceDescriptor；
- ServiceDescriptor包装一下，摇身一变成为Service，并且得到了一个关键方法CreateCallSite()；
- 为什么要callsite这种东西，主要是为了配合Provider管理服务的生命周期，以及实现一些特殊的解析服务的功能。如上所述，callsite的Invoke()得到ResolvedService，callsite的Build()方法得到RealizedService；
- 由Provider根据生命周期负责回收服务。
