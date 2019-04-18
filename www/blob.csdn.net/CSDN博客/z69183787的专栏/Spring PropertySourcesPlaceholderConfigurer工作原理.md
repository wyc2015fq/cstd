# Spring PropertySourcesPlaceholderConfigurer工作原理 - z69183787的专栏 - CSDN博客
2018年05月09日 12:26:37[OkidoGreen](https://me.csdn.net/z69183787)阅读数：521
[https://blog.csdn.net/xczzmn/article/details/77744627](https://blog.csdn.net/xczzmn/article/details/77744627)
# 前言
[spring](http://lib.csdn.net/base/javaee)提供配置解析功能，就是这种：
```
<bean id="dataSource" class="com.alibaba.druid.pool.DruidDataSource" 
        init-method="init" destroy-method="close">
    <property name="url" value="${jdbc.url}"/>
    <property name="username" value="${jdbc.username}"/>
    <property name="password" value="${jdbc.password}"/>
</bean>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 1
- 2
- 3
- 4
- 5
- 6
- 1
- 2
- 3
- 4
- 5
- 6
或者是这种：
```java
@Value("${spring_only}")
private String springOnly;
```
- 1
- 2
- 1
- 2
- 1
- 2
可以很方便的通过配置XML来实现对Classpath下的配置文件的注入。 
在Spring3.1版本之前是通过`PropertyPlaceholderConfigurer`实现的。 
而3.1之后则是通过`PropertySourcesPlaceholderConfigurer` 实现的。
> 
PropertyPlaceholderConfigurer 和 PropertyPlaceholderConfigurer 在使用上并无本质的区别。两者的根本目标是将配置文件生成KV对。 真正的注入工作并不由它们本身执行。
# Spring生命周期
配置可以实现注入， 则必须得遵从Spring生命周期。[Spring Bean的生命周期](http://blog.csdn.net/qyp199312/article/details/60762194)可以参考。
> 
如下图（图片来源：[http://blog.csdn.net/qyp199312/article/details/60762194](http://blog.csdn.net/qyp199312/article/details/60762194)）
![Spring生命周期图一](https://img-blog.csdn.net/20170309205843575?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXlwMTk5MzEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 
途中的`实例化`指的是生成一个[Java](http://lib.csdn.net/base/java)的对象。
## 元素注入时机
元素的注入依赖于 `AutowiredAnnotationBeanPostProcessor#postProcessPropertyValues`[1](https://blog.csdn.net/xczzmn/article/details/77744627#fn:postprocess)。 通过解析了的`PropertySourcesPlaceholderConfigurer` 查询得到元素值。 没有则抛出异常。如下源码:
> 
源码摘自 DefaultListableBeanFactory#doResolveDependency
```
// 获取注解的 value() 值。被写死为 Class<? extends Annotation> valueAnnotationType = Value.class;
// 见类 QualifierAnnotationAutowireCandidateResolver
Object value = getAutowireCandidateResolver().getSuggestedValue(descriptor);
if (value != null) {
    if (value instanceof String) {
        // 通过PropertySourcesPlaceholderConfigurer写入的键值对元素获取元素的值.
        // 方法内注册了多个StringValueResolver，循环查找值。提供者为PropertySourcesPlaceholderConfigurer,因此配置多个解析器的时候是以最后的配置为准的。
        String strVal = resolveEmbeddedValue((String) value);
        BeanDefinition bd = (beanName != null && containsBean(beanName) ? getMergedBeanDefinition(beanName) : null);
        value = evaluateBeanDefinitionString(strVal, bd);
    }
    TypeConverter converter = (typeConverter != null ? typeConverter : getTypeConverter());
    return (descriptor.getField() != null ?
            converter.convertIfNecessary(value, type, descriptor.getField()) :
            converter.convertIfNecessary(value, type, descriptor.getMethodParameter()));
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
元素注入的序列图如下：
> 
在Spring初始化流程中，执行 `AbstractApplicationContext#finishBeanFactoryInitializatin` 方法。 该方法里面发生的主要流程为 `Spring 业务 Bean初始化`。 实际流程跟`Spring Bean`的初始化没有任务区别。 
通过对接口 `InstantiationAwareBeanPostProcessor` 实现类的方法进行执行。 仅此而已
Created with Raphaël 2.1.0AutowiredAnnotationBeanPostProcessorAutowiredAnnotationBeanPostProcessorInjectionMetadataInjectionMetadataInjectedElementInjectedElement这个类是 InstantiationAwareBeanPostProcessor的一个实现类, 用于 @Value和@Autowired注解实际执行方法postProcessPropertyValues调度实际调度InjectedElement子类被注入值的获取来自于DefaultListableBeanFactory将@Value(“${needReplace}”)里面的值替换的来源值,就是PropertySourcesPlaceholderConfigurer生成的StringValueResolver。而Spring原生的Bean是单例的它直接被储存在了AbstractBeanFactory执行Field.set(Object, Object)或者Method.invoke(Object, Object[])
> 
可以看出 `PropertySourcesPlaceholderConfigurer` 或者 `PropertyPlaceholderConfigurer` 仅仅是做了一个配置文件的解析工作，真正的注入并不由它们完成，而是托付给了Spring 的Bean初始化流程。 
之所以这么做可以生效，是因为这两个类实现了 `BeanFactoryPostProcessor` 接口，这个接口的优先级高于后续的Spring Bean。
# 数据来源
## 配置Bean方式
单个配置文件
```xml
<bean id="propertyConfigurer"class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer">
   <property name="location">
     <value>conf/sqlmap/jdbc.properties</value>
   </property>
    <property name="fileEncoding">
      <value>UTF-8</value>
    </property>
</bean>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
多个配置文件
```xml
<bean id="propertyConfigurer"class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer">  
    <property name="locations">
        <list>
            <value>/WEB-INF/mail.properties</value>  
            <value>classpath: conf/sqlmap/jdbc.properties</value>//注意这两种value值的写法
     </list>
    </property>
</bean>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
> 
其中PropertyPlaceholderConfigurer是Spring3.1之前使用的。 
PropertySourcesPlaceholderConfigurer是Spring3.1之后使用的。 
写法都类似
## Spring标签方式
`<context:property-placeholder location="classpath*:/WEB-INF/mail.properties" />`- 1
- 1
- 1
这总方式的原理就是构造一个`PropertySourcesPlaceholderConfigurer`, (3.1之前是`PropertyPlaceholderConfigurer`)
- ContextNamespaceHandler#init
- PropertyPlaceholderBeanDefinitionParser#doParse
触发点为： 
`AbstractApplicationContext#obtainFreshBeanFactory` 。Spring初始化Context的时候读取XML配置（基于XML）， 这个流程优先于`Spring 普通Bean`初始化。配合扫包(`<context:component-scan />`)得到的Bean进而实现对XML里面配置的Bean的载入。
`PropertySourcesPlaceholderConfigurer`本质上是一个`BeanFactoryPostProcessor`。解析XML的流程在`BeanFactoryPostProcessor`之前， 优先将配置文件的路径以及名字通过Setter传入`PropertySourcesPlaceholderConfigurer`。
如上`BeanFactoryPostProcessor`的优先级又优于其余的Bean。因此可以实现在bean初始化之前的注入。
## 引申mybatis数据源配置
通常在配置mybatis的时候，配置 `org.mybatis.spring.mapper.MapperScannerConfigurer`需要使用`<property name="sqlSessionFactoryBeanName" value="sqlSessionFactoryAccount" />` 参数实现SqlSessionFactory的注入。
这是由于`MapperScannerConfigurer` 本质上也是一个 `BeanFactoryPostProcessor`。 而SqlSessionFactory往往只是一个普通的Spring Bean, 它的优先级是低于 `MapperScannerConfigurer` 的， 如果在初始化 `MapperScannerConfigurer` 的时候去寻找SqlSessionFactory, 肯定是会报依赖错误的， 因此之后在后续的流程中实现注入。
# 发生找不到配置的情况
在工作中我们会习惯性的使用多个Spring配置文件， 例如`spring.xml/spring-db.xml/spring-web.xml`灯。 里面就配置多个Spring标签，或者多个 `PropertySourcesPlaceholderConfigurer` 。但是时常会发生找不到配置的情况。
## 为什么`<context:property-placeholder />`优先级更高
因为基于XML配置的Spring， 不管是`<context:property-placeholder />`还是 `<Bean />`标签都依赖于 `NamespaceHandler`去解析。 而`<context:property-placeholder />`在解析完毕之后就已经生成了 `PropertySourcesPlaceholderConfigurer` ， `<Bean />`标签还需要等待后续流程。
## 为什么PropertySourcesPlaceholderConfigurer唯一
> 
这里的”为什么”， 指的不是它为什么这么设计，而是为什么会有这么样的结果。 
另外 `PropertySourcesPlaceholderConfigurer` 并不唯一，只是在对外体现上后续的配置无法去到值，因此看起来是唯一的。
PropertySourcesPlaceholderConfigurer Bean的唯一是由[Java](http://lib.csdn.net/base/java)web特性和Spring本身的设计决定的。
- Servlet的启动，在web.xml中Listener串行单线程启动。
- Spring内置的Bean模式为单例模式。
- Spring在初始化的时候会直接将需要的Bean给初始化成功。
- 启动的先后顺序依赖于它们在 xml 里面配置的上下关系。
不管 `<context:property-placeholder />`或者`PropertySourcesPlaceholderConfigurer`或者`PropertyPlaceholderConfigurer`方式配置的配置解析器，其本质就是得到一个 `BeanFactoryPostProcessor`， 并执行其 `#postProcessBeanFactory(ConfigurableListableBeanFactory)`方法。
其执行的根本目的在于 `PlaceholderConfigurerSupport#doProcessProperties(ConfigurableListableBeanFactory, StringValueResolver )`。
```
protected void doProcessProperties(ConfigurableListableBeanFactory beanFactoryToProcess,
            StringValueResolver valueResolver) {
        // ignore
        ....
        // New in Spring 2.5: resolve placeholders in alias target names and aliases as well.
        beanFactoryToProcess.resolveAliases(valueResolver);
        // New in Spring 3.0: resolve placeholders in embedded values such as annotation attributes.
        // 目的是为了添加解析器
        beanFactoryToProcess.addEmbeddedValueResolver(valueResolver);
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
这里的 `addEmbeddedValueResolver(StringValueResolver)` 是为一个 `LinkedList`添加值。
在取用的时候是优先从链表头开始取用的。 一旦发现无法找到值，直接就抛异常了。这个就对外体现出 `PropertySourcesPlaceholderConfigurer` 的唯一性。 (然而Spring内部还是有多个`PropertySourcesPlaceholderConfigurer`， 只不过除了排列在队首的 `PropertySourcesPlaceholderConfigurer` 之外全都被忽略掉了 )。
# 总结Spring Value注入流程
最后的总结：
配置Spring `@Value("val2Inject")` 方式获取配置文件的属性，需要依赖于在Spring XML里面配置`<context:property-placeholder />` 或者`PropertySourcesPlaceholderConfigurer`Bean来添加配置文件的名称。流程如下：
- Spring Context 的初始化开始
- 读取到`context:property-placeholder`标签或者`PropertySourcesPlaceholderConfigurer`
- 解析并实例化一个`PropertySourcesPlaceholderConfigurer`。同时向其中注入配置文件路径、名称
- `PropertySourcesPlaceholderConfigurer`自身生成多个`StringValueResolver`备用，Bean准备完毕
- Spring在初始化非`BeanFactoryPostProcessor`的Bean的时候，`AutowiredAnnotationBeanPostProcessor` 负责找到Bean内有`@Value`注解的`Field`或者`Method`
- 通过`PropertySourcesPlaceholderConfigurer`寻找合适的`StringValueResolver`并解析得到val值。注入给`@Value`的`Field`或`Method`。(`Method优先`)[2](https://blog.csdn.net/xczzmn/article/details/77744627#fn:autowired)
- Spring的其他流程。
- 这个`AutowiredAnnotationBeanPostProcessor`负责`@Autowired`和`@Value`两个注解的解析。解析`@WebServiceRef`和`@EJB`和`Resource`三个注解的是`CommonAnnotationBeanPostProcessor`[↩](https://blog.csdn.net/xczzmn/article/details/77744627#fnref:postprocess)
- `@Autowired`的注入也在这儿发生 [↩](https://blog.csdn.net/xczzmn/article/details/77744627#fnref:autowired)
