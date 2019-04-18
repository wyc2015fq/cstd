# Spring mybatis源码篇章-sql mapper配置文件绑定mapper class类 - z69183787的专栏 - CSDN博客
2017年10月31日 14:29:59[OkidoGreen](https://me.csdn.net/z69183787)阅读数：514
[http://www.cnblogs.com/question-sky/p/6654101.html](http://www.cnblogs.com/question-sky/p/6654101.html)
### 背景知识
- MappedStatement是mybatis操作sql语句的持久层对象，其id由注解模式的`${mapperInterface类全名}.${methodName}`或者XML模式的`${namespace}.${CRUD标签的id}`确定，且是唯一的
- Mybatis对每个CRUD语句都会生成唯一的MappedStatement对象保存至Configuration的`mappedStatements`Map集合中
- Mybatis提供注解模式和XML模式生成MappedStatement，在两者同时存在的情况下，注解模式的MappedStatement会覆盖同id的XML模式的MappedStatement
- 针对非注解模式即XML模式的生成MappedStatement，还必须拥有对应的mapperInterface接口供Service层调用，即mapperInterface接口是需要注册到Configuration的`MapperRegistry`类中，方便Service层找寻调用（**这也是本文的重点讲述**）
- 注解模式生成MappedStatement的途径有两个，一个是在其同目录下存在与类名一致的sql mapper文件；另一个是其方法名实现了CRUD的注解，其中注解与sql mapper存在同id，遵循`第3点`
### 本章主题
前文介绍到`XMLMapperBuilder`是如何通过扫描SQL
 Mapper配置文件中的标签后组装成MappedStament对象，本章则针对**背景知识**中的第四点来展开
```
如何绑定mapper
 sql配置文件到interface
```
### Spring Mybatis 接口注入老配置
```
<bean id="userMapper" class="org.mybatis.spring.mapper.MapperFactoryBean">
    <property name="mapperInterface" value="com.test.sqlmapper.UserMapper"/>
    <property name="sqlSessionFactory" ref="sqlSessionFactory" />
</bean>
```
上述的配置是针对单个的mapperInterface注入到应用程序中，试想如果有很多的接口则会导致Spring 主配置文件臃肿，所以上述的办法已过时
### Spring Mybatis 接口注入新配置
```
<bean class="org.mybatis.spring.mapper.MapperScannerConfigurer">
      <property name="basePackage" value="org.mybatis.spring.sample.mapper" />
      <!-- optional unless there are multiple session factories defined -->
      <property name="sqlSessionFactoryBeanName" value="sqlSessionFactory" />
  </bean>
```
采用`MapperScannerConfigurer`扫描类来实现对`basePackage`指定的路径进行接口的全部注入，节省了之前老配置很多的代码空间。
- 
直接查看`MapperScannerConfigurer`类的源码了解其中的来龙去脉，内部属性如下：
```
public class MapperScannerConfigurer implements BeanDefinitionRegistryPostProcessor, InitializingBean, ApplicationContextAware, BeanNameAware {
  private String basePackage;
  private boolean addToConfig = true;
  private SqlSessionFactory sqlSessionFactory;
  private SqlSessionTemplate sqlSessionTemplate;
  private String sqlSessionFactoryBeanName;
  private String sqlSessionTemplateBeanName;
  private Class<? extends Annotation> annotationClass;
  private Class<?> markerInterface;
  private ApplicationContext applicationContext;
  private String beanName;
  private boolean processPropertyPlaceHolders;
  private BeanNameGenerator nameGenerator;
  ....
 }
```
> 
其源码上的注释其实写的很清楚了，注释篇幅过长，就不在这里展示了，稍微提下这个类的相关使用：
- `basePackage` 基本属性，接口类扫描的包路径，支持,;分隔
- `sqlSessionFactoryBeanName` 当有多个SqlSessionFactory环境时，官方通过其来指定加载特定的sqlSessionFactory，value即为bean的id值。与`sqlSessionFactory`属性的区别在于可以在使用mybatis的时候才会去调用sqlSessionFactory实例，建议使用此属性
- `sqlSessionFactoty` 默认是不用填的，其会去寻找id为`sqlSessionFactory`的sqlSessionFactory实例，`sqlSessionTemplate`的操作与其是一致的
- `annotationClass` 注解类，其会去寻找拥有此注解的接口类，并忽略`basePackage`的属性，默认为null
- `markerInterface` 父类接口类，其会去寻找继承此接口类的子接口类并不包括其父类接口，并忽略`basePackage`的属性，与`annotationClass`并存，默认为null
- 
具体调用接口方法
```
MapperScannerConfigurer#postProcessBeanDefinitionRegistry(BeanDefinitionRegistry
 registry)
```
```cpp
//支持${basePackage}形式
if (this.processPropertyPlaceHolders) {
  processPropertyPlaceHolders();
}
//容易知道其是根据classpath路径来寻找资源的
ClassPathMapperScanner scanner = new ClassPathMapperScanner(registry);
scanner.setAddToConfig(this.addToConfig);
scanner.setAnnotationClass(this.annotationClass);
scanner.setMarkerInterface(this.markerInterface);
scanner.setSqlSessionFactory(this.sqlSessionFactory);
scanner.setSqlSessionTemplate(this.sqlSessionTemplate);
scanner.setSqlSessionFactoryBeanName(this.sqlSessionFactoryBeanName);
scanner.setSqlSessionTemplateBeanName(this.sqlSessionTemplateBeanName);
scanner.setResourceLoader(this.applicationContext);
scanner.setBeanNameGenerator(this.nameGenerator);
//使用过滤器，主要是annotationClass和markerInterface过滤器
scanner.registerFilters();
//扫描指定的basePackage
scanner.scan(StringUtils.tokenizeToStringArray(this.basePackage, ConfigurableApplicationContext.CONFIG_LOCATION_DELIMITERS));
```
查看`ClassPathMapperScanner`的
```
doScan(String..
 packages)
```
方法：
```
//由父类去找到符合条件的interface类，并转化为bean类
Set<BeanDefinitionHolder> beanDefinitions = super.doScan(basePackages);
if (beanDefinitions.isEmpty()) {
  logger.warn("No MyBatis mapper was found in '" + Arrays.toString(basePackages) + "' package. Please check your configuration.");
} else {
//处理找到的interface bean类
  processBeanDefinitions(beanDefinitions);
}
return beanDefinitions;
```
转而看
```
ClassPathMapperScanner#processBeanDefinitions(Set<BeanDefinitionHolder>
 beanDefinitions)
```
方法
```
GenericBeanDefinition definition;
for (BeanDefinitionHolder holder : beanDefinitions) {
  definition = (GenericBeanDefinition) holder.getBeanDefinition();
  if (logger.isDebugEnabled()) {
    logger.debug("Creating MapperFactoryBean with name '" + holder.getBeanName() 
      + "' and '" + definition.getBeanClassName() + "' mapperInterface");
  }
  // the mapper interface is the original class of the bean
  // but, the actual class of the bean is MapperFactoryBean       
  definition.getConstructorArgumentValues().addGenericArgumentValue(definition.getBeanClassName()); // issue #59
  //最终将definition包装成MapperFactoryBean，beanClass设置为其内部属性MapperInterface
  definition.getPropertyValues().add("mapperInterface", definition.getBeanClassName());
  definition.setBeanClass(MapperFactoryBean.class);
  definition.getPropertyValues().add("addToConfig", this.addToConfig);
  boolean explicitFactoryUsed = false;
  if (StringUtils.hasText(this.sqlSessionFactoryBeanName)) {
      //根据sqlsessionFactoryBeanName寻找运行状态的SqlsessionFactory的虚引用，但并没有去真实加载
    definition.getPropertyValues().add("sqlSessionFactory", new RuntimeBeanReference(this.sqlSessionFactoryBeanName));
    explicitFactoryUsed = true;
  } else if (this.sqlSessionFactory != null) {
    definition.getPropertyValues().add("sqlSessionFactory", this.sqlSessionFactory);
    explicitFactoryUsed = true;
  }
  if (StringUtils.hasText(this.sqlSessionTemplateBeanName)) {
    if (explicitFactoryUsed) {
      logger.warn("Cannot use both: sqlSessionTemplate and sqlSessionFactory together. sqlSessionFactory is ignored.");
    }
    definition.getPropertyValues().add("sqlSessionTemplate", new RuntimeBeanReference(this.sqlSessionTemplateBeanName));
    explicitFactoryUsed = true;
  } else if (this.sqlSessionTemplate != null) {
    if (explicitFactoryUsed) {
      logger.warn("Cannot use both: sqlSessionTemplate and sqlSessionFactory together. sqlSessionFactory is ignored.");
    }
    definition.getPropertyValues().add("sqlSessionTemplate", this.sqlSessionTemplate);
    explicitFactoryUsed = true;
  }
//当没有指定SqlSession对象，则需要@Autowired注解去注入
  if (!explicitFactoryUsed) {
    if (logger.isDebugEnabled()) {
      logger.debug("Enabling autowire by type for MapperFactoryBean with name '" + holder.getBeanName() + "'.");
    }
    definition.setAutowireMode(AbstractBeanDefinition.AUTOWIRE_BY_TYPE);
  }
}
```
接着看`MapperFactoryBean`的实现代码，主要关注其的`checkDaoConfig()`方法：
```java
super.checkDaoConfig();
notNull(this.mapperInterface, "Property 'mapperInterface' is required");
Configuration configuration = getSqlSession().getConfiguration();
//我们看到了熟悉的configuration.addMapper，也就是会走到MapperRegistry#addMapper，详情可见
//Spring mybatis源码篇章-MybatisDAO文件解析(一)
if (this.addToConfig && !configuration.hasMapper(this.mapperInterface)) {
  try {
  //这里顺便提一下，此方法的最主要目的是保存至MapperRegistry#knowMappers集合中，方便service层找寻调用
    configuration.addMapper(this.mapperInterface);
  } catch (Exception e) {
    logger.error("Error while adding the mapper '" + this.mapperInterface + "' to configuration.", e);
    throw new IllegalArgumentException(e);
  } finally {
    ErrorContext.instance().reset();
  }
}
```
### 总结
> - 
`MapperScannerConfigurer`类主要实现将`basePackage`包下的所有接口类注册到`Configuration#MapperRegister#knowMappers<Class<?>,MapperProxyFactory<T>(Class<?>)>`集合中
- 
`MapperScannerConfigurer`类默认情况下在形成`MappedStatement`的过程中会优先去找寻与接口同目录下的XML文件来加载生成，如果想应用XML配置文件且可以任意放置，则可以结合`sqlSessionFactoryBean`的`mapperLocations`属性来完成自由化绑定的过程，同样本文就是建立在此属性已指定的基础上的
- MappedStatement对象的生成与MapperInterfaces接口类是一一对应的
- MapperInterfaces接口类可通过注解例如`@Select`方式生成注解，即脱离XML配置方式
- MapperInterfaces接口类如果在SqlsessionFactory不使用`mapperLocations`属性时且不使用注解方式，则必须在其同目录下有同名字的XML
 mapper文件，否则无法访问数据库；反之使用`mapperLocations`属性且不使用注解方式，则XML
 mapper文件只需放在classpath路径下即可
