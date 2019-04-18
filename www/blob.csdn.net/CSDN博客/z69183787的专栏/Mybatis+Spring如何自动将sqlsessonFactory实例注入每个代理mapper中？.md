# Mybatis+Spring如何自动将sqlsessonFactory实例注入每个代理mapper中？ - z69183787的专栏 - CSDN博客
2017年10月31日 10:44:06[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1091
参考：
[http://blog.csdn.net/liuxiao723846/article/details/52424802](http://blog.csdn.net/liuxiao723846/article/details/52424802)
[http://www.cnblogs.com/ChenLLang/p/5307590.html](http://www.cnblogs.com/ChenLLang/p/5307590.html)
看到许多网站和自身的关于mybatis+spring 自动注册所有代理mapper类的文章，配置文件不外乎如下：
```
<bean class="org.mybatis.spring.mapper.MapperScannerConfigurer">
        <property name="basePackage" value="com.dianping.lotteryvip.dao.mapper" />
        <!--<property name="sqlSessionFactoryBeanName" value="sqlSessionFactory"/>-->
    </bean>
    <bean id="sqlSessionFactory" class="org.mybatis.spring.SqlSessionFactoryBean">
        <!--dataource-->
        <property name="dataSource" ref="assistantSource"/>
        <!--Mapper files-->
        <property name="mapperLocations" value="classpath*:config/mybatis/local/*.xml" />
        <!--Entity package-->
        <!--<property name="typeAliasesPackage" value="com.dianping.zebra.dao.entity" />-->
        <property name="typeAliasesPackage" value="com.dianping.lotteryvip.dao.entity"/>
        <property name="configLocation" value="classpath:config/mybatis/mybatis-configuration.xml" />
    </bean>
```
一直有个疑问，当项目中仅存在单个数据源的情况时，为何 MappeerScanerConfiguer中可以不指定sqlSession，很多资料上都说会自动注入，但不知道如何实现的。
参考了很多网上的文段加上源代码的观察，这里简单总结一下。
源代码版本 mybatis 3.2.8，mybatis-spring 1.2.2
首先简单描述一下 sqlSessionFactory的实例化过程和具体功能：
根据给定的dataSource及mapper xml配置文件、pojo类路径及mybatis全局配置文件生成对应的 mapper与statement的 配置对象Configuration
存储 mapper代理类中方法及sql 语句的映射关系 MapperStatement
总结一下就是，保存所有的配置Sql与 mapper类的 映射关系，供之后的数据库操作使用。
其次，MapperScannerConfigurer类主要实现将basePackage包下的所有接口类注册到
Configuration#MapperRegister#knowMappers<Class<?>,MapperProxyFactory<T>(Class<?>)>集合中；
 可以看到 它 实现了 这几个spring 自身生命周期中的 接口 
BeanDefinitionRegistryPostProcessor，InitializingBean，ApplicationContextAware，BeanNameAware
重写了 BeanDefinitionRegistryPostProcessor 中的 postProcessBeanDefinitionRegistry 方法，用于在 Spring 的 对象配置池中增加 mapper代理类的 BeanDefinition
官方对 这个 processor 的描述 ，可以注册自己的Bean配置，在BeanFactoryPostProcessor（spring实例化之前） 加入之前。
```
Extension to the standard {@link BeanFactoryPostProcessor} SPI, allowing for
 * the registration of further bean definitions <i>before</i> regular
 * BeanFactoryPostProcessor detection kicks in. In particular,
 * BeanDefinitionRegistryPostProcessor may register further bean definitions
 * which in turn define BeanFactoryPostProcessor instances.
```
使用ClassPathMapperScanner对 basePackage下路径中的 接口和类 进行了扫描
```java
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
    scanner.registerFilters();
    scanner.scan(StringUtils.tokenizeToStringArray(this.basePackage, ConfigurableApplicationContext.CONFIG_LOCATION_DELIMITERS));
```
实际是使用了父类ClassPathBeanDefinitionScanner的 public int scan(String... basePackages)  方法
```java
public int scan(String... basePackages) {
		int beanCountAtScanStart = this.registry.getBeanDefinitionCount();
		doScan(basePackages);
		// Register annotation config processors, if necessary.
		if (this.includeAnnotationConfig) {
			AnnotationConfigUtils.registerAnnotationConfigProcessors(this.registry);
		}
		return (this.registry.getBeanDefinitionCount() - beanCountAtScanStart);
	}
```
代码段中的doScan 又再次使用了  ClassPathMapperScanner 重写的 doScan方法
```java
/**
   * Calls the parent search that will search and register all the candidates.
   * Then the registered objects are post processed to set them as
   * MapperFactoryBeans
   */
  @Override
  public Set<BeanDefinitionHolder> doScan(String... basePackages) {
    Set<BeanDefinitionHolder> beanDefinitions = super.doScan(basePackages);
    if (beanDefinitions.isEmpty()) {
      logger.warn("No MyBatis mapper was found in '" + Arrays.toString(basePackages) + "' package. Please check your configuration.");
    } else {
      for (BeanDefinitionHolder holder : beanDefinitions) {
        GenericBeanDefinition definition = (GenericBeanDefinition) holder.getBeanDefinition();
        if (logger.isDebugEnabled()) {
          logger.debug("Creating MapperFactoryBean with name '" + holder.getBeanName() 
              + "' and '" + definition.getBeanClassName() + "' mapperInterface");
        }
        // the mapper interface is the original class of the bean
        // but, the actual class of the bean is MapperFactoryBean
        definition.getPropertyValues().add("mapperInterface", definition.getBeanClassName());
        definition.setBeanClass(MapperFactoryBean.class);
        definition.getPropertyValues().add("addToConfig", this.addToConfig);
        boolean explicitFactoryUsed = false;
        if (StringUtils.hasText(this.sqlSessionFactoryBeanName)) {
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
        if (!explicitFactoryUsed) {
          if (logger.isDebugEnabled()) {
            logger.debug("Enabling autowire by type for MapperFactoryBean with name '" + holder.getBeanName() + "'.");
          }
          **definition.setAutowireMode(AbstractBeanDefinition.AUTOWIRE_BY_TYPE);**
        }
      }
    }
    return beanDefinitions;
  }
```
红色行 其实 就是解决 本文疑惑的一个关键点，作用是 设置该Bean 的 自动注入类型为 ByType（默认是不注入），猜测一下，应该是 该Bean里有 对应的sqlSessionFactory的 set方法，所以才实例化的时候，会自动根据type注入，这也是为什么说 当只存在一个数据源时，无需指定sqlSessionFactory的原因。
而且红色行之前，也有应对多个数据源，根据sqlSessionFactoryName进行 数据源指定的配置。下文中会具体说明 这个猜测的 缘由。
可以看到这个方法实际上是实现 mapper代理类的关键，父类ClassPathBeanDefinitionScanner中doScan方法扫描basePackage下的所有mapper接口，
依据拿到的 所有 BeanDefinition返回值 Set<BeanDefinitionHolder>，遍历
```java
definition.getPropertyValues().add("mapperInterface", definition.getBeanClassName());
        definition.setBeanClass(MapperFactoryBean.class);
        definition.getPropertyValues().add("addToConfig", this.addToConfig);
```
上述三行设置里 mapper代理类的原始接口类，也设置了 Bean实例化 类型为 MapperFactoryBean。
MapperFactoryBean 实际上是一个 继承了 SqlSessionDaoSupport 并实现了 Spring FactoryBean 接口的 mapper 工厂类
```java
public class MapperFactoryBean<T> extends SqlSessionDaoSupport implements FactoryBean<T> {
```
该类中的关键代码有 ：
```java
private Class<T> mapperInterface;
```
```java
/**
   * {@inheritDoc}
   */
  public T getObject() throws Exception {
    return getSqlSession().getMapper(this.mapperInterface);
  }
  /**
   * {@inheritDoc}
   */
  public Class<T> getObjectType() {
    return this.mapperInterface;
  }
  /**
   * {@inheritDoc}
   */
  public boolean isSingleton() {
    return true;
  }
```
可以看到，他最终保存了 mapper的接口Class，并根据之前 sqlSessionFactory中保存的 mapper与xml与Sql配置关系，生成 mapper接口的代理类。
现在，回过头来说 红色那行代码的含义 ，可以看到MapperFactoryBean  继承了 SqlSessionDaoSupport ，那这个类里有什么呢？没错 就是 sqlSessionFactory
```java
public abstract class SqlSessionDaoSupport extends DaoSupport {
  private SqlSession sqlSession;
  private boolean externalSqlSession;
  public void setSqlSessionFactory(SqlSessionFactory sqlSessionFactory) {
    if (!this.externalSqlSession) {
      this.sqlSession = new SqlSessionTemplate(sqlSessionFactory);
    }
  }
  public void setSqlSessionTemplate(SqlSessionTemplate sqlSessionTemplate) {
    this.sqlSession = sqlSessionTemplate;
    this.externalSqlSession = true;
  }
```
熟悉的 setSqlSessionFactory 方法，结合上面 红色行，就能自圆其说了。 这里，总结一下：
1、sqlSessionFactory 初始化，映射 mapper 接口与Sql配置的关联；
2、根据basePackage 扫描mapper 接口
3、对于2中生成的 BeanDefinition 进行重写，设定实现类型为MapperFacoryBean，将mapperInterface接口设为属性，设置 自动注入模式 为 ByType
4、由于MapperFactoryBean继承了sqlSessionDaoSupport，同时实现了FactoryBean接口，并拥有sqlSessionFactory及set方法，
根据 3 中的 注入模式 ，实例化是 所以最终会生成 mapperInterface的实现类，sqlSessionFactory也会自动注入
接下来，继续说 mapper 代理类 是如何生成的 。。。。。。。。。。。。。。。。。。。。。。。
通过之前的MapperFactoryBean 中的 getObject 层层进入 至 MapperRegistry 类 中的 getMapper方法
```java
@SuppressWarnings("unchecked")
  public <T> T getMapper(Class<T> type, SqlSession sqlSession) {
    final MapperProxyFactory<T> mapperProxyFactory = (MapperProxyFactory<T>) knownMappers.get(type);
    if (mapperProxyFactory == null)
      throw new BindingException("Type " + type + " is not known to the MapperRegistry.");
    try {
      return mapperProxyFactory.newInstance(sqlSession);
    } catch (Exception e) {
      throw new BindingException("Error getting mapper instance. Cause: " + e, e);
    }
  }
```
发现了关键类 MapperProxyFactory，进入 
```java
@SuppressWarnings("unchecked")
  **protected T newInstance(MapperProxy<T> mapperProxy) {    return (T) Proxy.newProxyInstance(mapperInterface.getClassLoader(), new Class[] { mapperInterface }, mapperProxy);  }**
  public T newInstance(SqlSession sqlSession) {
    final MapperProxy<T> mapperProxy = new MapperProxy<T>(sqlSession, mapperInterface, methodCache);
    return newInstance(mapperProxy);
  }
```
发现 MapperProxy 代理类的 原型，该类保存了
```java
private final SqlSession sqlSession;
  private final Class<T> mapperInterface;
  private final Map<Method, MapperMethod> methodCache;
```
sqlSession  接口类，接口中所有映射方法的 cache 缓存（MapperRegistry 中的 addMapper方法 进行 mapper类中放的注册及 Select等注解的 解析）
最终，使用JDK 动态代理，进行代理类的生成  
