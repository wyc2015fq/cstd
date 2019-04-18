# Dubbo之——改造Dubbo，使其能够兼容Spring 4注解配置 - z69183787的专栏 - CSDN博客
2017年12月21日 14:45:37[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2323
[http://blog.csdn.net/l1028386804/article/details/70040928](http://blog.csdn.net/l1028386804/article/details/70040928)
[转载请注明出处：http://blog.csdn.net/l1028386804/article/details/70040928](http://blog.csdn.net/l1028386804/article/details/70040928)
Dubbo本身就是基于Spring环境的，但是Dubbo当年Spring才2.版本。 而现如今Spring 已经发展到4.3.，今年（2017）应该会发布Spring 5。
而随着Spring Boot的大热，Java-Base方式配置Spring也变得越来越流行。
Dubbo + Boot的开发模式，也是较为常见的组合方式。
但是，当使用Dubbo在高版本Spring环境中使用注解方式配置时，会因为一些代码版本的原因导致整合出现问题。
### 1. Dubbo原生的注解配置
Dubbo本身就是基于Spring的，而且原生就提供注解配置：
服务提供方配置：
**[html]**[view
 plain](http://blog.csdn.net/l1028386804/article/details/70040928#)[copy](http://blog.csdn.net/l1028386804/article/details/70040928#)
- <!-- 扫描注解包路径，多个包用逗号分隔，不填pacakge表示扫描当前ApplicationContext中所有的类 -->
- <dubbo:annotationpackage="com.foo.bar.service"/>
服务提供方注解：
**[java]**[view
 plain](http://blog.csdn.net/l1028386804/article/details/70040928#)[copy](http://blog.csdn.net/l1028386804/article/details/70040928#)
- import com.alibaba.dubbo.config.annotation.Service;  
- 
- @Service(version="1.0.0")  
- publicclass FooServiceImpl implements FooService {  
- // ......
- }  
服务消费方注解：
**[java]**[view
 plain](http://blog.csdn.net/l1028386804/article/details/70040928#)[copy](http://blog.csdn.net/l1028386804/article/details/70040928#)
- import com.alibaba.dubbo.config.annotation.Reference;  
- import org.springframework.stereotype.Component;  
- 
- @Component
- publicclass BarAction {  
- 
- @Reference(version="1.0.0")  
- private FooService fooService;  
- 
- }  
服务消费方配置：
**[html]**[view
 plain](http://blog.csdn.net/l1028386804/article/details/70040928#)[copy](http://blog.csdn.net/l1028386804/article/details/70040928#)
- <!-- 扫描注解包路径，多个包用逗号分隔，不填pacakge表示扫描当前ApplicationContext中所有的类 -->
- <dubbo:annotationpackage="com.foo.bar.action"/>
通过官方的例子，就可以看出Dubbo使用xml配置<dubbo:annotation /> 来开启注解配置，并提供 com.alibaba.dubbo.config.annotation.Service注解进行服务注册，提供com.alibaba.dubbo.config.annotation.Reference注解进行服务注入。
### 2.实现机制
可以看出，内部机制都是依托于<dubbo:annotation />标签。 通过源码分析，Dubbo对于Spring xml解析处理由com.alibaba.dubbo.config.spring.schema.DubboNamespaceHandler提供：
DubboNamespaceHandler.java
**[java]**[view
 plain](http://blog.csdn.net/l1028386804/article/details/70040928#)[copy](http://blog.csdn.net/l1028386804/article/details/70040928#)
- package com.alibaba.dubbo.config.spring.schema;  
- 
- import org.springframework.beans.factory.xml.NamespaceHandlerSupport;  
- 
- import com.alibaba.dubbo.common.Version;  
- import com.alibaba.dubbo.config.ApplicationConfig;  
- import com.alibaba.dubbo.config.ConsumerConfig;  
- import com.alibaba.dubbo.config.ModuleConfig;  
- import com.alibaba.dubbo.config.MonitorConfig;  
- import com.alibaba.dubbo.config.ProtocolConfig;  
- import com.alibaba.dubbo.config.ProviderConfig;  
- import com.alibaba.dubbo.config.RegistryConfig;  
- import com.alibaba.dubbo.config.spring.AnnotationBean;  
- import com.alibaba.dubbo.config.spring.ReferenceBean;  
- import com.alibaba.dubbo.config.spring.ServiceBean;  
- 
- /**
-  * DubboNamespaceHandler
-  * 
-  * @author william.liangf
-  * @export
-  */
- publicclass DubboNamespaceHandler extends NamespaceHandlerSupport {  
- 
- static {  
-         Version.checkDuplicate(DubboNamespaceHandler.class);  
-     }  
- 
- publicvoid init() {  
-         registerBeanDefinitionParser("application", new DubboBeanDefinitionParser(ApplicationConfig.class, true));  
-         registerBeanDefinitionParser("module", new DubboBeanDefinitionParser(ModuleConfig.class, true));  
-         registerBeanDefinitionParser("registry", new DubboBeanDefinitionParser(RegistryConfig.class, true));  
-         registerBeanDefinitionParser("monitor", new DubboBeanDefinitionParser(MonitorConfig.class, true));  
-         registerBeanDefinitionParser("provider", new DubboBeanDefinitionParser(ProviderConfig.class, true));  
-         registerBeanDefinitionParser("consumer", new DubboBeanDefinitionParser(ConsumerConfig.class, true));  
-         registerBeanDefinitionParser("protocol", new DubboBeanDefinitionParser(ProtocolConfig.class, true));  
-         registerBeanDefinitionParser("service", new DubboBeanDefinitionParser(ServiceBean.class, true));  
-         registerBeanDefinitionParser("reference", new DubboBeanDefinitionParser(ReferenceBean.class, false));  
-         registerBeanDefinitionParser("annotation", new DubboBeanDefinitionParser(AnnotationBean.class, true));  
-     }  
- 
- }  
通过上面的代码可以很直观的发现，<dubbo:annotation />标签实际是由com.alibaba.dubbo.config.spring.schema.DubboBeanDefinitionParser解析：
DubboBeanDefinitionParser.java
**[java]**[view
 plain](http://blog.csdn.net/l1028386804/article/details/70040928#)[copy](http://blog.csdn.net/l1028386804/article/details/70040928#)
- /**
-  * AbstractBeanDefinitionParser
-  * 
-  * @author william.liangf
-  * @export
-  */
- publicclass DubboBeanDefinitionParser implements BeanDefinitionParser {  
- 
- privatestaticfinal Logger logger = LoggerFactory.getLogger(DubboBeanDefinitionParser.class);  
- 
- privatefinal Class<?> beanClass;  
- 
- privatefinalboolean required;  
- 
- public DubboBeanDefinitionParser(Class<?> beanClass, boolean required) {  
- this.beanClass = beanClass;  
- this.required = required;  
-     }  
- 
- public BeanDefinition parse(Element element, ParserContext parserContext) {  
- return parse(element, parserContext, beanClass, required);  
-     }  
- 
- @SuppressWarnings("unchecked")  
- privatestatic BeanDefinition parse(Element element, ParserContext parserContext, Class<?> beanClass, boolean required) {  
- //略
-     }  
可以看到这个类实现了Spring的org.springframework.beans.factory.xml.BeanDefinitionParser接口，从而完成Spring Bean的解析工作。
而registerBeanDefinitionParser("annotation", new DubboBeanDefinitionParser(AnnotationBean.class, true));就是将<dubbo:annotation />标签，解析成com.alibaba.dubbo.config.spring.AnnotationBean并注册到Spring中。
### 3.AnnotationBean分析
先来看看源码：
AnnotationBean.java
**[java]**[view
 plain](http://blog.csdn.net/l1028386804/article/details/70040928#)[copy](http://blog.csdn.net/l1028386804/article/details/70040928#)
- package com.alibaba.dubbo.config.spring;  
- 
- 
- 
- /**
-  * AnnotationBean
-  * 
-  * @author william.liangf
-  * @export
-  */
- publicclass AnnotationBean extends AbstractConfig implements DisposableBean, BeanFactoryPostProcessor, BeanPostProcessor, ApplicationContextAware {  
- 
- privatestaticfinallong serialVersionUID = -7582802454287589552L;  
- 
- privatestaticfinal Logger logger = LoggerFactory.getLogger(Logger.class);  
- 
- private String annotationPackage;  
- 
- private String[] annotationPackages;  
- 
- privatefinal Set<ServiceConfig<?>> serviceConfigs = new ConcurrentHashSet<ServiceConfig<?>>();  
- 
- privatefinal ConcurrentMap<String, ReferenceBean<?>> referenceConfigs = new ConcurrentHashMap<String, ReferenceBean<?>>();  
- 
- public String getPackage() {  
- return annotationPackage;  
-     }  
- 
- publicvoid setPackage(String annotationPackage) {  
- this.annotationPackage = annotationPackage;  
- this.annotationPackages = (annotationPackage == null || annotationPackage.length() == 0) ? null
-                 : Constants.COMMA_SPLIT_PATTERN.split(annotationPackage);  
-     }  
- 
- private ApplicationContext applicationContext;  
- 
- publicvoid setApplicationContext(ApplicationContext applicationContext) throws BeansException {  
- this.applicationContext = applicationContext;  
-     }  
- 
- publicvoid postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory)  
- throws BeansException {  
- if (annotationPackage == null || annotationPackage.length() == 0) {  
- return;  
-         }  
- if (beanFactory instanceof BeanDefinitionRegistry) {  
- try {  
- // init scanner
-                 Class<?> scannerClass = ReflectUtils.forName("org.springframework.context.annotation.ClassPathBeanDefinitionScanner");  
-                 Object scanner = scannerClass.getConstructor(new Class<?>[] {BeanDefinitionRegistry.class, boolean.class}).newInstance(new Object[] {(BeanDefinitionRegistry) beanFactory, true});  
- // add filter
-                 Class<?> filterClass = ReflectUtils.forName("org.springframework.core.type.filter.AnnotationTypeFilter");  
-                 Object filter = filterClass.getConstructor(Class.class).newInstance(Service.class);  
-                 Method addIncludeFilter = scannerClass.getMethod("addIncludeFilter", ReflectUtils.forName("org.springframework.core.type.filter.TypeFilter"));  
-                 addIncludeFilter.invoke(scanner, filter);  
- // scan packages
-                 String[] packages = Constants.COMMA_SPLIT_PATTERN.split(annotationPackage);  
-                 Method scan = scannerClass.getMethod("scan", new Class<?>[]{String[].class});  
-                 scan.invoke(scanner, new Object[] {packages});  
-             } catch (Throwable e) {  
- // spring 2.0
-             }  
-         }  
-     }  
- 
- publicvoid destroy() throws Exception {  
- for (ServiceConfig<?> serviceConfig : serviceConfigs) {  
- try {  
-                 serviceConfig.unexport();  
-             } catch (Throwable e) {  
-                 logger.error(e.getMessage(), e);  
-             }  
-         }  
- for (ReferenceConfig<?> referenceConfig : referenceConfigs.values()) {  
- try {  
-                 referenceConfig.destroy();  
-             } catch (Throwable e) {  
-                 logger.error(e.getMessage(), e);  
-             }  
-         }  
-     }  
- 
- public Object postProcessAfterInitialization(Object bean, String beanName)  
- throws BeansException {  
- if (! isMatchPackage(bean)) {  
- return bean;  
-         }  
-         Service service = bean.getClass().getAnnotation(Service.class);  
- if (service != null) {  
-             ServiceBean<Object> serviceConfig = new ServiceBean<Object>(service);  
- if (void.class.equals(service.interfaceClass())  
-                     && "".equals(service.interfaceName())) {  
- if (bean.getClass().getInterfaces().length > 0) {  
-                     serviceConfig.setInterface(bean.getClass().getInterfaces()[0]);  
-                 } else {  
- thrownew IllegalStateException("Failed to export remote service class " + bean.getClass().getName() + ", cause: The @Service undefined interfaceClass or interfaceName, and the service class unimplemented any interfaces.");  
-                 }  
-             }  
- if (applicationContext != null) {  
-                 serviceConfig.setApplicationContext(applicationContext);  
- if (service.registry() != null && service.registry().length > 0) {  
-                     List<RegistryConfig> registryConfigs = new ArrayList<RegistryConfig>();  
- for (String registryId : service.registry()) {  
- if (registryId != null && registryId.length() > 0) {  
-                             registryConfigs.add((RegistryConfig)applicationContext.getBean(registryId, RegistryConfig.class));  
-                         }  
-                     }  
-                     serviceConfig.setRegistries(registryConfigs);  
-                 }  
- if (service.provider() != null && service.provider().length() > 0) {  
-                     serviceConfig.setProvider((ProviderConfig)applicationContext.getBean(service.provider(),ProviderConfig.class));  
-                 }  
- if (service.monitor() != null && service.monitor().length() > 0) {  
-                     serviceConfig.setMonitor((MonitorConfig)applicationContext.getBean(service.monitor(), MonitorConfig.class));  
-                 }  
- if (service.application() != null && service.application().length() > 0) {  
-                     serviceConfig.setApplication((ApplicationConfig)applicationContext.getBean(service.application(), ApplicationConfig.class));  
-                 }  
- if (service.module() != null && service.module().length() > 0) {  
-                     serviceConfig.setModule((ModuleConfig)applicationContext.getBean(service.module(), ModuleConfig.class));  
-                 }  
- if (service.provider() != null && service.provider().length() > 0) {  
-                     serviceConfig.setProvider((ProviderConfig)applicationContext.getBean(service.provider(), ProviderConfig.class));  
-                 } else {  
- 
-                 }  
- if (service.protocol() != null && service.protocol().length > 0) {  
-                     List<ProtocolConfig> protocolConfigs = new ArrayList<ProtocolConfig>();  
- for (String protocolId : service.registry()) {  
- if (protocolId != null && protocolId.length() > 0) {  
-                             protocolConfigs.add((ProtocolConfig)applicationContext.getBean(protocolId, ProtocolConfig.class));  
-                         }  
-                     }  
-                     serviceConfig.setProtocols(protocolConfigs);  
-                 }  
- try {  
-                     serviceConfig.afterPropertiesSet();  
-                 } catch (RuntimeException e) {  
- throw (RuntimeException) e;  
-                 } catch (Exception e) {  
- thrownew IllegalStateException(e.getMessage(), e);  
-                 }  
-             }  
-             serviceConfig.setRef(bean);  
-             serviceConfigs.add(serviceConfig);  
-             serviceConfig.export();  
-         }  
- return bean;  
-     }  
- 
- public Object postProcessBeforeInitialization(Object bean, String beanName)  
- throws BeansException {  
- if (! isMatchPackage(bean)) {  
- return bean;  
-         }  
-         Method[] methods = bean.getClass().getMethods();  
- for (Method method : methods) {  
-             String name = method.getName();  
- if (name.length() > 3 && name.startsWith("set")  
-                     && method.getParameterTypes().length == 1
-                     && Modifier.isPublic(method.getModifiers())  
-                     && ! Modifier.isStatic(method.getModifiers())) {  
- try {  
-                     Reference reference = method.getAnnotation(Reference.class);  
- if (reference != null) {  
-                         Object value = refer(reference, method.getParameterTypes()[0]);  
- if (value != null) {  
-                             method.invoke(bean, new Object[] {  });  
-                         }  
-                     }  
-                 } catch (Throwable e) {  
-                     logger.error("Failed to init remote service reference at method " + name + " in class " + bean.getClass().getName() + ", cause: " + e.getMessage(), e);  
-                 }  
-             }  
-         }  
-         Field[] fields = bean.getClass().getDeclaredFields();  
- for (Field field : fields) {  
- try {  
- if (! field.isAccessible()) {  
-                     field.setAccessible(true);  
-                 }  
-                 Reference reference = field.getAnnotation(Reference.class);  
- if (reference != null) {  
-                     Object value = refer(reference, field.getType());  
- if (value != null) {  
-                         field.set(bean, value);  
-                     }  
-                 }  
-             } catch (Throwable e) {  
-                 logger.error("Failed to init remote service reference at filed " + field.getName() + " in class " + bean.getClass().getName() + ", cause: " + e.getMessage(), e);  
-             }  
-         }  
- return bean;  
-     }  
- 
- private Object refer(Reference reference, Class<?> referenceClass) { //method.getParameterTypes()[0]
-         String interfaceName;  
- if (! "".equals(reference.interfaceName())) {  
-             interfaceName = reference.interfaceName();  
-         } elseif (! void.class.equals(reference.interfaceClass())) {  
-             interfaceName = reference.interfaceClass().getName();  
-         } elseif (referenceClass.isInterface()) {  
-             interfaceName = referenceClass.getName();  
-         } else {  
- thrownew IllegalStateException("The @Reference undefined interfaceClass or interfaceName, and the property type " + referenceClass.getName() + " is not a interface.");  
-         }  
-         String key = reference.group() + "/" + interfaceName + ":" + reference.version();  
-         ReferenceBean<?> referenceConfig = referenceConfigs.get(key);  
- if (referenceConfig == null) {  
-             referenceConfig = new ReferenceBean<Object>(reference);  
- if (void.class.equals(reference.interfaceClass())  
-                     && "".equals(reference.interfaceName())  
-                     && referenceClass.isInterface()) {  
-                 referenceConfig.setInterface(referenceClass);  
-             }  
- if (applicationContext != null) {  
-                 referenceConfig.setApplicationContext(applicationContext);  
- if (reference.registry() != null && reference.registry().length > 0) {  
-                     List<RegistryConfig> registryConfigs = new ArrayList<RegistryConfig>();  
- for (String registryId : reference.registry()) {  
- if (registryId != null && registryId.length() > 0) {  
-                             registryConfigs.add((RegistryConfig)applicationContext.getBean(registryId, RegistryConfig.class));  
-                         }  
-                     }  
-                     referenceConfig.setRegistries(registryConfigs);  
-                 }  
- if (reference.consumer() != null && reference.consumer().length() > 0) {  
-                     referenceConfig.setConsumer((ConsumerConfig)applicationContext.getBean(reference.consumer(), ConsumerConfig.class));  
-                 }  
- if (reference.monitor() != null && reference.monitor().length() > 0) {  
-                     referenceConfig.setMonitor((MonitorConfig)applicationContext.getBean(reference.monitor(), MonitorConfig.class));  
-                 }  
- if (reference.application() != null && reference.application().length() > 0) {  
-                     referenceConfig.setApplication((ApplicationConfig)applicationContext.getBean(reference.application(), ApplicationConfig.class));  
-                 }  
- if (reference.module() != null && reference.module().length() > 0) {  
-                     referenceConfig.setModule((ModuleConfig)applicationContext.getBean(reference.module(), ModuleConfig.class));  
-                 }  
- if (reference.consumer() != null && reference.consumer().length() > 0) {  
-                     referenceConfig.setConsumer((ConsumerConfig)applicationContext.getBean(reference.consumer(), ConsumerConfig.class));  
-                 }  
- try {  
-                     referenceConfig.afterPropertiesSet();  
-                 } catch (RuntimeException e) {  
- throw (RuntimeException) e;  
-                 } catch (Exception e) {  
- thrownew IllegalStateException(e.getMessage(), e);  
-                 }  
-             }  
-             referenceConfigs.putIfAbsent(key, referenceConfig);  
-             referenceConfig = referenceConfigs.get(key);  
-         }  
- return referenceConfig.get();  
-     }  
- 
- privateboolean isMatchPackage(Object bean) {  
- if (annotationPackages == null || annotationPackages.length == 0) {  
- returntrue;  
-         }  
-         String beanClassName = bean.getClass().getName();  
- for (String pkg : annotationPackages) {  
- if (beanClassName.startsWith(pkg)) {  
- returntrue;  
-             }  
-         }  
- returnfalse;  
-     }  
- 
- }  
这个AnnotationBean实现了几个Spring生命周期接口，从而完成Dubbo整合Spring 的操作。
org.springframework.beans.factory.config.BeanFactoryPostProcessor
先来看看Spring文档中的介绍：
**[plain]**[view
 plain](http://blog.csdn.net/l1028386804/article/details/70040928#)[copy](http://blog.csdn.net/l1028386804/article/details/70040928#)
- BeanFactoryPostProcessor operates on the bean configuration metadata; that is, the Spring IoC container allows a BeanFactoryPostProcessor to read the configuration metadata and potentially change it before the container instantiates any beans other than BeanFactoryPostProcessors.  
- BeanFactoryPostProcessor可以用于在Spring IoC容器实例化Bean之前，对Spring Bean配置信息进行一些操作  
通过Spring文档，可以清楚这个接口的功能，那再来看看Dubbo的AnnotationBean是如何实现这个接口的：
**[java]**[view
 plain](http://blog.csdn.net/l1028386804/article/details/70040928#)[copy](http://blog.csdn.net/l1028386804/article/details/70040928#)
- publicvoid postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory)  
- throws BeansException {  
- if (annotationPackage == null || annotationPackage.length() == 0) {  
- return;  
-     }  
- if (beanFactory instanceof BeanDefinitionRegistry) {  
- try {  
- // init scanner
-             Class<?> scannerClass = ReflectUtils.forName("org.springframework.context.annotation.ClassPathBeanDefinitionScanner");  
-             Object scanner = scannerClass.getConstructor(new Class<?>[] {BeanDefinitionRegistry.class, boolean.class}).newInstance(new Object[] {(BeanDefinitionRegistry) beanFactory, true});  
- // add filter
-             Class<?> filterClass = ReflectUtils.forName("org.springframework.core.type.filter.AnnotationTypeFilter");  
-             Object filter = filterClass.getConstructor(Class.class).newInstance(Service.class);  
-             Method addIncludeFilter = scannerClass.getMethod("addIncludeFilter", ReflectUtils.forName("org.springframework.core.type.filter.TypeFilter"));  
-             addIncludeFilter.invoke(scanner, filter);  
- // scan packages
-             String[] packages = Constants.COMMA_SPLIT_PATTERN.split(annotationPackage);  
-             Method scan = scannerClass.getMethod("scan", new Class<?>[]{String[].class});  
-             scan.invoke(scanner, new Object[] {packages});  
-         } catch (Throwable e) {  
- // spring 2.0
-         }  
-     }  
- }  
源码中已经标出了，Dubbo做了以下几件事：
- 增加了一个class扫描器，用于处理Dubbo服务类的扫描
- 增加过滤器，只扫描带有com.alibaba.dubbo.config.annotation.Service注解的class
- 指定扫描的包，对应这<dubbo:annotation package="com.foo.bar.service" />标签中的package属性。
这个扫描器，就是将那些带有com.alibaba.dubbo.config.annotation.Service注解的class纳入Spring容器中，而这些Dubbo服务类上不需要单独加上Spring的注解，也不需要额外配置Spring Bean定义。
org.springframework.beans.factory.DisposableBean
再来看看这个接口，还是先看看Spring文档：
**[plain]**[view
 plain](http://blog.csdn.net/l1028386804/article/details/70040928#)[copy](http://blog.csdn.net/l1028386804/article/details/70040928#)
- Implementing the org.springframework.beans.factory.DisposableBean interface allows a bean to get a callback when the container containing it is destroyed. The DisposableBean interface specifies a single method:void destroy() throws Exception;  
这个接口实际上就是当Spring容器要销毁时的一个回调接口。
那再来看看Dubbo是如何实现的：
**[java]**[view
 plain](http://blog.csdn.net/l1028386804/article/details/70040928#)[copy](http://blog.csdn.net/l1028386804/article/details/70040928#)
- publicvoid destroy() throws Exception {  
- for (ServiceConfig<?> serviceConfig : serviceConfigs) {  
- try {  
-             serviceConfig.unexport();  
-         } catch (Throwable e) {  
-             logger.error(e.getMessage(), e);  
-         }  
-     }  
- for (ReferenceConfig<?> referenceConfig : referenceConfigs.values()) {  
- try {  
-             referenceConfig.destroy();  
-         } catch (Throwable e) {  
-             logger.error(e.getMessage(), e);  
-         }  
-     }  
- }  
也比较简单，实际上就是当Spring容器销毁时，对Dubbo服务进行反注册操作。
org.springframework.beans.factory.config.BeanPostProcessor
最后来看看这个接口，照例先看看Spring文档：
**[plain]**[view
 plain](http://blog.csdn.net/l1028386804/article/details/70040928#)[copy](http://blog.csdn.net/l1028386804/article/details/70040928#)
- The BeanPostProcessor interface defines callback methods that you can implement to provide your own (or override the container’s default) instantiation logic, dependency-resolution logic, and so forth. If you want to implement some custom logic after the Spring container finishes instantiating, configuring, and initializing a bean, you can plug in one or more BeanPostProcessor implementations.  
这个接口也是一个回调接口，只是回调时机不同，它发生在Spring容器初始化过程中，Bean实例化前后之时。也可以理解为Spring容器初始化完成后，Bean实例化前后但还没有给容器外使用前。
再来看看Dubbbo在这个时间点上做了什么：
**[java]**[view
 plain](http://blog.csdn.net/l1028386804/article/details/70040928#)[copy](http://blog.csdn.net/l1028386804/article/details/70040928#)
- public Object postProcessBeforeInitialization(Object bean, String beanName)  
- throws BeansException {  
- if (! isMatchPackage(bean)) {  
- return bean;  
-     }  
-     Method[] methods = bean.getClass().getMethods();  
- for (Method method : methods) {  
-         String name = method.getName();  
- if (name.length() > 3 && name.startsWith("set")  
-                 && method.getParameterTypes().length == 1
-                 && Modifier.isPublic(method.getModifiers())  
-                 && ! Modifier.isStatic(method.getModifiers())) {  
- try {  
-                 Reference reference = method.getAnnotation(Reference.class);  
- if (reference != null) {  
-                     Object value = refer(reference, method.getParameterTypes()[0]);  
- if (value != null) {  
-                         method.invoke(bean, new Object[] {  });  
-                     }  
-                 }  
-             } catch (Throwable e) {  
-                 logger.error("Failed to init remote service reference at method " + name + " in class " + bean.getClass().getName() + ", cause: " + e.getMessage(), e);  
-             }  
-         }  
-     }  
-     Field[] fields = bean.getClass().getDeclaredFields();  
- for (Field field : fields) {  
- try {  
- if (! field.isAccessible()) {  
-                 field.setAccessible(true);  
-             }  
-             Reference reference = field.getAnnotation(Reference.class);  
- if (reference != null) {  
-                 Object value = refer(reference, field.getType());  
- if (value != null) {  
-                     field.set(bean, value);  
-                 }  
-             }  
-         } catch (Throwable e) {  
-             logger.error("Failed to init remote service reference at filed " + field.getName() + " in class " + bean.getClass().getName() + ", cause: " + e.getMessage(), e);  
-         }  
-     }  
- return bean;  
- }  
postProcessBeforeInitialization方法，顾名思义，发生Bean实例化之前。 通过源码就可以发现，主要用于处理Bean中的com.alibaba.dubbo.config.annotation.Reference注解，从而让Dubbo服务能够注入到Bean中。期间利用JAVA反射机制对Bean的方法和属性进行注入。
**[java]**[view
 plain](http://blog.csdn.net/l1028386804/article/details/70040928#)[copy](http://blog.csdn.net/l1028386804/article/details/70040928#)
- public Object postProcessAfterInitialization(Object bean, String beanName)  
- throws BeansException {  
- if (! isMatchPackage(bean)) {  
- return bean;  
-     }  
-     Service service = bean.getClass().getAnnotation(Service.class);  
- if (service != null) {  
-         ServiceBean<Object> serviceConfig = new ServiceBean<Object>(service);  
- if (void.class.equals(service.interfaceClass())  
-                 && "".equals(service.interfaceName())) {  
- if (bean.getClass().getInterfaces().length > 0) {  
-                 serviceConfig.setInterface(bean.getClass().getInterfaces()[0]);  
-             } else {  
- thrownew IllegalStateException("Failed to export remote service class " + bean.getClass().getName() + ", cause: The @Service undefined interfaceClass or interfaceName, and the service class unimplemented any interfaces.");  
-             }  
-         }  
- if (applicationContext != null) {  
-             serviceConfig.setApplicationContext(applicationContext);  
- if (service.registry() != null && service.registry().length > 0) {  
-                 List<RegistryConfig> registryConfigs = new ArrayList<RegistryConfig>();  
- for (String registryId : service.registry()) {  
- if (registryId != null && registryId.length() > 0) {  
-                         registryConfigs.add((RegistryConfig)applicationContext.getBean(registryId, RegistryConfig.class));  
-                     }  
-                 }  
-                 serviceConfig.setRegistries(registryConfigs);  
-             }  
- if (service.provider() != null && service.provider().length() > 0) {  
-                 serviceConfig.setProvider((ProviderConfig)applicationContext.getBean(service.provider(),ProviderConfig.class));  
-             }  
- if (service.monitor() != null && service.monitor().length() > 0) {  
-                 serviceConfig.setMonitor((MonitorConfig)applicationContext.getBean(service.monitor(), MonitorConfig.class));  
-             }  
- if (service.application() != null && service.application().length() > 0) {  
-                 serviceConfig.setApplication((ApplicationConfig)applicationContext.getBean(service.application(), ApplicationConfig.class));  
-             }  
- if (service.module() != null && service.module().length() > 0) {  
-                 serviceConfig.setModule((ModuleConfig)applicationContext.getBean(service.module(), ModuleConfig.class));  
-             }  
- if (service.provider() != null && service.provider().length() > 0) {  
-                 serviceConfig.setProvider((ProviderConfig)applicationContext.getBean(service.provider(), ProviderConfig.class));  
-             } else {  
- 
-             }  
- if (service.protocol() != null && service.protocol().length > 0) {  
-                 List<ProtocolConfig> protocolConfigs = new ArrayList<ProtocolConfig>();  
- for (String protocolId : service.registry()) {  
- if (protocolId != null && protocolId.length() > 0) {  
-                         protocolConfigs.add((ProtocolConfig)applicationContext.getBean(protocolId, ProtocolConfig.class));  
-                     }  
-                 }  
-                 serviceConfig.setProtocols(protocolConfigs);  
-             }  
- try {  
-                 serviceConfig.afterPropertiesSet();  
-             } catch (RuntimeException e) {  
- throw (RuntimeException) e;  
-             } catch (Exception e) {  
- thrownew IllegalStateException(e.getMessage(), e);  
-             }  
-         }  
-         serviceConfig.setRef(bean);  
-         serviceConfigs.add(serviceConfig);  
-         serviceConfig.export();  
-     }  
- return bean;  
- }  
postProcessAfterInitialization方法，同样从名字就能看出，其发生在Bean实例化之后。 通过源码就能发现，其作用就是将带有com.alibaba.dubbo.config.annotation.Service注解的Bean自动注册到Dubbo的注册中心，完成服务注册动作。
### 4.问题分析
通过上面的分析，对于Dubbo注解方式整合到Spring已经比较清楚了。 而在使用高版本Spring时，上面的AnnotationBean会出现一些问题。
问题主要集中在：
- Dubbo的注解Service有时不能进行服务注册
- Dubbo的注解Reference有时不能注入服务
从代码分析问题主要集中在postProcessBeforeInitialization和postProcessAfterInitialization方法上。
**[java]**[view
 plain](http://blog.csdn.net/l1028386804/article/details/70040928#)[copy](http://blog.csdn.net/l1028386804/article/details/70040928#)
- Service service = bean.getClass().getAnnotation(Service.class);  
**[java]**[view
 plain](http://blog.csdn.net/l1028386804/article/details/70040928#)[copy](http://blog.csdn.net/l1028386804/article/details/70040928#)
- Method[] methods = bean.getClass().getMethods();  
方法中都是利用Java反射来处理Bean的Class。
众所周知，Spring有两大利器IOC和AOP。在早期AOP基本都是利用JAVA动态代理实现的，而随着字节码技术的发展，现在Spring已经基本以ASM为代表的字节码增强框架为基础实现AOP,以及一些Bean的代理。
有兴趣的小伙伴可以打开spring-core-x.x.x.jar,会发现spring已经内嵌了asm以及cglib （org.springframework.asm）（org.springframework.cglib）
而在Spring环境中大量出现经过Cglib增强的Class，这些Class不再是简单的基于接口的代理机制了，其内部大量使用委派方式，以及访问器模式。最常见的就是Spring的事物，对于使用了@Transactional的Bean，都是以这种方式来增加事物处理能力的。
那基于这种方式扩展的Class，再通过反射getClass得到的是经过cglib改写后的class。在这些class上getAnnotation以及getMethods都不再是预期中的结果了。
所以，这也也就导致了上面罗列的两个问题。
### 5. 解决方案
经过对问题的分析，可以发现，问题主要是集中在Dubbo获取Bean的class上。
在Spring中，提供了很多Utils类。其中org.springframework.aop.support.AopUtils就可以用来对Aop代理的类进行操作的API。
于是，我们可以利用这个类来完成Class的获取。
**[java]**[view
 plain](http://blog.csdn.net/l1028386804/article/details/70040928#)[copy](http://blog.csdn.net/l1028386804/article/details/70040928#)
- private Class<?> getBeanClass(Object bean) {  
-     Class<?> clazz = bean.getClass();  
- if (AopUtils.isAopProxy(bean)) {  
-         clazz = AopUtils.getTargetClass(bean);  
-     }  
- return clazz;  
- }  
这样获取Class就可以这样：
**[java]**[view
 plain](http://blog.csdn.net/l1028386804/article/details/70040928#)[copy](http://blog.csdn.net/l1028386804/article/details/70040928#)
- Class<?> clazz = getBeanClass(bean);  
- Service service = clazz.getAnnotation(Service.class);  
**[java]**[view
 plain](http://blog.csdn.net/l1028386804/article/details/70040928#)[copy](http://blog.csdn.net/l1028386804/article/details/70040928#)
- Class<?> clazz = getBeanClass(bean);  
修改后完整的代码：
AnnotationBean.java
**[java]**[view
 plain](http://blog.csdn.net/l1028386804/article/details/70040928#)[copy](http://blog.csdn.net/l1028386804/article/details/70040928#)
- /*
-  * Copyright 1999-2012 Alibaba Group.
-  *  
-  * Licensed under the Apache License, Version 2.0 (the "License");
-  * you may not use this file except in compliance with the License.
-  * You may obtain a copy of the License at
-  *  
-  *      http://www.apache.org/licenses/LICENSE-2.0
-  *  
-  * Unless required by applicable law or agreed to in writing, software
-  * distributed under the License is distributed on an "AS IS" BASIS,
-  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
-  * See the License for the specific language governing permissions and
-  * limitations under the License.
-  */
- package com.roc.dubbo.config.spring;  
- 
- import java.lang.reflect.Field;  
- import java.lang.reflect.Method;  
- import java.lang.reflect.Modifier;  
- import java.util.ArrayList;  
- import java.util.List;  
- import java.util.Set;  
- import java.util.concurrent.ConcurrentHashMap;  
- import java.util.concurrent.ConcurrentMap;  
- 
- import org.springframework.aop.support.AopUtils;  
- import org.springframework.beans.BeansException;  
- import org.springframework.beans.factory.DisposableBean;  
- import org.springframework.beans.factory.config.BeanFactoryPostProcessor;  
- import org.springframework.beans.factory.config.BeanPostProcessor;  
- import org.springframework.beans.factory.config.ConfigurableListableBeanFactory;  
- import org.springframework.beans.factory.support.BeanDefinitionRegistry;  
- import org.springframework.context.ApplicationContext;  
- import org.springframework.context.ApplicationContextAware;  
- 
- import com.alibaba.dubbo.common.Constants;  
- import com.alibaba.dubbo.common.logger.Logger;  
- import com.alibaba.dubbo.common.logger.LoggerFactory;  
- import com.alibaba.dubbo.common.utils.ConcurrentHashSet;  
- import com.alibaba.dubbo.common.utils.ReflectUtils;  
- import com.alibaba.dubbo.config.AbstractConfig;  
- import com.alibaba.dubbo.config.ApplicationConfig;  
- import com.alibaba.dubbo.config.ConsumerConfig;  
- import com.alibaba.dubbo.config.ModuleConfig;  
- import com.alibaba.dubbo.config.MonitorConfig;  
- import com.alibaba.dubbo.config.ProtocolConfig;  
- import com.alibaba.dubbo.config.ProviderConfig;  
- import com.alibaba.dubbo.config.ReferenceConfig;  
- import com.alibaba.dubbo.config.RegistryConfig;  
- import com.alibaba.dubbo.config.ServiceConfig;  
- import com.alibaba.dubbo.config.annotation.Reference;  
- import com.alibaba.dubbo.config.annotation.Service;  
- import com.alibaba.dubbo.config.spring.ReferenceBean;  
- import com.alibaba.dubbo.config.spring.ServiceBean;  
- 
- /**
-  * AnnotationBean
-  * 
-  * @author william.liangf
-  * @export
-  * 
-  *         >>>修复@com.alibaba.dubbo.config.annotation.Service
-  *         与Spring @Transactional 冲突
-  */
- publicclass AnnotationBean extends AbstractConfig  
- implements DisposableBean, BeanFactoryPostProcessor, BeanPostProcessor, ApplicationContextAware {  
- 
- privatestaticfinallong serialVersionUID = -7582802454287589552L;  
- 
- privatestaticfinal Logger logger = LoggerFactory.getLogger(Logger.class);  
- 
- private String annotationPackage;  
- 
- private String[] annotationPackages;  
- 
- privatefinal Set<ServiceConfig<?>> serviceConfigs = new ConcurrentHashSet<ServiceConfig<?>>();  
- 
- privatefinal ConcurrentMap<String, ReferenceBean<?>> referenceConfigs = new ConcurrentHashMap<String, ReferenceBean<?>>();  
- 
- public String getPackage() {  
- return annotationPackage;  
-     }  
- 
- publicvoid setPackage(String annotationPackage) {  
- this.annotationPackage = annotationPackage;  
- this.annotationPackages = (annotationPackage == null || annotationPackage.length() == 0) ? null
-                 : Constants.COMMA_SPLIT_PATTERN.split(annotationPackage);  
-     }  
- 
- private ApplicationContext applicationContext;  
- 
- publicvoid setApplicationContext(ApplicationContext applicationContext) throws BeansException {  
- this.applicationContext = applicationContext;  
-     }  
- 
- publicvoid postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory) throws BeansException {  
- if (annotationPackage == null || annotationPackage.length() == 0) {  
-             logger.debug("未定义注解扫描包");  
- return;  
-         }  
-         logger.info("扫描：" + annotationPackage);  
- if (beanFactory instanceof BeanDefinitionRegistry) {  
- try {  
- // init scanner
-                 Class<?> scannerClass = ReflectUtils  
-                         .forName("org.springframework.context.annotation.ClassPathBeanDefinitionScanner");  
-                 Object scanner = scannerClass  
-                         .getConstructor(new Class<?>[] { BeanDefinitionRegistry.class, boolean.class })  
-                         .newInstance(new Object[] { (BeanDefinitionRegistry) beanFactory, true });  
- // add filter
-                 Class<?> filterClass = ReflectUtils  
-                         .forName("org.springframework.core.type.filter.AnnotationTypeFilter");  
-                 Object filter = filterClass.getConstructor(Class.class).newInstance(Service.class);  
-                 Method addIncludeFilter = scannerClass.getMethod("addIncludeFilter",  
-                         ReflectUtils.forName("org.springframework.core.type.filter.TypeFilter"));  
-                 addIncludeFilter.invoke(scanner, filter);  
- // scan packages
-                 String[] packages = Constants.COMMA_SPLIT_PATTERN.split(annotationPackage);  
-                 Method scan = scannerClass.getMethod("scan", new Class<?>[] { String[].class });  
-                 scan.invoke(scanner, new Object[] { packages });  
-             } catch (Throwable e) {  
- // spring 2.0
-             }  
-         }  
-     }  
- 
- publicvoid destroy() throws Exception {  
- for (ServiceConfig<?> serviceConfig : serviceConfigs) {  
- try {  
-                 serviceConfig.unexport();  
-             } catch (Throwable e) {  
-                 logger.error(e.getMessage(), e);  
-             }  
-         }  
- for (ReferenceConfig<?> referenceConfig : referenceConfigs.values()) {  
- try {  
-                 referenceConfig.destroy();  
-             } catch (Throwable e) {  
-                 logger.error(e.getMessage(), e);  
-             }  
-         }  
-     }  
- 
- public Object postProcessAfterInitialization(Object bean, String beanName) throws BeansException {  
- if (!isMatchPackage(bean)) {  
- return bean;  
-         }  
-         Class<?> clazz = getBeanClass(bean);  
-         Service service = clazz.getAnnotation(Service.class);  
- // >>>>>>>>>>>>>>>>>>
- if (service != null) {  
-             ServiceBean<Object> serviceConfig = new ServiceBean<Object>(service);  
- if (void.class.equals(service.interfaceClass()) && "".equals(service.interfaceName())) {  
- if (bean.getClass().getInterfaces().length > 0) {  
-                     serviceConfig.setInterface(bean.getClass().getInterfaces()[0]);  
-                 } else {  
- thrownew IllegalStateException("Failed to export remote service class " + bean.getClass().getName()  
-                             + ", cause: The @Service undefined interfaceClass or interfaceName, and the service class unimplemented any interfaces.");  
-                 }  
-             }  
- if (applicationContext != null) {  
-                 serviceConfig.setApplicationContext(applicationContext);  
- if (service.registry() != null && service.registry().length > 0) {  
-                     List<RegistryConfig> registryConfigs = new ArrayList<RegistryConfig>();  
- for (String registryId : service.registry()) {  
- if (registryId != null && registryId.length() > 0) {  
-                             registryConfigs  
-                                     .add((RegistryConfig) applicationContext.getBean(registryId, RegistryConfig.class));  
-                         }  
-                     }  
-                     serviceConfig.setRegistries(registryConfigs);  
-                 }  
- if (service.provider() != null && service.provider().length() > 0) {  
-                     serviceConfig.setProvider(  
-                             (ProviderConfig) applicationContext.getBean(service.provider(), ProviderConfig.class));  
-                 }  
- if (service.monitor() != null && service.monitor().length() > 0) {  
-                     serviceConfig.setMonitor(  
-                             (MonitorConfig) applicationContext.getBean(service.monitor(), MonitorConfig.class));  
-                 }  
- if (service.application() != null && service.application().length() > 0) {  
-                     serviceConfig.setApplication((ApplicationConfig) applicationContext.getBean(service.application(),  
-                             ApplicationConfig.class));  
-                 }  
- if (service.module() != null && service.module().length() > 0) {  
-                     serviceConfig  
-                             .setModule((ModuleConfig) applicationContext.getBean(service.module(), ModuleConfig.class));  
-                 }  
- if (service.provider() != null && service.provider().length() > 0) {  
-                     serviceConfig.setProvider(  
-                             (ProviderConfig) applicationContext.getBean(service.provider(), ProviderConfig.class));  
-                 } else {  
- 
-                 }  
- if (service.protocol() != null && service.protocol().length > 0) {  
-                     List<ProtocolConfig> protocolConfigs = new ArrayList<ProtocolConfig>();  
- for (String protocolId : service.registry()) {  
- if (protocolId != null && protocolId.length() > 0) {  
-                             protocolConfigs  
-                                     .add((ProtocolConfig) applicationContext.getBean(protocolId, ProtocolConfig.class));  
-                         }  
-                     }  
-                     serviceConfig.setProtocols(protocolConfigs);  
-                 }  
- try {  
-                     serviceConfig.afterPropertiesSet();  
-                 } catch (RuntimeException e) {  
- throw (RuntimeException) e;  
-                 } catch (Exception e) {  
- thrownew IllegalStateException(e.getMessage(), e);  
-                 }  
-             }  
-             serviceConfig.setRef(bean);  
-             serviceConfigs.add(serviceConfig);  
-             serviceConfig.export();  
-         }  
- return bean;  
-     }  
- 
- public Object postProcessBeforeInitialization(Object bean, String beanName) throws BeansException {  
- // logger.debug("扫描bean:"+bean);
- // System.out.println("扫描bean:"+bean);
- if (!isMatchPackage(bean)) {  
- return bean;  
-         }  
- // >>>>>>>>>>>>>>>>>>>>>>>>
-         Class<?> clazz = getBeanClass(bean);  
- // >>>>>>>>>>>>>>>>>>>>>>>>
-         Method[] methods = clazz.getMethods();  
- for (Method method : methods) {  
-             String name = method.getName();  
- if (name.length() > 3 && name.startsWith("set") && method.getParameterTypes().length == 1
-                     && Modifier.isPublic(method.getModifiers()) && !Modifier.isStatic(method.getModifiers())) {  
- try {  
-                     Reference reference = method.getAnnotation(Reference.class);  
- if (reference != null) {  
-                         Object value = refer(reference, method.getParameterTypes()[0]);  
- if (value != null) {  
-                             method.invoke(bean, new Object[] {});  
-                         }  
-                     }  
-                 } catch (Throwable e) {  
-                     logger.error("Failed to init remote service reference at method " + name + " in class "
-                             + bean.getClass().getName() + ", cause: " + e.getMessage(), e);  
-                 }  
-             }  
-         }  
- // >>>>>>>>>>>>>>>>>>>>>>>>
-         Field[] fields = clazz.getDeclaredFields();  
- for (Field field : fields) {  
- try {  
- if (!field.isAccessible()) {  
-                     field.setAccessible(true);  
-                 }  
-                 Reference reference = field.getAnnotation(Reference.class);  
- if (reference != null) {  
-                     Object value = refer(reference, field.getType());  
- if (value != null) {  
-                         field.set(bean, value);  
-                     }  
-                 }  
-             } catch (Throwable e) {  
-                 logger.error("Failed to init remote service reference at filed " + field.getName() + " in class "
-                         + bean.getClass().getName() + ", cause: " + e.getMessage(), e);  
-             }  
-         }  
- return bean;  
-     }  
- 
- private Object refer(Reference reference, Class<?> referenceClass) { // method.getParameterTypes()[0]
-         String interfaceName;  
- if (!"".equals(reference.interfaceName())) {  
-             interfaceName = reference.interfaceName();  
-         } elseif (!void.class.equals(reference.interfaceClass())) {  
-             interfaceName = reference.interfaceClass().getName();  
-         } elseif (referenceClass.isInterface()) {  
-             interfaceName = referenceClass.getName();  
-         } else {  
- thrownew IllegalStateException(  
- "The @Reference undefined interfaceClass or interfaceName, and the property type "
-                             + referenceClass.getName() + " is not a interface.");  
-         }  
-         String key = reference.group() + "/" + interfaceName + ":" + reference.version();  
-         ReferenceBean<?> referenceConfig = referenceConfigs.get(key);  
- if (referenceConfig == null) {  
-             referenceConfig = new ReferenceBean<Object>(reference);  
- if (void.class.equals(reference.interfaceClass()) && "".equals(reference.interfaceName())  
-                     && referenceClass.isInterface()) {  
-                 referenceConfig.setInterface(referenceClass);  
-             }  
- if (applicationContext != null) {  
-                 referenceConfig.setApplicationContext(applicationContext);  
- if (reference.registry() != null && reference.registry().length > 0) {  
-                     List<RegistryConfig> registryConfigs = new ArrayList<RegistryConfig>();  
- for (String registryId : reference.registry()) {  
- if (registryId != null && registryId.length() > 0) {  
-                             registryConfigs  
-                                     .add((RegistryConfig) applicationContext.getBean(registryId, RegistryConfig.class));  
-                         }  
-                     }  
-                     referenceConfig.setRegistries(registryConfigs);  
-                 }  
- if (reference.consumer() != null && reference.consumer().length() > 0) {  
-                     referenceConfig.setConsumer(  
-                             (ConsumerConfig) applicationContext.getBean(reference.consumer(), ConsumerConfig.class));  
-                 }  
- if (reference.monitor() != null && reference.monitor().length() > 0) {  
-                     referenceConfig.setMonitor(  
-                             (MonitorConfig) applicationContext.getBean(reference.monitor(), MonitorConfig.class));  
-                 }  
- if (reference.application() != null && reference.application().length() > 0) {  
-                     referenceConfig.setApplication((ApplicationConfig) applicationContext  
-                             .getBean(reference.application(), ApplicationConfig.class));  
-                 }  
- if (reference.module() != null && reference.module().length() > 0) {  
-                     referenceConfig.setModule(  
-                             (ModuleConfig) applicationContext.getBean(reference.module(), ModuleConfig.class));  
-                 }  
- if (reference.consumer() != null && reference.consumer().length() > 0) {  
-                     referenceConfig.setConsumer(  
-                             (ConsumerConfig) applicationContext.getBean(reference.consumer(), ConsumerConfig.class));  
-                 }  
- try {  
-                     referenceConfig.afterPropertiesSet();  
-                 } catch (RuntimeException e) {  
- throw (RuntimeException) e;  
-                 } catch (Exception e) {  
- thrownew IllegalStateException(e.getMessage(), e);  
-                 }  
-             }  
-             referenceConfigs.putIfAbsent(key, referenceConfig);  
-             referenceConfig = referenceConfigs.get(key);  
-         }  
- return referenceConfig.get();  
-     }  
- 
- privateboolean isMatchPackage(Object bean) {  
- if (annotationPackages == null || annotationPackages.length == 0) {  
- returntrue;  
-         }  
-         Class<?> clazz = getBeanClass(bean);  
-         String beanClassName = clazz.getName();  
- for (String pkg : annotationPackages) {  
- if (beanClassName.startsWith(pkg)) {  
- returntrue;  
-             }  
-         }  
- returnfalse;  
-     }  
- 
- private Class<?> getBeanClass(Object bean) {  
-         Class<?> clazz = bean.getClass();  
- if (AopUtils.isAopProxy(bean)) {  
-             clazz = AopUtils.getTargetClass(bean);  
-         }  
- return clazz;  
-     }  
- 
- }  
### 6. 整合
同样，我不太赞同自己修改Dubbo源码，然后打包。侵入性太强。 通过前面几节的分析发现，其实只要AnnotationBean能够注册到Spring环境就行。而AnnotationBean无所谓放在哪个jar中。
所以，完全可以把修改后的AnnotationBean打入到自己写的jar中，然后注入Spring环境，以外挂的方式完成修改。
下面示例代码展示了，在Spring Boot中以外挂方式整合：
**[java]**[view
 plain](http://blog.csdn.net/l1028386804/article/details/70040928#)[copy](http://blog.csdn.net/l1028386804/article/details/70040928#)
- package com.roc.dubbo.boot.configuration;  
- 
- import org.apache.logging.log4j.LogManager;  
- import org.apache.logging.log4j.Logger;  
- import com.roc.dubbo.config.spring.AnnotationBean;  
- import org.springframework.boot.autoconfigure.AutoConfigureAfter;  
- import org.springframework.context.annotation.Bean;  
- import org.springframework.context.annotation.Configuration;  
- import org.springframework.context.annotation.Import;  
- 
- /**
-  * @author liuyazhuang
-  * @date 2017-04-11 00:25:27
-  * @description
-  */
- @Configuration
- @Import(DubboBasicConfiguration.class)  
- @AutoConfigureAfter(DubboBasicConfiguration.class)  
- publicclass DubboConfiguration {  
- 
- privatefinal Logger logger = LogManager.getLogger(getClass());  
- 
- @Bean
- // @DependsOn("dubboConfiguration")
- public AnnotationBean dubboAnnotationBean() {  
-         AnnotationBean annotationBean = new AnnotationBean();  
- // annotationBean.setApplicationContext(applicationContext);
- // annotationBean.setPackage(dubboProperties.getAnnotationPackage());
- // annotationBean.setPackage("com.roc.pay");
-         logger.info("加入自定义" + annotationBean);  
- return annotationBean;  
-     }  
- 
- }  
- 
