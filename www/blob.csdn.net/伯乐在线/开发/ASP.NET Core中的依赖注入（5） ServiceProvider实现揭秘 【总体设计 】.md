# ASP.NET Core中的依赖注入（5）: ServiceProvider实现揭秘 【总体设计 】 - 文章 - 伯乐在线
原文出处： [蒋金楠](http://www.cnblogs.com/artech/p/asp-net-core-di-service-provider-1.html)
本系列前面的文章我们主要以编程的角度对ASP.NET Core的依赖注入系统进行了详细的介绍，如果读者朋友们对这些内容具有深刻的理解，我相信你们已经可以正确是使用这些与依赖注入相关的API了。如果你还对这个依赖注入系统底层的实现原理具有好奇心，可以继续阅读这一节的内容。
> 
目录
一、ServiceCallSite
二、Service
三、ServiceEntry
四、ServiceTable
五、ServiceProvider
作为DI容器的体现，ServiceProvider是ASP.NET Core依赖注入系统的一个核心对象，但是默认的实现者是一个定义在程序集 “Microsoft.Extensions.DependencyInjection.dll” 中的一个名为 “ServiceProvider” 内部（Internal）类型，而且它所依赖的很多接口和类型也是如此，所以我相信实现在这个ServiceProvider类中的服务提供机制对于绝大部分人是陌生的。本节提及的ServiceProvider不是泛指实现了IServiceProvider接口的类型，而是专指ServiceProvider这个内部类型。
为了让读者朋友们能够深刻地了解ServiceProvider内部的实现原理，我会在本节内容中重新定义它。在这里需要特别说明的是我们重建的ServiceProvider以及其他重建的接口和类旨在体现真实ServiceProvider设计思想和实现原理，在具体的源代码层面是有差异的。考虑到篇幅的问题，很多细节的内容将不会体现在我们重建的接口和类型中。如果想了解原始的实现逻辑，可以从GitHub上下载源代码。
从总体设计的角度来审视ServiceProvider，需要涉及与之相关的4个核心对象，包括ServiceCallSite、Service、ServiceEntry和ServiceTable，它们均体现为相应的接口和类，并且这些接口和泪都是内部的，接下来我们就来逐一认识它们。
# 一、ServiceCallSite
ServiceProvider的核心功能就是针对服务类型提供相应的服务实例，而服务实例的提供最终是通过ServiceCallSite来完成的。ServiceCallSite体现为具有如下定义的IServiceCallSite接口，除了直接提供服务实例的Invoke方法之外，它还具有另一个返回类型为Expression的Build方法，该方法将定义在Invoke方法中的逻辑定义成一个表达式。


```
internal interface IServiceCallSite
 {
     object Invoke(ServiceProvider provider);
     Expression Build(Expression provider);
 }
```
在真正提供服务实例的时候，ServiceProvider在收到针对某个服务类型的第一个服务获取请求时，他会直接调用对应ServiceCallSite的Invoke方法返回提供的服务实例。与此同时，这个ServiceCallSite的Build方法会被调用并生成一个表达式，该表达式进一步编译成一个类型为Func的委托对象并被缓存起来。针对同一个服务类型的后续服务实例将直接使用这个缓存的委托对象来提供。
# 二、Service
我们知道ServiceProvider提供服务的依据来源于创建它指定一个ServiceCollection对象，用于指导ServiceProvider如何提供所需服务的信息以ServiceDescriptor对象的形式保存在这个集合对象中。当ServiceProvider被初始化后，每一个ServiceDescriptor将会被转换成一个Service对象，后者体现为如下一个IService接口。


```
internal interface IService
{
     IService Next { get; set; }
     ServiceLifetime Lifetime { get; }
     IServiceCallSite CreateCallSite(ServiceProvider provider, ISet<Type> callSiteChain);
}
```
Service的Lifetime属性自然来源于ServiceDescriptor的同名属性，它的CreateCallSite方法返回一个针对用于提供对应服务实例的ServiceCallSite对象。由于Service对象可以创建ServiceCallSite，所以它自然具有提供服务实例的能力。Service总是作为链表的某个节点存在，这个链表是具有相同服务类型（对应ServiceType属性）的多个ServiceDescriptot生成的，Service的Next属性保持着对链表后一个节点的引用。
# 三、ServiceEntry
上面我们所说的由Service对象组成的链表体现为如下一个ServiceEntry类。我们为ServiceEntry定义了三个属性（First、Last、All）分别代笔这个链表的第一个节点、最后一个节点以及所有节点，节点类型为IService。如果需要在链尾追加一个Service对象，可以直接调用Add方法。


```
internal class ServiceEntry
 {
     public IService         First { get; private set; }
     public IService         Last { get; private set; }
     public IList<IService>     All { get; private set; } = new List<IService>();
  
     public ServiceEntry(IService service)
     {
         this.First = service;
         this.Last = service;
         this.All.Add(service);
     }
  
     public void Add(IService service)
     {
         this.Last.Next = service;
         this.Add(service);
     }
 }
```
# 四、ServiceTable
多个ServiceEntry组成一个ServiceTable。如下面的代码片段所示，一个ServiceTable通过其只读属性ServieEntries维护着一组ServiceEntry对象与它们对应的服务类型之间的映射关系。一个ServiceTable对象通过一个ServiceCollection对象创建出来。如下面的代码片段所示，组成ServiceCollection的所有ServiceDescriptor对象先根据其ServiceType属性体现的服务类型进行分组，由每组ServiceDescriptor创建的ServiceEntry对象与对应的服务类型之间的映射会被添加到ServiceEntries属性中。


```
internal class ServiceTable
{
     public IDictionary<Type, ServiceEntry> ServieEntries { get; private set; } = new Dictionary<Type, ServiceEntry>();
  
     public ServiceTable(IServiceCollection services)
     {
         foreach (var group in services.GroupBy(it=>it.ServiceType))
             ServiceDescriptor[] descriptors = group.ToArray();
             ServiceEntry entry = new ServiceEntry(new Service(descriptors[0]));
             for (int index = 1; index < descriptors.Length; index++)
             {
                 entry.Add(new Service(descriptors[index]));
             }
             this.ServieEntries[group.Key] = entry;
         }
         //省略其他代码
     }
 }
```
从上面的代码片段可以看出组成ServiceEntry的是一个类型为Service的对象，该类型定义如下。Service类实现了IService接口并通过一个ServiceDescriptor对象创建而成。我们省略了定义在方法CreateCallSite中创建ServiceCallSite的逻辑，后续在介绍各种类型的ServiceCallSite的时候我们会回来讲述该方法的实现。


```
internal class Service : IService
 {
     public ServiceDescriptor     ServiceDescriptor { get; private set; }
     public ServiceLifetime         Lifetime => this.ServiceDescriptor.Lifetime;
     public IService             Next { get; set; }
  
     public Service(ServiceDescriptor serviceDescriptor)
     {
         this.ServiceDescriptor = serviceDescriptor;
     }
  
     public IServiceCallSite CreateCallSite(ServiceProvider provider, ISet<Type> callSiteChain)
     {
         <<省略实现>>
     }
 }
```
# 五、ServiceProvider
如下所示的代码片段揭示了实现在ServiceProvider之中与服务提供和回收相关的基本实现原理。我们先来简单介绍定义在它内部的几个属性。Root属性返回的ServiceProvider代表它的根，对于一个独立的ServiceProvider来说，这个根就是它自己。ServiceTable属性返回根据ServiceCollection创建的ServiceTable对象。上面介绍ServiceCallSite的时候，我们提到它的Build方法返回的表达式会编译成一个类型为Func 的委托，并被缓存起来服务于后续针对同一个类型的服务提供请求，该委托对象与对应服务类型之间的映射关系就保存在RealizedServices属性中。


```
internal class ServiceProvider : IServiceProvider, IDisposable
 {
     public ServiceProvider Root { get; private set; }
     public ServiceTable ServiceTable { get; private set; }
     public ConcurrentDictionary<Type, Func<ServiceProvider, object>> RealizedServices { get; private set; } = new ConcurrentDictionary<Type, Func<ServiceProvider, object>>();
     public IList<IDisposable> TransientDisposableServices { get; private set; } = new List<IDisposable>();
     public ConcurrentDictionary<IService, object> ResolvedServices { get; private set; } = new ConcurrentDictionary<IService, object>();
    
     public ServiceProvider(IServiceCollection services)
     {
         this.Root         = this;
         this.ServiceTable     = new ServiceTable(services);
     }
  
     public object GetService(Type serviceType)
     {
         Func<ServiceProvider, object> serviceAccessor;
         if (this.RealizedServices.TryGetValue(serviceType, out serviceAccessor))
         {
             return serviceAccessor(this);
         }
  
         IServiceCallSite serviceCallSite = this.GetServiceCallSite(serviceType, new HashSet<Type>());
         if (null != serviceCallSite)
         {
             var providerExpression = Expression.Parameter(typeof(ServiceProvider), "provider");
             this.RealizedServices[serviceType] = Expression.Lambda<Func<ServiceProvider, object>>(serviceCallSite.Build(providerExpression), providerExpression).Compile();
             return serviceCallSite.Invoke(this);
         }
  
         this.RealizedServices[serviceType] = _ => null;
        return null;
    }
  
     public IServiceCallSite GetServiceCallSite(Type serviceType, ISet<Type> callSiteChain)
     {
             try
             {
                 if (callSiteChain.Contains(serviceType))
                 {
                     throw new InvalidOperationException(string.Format("A circular dependency was detected for the service of type '{0}'", serviceType.FullName);
                 }
                 callSiteChain.Add(serviceType);
 
                 ServiceEntry serviceEntry;
                 if (this.ServiceTable.ServieEntries.TryGetValue(serviceType, 
                     out serviceEntry))
                 {
                     return serviceEntry.Last.CreateCallSite(this, callSiteChain);
                 }
  
                 //省略其他代码
  
                 return null;
             }
             finally
             {
                 callSiteChain.Remove(serviceType);
             }
    }    
  
     public void Dispose()
     {
         Array.ForEach(this.TransientDisposableServices.ToArray(), _ => _.Dispose());
         Array.ForEach(this.ResolvedServices.Values.ToArray(), _ => (_ as IDisposable)?.Dispose());
         this.TransientDisposableServices.Clear();
         this.ResolvedServices.Clear();
     }
     //其他成员
 }
```
对于采用Scoped模式提供的服务实例，ServiceProvider需要自行对它们进行维护，具体来说它们会和对应的Service对象之间的映射关系会保存在ResolvedServices属性中。如果采用Transient模式，对于提供过的服务实例，如果自身类型实现了IDisposble接口，它们会被添加到TransientDisposableServices属性返回的列表中。当Dispose方法执行的时候，这两组对象的Dispose方法会被执行。
真正的服务提供机制体现在ServiceProvider实现的GetService方法中，实现逻辑其实很简单：ServiceProvider会根据指定的服务类型从RealizedServices属性中查找是否有通过编译表达式生成的Func委托生成出来，如果存在则直接使用它生成提供的服务实例。如果这样的委托不存在，则会试着从ServiceTable中找到对应的ServiceEntry，如果不存在直接返回Null，否则会调用ServiceEntry所在列表最后一个Service的CreateServiceCallSite方法创建一个ServiceCallSite对象（这一点说明了如果针对同一个服务类型注册了多个ServiceDescriptor，在提供单个服务的时候总是使用最后一个ServiceDescriptor）。
接下来这个ServiceCallSite的Invoke方法被调用来创建服务实例，在返回该实例之前它的Build方法会被调用，返回的表达式被编译成Func委托并被添加到RealizedServices属性中。如果ServiceProvider后续需要提供同类型的服务，这个委托对象将被启用。
