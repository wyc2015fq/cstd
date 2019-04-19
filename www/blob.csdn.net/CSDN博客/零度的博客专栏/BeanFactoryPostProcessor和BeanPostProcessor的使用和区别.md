# BeanFactoryPostProcessor和BeanPostProcessor的使用和区别 - 零度的博客专栏 - CSDN博客
2016年04月06日 13:37:42[零度anngle](https://me.csdn.net/zmx729618)阅读数：482
BeanFactoryPostProcessor和BeanPostProcessor都是Spring初始化bean的扩展点，两个接口非常相似。BeanFactoryPostProcessor可以对bean的定义（配置元数据）进行处理。也就是说，Spring IoC容器允许BeanFactoryPostProcessor在容器实际实例化任何其它的bean之前读取配置元数据，并有可能修改它。如果你愿意，你可以配置多个BeanFactoryPostProcessor。你还能通过设置'order'属性来控制BeanFactoryPostProcessor的执行次序。主要区别就是： BeanFactoryPostProcessor可以修改BEAN的配置信息而BeanPostProcessor不能。
下面举个例子说明
定义BEAN类：
```java
package com.springdemo.postProcessor;
public class PostProcessorBean {
	private String username;
	
	private String password;
	public String getPassword() {
		return password;
	}
	public void setPassword(String password) {
		this.password = password;
	}
	public String getUsername() {
		return username;
	}
	public void setUsername(String username) {
		this.username = username;
	}
}
```
定义MyBeanPostProcessor类，实现了BeanPostProcessor接口:
```java
package com.springdemo.postProcessor;
import org.springframework.beans.BeansException;
import org.springframework.beans.factory.config.BeanPostProcessor;
import com.springdemo.form.LoginForm;
public class MyBeanPostProcessor implements BeanPostProcessor {
	
	
	public Object postProcessAfterInitialization(Object bean, String beanName)
			throws BeansException {
		// TODO Auto-generated method stub
		//如果是PostProcessorBean则username信息
		if(bean instanceof PostProcessorBean)
		{
			System.out.println("PostProcessorBean Bean initialized");
			PostProcessorBean pb = (PostProcessorBean)bean;
			
			System.out.println("username:"+pb.getUsername());
		}
		return bean;
	}
	public Object postProcessBeforeInitialization(Object bean, String beanName)
			throws BeansException {
		// TODO Auto-generated method stub
		if(bean instanceof PostProcessorBean)
		{
			System.out.println("PostProcessorBean Bean initializing");
			PostProcessorBean pb = (PostProcessorBean)bean;
			
			System.out.println("username:"+pb.getUsername());
		}
		return bean;
	}
}
```
定义MyBeanFactoryPostProcessor实现了BeanFactoryPostProcessor接口:
```java
package com.springdemo.postProcessor;
import org.springframework.beans.BeansException;
import org.springframework.beans.MutablePropertyValues;
import org.springframework.beans.factory.config.BeanDefinition;
import org.springframework.beans.factory.config.BeanFactoryPostProcessor;
import org.springframework.beans.factory.config.ConfigurableListableBeanFactory;
public class MyBeanFactoryPostProcessor implements BeanFactoryPostProcessor {
	public void postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory)
			throws BeansException {
		// TODO Auto-generated method stub
		//BeanFactoryPostProcessor可以修改BEAN的配置信息而BeanPostProcessor不能
		//我们在这里修改postProcessorBean的username注入属性
		BeanDefinition bd = beanFactory.getBeanDefinition("postProcessorBean");
		MutablePropertyValues pv =  bd.getPropertyValues();
		if(pv.contains("username"))
		{
			pv.addPropertyValue("username", "xiaojun");
		}
		
	}
}
```
测试用例：
```java
package com.springdemo.test;
import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;
import com.springdemo.factory.ApplicationContextFactory;
import com.springdemo.postProcessor.PostProcessorBean;
import junit.framework.TestCase;
public class BeanPostPorcessorTest extends TestCase {
	private ApplicationContext context;
	protected void setUp() throws Exception {
		super.setUp();
		String[] paths = {"classpath*:/spring/applicationContext-*.xml"};
		context = new ClassPathXmlApplicationContext(paths);
		
	}
	protected void tearDown() throws Exception {
		super.tearDown();
	}
	
	public void testBeanPostProcessor()
	{
		
	}
	public void testBeanFactoryPostProcessor()
	{
		//PostProcessorBean bean =(PostProcessorBean)ServiceLocator.getService("postProcessorBean");
		PostProcessorBean bean =(PostProcessorBean)context.getBean("postProcessorBean");
		System.out.println("---------------testBeanFactoryPostProcessor----------------");
		System.out.println("username:"+bean.getUsername());
		System.out.println("password:"+bean.getPassword());
		//
	}
}
```
spring配置文件如下（先不启用MyBeanFactoryPostProcessor）：
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-2.0.xsd">
	
	<bean class="com.springdemo.postProcessor.MyBeanPostProcessor"></bean>
	<!--我们先把BeanFactoryPostProcessor注释掉，不启用,然后查看测试输出结果  
	<bean class="com.springdemo.postProcessor.MyBeanFactoryPostProcessor"></bean>
	-->
	<bean id="postProcessorBean" class="com.springdemo.postProcessor.PostProcessorBean" >
		<property name="username" value="test"></property>
		<property name="password" value="test"></property>
	</bean>
</beans>
```
测试输出结果如下：
PostProcessorBean Bean initializing
username:test
PostProcessorBean Bean initialized
username:test
---------------testBeanFactoryPostProcessor----------------
username:test
password:test
然后我们取消注释启用MyBeanFactoryPostProcessor，测试结果如下：
PostProcessorBean Bean initializing
username:xiaojun
PostProcessorBean Bean initialized
username:xiaojun
---------------testBeanFactoryPostProcessor----------------
username:xiaojun
password:test
从结果可以看出：BeanFactoryPostProcessor的回调比BeanPostProcessor要早，因为BeanPostProcess中输出的username已经变成了xiaojun,而不是test.还有就是BeanFactoryPostProcessor确实有能力改变初始化BEAN的内容，您可以试试在MyBeanPostProcess中试一试set一下username看看能不能改变BEAN实例的内容(答案应该是否定的).
