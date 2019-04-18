# Spring中Bean的生成过程 - z69183787的专栏 - CSDN博客
2017年11月01日 16:36:09[OkidoGreen](https://me.csdn.net/z69183787)阅读数：325
Spring是个Bean大工厂，负责Bean的创建和注入。
## 核心接口
BeanFactory是Spring最核心接口，它是Bean的容器，所有Bean的创建和注入几乎都与它有关。很多后缀是“Context”的类都继承或扩展了它，比如ClassPathXmlApplicationContext。最核心的方法是getBean，当然getBean有很多种重载形式，这里不一一列出。
```java
public interface BeanFactory {
    Object getBean(String name) throws BeansException;
    <T> T getBean(String name, Class<T> requiredType) throws BeansException;
    <T> T getBean(Class<T> requiredType) throws BeansException;
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
## Bean的属性定义
BeanDefinition是Bean的配置信息。无论何种XML还是JavaCode还是其他任何形式定义的Bean属性，Spring都会把他们解析成一个BeanDefinition。每一个Bean对应一个BeanDefinition。后续Bean的创建都依赖于BeanDefinition，因为需要从BeanDefinition获取创建时需要的必要属性。
```java
public interface BeanDefinition extends AttributeAccessor, BeanMetadataElement {
    String getScope();
    ConstructorArgumentValues getConstructorArgumentValues();
    MutablePropertyValues getPropertyValues();
    boolean isSingleton();
    boolean isPrototype();
    ...
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
## Bean定义的注册中心
Spring把解析后Bean配置BeanDefinition注册到BeanDefinitionRegistry。可以想象它为一个map，可以随时根据Bean的名字取出BeanDefinition。
```java
public interface BeanDefinitionRegistry extends AliasRegistry { //Bean的注册中心
    void registerBeanDefinition(String beanName, BeanDefinition beanDefinition)
            throws BeanDefinitionStoreException;
    BeanDefinition getBeanDefinition(String beanName) throws NoSuchBeanDefinitionException;
    ...
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
## 单例Bean的注册中心
Spring默认Bean的创建方式为单例模式，这也是我们最常用的模式。SingletonBeanRegistry是单例Bean注册中心，其实就是一个单例Bean的容器。Spring把创建好的单例Bean缓存起来，提高性能。DefaultSingletonBeanRegistry是SingletonBeanRegistry默认实现方法。从下面的源码可以看出，大都是Map和Set的集合，缓存了Bean的名字和Bean的实例。
```java
public class DefaultSingletonBeanRegistry extends SimpleAliasRegistry implements SingletonBeanRegistry {
    /** Cache of singleton objects: bean name --> bean instance */
    private final Map<String, Object> singletonObjects = new ConcurrentHashMap<String, Object>(64);
    /** Cache of singleton factories: bean name --> ObjectFactory */
    private final Map<String, ObjectFactory<?>> singletonFactories = new HashMap<String, ObjectFactory<?>>(16);
    /** Cache of early singleton objects: bean name --> bean instance */
    private final Map<String, Object> earlySingletonObjects = new HashMap<String, Object>(16);
    /** Set of registered singletons, containing the bean names in registration order */
    private final Set<String> registeredSingletons = new LinkedHashSet<String>(64);
    /** Names of beans that are currently in creation */
    private final Set<String> singletonsCurrentlyInCreation =
            Collections.newSetFromMap(new ConcurrentHashMap<String, Boolean>(16));
    /** Names of beans currently excluded from in creation checks */
    private final Set<String> inCreationCheckExclusions =
            Collections.newSetFromMap(new ConcurrentHashMap<String, Boolean>(16));
    /** List of suppressed Exceptions, available for associating related causes */
    private Set<Exception> suppressedExceptions;
    /** Flag that indicates whether we're currently within destroySingletons */
    private boolean singletonsCurrentlyInDestruction = false;
    /** Disposable bean instances: bean name --> disposable instance */
    private final Map<String, Object> disposableBeans = new LinkedHashMap<String, Object>();
    /** Map between containing bean names: bean name --> Set of bean names that the bean contains */
    private final Map<String, Set<String>> containedBeanMap = new ConcurrentHashMap<String, Set<String>>(16);
    /** Map between dependent bean names: bean name --> Set of dependent bean names */
    private final Map<String, Set<String>> dependentBeanMap = new ConcurrentHashMap<String, Set<String>>(64);
    /** Map between depending bean names: bean name --> Set of bean names for the bean's dependencies */
    private final Map<String, Set<String>> dependenciesForBeanMap = new ConcurrentHashMap<String, Set<String>>(64);
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
## Bean的生成
DefaultListableBeanFactory是Spring的默认容器。核心方法是getBean方法，最终调用的都是AbstractBeanFactory的doGetBean方法。 
首先是从缓存获取Bean，如果缓存存在并且Bean是FactoryBean的实例，走FactoryBean获取Bean的逻辑，否则直接返回缓存中的Bean。 
![这里写图片描述](https://img-blog.csdn.net/20161102152146874)
如果缓存不存在Bean，则需要创建Bean，创建成功后在放入缓存。如果Bean是FactoryBean的实例，走FactoryBean获取Bean的逻辑，以后不再赘述。 
![这里写图片描述](https://img-blog.csdn.net/20161102152241219)
这里比较重要的方法是createBean方法，这个方法主要做了两件事。第一件事是在创建Bean前给InstantiationAwareBeanPostProcess机会处理创建Bean返回；第二件事是调用doCreateBean创建Bean。 
![这里写图片描述](https://img-blog.csdn.net/20161102152313184)
doCreateBean做了很多事情，几乎Spring创建Bean中最关键方法之一。第一步：调用Bean的构造函数创建Bean，这时候并没有初始化Bean； 
![这里写图片描述](https://img-blog.csdn.net/20161102152510597)
第二步是判断是是否提前暴露Bean（Spring默认是提前暴露Bean的，为了解决属性注入循环依赖的问题）； 
![这里写图片描述](https://img-blog.csdn.net/20161102152731545)
第三步就是实例化Bean的字段 
![这里写图片描述](https://img-blog.csdn.net/20161102152557395)
其中populateBean中会自动注入带有@Autowired,@Resource等注解的Bean，实际实现为AutowiredAnnotationBeanPostProcessor.postProcessPropertyValues方法。 
![这里写图片描述](https://img-blog.csdn.net/20161102152757483)
接下来终于到了初始化Bean了。 
![这里写图片描述](https://img-blog.csdn.net/20161102152852077)
首先调用invokeAwareMethods方法填入带有Aware的资源 
![这里写图片描述](https://img-blog.csdn.net/20161102152926092)
然后给BeanPostProcessor有机会调用postProcessBeforeInitialization方法处理 
![这里写图片描述](https://img-blog.csdn.net/20161102153006853)
最后就是调用初始化方法，比如InitializingBean.afterPropertiesSet()就在这个时候调用，还有一些自定义初始化方法也会子在这个时候调用. 
![这里写图片描述](https://img-blog.csdn.net/20161102152950852)
这样整个Bean的生成过程就结束了。
