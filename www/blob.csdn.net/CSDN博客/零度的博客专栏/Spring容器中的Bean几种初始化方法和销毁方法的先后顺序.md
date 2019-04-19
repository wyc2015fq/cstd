# Spring容器中的Bean几种初始化方法和销毁方法的先后顺序 - 零度的博客专栏 - CSDN博客
2016年04月06日 13:44:32[零度anngle](https://me.csdn.net/zmx729618)阅读数：1622
[Spring 容器中的 Bean 是有生命周期的](http://sexycoding.iteye.com/blog/1046775)，Spring 允许 Bean 在初始化完成后以及销毁前执行特定的操作。下面是常用的三种指定特定操作的方法：
- 通过实现InitializingBean/DisposableBean 接口来定制初始化之后/销毁之前的操作方法； 
- 通过<bean> 元素的 init-method/destroy-method属性指定初始化之后 /销毁之前调用的操作方法； 
- 在指定方法上加上@PostConstruct或@PreDestroy注解来制定该方法是在初始化之后还是销毁之前调用。
这几种配置方式，执行顺序是怎样的呢？我们通过例子来研究下：
Java类：
**[java]**[view plain](http://blog.csdn.net/caihaijiang/article/details/8629725#)[copy](http://blog.csdn.net/caihaijiang/article/details/8629725#)
- import javax.annotation.PostConstruct;  
- import javax.annotation.PreDestroy;  
- 
- import org.springframework.beans.factory.DisposableBean;  
- import org.springframework.beans.factory.InitializingBean;  
- import org.springframework.context.support.ClassPathXmlApplicationContext;  
- 
- publicclass InitAndDestroySeqBean implements InitializingBean,DisposableBean {  
- 
- public InitAndDestroySeqBean(){  
-         System.out.println("执行InitAndDestroySeqBean: 构造方法");  
-     }  
- 
- @PostConstruct
- publicvoid postConstruct() {    
-        System.out.println("执行InitAndDestroySeqBean: postConstruct");    
-     }    
- 
- @Override
- publicvoid afterPropertiesSet() throws Exception {  
-         System.out.println("执行InitAndDestroySeqBean: afterPropertiesSet");   
-     }  
- 
- publicvoid initMethod() {  
-         System.out.println("执行InitAndDestroySeqBean: init-method");  
-     }  
- 
- @PreDestroy
- publicvoid preDestroy()  {  
-         System.out.println("执行InitAndDestroySeqBean: preDestroy");  
-     }  
- 
- @Override
- publicvoid destroy() throws Exception {  
-         System.out.println("执行InitAndDestroySeqBean: destroy");  
-     }  
- 
- publicvoid destroyMethod() {  
-         System.out.println("执行InitAndDestroySeqBean: destroy-method");  
-     }  
- 
- publicstaticvoid main(String[] args) {  
-         ClassPathXmlApplicationContext context = new ClassPathXmlApplicationContext("com/chj/spring/bean.xml");  
-         context.close();  
-     }  
- }  
Spring配置文件：
**[html]**[view plain](http://blog.csdn.net/caihaijiang/article/details/8629725#)[copy](http://blog.csdn.net/caihaijiang/article/details/8629725#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlns="http://www.springframework.org/schema/beans"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xmlns:context="http://www.springframework.org/schema/context"
- xsi:schemaLocation="http://www.springframework.org/schema/beans    
-  http://www.springframework.org/schema/beans/spring-beans-2.5.xsd    
-  http://www.springframework.org/schema/context    
-  http://www.springframework.org/schema/context/spring-context-2.5.xsd">
- 
- <context:annotation-config/>
- 
- <beanid="initAndDestroySeqBean"class="com.chj.spring.InitAndDestroySeqBean"init-method="initMethod"destroy-method="destroyMethod"/>
- </beans>
运行InitAndDestroySeqBean的main方法，结果如下：
**[html]**[view plain](http://blog.csdn.net/caihaijiang/article/details/8629725#)[copy](http://blog.csdn.net/caihaijiang/article/details/8629725#)
- 2013-03-03 17:11:19,098 DEBUG support.DefaultListableBeanFactory - Creating instance of bean 'initAndDestroySeqBean'  
- 执行InitAndDestroySeqBean: 构造方法  
- 2013-03-03 17:11:19,114 DEBUG annotation.CommonAnnotationBeanPostProcessor - Found init method on class [com.alibaba.chj.spring.InitAndDestroySeqBean]: public void com.alibaba.chj.spring.InitAndDestroySeqBean.postConstruct()  
- 2013-03-03 17:11:19,114 DEBUG annotation.CommonAnnotationBeanPostProcessor - Found destroy method on class [com.alibaba.chj.spring.InitAndDestroySeqBean]: public void com.alibaba.chj.spring.InitAndDestroySeqBean.preDestroy()  
- 2013-03-03 17:11:19,129 DEBUG support.DefaultListableBeanFactory - Eagerly caching bean 'initAndDestroySeqBean' to allow for resolving potential circular references  
- 2013-03-03 17:11:19,129 DEBUG annotation.CommonAnnotationBeanPostProcessor - Invoking init method on bean 'initAndDestroySeqBean': public void com.alibaba.chj.spring.InitAndDestroySeqBean.postConstruct()  
- 执行InitAndDestroySeqBean: postConstruct  
- 2013-03-03 17:11:19,129 DEBUG support.DefaultListableBeanFactory - Invoking afterPropertiesSet() on bean with name 'initAndDestroySeqBean'  
- 执行InitAndDestroySeqBean: afterPropertiesSet  
- 2013-03-03 17:11:19,129 DEBUG support.DefaultListableBeanFactory - Invoking init method  'initMethod' on bean with name 'initAndDestroySeqBean'  
- 执行InitAndDestroySeqBean: init-method  
- 2013-03-03 17:11:19,129 DEBUG support.DefaultListableBeanFactory - Finished creating instance of bean 'initAndDestroySeqBean'  
- 2013-03-03 17:11:19,129 INFO  support.ClassPathXmlApplicationContext - Closing org.springframework.context.support.ClassPathXmlApplicationContext@56a499: display name [org.springframework.context.support.ClassPathXmlApplicationContext@56a499]; startup date [Sun Mar 03 17:11:17 CST 2013]; root of context hierarchy  
- 2013-03-03 17:11:19,129 INFO  support.DefaultListableBeanFactory - Destroying singletons in org.springframework.beans.factory.support.DefaultListableBeanFactory@1292d26: defining beans [org.springframework.context.annotation.internalCommonAnnotationProcessor,org.springframework.context.annotation.internalAutowiredAnnotationProcessor,org.springframework.context.annotation.internalRequiredAnnotationProcessor,initAndDestroySeqBean]; root of factory hierarchy  
- 2013-03-03 17:11:19,129 DEBUG annotation.CommonAnnotationBeanPostProcessor - Invoking destroy method on bean 'initAndDestroySeqBean': public void com.alibaba.chj.spring.InitAndDestroySeqBean.preDestroy()  
- 执行InitAndDestroySeqBean: preDestroy  
- 2013-03-03 17:11:19,145 DEBUG support.DisposableBeanAdapter - Invoking destroy() on bean with name 'initAndDestroySeqBean'  
- 执行InitAndDestroySeqBean: destroy  
- 2013-03-03 17:11:19,145 DEBUG support.DisposableBeanAdapter - Invoking destroy method 'destroyMethod' on bean with name 'initAndDestroySeqBean'  
- 执行InitAndDestroySeqBean: destroy-method  
从执行结果可以看出：
Bean在实例化的过程中：**Constructor > @PostConstruct >InitializingBean > init-method**
Bean在销毁的过程中：**@PreDestroy > DisposableBean > destroy-method**
