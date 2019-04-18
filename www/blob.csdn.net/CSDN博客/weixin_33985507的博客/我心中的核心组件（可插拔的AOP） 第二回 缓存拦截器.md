# 我心中的核心组件（可插拔的AOP）~第二回 缓存拦截器 - weixin_33985507的博客 - CSDN博客
2013年08月05日 11:30:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
[回到目录](http://www.cnblogs.com/lori/p/3247905.html)
AOP面向切面的编程，也称面向方面的编程，我更青睐于前面的叫法，将一个大系统切成多个独立的部分，而这个独立的部分又可以方便的插拔在其它领域的系统之中，这种编程的方式我们叫它面向切面，而这些独立的部分，我们很早之前叫它部件，在SOA里，它叫做服务，而我认为叫它模块更加贴切，确实，这些与领域无关的东西，是像是一个个的功能模块。
之前讲过一个日志组件，有兴趣的同学可以查看：[第一回 日志记录组件](http://www.cnblogs.com/lori/archive/2013/01/14/2860466.html)
今天主要说一下缓存组件，就是缓存模块，这些模块可以很方便的为每个方法添加缓存机制，事实上是在方法体执行之前，进行缓存对象的检索，当检索到有缓存，就直接加载缓存对象了，这对于数据高并发情况下，尤其有用，呵呵。
### **实现缓存的武器：Microsoft.Practices.EnterpriseLibrary.Caching**
### **辅助兵器（IOC）：Microsoft.Practices.Unity**
### **实现的效果：根据在配置文件中对要缓存的部分进行配置后，使它减少对数据库的交互，提高程序的相应能力**
下面开始我们的Caching之旅
1 使用nuget添加caching和Unity组件，添加好了之后在引用中自己出现
在package.config中有我们的组件的相关说明
```
<packages> 
  <package id="Unity" version="3.0.1304.0" targetFramework="net45" />
  <package id="Unity.Interception" version="3.0.1304.0" targetFramework="net45" />
  <package id="EnterpriseLibrary.Caching" version="5.0.505.0" targetFramework="net45" />
  <package id="EnterpriseLibrary.Common" version="5.0.505.0" targetFramework="net45" />
  <package id="CommonServiceLocator" version="1.0" targetFramework="net45" />
</packages>
```
2 在web.config添加相应的unity注入信息和拦截信息的配置
```
<configSections>
    <!-- For more information on Entity Framework configuration, visit http://go.microsoft.com/fwlink/?LinkID=237468 -->
    <section name="entityFramework" type="System.Data.Entity.Internal.ConfigFile.EntityFrameworkSection, EntityFramework, Version=5.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089" requirePermission="false" />
    <section name="unity" type="Microsoft.Practices.Unity.Configuration.UnityConfigurationSection, Microsoft.Practices.Unity.Configuration" />
    <section name="log4net" type="log4net.Config.Log4NetConfigurationSectionHandler, log4net" />
    <section name="cachingConfiguration" type="Microsoft.Practices.EnterpriseLibrary.Caching.Configuration.CacheManagerSettings, Microsoft.Practices.EnterpriseLibrary.Caching, Version=5.0.505.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35" />
  </configSections>
```
```
<unity xmlns="http://schemas.microsoft.com/practices/2010/unity">
    <sectionExtension type="Microsoft.Practices.Unity.InterceptionExtension.Configuration.InterceptionConfigurationExtension, Microsoft.Practices.Unity.Interception.Configuration" />
    <container>
      <extension type="Interception" />
      <register type="Infrastructure.Caching.ICacheProvider,  DDD_AOP_WCF" mapTo="Infrastructure.Caching.EntLibCacheProvider,  DDD_AOP_WCF" />
      <!--Repository Context & Repositories-->
      <register type="DDD_AOP_WCF.Repository.IProductRepository, DDD_AOP_WCF" mapTo="DDD_AOP_WCF.Repository.ProductRepository, DDD_AOP_WCF">
       <!--  <interceptor type="VirtualMethodInterceptor" />-->
        <interceptor type="InterfaceInterceptor"/>
        <interceptionBehavior type="Infrastructure.InterceptionBehaviors.CachingBehavior,DDD_AOP_WCF" />
        <interceptionBehavior type="Infrastructure.InterceptionBehaviors.ExceptionLoggingBehavior, DDD_AOP_WCF" />
      </register>
    </container>
  </unity>
```
下面是缓存组件的配置：
```
<!--BEGIN: Caching-->
  <cachingConfiguration defaultCacheManager="ByteartRetailCacheManager">
    <cacheManagers>
      <add name="ByteartRetailCacheManager" type="Microsoft.Practices.EnterpriseLibrary.Caching.CacheManager, Microsoft.Practices.EnterpriseLibrary.Caching, Version=5.0.505.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35"
          expirationPollFrequencyInSeconds="600" maximumElementsInCacheBeforeScavenging="1000"
          numberToRemoveWhenScavenging="10" backingStoreName="NullBackingStore" />
    </cacheManagers>
    <backingStores>
      <add type="Microsoft.Practices.EnterpriseLibrary.Caching.BackingStoreImplementations.NullBackingStore, Microsoft.Practices.EnterpriseLibrary.Caching, Version=5.0.505.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35"
          name="NullBackingStore" />
    </backingStores>
  </cachingConfiguration>
  <!--END: Caching-->
```
3 建立一个测试用的IRepository接口和一个个性化操作的接口IProductRepository
```
public interface IRepository<TEntity> where TEntity : class
    {
        void Insert(TEntity entity);
        string Hello();
        IQueryable<TEntity> GetEntities();
    }
```
```
public interface IProductRepository : IRepository<Product>
    {
        /// <summary>
        /// 获取产品列表
        /// </summary>
        /// <returns></returns>
        [Caching(CachingMethod.Get)]
        List<Product> GetProduct();
        /// <summary>
        /// 建立产品
        /// </summary>
        [Caching(CachingMethod.Remove, "GetProduct")]
        void AddProduct(Product entity);
        /// <summary>
        /// 修改产品
        /// </summary>
        [Caching(CachingMethod.Remove, "GetProduct")]
        void ModifyProduct(Product entity);
    }
```
对这个接口进行实现，当然，它可以有多个实现版本，这也是IoC出现的原因
4 建立一个本地服务器，它是与IoC实现松耦合的前提，而IoC是我们实现程序代码松耦合的前提，呵呵。
```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
  /// <summary>
    /// Represents the Service Locator.
    /// </summary>
    public sealed class ServiceLocator : IServiceProvider
    {
        #region Private Fields
        private readonly IUnityContainer container;
        #endregion
        #region Private Static Fields
        private static readonly ServiceLocator instance = new ServiceLocator();
        #endregion
        #region Ctor
        /// <summary>
        /// Initializes a new instance of ServiceLocator class.
        /// </summary>
        private ServiceLocator()
        {
            UnityConfigurationSection section = (UnityConfigurationSection)ConfigurationManager.GetSection("unity");
            container = new UnityContainer();
            section.Configure(container);
        }
        #endregion
        #region Public Static Properties
        /// <summary>
        /// Gets the singleton instance of the ServiceLocator class.
        /// </summary>
        public static ServiceLocator Instance
        {
            get { return instance; }
        }
        #endregion
        #region Private Methods
        private IEnumerable<ParameterOverride> GetParameterOverrides(object overridedArguments)
        {
            List<ParameterOverride> overrides = new List<ParameterOverride>();
            Type argumentsType = overridedArguments.GetType();
            argumentsType.GetProperties(BindingFlags.Public | BindingFlags.Instance)
                .ToList()
                .ForEach(property =>
                {
                    var propertyValue = property.GetValue(overridedArguments, null);
                    var propertyName = property.Name;
                    overrides.Add(new ParameterOverride(propertyName, propertyValue));
                });
            return overrides;
        }
        #endregion
        #region Public Methods
        /// <summary>
        /// Gets the service instance with the given type.
        /// </summary>
        /// <typeparam name="T">The type of the service.</typeparam>
        /// <returns>The service instance.</returns>
        public T GetService<T>()
        {
            return container.Resolve<T>();
        }
        /// <summary>
        /// Gets the service instance with the given type by using the overrided arguments.
        /// </summary>
        /// <typeparam name="T">The type of the service.</typeparam>
        /// <param name="overridedArguments">The overrided arguments.</param>
        /// <returns>The service instance.</returns>
        public T GetService<T>(object overridedArguments)
        {
            var overrides = GetParameterOverrides(overridedArguments);
            return container.Resolve<T>(overrides.ToArray());
        }
        /// <summary>
        /// Gets the service instance with the given type by using the overrided arguments.
        /// </summary>
        /// <param name="serviceType">The type of the service.</param>
        /// <param name="overridedArguments">The overrided arguments.</param>
        /// <returns>The service instance.</returns>
        public object GetService(Type serviceType, object overridedArguments)
        {
            var overrides = GetParameterOverrides(overridedArguments);
            return container.Resolve(serviceType, overrides.ToArray());
        }
        #endregion
        #region IServiceProvider Members
        /// <summary>
        /// Gets the service instance with the given type.
        /// </summary>
        /// <param name="serviceType">The type of the service.</param>
        /// <returns>The service instance.</returns>
        public object GetService(Type serviceType)
        {
            return container.Resolve(serviceType);
        }
        #endregion
    }
View Code
```
5 建立一个缓存拦截器，它是与具体领域没有关系的，我们的拦截器Interception，可以有两个，如缓存拦截，日志拦截，异常拦截等等，我会在后面的文章中进
行介绍
```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
  /// <summary>
    /// 表示用于方法缓存功能的拦截行为。
    /// </summary>
    public class CachingBehavior : IInterceptionBehavior
    {
        #region Private Methods
        /// <summary>
        /// 根据指定的<see cref="CachingAttribute"/>以及<see cref="IMethodInvocation"/>实例，
        /// 获取与某一特定参数值相关的键名。
        /// </summary>
        /// <param name="cachingAttribute"><see cref="CachingAttribute"/>实例。</param>
        /// <param name="input"><see cref="IMethodInvocation"/>实例。</param>
        /// <returns>与某一特定参数值相关的键名。</returns>
        private string GetValueKey(CachingAttribute cachingAttribute, IMethodInvocation input)
        {
            switch (cachingAttribute.Method)
            {
                // 如果是Remove，则不存在特定值键名，所有的以该方法名称相关的缓存都需要清除
                case CachingMethod.Remove:
                    return null;
                // 如果是Get或者Put，则需要产生一个针对特定参数值的键名
                case CachingMethod.Get:
                case CachingMethod.Put:
                    if (input.Arguments != null &&
                        input.Arguments.Count > 0)
                    {
                        var sb = new StringBuilder();
                        for (int i = 0; i < input.Arguments.Count; i++)
                        {
                            sb.Append(input.Arguments[i].ToString());
                            if (i != input.Arguments.Count - 1)
                                sb.Append("_");
                        }
                        return sb.ToString();
                    }
                    else
                        return "NULL";
                default:
                    throw new InvalidOperationException("无效的缓存方式。");
            }
        }
        #endregion
        #region IInterceptionBehavior Members
        /// <summary>
        /// 获取当前行为需要拦截的对象类型接口。
        /// </summary>
        /// <returns>所有需要拦截的对象类型接口。</returns>
        public IEnumerable<Type> GetRequiredInterfaces()
        {
            return Type.EmptyTypes;
        }
        /// <summary>
        /// 通过实现此方法来拦截调用并执行所需的拦截行为。
        /// </summary>
        /// <param name="input">调用拦截目标时的输入信息。</param>
        /// <param name="getNext">通过行为链来获取下一个拦截行为的委托。</param>
        /// <returns>从拦截目标获得的返回信息。</returns>
        public IMethodReturn Invoke(IMethodInvocation input, GetNextInterceptionBehaviorDelegate getNext)
        {
            var method = input.MethodBase;
            var key = method.Name;
            if (method.IsDefined(typeof(CachingAttribute), false))
            {
                var cachingAttribute = (CachingAttribute)method.GetCustomAttributes(typeof(CachingAttribute), false)[0];
                var valKey = GetValueKey(cachingAttribute, input);
                switch (cachingAttribute.Method)
                {
                    case CachingMethod.Get:
                        try
                        {
                            if (CacheManager.Instance.Exists(key, valKey))
                            {
                                var obj = CacheManager.Instance.Get(key, valKey);
                                var arguments = new object[input.Arguments.Count];
                                input.Arguments.CopyTo(arguments, 0);
                                return new VirtualMethodReturn(input, obj, arguments);
                            }
                            else
                            {
                                var methodReturn = getNext().Invoke(input, getNext);
                                CacheManager.Instance.Add(key, valKey, methodReturn.ReturnValue);
                                return methodReturn;
                            }
                        }
                        catch (Exception ex)
                        {
                            return new VirtualMethodReturn(input, ex);
                        }
                    case CachingMethod.Put:
                        try
                        {
                            var methodReturn = getNext().Invoke(input, getNext);
                            if (CacheManager.Instance.Exists(key))
                            {
                                if (cachingAttribute.Force)
                                {
                                    CacheManager.Instance.Remove(key);
                                    CacheManager.Instance.Add(key, valKey, methodReturn.ReturnValue);
                                }
                                else
                                    CacheManager.Instance.Put(key, valKey, methodReturn.ReturnValue);
                            }
                            else
                                CacheManager.Instance.Add(key, valKey, methodReturn.ReturnValue);
                            return methodReturn;
                        }
                        catch (Exception ex)
                        {
                            return new VirtualMethodReturn(input, ex);
                        }
                    case CachingMethod.Remove:
                        try
                        {
                            var removeKeys = cachingAttribute.CorrespondingMethodNames;
                            foreach (var removeKey in removeKeys)
                            {
                                if (CacheManager.Instance.Exists(removeKey))
                                    CacheManager.Instance.Remove(removeKey);
                            }
                            var methodReturn = getNext().Invoke(input, getNext);
                            return methodReturn;
                        }
                        catch (Exception ex)
                        {
                            return new VirtualMethodReturn(input, ex);
                        }
                    default: break;
                }
            }
            return getNext().Invoke(input, getNext);
        }
        /// <summary>
        /// 获取一个<see cref="Boolean"/>值，该值表示当前拦截行为被调用时，是否真的需要执行
        /// 某些操作。
        /// </summary>
        public bool WillExecute
        {
            get { return true; }
        }
        #endregion
    }
View Code
```
6 下面是前台程序的调用方法
```
IProductRepository productRepository = ServiceLocator.Instance.GetService<IProductRepository>();
   ViewBag.Product = productRepository.GetProduct();
```
```
@{var Model = ViewBag.Product as List<学习陈晴阳的DDD_AOP_WCF.Product>; }
@if (Model != null && Model.Count > 0)
{
    foreach (var item in Model)
    {
    <p>@item.ProductName</p>
    }
}
```
好了，当我们为程序加上缓存拦截器之后，当它的数据没有发生变化时，会直接从缓存中读取对象，而不会与数据库发生访问！
[回到目录](http://www.cnblogs.com/lori/p/3247905.html)
