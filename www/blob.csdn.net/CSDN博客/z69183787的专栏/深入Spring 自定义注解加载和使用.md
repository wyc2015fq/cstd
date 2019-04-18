# 深入Spring:自定义注解加载和使用 - z69183787的专栏 - CSDN博客
2016年12月21日 14:56:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：16998
### 前言
在工作中经常使用Spring的相关框架，免不了去看一下Spring的实现方法，了解一下Spring内部的处理逻辑。特别是开发Web应用时，我们会频繁的定义*@Controller*，*@Service*等JavaBean组件，通过注解，Spring自动扫描加载了这些组件，并提供相关的服务。
Spring是如何读取注解信息，并注入到bean容器中的，本文就是通过嵌入Spring的Bean加载，来描述Spring的实现方法。完整的例子都在[Github](https://github.com/wcong/learn-java/tree/master/src/main/java/org/wcong/test/spring)上了。
### 自定义注解
先看一个最简单的例子，在使用SpringWeb应用中的过程中，大家免不了会使用*@Controller*，*@Service*，*@Repository*等注解来定义JavaBean。那么怎么自己定义一个注解，Spring可以自动加载呢。所以就有了第一个例子。
```java
```java
@Target({ ElementType.TYPE })
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Component
public @interface MyComponent {
    String value() default "";
}
```
```
```java
```java
@Configuration
public class ComponentAnnotationTest {
  public static void main(String[] args) {
    AnnotationConfigApplicationContext annotationConfigApplicationContext = new AnnotationConfigApplicationContext();
annotationConfigApplicationContext.register(ComponentAnnotationTest.class);
    annotationConfigApplicationContext.refresh();
    InjectClass injectClass = annotationConfigApplicationContext.getBean(InjectClass.class);
        injectClass.print();
  }
  @MyComponent
  public static class InjectClass {
    public void print() {
        System.out.println("hello world");
    }
  }
}
```
```
运行这个例子，就会发现，*@MyComponent* 注解的类，也被Spring加载进来了，而且可以当成普通的JavaBean正常的使用。查看Spring的源码会发现，Spring是使用*ClassPathScanningCandidateComponentProvider*扫描package,这个类有这样的注释
```
```
A component provider that scans the classpath from a base package. 
It then applies exclude and include filters to the resulting classes to find candidates.
```
```
这个类的 *registerDefaultFilters* 方法有这样几行代码
```
protected void registerDefaultFilters() {   
   this.includeFilters.add(new AnnotationTypeFilter(Component.class));
   ClassLoader cl = ClassPathScanningCandidateComponentProvider.class.getClassLoader();
   try {    
      this.includeFilters.add(new AnnotationTypeFilter(((Class<? extends Annotation>) ClassUtils.forName("javax.annotation.ManagedBean", cl)), false)); 
      logger.debug("JSR-250 'javax.annotation.ManagedBean' found and supported for component scanning"); 
   }   catch (ClassNotFoundException ex) {     
     // JSR-250 1.1 API (as included in Java EE 6) not available - simply skip.   
   }   
   try {      
      this.includeFilters.add(new AnnotationTypeFilter(((Class<? extends Annotation>) ClassUtils.forName("javax.inject.Named", cl)), false));      
      logger.debug("JSR-330 'javax.inject.Named' annotation found and supported for component scanning");   
   }  
   catch (ClassNotFoundException ex) {     
   // JSR-330 API not available - simply skip.  
   }
}
```
这里就会发现Spring在扫描类信息的使用只会判断被*@Component*注解的类，所以任何自定义的注解只要带上*@Component*（当然还要有*String value() default "";*的方法，因为Spring的Bean都是有beanName唯一标示的），都可以被Spring扫描到，并注入容器内。
#### 定制功能
但上面的方法太局限了，没办法定制，而且也没有实际的意义。如何用特殊的注解来实现定制的功能呢，一般有两种方式：
- 
还是用上面的方法，在注入Spring的容器后，再取出来做自己定制的功能，Spring-MVC就是使用这样的方法。*AbstractDetectingUrlHandlerMapping* 中的*detectHandlers*方法,这个方法取出了所有的bean，然后循环查找带有*Controller*的bean，并提取其中的*RequestMapping*信息
```java
```java
protected void detectHandlers() throws BeansException {
     if (logger.isDebugEnabled()) {
         logger.debug("Looking for URL mappings in application context: " + getApplicationContext());
     }
     String[] beanNames = (this.detectHandlersInAncestorContexts ?
             BeanFactoryUtils.beanNamesForTypeIncludingAncestors(getApplicationContext(), Object.class) :
             getApplicationContext().getBeanNamesForType(Object.class));
     // Take any bean name that we can determine URLs for.
     for (String beanName : beanNames) {
         String[] urls = determineUrlsForHandler(beanName);
         if (!ObjectUtils.isEmpty(urls)) {
             // URL paths found: Let's consider it a handler.
             registerHandler(urls, beanName);
         }
         else {
             if (logger.isDebugEnabled()) {
                 logger.debug("Rejected bean name '" + beanName + "': no URL paths identified");
             }
         }
     }
 }
```
```
- 
不依赖*@Component*，自定义扫描。所以就有了第二个例子。
#### 自定义扫描
结构比较复杂，可以参考完整的[例子](https://github.com/wcong/learn-java/blob/master/src/main/java/org/wcong/test/spring/CustomizeScanTest.java)，这里是关键的几个类
- 
还是定义一个注解，只不过不再需要*@Component*了
```java
```java
@Target({ ElementType.TYPE })
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface CustomizeComponent {
  String value() default "";
}
```
```
- 
注解修饰的类
```
@CustomizeComponent
public class ScanClass1 {
 public void print() {
     System.out.println("scanClass1");
 }
}
```
- 
BeanScannerConfigurer用于嵌入到Spring的加载过程的中，这里用到了*BeanFactoryPostProcessor* 和*ApplicationContextAware*。
Spring提供了一些的接口使程序可以嵌入Spring的加载过程。这个类中的继承*ApplicationContextAware*接口，Spring会读取*ApplicationContextAware*类型的的JavaBean，并调用*setApplicationContext(ApplicationContext applicationContext)*传入Spring的*applicationContext*。
同样继承*BeanFactoryPostProcessor*接口，Spring会在*BeanFactory*的相关处理完成后调用*postProcessBeanFactory*方法，进行定制的功能。
```java
```java
@Component
public static class BeanScannerConfigurer implements  BeanFactoryPostProcessor, ApplicationContextAware {
 private ApplicationContext applicationContext;
 public void setApplicationContext(ApplicationContext applicationContext) throws BeansException {
   this.applicationContext = applicationContext;
 }
 public void postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory) throws BeansException {
   Scanner scanner = new Scanner((BeanDefinitionRegistry) beanFactory);
   scanner.setResourceLoader(this.applicationContext);
   scanner.scan("org.wcong.test.spring.scan");
 }
}
```
```
- Scanner继承的*ClassPathBeanDefinitionScanner*是Spring内置的Bean定义的扫描器。
*includeFilter*里定义了类的过滤器，*newAnnotationTypeFilter(CustomizeComponent.class)*表示只取被*CustomizeComponent*修饰的类。
*doScan*里扫面了包底下的读取道德*BeanDefinitionHolder*，自定义*GenericBeanDefinition*相关功能。
```
public final static class Scanner extends ClassPathBeanDefinitionScanner {
   public Scanner(BeanDefinitionRegistry registry) {
       super(registry);
   }
   public void registerDefaultFilters() {
       this.addIncludeFilter(new AnnotationTypeFilter(CustomizeComponent.class));
   }
   public Set<BeanDefinitionHolder> doScan(String... basePackages) {
       Set<BeanDefinitionHolder> beanDefinitions =   super.doScan(basePackages);
       for (BeanDefinitionHolder holder : beanDefinitions) {
           GenericBeanDefinition definition = (GenericBeanDefinition) holder.getBeanDefinition();
           definition.getPropertyValues().add("innerClassName", definition.getBeanClassName());
           definition.setBeanClass(FactoryBeanTest.class);
       }
       return beanDefinitions;
   }
   public boolean isCandidateComponent(AnnotatedBeanDefinition beanDefinition) {
      return super.isCandidateComponent(beanDefinition) && beanDefinition.getMetadata()
.hasAnnotation(CustomizeComponent.class.getName());
   }
}
```
- FactoryBean是Spring中比较重要的一个类。它的描述如下
```python
```python
Interface to be implemented by objects used within a BeanFactory which are themselves factories. 
If a bean implements this interface, it is used as a factory for an object to expose, not directly as a bean* instance that will be exposed itself
```
```
普通的JavaBean是直接使用类的实例，但是如果一个Bean继承了这个借口，就可以通过*getObject()*方法来自定义实例的内容，在FactoryBeanTest的*getObject()*就通过代理了原始类的方法，自定义类的方法。
```java
```java
public static class FactoryBeanTest<T> implements InitializingBean, FactoryBean<T> {
   private String innerClassName;
   public void setInnerClassName(String innerClassName) {
       this.innerClassName = innerClassName;
   }
   public T getObject() throws Exception {
       Class innerClass = Class.forName(innerClassName);
       if (innerClass.isInterface()) {
           return (T) InterfaceProxy.newInstance(innerClass);
       } else {
           Enhancer enhancer = new Enhancer();
           enhancer.setSuperclass(innerClass);
           enhancer.setNamingPolicy(SpringNamingPolicy.INSTANCE);
           enhancer.setCallback(new MethodInterceptorImpl());
           return (T) enhancer.create();
       }
   }
   public Class<?> getObjectType() {
       try {
             return Class.forName(innerClassName);
       } catch (ClassNotFoundException e) {
             e.printStackTrace();
       }
       return null;
   }
   public boolean isSingleton() {
       return true;
   }
   public void afterPropertiesSet() throws Exception {
   }
}
public static class InterfaceProxy implements InvocationHandler {
   public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
       System.out.println("ObjectProxy execute:" + method.getName());
       return method.invoke(proxy, args);
   }
   public static <T> T newInstance(Class<T> innerInterface) {
       ClassLoader classLoader = innerInterface.getClassLoader();
       Class[] interfaces = new Class[] { innerInterface };
       InterfaceProxy proxy = new InterfaceProxy();
       return (T) Proxy.newProxyInstance(classLoader, interfaces, proxy);
   }
  }
  public static class MethodInterceptorImpl implements MethodInterceptor {
       public Object intercept(Object o, Method method, Object[] objects, MethodProxy methodProxy) throws Throwable {
       System.out.println("MethodInterceptorImpl:" + method.getName());
       return methodProxy.invokeSuper(o, objects);
   }
}
```
```
- main函数
```cpp
```cpp
@Configuration
public class CustomizeScanTest {
 public static void main(String[] args) {
     AnnotationConfigApplicationContext annotationConfigApplicationContext = new AnnotationConfigApplicationContext();                
     annotationConfigApplicationContext.register(CustomizeScanTest.class);
     annotationConfigApplicationContext.refresh();
     ScanClass1 injectClass = annotationConfigApplicationContext.getBean(ScanClass1.class);
     injectClass.print();
 }
}
```
```
至此一个完整的例子就完成了，这里主要用到了*BeanFactoryPostProcessor*，*ApplicationContextAware*，*FactoryBean*等Spring内置的接口，来嵌入Spring的加载和使用过程，这样就实现了自定义注解，和自定义代理了。
文／wcong（简书作者）
原文链接：http://www.jianshu.com/p/7c2948f64b1c
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
