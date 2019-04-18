# Mybatis MapperScannerConfigurer 自动扫描 将Mapper接口生成代理注入到Spring - z69183787的专栏 - CSDN博客
2017年10月30日 18:22:41[OkidoGreen](https://me.csdn.net/z69183787)阅读数：384
Mybatis MapperScannerConfigurer 自动扫描 将Mapper接口生成代理注入到Spring
Mybatis在与Spring集成的时候可以配置MapperFactoryBean来生成Mapper接口的代理.
 例如
```
<bean id="userMapper" class="org.mybatis.spring.mapper.MapperFactoryBean">
  <property name="mapperInterface" value="org.mybatis.spring.sample.mapper.UserMapper" />
  <property name="sqlSessionFactory" ref="sqlSessionFactory" />
</bean>
```
MapperFactoryBean 创建的代理类实现了 UserMapper 接口,并且注入到应用程序中。 因为代理创建在运行时环境中(Runtime,译者注) ,那么指定的映射器必须是一个接口,而 不是一个具体的实现类。
上面的配置有一个很大的缺点，就是系统有很多的配置文件时 全部需要手动编写，所以上述的方式已经很用了。
没有必要在 Spring 的 XML 配置文件中注册所有的映射器。相反,你可以使用一个 MapperScannerConfigurer , 它 将 会 查 找 类 路 径 下 的 映 射 器 并 自 动 将 它 们 创 建 成 MapperFactoryBean。
要创建 MapperScannerConfigurer,可以在 Spring 的配置中添加如下代码:
```
<bean class="org.mybatis.spring.mapper.MapperScannerConfigurer">
  <property name="basePackage" value="org.mybatis.spring.sample.mapper" />
</bean>
```
basePackage 属性是让你为映射器接口文件设置基本的包路径。 你可以使用分号或逗号 作为分隔符设置多于一个的包路径。每个映射器将会在指定的包路径中递归地被搜索到。
注 意 , 没 有 必 要 去 指 定 SqlSessionFactory 或 SqlSessionTemplate , 因 为 MapperScannerConfigurer 将会创建 MapperFactoryBean,之后自动装配。但是,如果你使 用了一个 以上的 DataSource
 ,那 么自动 装配可 能会失效 。这种 情况下 ,你可 以使用 sqlSessionFactoryBeanName 或 sqlSessionTemplateBeanName 属性来设置正确的 bean 名 称来使用。这就是它如何来配置的,注意 bean 的名称是必须的,而不是 bean 的引用,因 此,value 属性在这里替代通常的 ref:
```
<property name="sqlSessionFactoryBeanName" value="sqlSessionFactory" />
```
MapperScannerConfigurer 支 持 过 滤 由 指 定 的 创 建 接 口 或 注 解 创 建 映 射 器 。 annotationClass 属性指定了要寻找的注解名称。 markerInterface 属性指定了要寻找的父 接口。如果两者都被指定了,加入到接口中的映射器会匹配两种标准。默认情况下,这两个
 属性都是 null,所以在基包中给定的所有接口可以作为映射器加载。
被发现的映射器将会使用 Spring 对自动侦测组件(参考 Spring 手册的 3.14.4)默认的命 名策略来命名。也就是说,如果没有发现注解,它就会使用映射器的非大写的非完全限定类 名。但是如果发现了@Component 或 JSR-330 的@Named 注解,它会获取名称。注意你可以
 配 置 到 org.springframework.stereotype.Component , javax.inject.Named(如果你使用 JSE 6 的话)或你自己的注解(肯定是自我注解)中,这 样注解将会用作生成器和名称提供器。
接下来让我们看一下MapperScannerConfigurer类的源码 看看是如何自动扫描的。
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 public void postProcessBeanDefinitionRegistry(BeanDefinitionRegistry registry) throws BeansException {
 2     if (this.processPropertyPlaceHolders) {
 3       processPropertyPlaceHolders();
 4     }
 5 
 6     ClassPathMapperScanner scanner = new ClassPathMapperScanner(registry);
 7     scanner.setAddToConfig(this.addToConfig);
 8     scanner.setAnnotationClass(this.annotationClass);
 9     scanner.setMarkerInterface(this.markerInterface);
10     scanner.setSqlSessionFactory(this.sqlSessionFactory);
11     scanner.setSqlSessionTemplate(this.sqlSessionTemplate);
12     scanner.setSqlSessionFactoryBeanName(this.sqlSessionFactoryBeanName);
13     scanner.setSqlSessionTemplateBeanName(this.sqlSessionTemplateBeanName);
14     scanner.setResourceLoader(this.applicationContext);
15     scanner.setBeanNameGenerator(this.nameGenerator);
16     scanner.registerFilters();
17     scanner.scan(StringUtils.tokenizeToStringArray(this.basePackage, ConfigurableApplicationContext.CONFIG_LOCATION_DELIMITERS));
18   }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
把Mapper接口转换成MapperFactoryBean的代码在地17行这个方法里，让我们跟踪进去看一下。
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1   @Override
 2   public Set<BeanDefinitionHolder> doScan(String... basePackages) {
 3     Set<BeanDefinitionHolder> beanDefinitions = super.doScan(basePackages);
 4 
 5     if (beanDefinitions.isEmpty()) {
 6       logger.warn("No MyBatis mapper was found in '" + Arrays.toString(basePackages) + "' package. Please check your configuration.");
 7     } else {
 8       for (BeanDefinitionHolder holder : beanDefinitions) {
 9         GenericBeanDefinition definition = (GenericBeanDefinition) holder.getBeanDefinition();
10 
11         if (logger.isDebugEnabled()) {
12           logger.debug("Creating MapperFactoryBean with name '" + holder.getBeanName() 
13               + "' and '" + definition.getBeanClassName() + "' mapperInterface");
14         }
15 
16         // the mapper interface is the original class of the bean
17         // but, the actual class of the bean is MapperFactoryBean
18         //把接口的类型设置进去
19         definition.getPropertyValues().add("mapperInterface", definition.getBeanClassName());
20         //设置Bean的真实类型MapperFactoryBean 
21         definition.setBeanClass(MapperFactoryBean.class);
22         //是否把Mapper接口加入到Mybatis的Config当中去
23         definition.getPropertyValues().add("addToConfig", this.addToConfig);
24 
25         boolean explicitFactoryUsed = false;
26         //如果sqlSessionFactoryBeanName的名字不为空 则在Spring容器中查询
27         //适合多数据源
28         if (StringUtils.hasText(this.sqlSessionFactoryBeanName)) {
29           definition.getPropertyValues().add("sqlSessionFactory", new RuntimeBeanReference(this.sqlSessionFactoryBeanName));
30           explicitFactoryUsed = true;
31         } else if (this.sqlSessionFactory != null) {
32           definition.getPropertyValues().add("sqlSessionFactory", this.sqlSessionFactory);
33           explicitFactoryUsed = true;
34         }
35 
36         //如果sqlSessionTemplateBeanName的名字不为空 则在Spring容器中查询
37         //适合多数据源
38         if (StringUtils.ha
39         if (StringUtils.hasText(this.sqlSessionTemplateBeanName)) {
40           if (explicitFactoryUsed) {
41             logger.warn("Cannot use both: sqlSessionTemplate and sqlSessionFactory together. sqlSessionFactory is ignored.");
42           }
43           definition.getPropertyValues().add("sqlSessionTemplate", new RuntimeBeanReference(this.sqlSessionTemplateBeanName));
44           explicitFactoryUsed = true;
45         } else if (this.sqlSessionTemplate != null) {
46           if (explicitFactoryUsed) {
47             logger.warn("Cannot use both: sqlSessionTemplate and sqlSessionFactory together. sqlSessionFactory is ignored.");
48           }
49           definition.getPropertyValues().add("sqlSessionTemplate", this.sqlSessionTemplate);
50           explicitFactoryUsed = true;
51         }
52 
53         if (!explicitFactoryUsed) {
54           if (logger.isDebugEnabled()) {
55             logger.debug("Enabling autowire by type for MapperFactoryBean with name '" + holder.getBeanName() + "'.");
56           }
57           definition.setAutowireMode(AbstractBeanDefinition.AUTOWIRE_BY_TYPE);
58         }
59       }
60     }
61     //这个集合返回以后 Spring容器会将里面的所有内容注册到容器中
62     return beanDefinitions;
63   }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
