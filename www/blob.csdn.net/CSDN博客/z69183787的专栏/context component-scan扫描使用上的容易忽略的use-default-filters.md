# context:component-scan扫描使用上的容易忽略的use-default-filters - z69183787的专栏 - CSDN博客
2014年07月15日 16:41:27[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4840
## 问题
如下方式可以成功扫描到@Controller注解的Bean，不会扫描@Service/@Repository的Bean。正确
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
-  <context:component-scan base-package="org.bdp.system.test.controller">   
-      <context:include-filter type="annotation" expression="org.springframework.stereotype.Controller"/>   
- </context:component-scan>  
但是如下方式，不仅仅扫描@Controller，还扫描@Service/@Repository的Bean，可能造成一些问题
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
-  <context:component-scan base-package="org.bdp">   
-      <context:include-filter type="annotation" expression="org.springframework.stereotype.Controller"/>   
- </context:component-scan>  
这个尤其在springmvc+spring+hibernate等集成时最容易出问题的地，最典型的错误就是：
事务不起作用
这是什么问题呢？
## 分析
1、<context:component-scan>会交给org.springframework.context.config.ContextNamespaceHandler处理；
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- registerBeanDefinitionParser("component-scan", new ComponentScanBeanDefinitionParser());  
2、ComponentScanBeanDefinitionParser会读取配置文件信息并组装成org.springframework.context.annotation.ClassPathBeanDefinitionScanner进行处理；
3、如果没有配置<context:component-scan>的use-default-filters属性，则默认为true，在创建ClassPathBeanDefinitionScanner时会根据use-default-filters是否为true来调用如下代码：
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- protectedvoid registerDefaultFilters() {  
- this.includeFilters.add(new AnnotationTypeFilter(Component.class));  
- ClassLoader cl = ClassPathScanningCandidateComponentProvider.class.getClassLoader();  
- try {  
- this.includeFilters.add(new AnnotationTypeFilter(  
-             ((Class<? extends Annotation>) cl.loadClass("javax.annotation.ManagedBean")), false));  
-     logger.info("JSR-250 'javax.annotation.ManagedBean' found and supported for component scanning");  
- }  
- catch (ClassNotFoundException ex) {  
- // JSR-250 1.1 API (as included in Java EE 6) not available - simply skip.
- }  
- try {  
- this.includeFilters.add(new AnnotationTypeFilter(  
-             ((Class<? extends Annotation>) cl.loadClass("javax.inject.Named")), false));  
-     logger.info("JSR-330 'javax.inject.Named' annotation found and supported for component scanning");  
- }  
- catch (ClassNotFoundException ex) {  
- // JSR-330 API not available - simply skip.
- }  
//@Service和@Controller都是Component，因为这些注解都添加了@Component注解
可以看到默认ClassPathBeanDefinitionScanner会自动注册对@Component、@ManagedBean、@Named注解的Bean进行扫描。如果细心，到此我们就找到问题根源了。
4、在进行扫描时会通过include-filter/exclude-filter来判断你的Bean类是否是合法的：
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- protectedboolean isCandidateComponent(MetadataReader metadataReader) throws IOException {  
- for (TypeFilter tf : this.excludeFilters) {  
- if (tf.match(metadataReader, this.metadataReaderFactory)) {  
- returnfalse;  
-         }  
-     }  
- for (TypeFilter tf : this.includeFilters) {  
- if (tf.match(metadataReader, this.metadataReaderFactory)) {  
-             AnnotationMetadata metadata = metadataReader.getAnnotationMetadata();  
- if (!metadata.isAnnotated(Profile.class.getName())) {  
- returntrue;  
-             }  
-             AnnotationAttributes profile = MetadataUtils.attributesFor(metadata, Profile.class);  
- returnthis.environment.acceptsProfiles(profile.getStringArray("value"));  
-         }  
-     }  
- returnfalse;  
- }  
默认扫描指定包下的全部 @Component, exclude-filter 指定的不扫描，include-filter指定的扫描， include-filter和exclude-filter 没有指定的仍然扫描。对；
指定了use-default-filters=“false” 是这样的
首先通过exclude-filter 进行黑名单过滤；
然后通过include-filter 进行白名单过滤；
否则默认排除
## 结论
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- <context:component-scan base-package="org.bdp">   
-      <context:include-filter type="annotation" expression="org.springframework.stereotype.Controller"/>   
- </context:component-scan>  
为什么这段代码不仅仅扫描@Controller注解的Bean，而且还扫描了@Component的子注解@Service、@Reposity。因为use-default-filters默认为true。所以如果不需要默认的，则use-default-filters=“false”禁用掉。
## 请参考
[《SpringMVC + spring3.1.1 + hibernate4.1.0 集成及常见问题总结》](http://jinnianshilongnian.iteye.com/blog/1423971)
[《第三章 DispatcherServlet详解 ——跟开涛学SpringMVC》](http://jinnianshilongnian.iteye.com/blog/1602617)中的ContextLoaderListener初始化的上下文和DispatcherServlet初始化的上下文关系。
如果在springmvc配置文件，不使用cn.javass.demo.web.controller前缀，而是使用cn.javass.demo，则service、dao层的bean可能也重新加载了，但事务的AOP代理没有配置在springmvc配置文件中，从而造成新加载的bean覆盖了老的bean，造成事务失效。只要使用use-default-filters=“false”禁用掉默认的行为就可以了。
问题不难，spring使用上的问题。总结一下方便再遇到类似问题的朋友参考。
