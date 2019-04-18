# 也谈Spring Bean的生命周期 - z69183787的专栏 - CSDN博客
2016年06月13日 14:30:06[OkidoGreen](https://me.csdn.net/z69183787)阅读数：657
开篇先用一张老图描述下Spring中Bean容器的生命周期。
![](http://dl.iteye.com/upload/picture/pic/90369/01701af4-fb1c-329c-9058-2cd5093aad98.png)
插叙一下，记得某个博文中提到：“Spring的Bean容器只管理非单例Bean的生命周期，单例Bean的生命周期不在管理范围内”，其实我认为这句话恰好说反了。首先明确一点，并非Spring容器中所有的Bean都有生命周期行为，只有接受容器管理生命周期的Bean才具有生命周期行为：而单例（Singleton）Bean接受容器管理，非单例（non-singleton）Bean在实例化后，完全交给了客户端代码管理，容器不再跟踪其生命周期，每次客户请求，容器都会创建一个新的实例，所以Spring容易无法知晓Bean何时销毁。
继续刚才的话题——Bean容器的生命周期。其实上图有个节点没有画出，就是在实例化所有Bean之前会执行BeanFactoryPostProcessors。不过也不care，因为这和Bean的生命周期没有太大关系，所以没有提及也属正常，权且忽略该节点。
从图中，我们可以看到实例化Bean的过程中有以下几个节点：
1）设置属性值；
2）调用Bean中的BeanNameAware.setBeanName()方法，如果该Bean实现了BeanNameAware接口；
3）调用Bean中的BeanFactoryAware.setBeanFactory()方法，如果该Bean实现了BeanFactoryAware接口；
4）调用BeanPostProcessors.postProcessBeforeInitialization()方法；
5）调用Bean中的afterPropertiesSet方法，如果该Bean实现了InitializingBean接口；
6）调用Bean中的init-method，通常是在配置bean的时候指定了init-method，例如：*<bean class="beanClass"init-method="init"></bean>*
7）调用BeanPostProcessors.postProcessAfterInitialization()方法；
8）如果该Bean是单例的，则当容器销毁并且该Bean实现了DisposableBean接口的时候，调用destory方法；如果该Bean是prototype，则将准备好的Bean提交给调用者，后续不再管理该Bean的生命周期。
好了，简单了描述了下那幅图。一切都还太抽象了，作为程序员，代码还是最直接的表达方式。那我们就一起看段演示代码吧。
首先，为达到演示效果，我们准备两个待测试的Bean，代码如下：
Java代码  
```java
@Component  
public class DemoBean implements BeanFactoryAware, BeanNameAware,  
        InitializingBean, DisposableBean {  
    @PostConstruct  
    public void init() {  
       System.out.println("DemoBean: PostConstruct");  
    }  
    public void destroy() throws Exception {  
       System.out.println("DemoBean: destroy-method!");  
    }  
    public void afterPropertiesSet() throws Exception {  
       System.out.println("DemoBean: after properties set!");  
    }  
    public void setBeanName(String name) {  
       System.out.println("DemoBean: beanName aware, [name=" + name + "]");  
    }  
    public void setBeanFactory(BeanFactory beanFactory) throws BeansException {  
       System.out.println("DemoBean: beanFactory aware, [beanFactory=" + beanFactory.toString() + "]");  
    }  
}
```
Java代码  
```java
public class AnotherDemoBean implements InitializingBean {  
   
    @PostConstruct  
    public void postConstruct() {  
       System.out.println("AnotherDemoBean: postConstruct-method");  
    }    
    public void init() {  
       System.out.println("AnotherDemoBean: init-method");  
    }  
    @Override  
    public void afterPropertiesSet() throws Exception {  
       System.out.println("AnotherDemoBean: after properties set!");  
    }  
}
```
上面两个Bean大致相同，区别在于第一个Bean使用注解方式注入，第二个Bean我们使用配置文件方式，并指定其init-method，用于观察init-method与postConstruct的执行先后。
我们这个演示Bean实现了BeanFactoryAware, BeanNameAware, InitializingBean,  DisposableBean这几个接口，其实这些接口也可理解为Spring容器的一个个扩展点。
然后，我们再编写一个BeanPostProcessor，用于演示生命周期中的步骤4和步骤7。 代码如下：
Java代码  
```java
@Component  
public class DemoBeanPostProcessor implements BeanPostProcessor {  
    public Object postProcessBeforeInitialization(Object bean, String beanName) throws BeansException {  
       System.out.println("DemoBeanPostProcessor: post process before initialization, [beanName=" + beanName + ", bean=" + bean + "]");  
       return bean;  
    }  
    public Object postProcessAfterInitialization(Object bean, String beanName) throws BeansException {  
       System.out.println("DemoBeanPostProcessor: post process after initialization, [beanName=" + beanName + ", bean=" + bean + "]");  
       return bean;  
    }  
}
```
最后，我们编写测试类，以及Spring的配置文件，这里我们使用ClassPathXMLApplicationContext加载配置文件和初始化Spring容器。一起看下配置文件和测试类代码：
applicationContext.xml:
Xml代码  
```
<?xml version="1.0" encoding="GBK"?>  
   
<beans xmlns="http://www.springframework.org/schema/beans"  
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:aop="http://www.springframework.org/schema/aop"  
    xmlns:context="http://www.springframework.org/schema/context" xmlns:tx="http://www.springframework.org/schema/tx"  
    xsi:schemaLocation="  
        http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-2.5.xsd  
        http://www.springframework.org/schema/aop http://www.springframework.org/schema/aop/spring-aop-2.5.xsd  
        http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context-2.5.xsd  
        http://www.springframework.org/schema/tx http://www.springframework.org/schema/tx/spring-tx-2.5.xsd">  
    <context:component-scan base-package="com.shansun.multidemo"></context:component-scan>  
    <bean class="com.shansun.multidemo.spring.lifecycle.AnotherDemoBean" init-method="init"></bean>  
</beans>
```
Main.java
Java代码  
```python
public class Main {  
    @SuppressWarnings("unused")  
    public static void main(String[] args) {  
        ApplicationContext ctx = new ClassPathXmlApplicationContext("applicationContext.xml");  
    }  
}
```
好了，一切就绪，我们就静观程序输出吧：
**postConstruct 在 BeanPostProcessor postProcessBeforeInitialization 之前**
Java代码  
```java
DemoBean: beanName aware, [name=demoBean]  
DemoBean: beanFactory aware, [beanFactory=org.springframework.beans.factory.support.DefaultListableBeanFactory@888e6c:defining 
	beans [demoBean,demoBeanFactoryPostProcessor,demoBeanPostProcessor,
	org.springframework.context.annotation.internalCommonAnnotationProcessor,
	org.springframework.context.annotation.internalAutowiredAnnotationProcessor,
	org.springframework.context.annotation.internalRequiredAnnotationProcessor,
	com.shansun.multidemo.spring.lifecycle.AnotherDemoBean#0]; 
	root of factory hierarchy]  
DemoBean: PostConstruct  
DemoBeanPostProcessor: post process before initialization, [beanName=demoBean, bean=com.shansun.multidemo.spring.lifecycle.DemoBean@1deeb40]  
DemoBean: after properties set!  
DemoBeanPostProcessor: post process after initialization, [beanName=demoBean, bean=com.shansun.multidemo.spring.lifecycle.DemoBean@1deeb40]  
AnotherDemoBean: postConstruct-method  
DemoBeanPostProcessor: post process before initialization, [beanName=com.shansun.multidemo.spring.lifecycle.AnotherDemoBean#0, 
bean=com.shansun.multidemo.spring.lifecycle.AnotherDemoBean@1a7ddcf]  
AnotherDemoBean: after properties set!  
AnotherDemoBean: init-method  
DemoBeanPostProcessor: post process before initialization, [beanName=com.shansun.multidemo.spring.lifecycle.AnotherDemoBean#0, 
bean=com.shansun.multidemo.spring.lifecycle.AnotherDemoBean@1a7ddcf]
```
和我们预期的是否一样呢？是的。观察结果发现一个有趣的地方：在配置文件中指定的init-method和使用@PostConstruct注解的方法，孰先孰后呢，两者是否等同呢？[后续我将通过分析源码给出结论](http://sexycoding.iteye.com/blog/1046993)。
我们通过演示代码也验证了Bean容器的生命周期，但是还缺点什么吧。对了，透过Spring源码讲述Bean容器的生命周期是否更加直观和令人信服呢？下面我们去Spring源码中一探究竟。这里我们选用的是spring-2.5.6.SEC02。
大家应该都知道Spring中BeanFactory和ApplicationContext的关系了吧，ApplicationContext继承自BeanFactory，所以可以操作到bean。更详细的内容可以参考许令波同学的《Spring框架的设计理念与设计模式分析》，里面有较清晰的分析。
好了，闲话不多说。
首先，我们探视下实例化Bean的方法initializeBean，该方法在org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory类下，一起看下该段代码：
Java代码  
```java
protected Object initializeBean(String beanName, Object bean, RootBeanDefinition mbd) {  
         if (bean instanceof BeanNameAware) {  
                   ((BeanNameAware) bean).setBeanName(beanName);  
         }  
   
         if (bean instanceof BeanClassLoaderAware) {  
                   ((BeanClassLoaderAware) bean).setBeanClassLoader(getBeanClassLoader());  
         }  
   
         if (bean instanceof BeanFactoryAware) {  
                   ((BeanFactoryAware) bean).setBeanFactory(this);  
         }  
   
         Object wrappedBean = bean;  
         if (mbd == null || !mbd.isSynthetic()) {  
                   wrappedBean = applyBeanPostProcessorsBeforeInitialization(wrappedBean, beanName);  
         }  
   
         try {  
                   invokeInitMethods(beanName, wrappedBean, mbd);  
         }  
         catch (Throwable ex) {  
                   throw new BeanCreationException(  
                                     (mbd != null ? mbd.getResourceDescription() : null),  
                                     beanName, "Invocation of init method failed", ex);  
         }  
   
         if (mbd == null || !mbd.isSynthetic()) {  
                   wrappedBean = applyBeanPostProcessorsAfterInitialization(wrappedBean, beanName);  
         }  
         return wrappedBean;  
}
```
```java
protected void invokeInitMethods(String beanName, final Object bean, RootBeanDefinition mbd)
			throws Throwable {
		boolean isInitializingBean = (bean instanceof InitializingBean);
		if (isInitializingBean && (mbd == null || !mbd.isExternallyManagedInitMethod("afterPropertiesSet"))) {
			if (logger.isDebugEnabled()) {
				logger.debug("Invoking afterPropertiesSet() on bean with name '" + beanName + "'");
			}
			if (System.getSecurityManager() != null) {
				try {
					AccessController.doPrivileged(new PrivilegedExceptionAction<Object>() {
						@Override
						public Object run() throws Exception {
							((InitializingBean) bean).afterPropertiesSet();
							return null;
						}
					}, getAccessControlContext());
				}
				catch (PrivilegedActionException pae) {
					throw pae.getException();
				}
			}
			else {
				((InitializingBean) bean).afterPropertiesSet();
			}
		}
		if (mbd != null) {
			String initMethodName = mbd.getInitMethodName();
			if (initMethodName != null && !(isInitializingBean && "afterPropertiesSet".equals(initMethodName)) &&
					!mbd.isExternallyManagedInitMethod(initMethodName)) {
				invokeCustomInitMethod(beanName, bean, mbd);
			}
		}
	}
```
这样够直观了吧，是不是和前文描述的一样呢，J
本文源代码下载：https://lb-multi-demo.googlecode.com/svn/trunk/spring-lifecycle-test
By [Mr.Chris](http://sexycoding.iteye.com/)
