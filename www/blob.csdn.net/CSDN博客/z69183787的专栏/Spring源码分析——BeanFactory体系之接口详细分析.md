# Spring源码分析——BeanFactory体系之接口详细分析 - z69183787的专栏 - CSDN博客
2016年05月04日 10:06:39[OkidoGreen](https://me.csdn.net/z69183787)阅读数：617
前面的博文分析了Spring的Resource资源类Resouce。今天开始分析Spring的IOC部分。众所周知，IOC是Spring框架最迷人的地方。它最重要的接口，就是BeanFactory了。BeanFactory有着庞大的继承、实现体系，有众多的子接口、实现类。本博文的目标就是抽丝剥茧，从源代码入手，分析Spring的实现和架构，从中进步。
　　在阅读的过程中，可以参照Spring文档来一起学习：Spring3.1.1文档
　　本人英文水平有限，部分翻译可能不恰当，欢迎指出！
一、BeanFactory的基本类体系结构（接口为主）：
　　这是我画的BeanFactory基本的类体系结构，这里没有包括强大的ApplicationContext体系，ApplicationContext我准备放到下一篇再分析。
具体：
　　1、BeanFactory作为一个主接口不继承任何接口，暂且称为一级接口。
　　2、有3个子接口继承了它，进行功能上的增强。这3个子接口称为二级接口。
　　3、ConfigurableBeanFactory可以被称为三级接口，对二级接口HierarchicalBeanFactory进行了再次增强，它还继承了另一个外来的接口SingletonBeanRegistry
　　4、ConfigurableListableBeanFactory是一个更强大的接口，继承了上述的所有接口，无所不包，称为四级接口。
　　（这4级接口是BeanFactory的基本接口体系。继续，下面是继承关系的2个抽象类和2个实现类：）
　　5、AbstractBeanFactory作为一个抽象类，实现了三级接口ConfigurableBeanFactory大部分功能。
　　6、AbstractAutowireCapableBeanFactory同样是抽象类，继承自AbstractBeanFactory，并额外实现了二级接口AutowireCapableBeanFactory
　　7、DefaultListableBeanFactory继承自AbstractAutowireCapableBeanFactory，实现了最强大的四级接口ConfigurableListableBeanFactory，并实现了一个外来接口BeanDefinitionRegistry，它并非抽象类。
　　8、最后是最强大的XmlBeanFactory，继承自DefaultListableBeanFactory，重写了一些功能，使自己更强大。
总结：
　　BeanFactory的类体系结构看似繁杂混乱，实际上由上而下井井有条，非常容易理解。
二、IOC的始祖——BeanFactory
　　来看一下BeanFactory的[源码](http://www.2cto.com/ym)，这么牛逼哄哄的接口就不折叠了吧：
复制代码
package org.springframework.beans.factory;
public interface BeanFactory {
    /**
     * 用来引用一个实例，或把它和工厂产生的Bean区分开，就是说，如果一个FactoryBean的名字为a，那么，$a会得到那个Factory
     */
    String FACTORY_BEAN_PREFIX = "&";
    /*
     * 四个不同形式的getBean方法，获取实例
     */
    Object getBean(String name) throws BeansException;
    <T> T getBean(String name, Class<T> requiredType) throws BeansException;
    <T> T getBean(Class<T> requiredType) throws BeansException;
    Object getBean(String name, Object... args) throws BeansException;
    boolean containsBean(String name); // 是否存在
    boolean isSingleton(String name) throws NoSuchBeanDefinitionException;// 是否为单实例
    boolean isPrototype(String name) throws NoSuchBeanDefinitionException;// 是否为原型（多实例）
    boolean isTypeMatch(String name, Class<?> targetType)
            throws NoSuchBeanDefinitionException;// 名称、类型是否匹配
    Class<?> getType(String name) throws NoSuchBeanDefinitionException; // 获取类型
    String[] getAliases(String name);// 根据实例的名字获取实例的别名
}
复制代码
 具体：
　　1、4个获取实例的方法。getBean的重载方法。
　　2、4个判断的方法。判断是否存在，是否为单例、原型，名称类型是否匹配。
　　3、1个获取类型的方法、一个获取别名的方法。根据名称获取类型、根据名称获取别名。一目了然！
总结：
　　这10个方法，很明显，这是一个典型的工厂模式的工厂接口。
三、可将Bean逐一列出的工厂——ListableBeanFactory
源码：
public interface ListableBeanFactory extends BeanFactory {
    boolean containsBeanDefinition(String beanName);   //  对于给定的名字有多少份BeanDefinition
    int getBeanDefinitionCount();  //  返回工厂的BeanDefinition总数
    String[] getBeanDefinitionNames(); //  返回工厂中所有Bean的名字
    String[] getBeanNamesForType(Class<?> type);   //  返回对于指定类型Bean（包括子类）的所有名字
    /*
     * 返回指定类型的名字
     * includeNonSingletons为false表示只取单例Bean，true则不是
     * allowEagerInit为true表示立刻加载，false表示延迟加载。
     * 注意：FactoryBeans都是立刻加载的。
     */
    String[] getBeanNamesForType(Class<?> type, boolean includeNonSingletons, boolean allowEagerInit);
    <T> Map<String, T> getBeansOfType(Class<T> type) throws BeansException; //根据类型（包括子类）返回指定Bean名和Bean的Map   
    <T> Map<String, T> getBeansOfType(Class<T> type, boolean includeNonSingletons, boolean allowEagerInit)
            throws BeansException;
    Map<String, Object> getBeansWithAnnotation(Class<? extends Annotation> annotationType)
            throws BeansException;   //  根据注解类型，查找所有有这个注解的Bean名和Bean的Map
    <A extends Annotation> A findAnnotationOnBean(String beanName, Class<A> annotationType);//根据指定Bean名和注解类型查找指定的Bean
}
复制代码
 具体：
　　1、3个跟BeanDefinition有关的总体操作。包括BeanDefinition的总数、名字的集合、指定类型的名字的集合。
　　（这里指出，BeanDefinition是Spring中非常重要的一个类，每个BeanDefinition实例都包含一个类在Spring工厂中所有属性。）
　　2、2个getBeanNamesForType重载方法。根据指定类型（包括子类）获取其对应的所有Bean名字。
　　3、2个getBeansOfType重载方法。根据类型（包括子类）返回指定Bean名和Bean的Map。
　　4、2个跟注解查找有关的方法。根据注解类型，查找Bean名和Bean的Map。以及根据指定Bean名和注解类型查找指定的Bean。
总结：
　　正如这个工厂接口的名字所示，这个工厂接口最大的特点就是可以列出工厂可以生产的所有实例。当然，工厂并没有直接提供返回所有实例的方法，也没这个必要。它可以返回指定类型的所有的实例。而且你可以通过getBeanDefinitionNames()得到工厂所有bean的名字，然后根据这些名字得到所有的Bean。这个工厂接口扩展了BeanFactory的功能，作为上文指出的BeanFactory二级接口，有9个独有的方法，扩展了跟BeanDefinition的功能，提供了BeanDefinition、BeanName、注解有关的各种操作。它可以根据条件返回Bean的集合，这就是它名字的由来——ListableBeanFactory。
四、分层的Bean工厂——HierarchicalBeanFactory
源码：
复制代码
public interface HierarchicalBeanFactory extends BeanFactory {
    BeanFactory getParentBeanFactory();    //  返回本Bean工厂的父工厂
    boolean containsLocalBean(String name);    //  本地工厂是否包含这个Bean
}
复制代码
 具体：
　　1、第一个方法返回本Bean工厂的父工厂。这个方法实现了工厂的分层。
　　2、第二个方法判断本地工厂是否包含这个Bean（忽略其他所有父工厂）。这也是分层思想的体现。
总结：这个工厂接口非常简单，实现了Bean工厂的分层。这个工厂接口也是继承自BeanFacotory，也是一个二级接口，相对于父接口，它只扩展了一个重要的功能——工厂分层。
五、自动装配的Bean工厂——AutowireCapableBeanFactory
源码：
复制代码
public interface AutowireCapableBeanFactory extends BeanFactory {
    int AUTOWIRE_NO = 0;   //  这个常量表明工厂没有自动装配的Bean
    int AUTOWIRE_BY_NAME = 1;  //表明根据名称自动装配
    int AUTOWIRE_BY_TYPE = 2;  //表明根据类型自动装配
    int AUTOWIRE_CONSTRUCTOR = 3;  //表明根据构造方法快速装配
    @Deprecated
    int AUTOWIRE_AUTODETECT = 4;   //表明通过Bean的class的内部来自动装配（有没翻译错...）Spring3.0被弃用。
    <T> T createBean(Class<T> beanClass) throws BeansException;    //  根据指定Class创建一个全新的Bean实例
    void autowireBean(Object existingBean) throws BeansException;  //  给定对象，根据注释、后处理器等，进行自动装配
    /*
     * 根据Bean名的BeanDefinition装配这个未加工的Object，执行回调和各种后处理器。
     */
    Object configureBean(Object existingBean, String beanName) throws BeansException;
    /*
     * 分解Bean在工厂中定义的这个指定的依赖descriptor
     */
    Object resolveDependency(DependencyDescriptor descriptor, String beanName) throws BeansException;
    /*
     * 根据给定的类型和指定的装配策略，创建一个新的Bean实例
     */
    Object createBean(Class<?> beanClass, int autowireMode, boolean dependencyCheck) throws BeansException;
    /*
     * 与上面类似，不过稍有不同。
     */
    Object autowire(Class<?> beanClass, int autowireMode, boolean dependencyCheck) throws BeansException;
    /*
     * 根据名称或类型自动装配
     */
    void autowireBeanProperties(Object existingBean, int autowireMode, boolean dependencyCheck)
            throws BeansException;
    /*
     * 也是自动装配
     */
    void applyBeanPropertyValues(Object existingBean, String beanName) throws BeansException;
    /*
     * 初始化一个Bean...
     */
    Object initializeBean(Object existingBean, String beanName) throws BeansException;
    /*
     * 初始化之前执行BeanPostProcessors
     */
    Object applyBeanPostProcessorsBeforeInitialization(Object existingBean, String beanName)
            throws BeansException;
    /*
     * 初始化之后执行BeanPostProcessors
     */
    Object applyBeanPostProcessorsAfterInitialization(Object existingBean, String beanName)
            throws BeansException;
    /*
     * 分解指定的依赖
     */
    Object resolveDependency(DependencyDescriptor descriptor, String beanName,
            Set<String> autowiredBeanNames, TypeConverter typeConverter) throws BeansException;
}
复制代码
 具体：
　　1、总共5个静态不可变常量来指明装配策略，其中一个常量被Spring3.0废弃、一个常量表示没有自动装配，另外3个常量指明不同的装配策略——根据名称、根据类型、根据构造方法。
　　2、8个跟自动装配有关的方法，实在是繁杂，具体的意义我们研究类的时候再分辨吧。
　　3、2个执行BeanPostProcessors的方法。
　　4、2个分解指定依赖的方法
总结：这个工厂接口继承自BeanFacotory，它扩展了自动装配的功能，根据类定义BeanDefinition装配Bean、执行前、后处理器等。
六、复杂的配置Bean工厂——ConfigurableBeanFactory
源码：
复制代码
public interface ConfigurableBeanFactory extends HierarchicalBeanFactory, SingletonBeanRegistry {
    String SCOPE_SINGLETON = "singleton";  //  单例
    String SCOPE_PROTOTYPE = "prototype";  //  原型
    /*
     * 搭配HierarchicalBeanFactory接口的getParentBeanFactory方法
     */
    void setParentBeanFactory(BeanFactory parentBeanFactory) throws IllegalStateException;
    /*
     * 设置、返回工厂的类加载器
     */
    void setBeanClassLoader(ClassLoader beanClassLoader);
    ClassLoader getBeanClassLoader();
    /*
     * 设置、返回一个临时的类加载器
     */
    void setTempClassLoader(ClassLoader tempClassLoader);
    ClassLoader getTempClassLoader();
    /*
     * 设置、是否缓存元数据，如果false，那么每次请求实例，都会从类加载器重新加载（热加载）
     */
    void setCacheBeanMetadata(boolean cacheBeanMetadata);
    boolean isCacheBeanMetadata();//是否缓存元数据
    /*
     * Bean表达式分解器
     */
    void setBeanExpressionResolver(BeanExpressionResolver resolver);
    BeanExpressionResolver getBeanExpressionResolver();
    /*
     * 设置、返回一个转换服务
     */
    void setConversionService(ConversionService conversionService);
    ConversionService getConversionService();
    /*
     * 设置属性编辑登记员...
     */
    void addPropertyEditorRegistrar(PropertyEditorRegistrar registrar);
    /*
     * 注册常用属性编辑器
     */
    void registerCustomEditor(Class<?> requiredType, Class<? extends PropertyEditor> propertyEditorClass);
    /*
     * 用工厂中注册的通用的编辑器初始化指定的属性编辑注册器
     */
    void copyRegisteredEditorsTo(PropertyEditorRegistry registry);
    /*
     * 设置、得到一个类型转换器
     */
    void setTypeConverter(TypeConverter typeConverter);
    TypeConverter getTypeConverter();
    /*
     * 增加一个嵌入式的StringValueResolver
     */
    void addEmbeddedValueResolver(StringValueResolver valueResolver);
    String resolveEmbeddedValue(String value);//分解指定的嵌入式的值
    void addBeanPostProcessor(BeanPostProcessor beanPostProcessor);//设置一个Bean后处理器
    int getBeanPostProcessorCount();//返回Bean后处理器的数量
    void registerScope(String scopeName, Scope scope);//注册范围
    String[] getRegisteredScopeNames();//返回注册的范围名
    Scope getRegisteredScope(String scopeName);//返回指定的范围
    AccessControlContext getAccessControlContext();//返回本工厂的一个安全访问上下文
    void copyConfigurationFrom(ConfigurableBeanFactory otherFactory);//从其他的工厂复制相关的所有配置
    /*
     * 给指定的Bean注册别名
     */
    void registerAlias(String beanName, String alias) throws BeanDefinitionStoreException;
    void resolveAliases(StringValueResolver valueResolver);//根据指定的StringValueResolver移除所有的别名
    /*
     * 返回指定Bean合并后的Bean定义
     */
    BeanDefinition getMergedBeanDefinition(String beanName) throws NoSuchBeanDefinitionException;
    boolean isFactoryBean(String name) throws NoSuchBeanDefinitionException;//判断指定Bean是否为一个工厂Bean
    void setCurrentlyInCreation(String beanName, boolean inCreation);//设置一个Bean是否正在创建
    boolean isCurrentlyInCreation(String beanName);//返回指定Bean是否已经成功创建
    void registerDependentBean(String beanName, String dependentBeanName);//注册一个依赖于指定bean的Bean
    String[] getDependentBeans(String beanName);//返回依赖于指定Bean的所欲Bean名
    String[] getDependenciesForBean(String beanName);//返回指定Bean依赖的所有Bean名
    void destroyBean(String beanName, Object beanInstance);//销毁指定的Bean
    void destroyScopedBean(String beanName);//销毁指定的范围Bean
    void destroySingletons();  //销毁所有的单例类
}
复制代码
在具体介绍之前，先看一下接口SingletonBeanRegistry的源码：
复制代码
public interface SingletonBeanRegistry {
    void registerSingleton(String beanName, Object singletonObject); //在容器内注册一个单例类  
    Object getSingleton(String beanName);//返回给定名称对应的单例类
    boolean containsSingleton(String beanName);//给定名称是否对应单例类
    String[] getSingletonNames();//返回容器内所有单例类的名字
    int getSingletonCount();//返回容器内注册的单例类数量
}
复制代码
 可以看到，SingletonBeanRegistry这个接口非常简单，5个方法，实现了单例类注册的功能。
ConfigurableBeanFactory同时继承了HierarchicalBeanFactory 和 SingletonBeanRegistry 这两个接口，即同时继承了分层和单例类注册的功能。
具体：
　　1、2个静态不可变常量分别代表单例类和原型类。
　　2、1个设置父工厂的方法，跟HierarchicalBeanFactory接口的getParentBeanFactory方法互补。
　　3、4个跟类加载器有关的方法：get/set工厂类加载器和get/set临时类加载器。
　　4、2个设置、是否缓存元数据的方法（热加载开关）。
　　5、11个处理Bean注册、加载等细节的方法，包括：Bean表达式分解器、转换服务、属性编辑登记员、属性编辑器、属性编辑注册器、类型转换器、嵌入式的字符串分解器
　　6、2个处理Bean后处理器的方法。
　　7、3个跟注册范围相关的方法。
　　8、1个返回安全访问上下文的方法、1个从其他的工厂复制相关的所有配置的方法。
　　9、2个跟Bean别名相关的方法、1个返回合并后的Bean定义的方法。
　　10、1个判断是否为工厂Bean的方法、2个跟当前Bean创建时机相关的方法。
　　11、3个跟Bean依赖相关的方法、3个销毁Bean相关的方法。
总结：这个巨大的工厂接口，继承自HierarchicalBeanFactory 和 SingletonBeanRegistry 这两个接口，并额外独有37个方法！！！（看的我都快疯了...）这37个方法包含了工厂创建、注册一个Bean的众多细节。这个工厂名为ConfigurableBeanFactory，真是名不虚传！统计一下此时的ConfigurableBeanFactory的方法数吧。自有的37个方法、HierarchicalBeanFactory的2个方法、SingletonBeanRegistry的5个方法、爷爷接口BeanFactory的10个方法，共有54个方法！虽然方法繁多，还算井井有条！
