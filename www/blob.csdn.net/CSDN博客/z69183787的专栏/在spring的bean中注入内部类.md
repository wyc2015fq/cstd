# 在spring的bean中注入内部类 - z69183787的专栏 - CSDN博客
2016年02月01日 15:14:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2614
在spring中注入内部类，有可能会遇到如下异常信息：
2014-5-1421:52:45 org.springframework.context.support.AbstractApplicationContext prepareRefresh
信息:Refreshing org.springframework.context.support.ClassPathXmlApplicationContext@1c56c60: startup date [WedMay1421:52:45 CST 2014]; root of context hierarchy
2014-5-1421:52:45 org.springframework.beans.factory.xml.XmlBeanDefinitionReader loadBeanDefinitions
信息:Loading XML bean definitions fromclass path resource [spring.xml]2014-5-1421:52:46 org.springframework.beans.factory.support.DefaultListableBeanFactory preInstantiateSingletons
信息:Pre-instantiating singletons in org.springframework.beans.factory.support.DefaultListableBeanFactory@7244ca: defining beans [person]; root of factory hierarchy
2014-5-1421:52:46 org.springframework.beans.factory.support.DefaultSingletonBeanRegistry destroySingletons
信息:Destroying singletons in org.springframework.beans.factory.support.DefaultListableBeanFactory@7244ca: defining beans [person]; root of factory hierarchy
Exceptionin thread "main" org.springframework.beans.factory.BeanCreationException:Error creating bean with name 'person'definedinclass path resource [spring.xml]:Cannot create inner bean 'cn.outofmemory.spring.Person$Hand#ab7165' of type [cn.outofmemory.spring.Person$Hand]while setting bean property'hands'with key [0]; nested exception is org.springframework.beans.factory.BeanCreationException:Error creating bean with name 'cn.outofmemory.spring.Person$Hand#ab7165'definedinclass path resource [spring.xml]:Instantiation of bean failed; nested exception is org.springframework.beans.BeanInstantiationException:Couldnot instantiate bean class[cn.outofmemory.spring.Person$Hand]:Nodefault constructor found; nested exception is java.lang.NoSuchMethodException: cn.outofmemory.spring.Person$Hand.<init>()
	at org.springframework.beans.factory.support.BeanDefinitionValueResolver.resolveInnerBean(BeanDefinitionValueResolver.java:281)
	at org.springframework.beans.factory.support.BeanDefinitionValueResolver.resolveValueIfNecessary(BeanDefinitionValueResolver.java:120)
	at org.springframework.beans.factory.support.BeanDefinitionValueResolver.resolveManagedList(BeanDefinitionValueResolver.java:353)
	at org.springframework.beans.factory.support.BeanDefinitionValueResolver.resolveValueIfNecessary(BeanDefinitionValueResolver.java:153)
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.applyPropertyValues(AbstractAutowireCapableBeanFactory.java:1325)
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.populateBean(AbstractAutowireCapableBeanFactory.java:1086)
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.doCreateBean(AbstractAutowireCapableBeanFactory.java:517)
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.createBean(AbstractAutowireCapableBeanFactory.java:456)
	at org.springframework.beans.factory.support.AbstractBeanFactory$1.getObject(AbstractBeanFactory.java:291)
	at org.springframework.beans.factory.support.DefaultSingletonBeanRegistry.getSingleton(DefaultSingletonBeanRegistry.java:222)
	at org.springframework.beans.factory.support.AbstractBeanFactory.doGetBean(AbstractBeanFactory.java:288)
	at org.springframework.beans.factory.support.AbstractBeanFactory.getBean(AbstractBeanFactory.java:190)
	at org.springframework.beans.factory.support.DefaultListableBeanFactory.preInstantiateSingletons(DefaultListableBeanFactory.java:580)
	at org.springframework.context.support.AbstractApplicationContext.finishBeanFactoryInitialization(AbstractApplicationContext.java:895)
	at org.springframework.context.support.AbstractApplicationContext.refresh(AbstractApplicationContext.java:425)
	at org.springframework.context.support.ClassPathXmlApplicationContext.<init>(ClassPathXmlApplicationContext.java:139)
	at org.springframework.context.support.ClassPathXmlApplicationContext.<init>(ClassPathXmlApplicationContext.java:83)
	at cn.outofmemory.spring.App.main(App.java:14)Causedby: org.springframework.beans.factory.BeanCreationException:Error creating bean with name 'cn.outofmemory.spring.Person$Hand#ab7165'definedinclass path resource [spring.xml]:Instantiation of bean failed; nested exception is org.springframework.beans.BeanInstantiationException:Couldnot instantiate bean class[cn.outofmemory.spring.Person$Hand]:Nodefault constructor found; nested exception is java.lang.NoSuchMethodException: cn.outofmemory.spring.Person$Hand.<init>()
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.instantiateBean(AbstractAutowireCapableBeanFactory.java:965)
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.createBeanInstance(AbstractAutowireCapableBeanFactory.java:911)
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.doCreateBean(AbstractAutowireCapableBeanFactory.java:485)
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.createBean(AbstractAutowireCapableBeanFactory.java:456)
	at org.springframework.beans.factory.support.BeanDefinitionValueResolver.resolveInnerBean(BeanDefinitionValueResolver.java:270)...17 more
Causedby: org.springframework.beans.BeanInstantiationException:Couldnot instantiate bean class[cn.outofmemory.spring.Person$Hand]:Nodefault constructor found; nested exception is java.lang.NoSuchMethodException: cn.outofmemory.spring.Person$Hand.<init>()
	at org.springframework.beans.factory.support.SimpleInstantiationStrategy.instantiate(SimpleInstantiationStrategy.java:70)
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.instantiateBean(AbstractAutowireCapableBeanFactory.java:958)...21 more
Causedby: java.lang.NoSuchMethodException: cn.outofmemory.spring.Person$Hand.<init>()
	at java.lang.Class.getConstructor0(Class.java:2715)
	at java.lang.Class.getDeclaredConstructor(Class.java:1987)
	at org.springframework.beans.factory.support.SimpleInstantiationStrategy.instantiate(SimpleInstantiationStrategy.java:65)...22 more
这个异常信息很长，他的意思是说我们没有给内部类指定构造函数，我们看下我们的代码和spring配置文件：
我们定义了一个Person类，这个类中有一个内部类Hand，Person类有一个hands的属性的类型是内部类数组
```java
package cn.outofmemory.spring;
public class Person {	
	private Hand[] hands;
	
	public Hand[] getHands() {
		return hands;
	}
	public void setHands(Hand[] hands) {
		this.hands = hands;
	}
	public class Hand {
		private int strength;
		public int getStrength() {
			return strength;
		}
		public void setStrength(int strength) {
			this.strength = strength;
		}
		
	}
}
```
我们的spring配置文件如下：
<?xml version="1.0" encoding="UTF-8"?><beansxmlns="http://www.springframework.org/schema/beans"xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xsi:schemaLocation="http://www.springframework.org/schema/beans
           http://www.springframework.org/schema/beans/spring-beans.xsd"><beanclass="cn.outofmemory.spring.Person"id="person"><propertyname="hands"><list><beanclass="cn.outofmemory.spring.Person$Hand"><propertyname="strength"value="90"/></bean></list></property></bean></beans>
配置文件很简单，内部类的类名由外部类的全称+$+内部类的名称，这是无疑的。
## 内部类注入方式一：添加内部类默认构造函数参数
**我们遇到上面的错误是因为非静态的内部类默认的构造函数有一个参数，这个参数指向其外部类的实例，所以我们需要给此内部类的bean添加constructor-arg节点，并指向外部类即可，我们修改下配置文件：**
<?xml version="1.0" encoding="UTF-8"?><beansxmlns="http://www.springframework.org/schema/beans"xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xsi:schemaLocation="http://www.springframework.org/schema/beans
           http://www.springframework.org/schema/beans/spring-beans.xsd"><beanclass="cn.outofmemory.spring.Person"id="person"><propertyname="hands"><list><beanclass="cn.outofmemory.spring.Person$Hand"><constructor-argref="person"></constructor-arg><propertyname="strength"value="90"/></bean></list></property></bean></beans>
App类代码如下：
package cn.outofmemory.spring;import org.springframework.context.ApplicationContext;import org.springframework.context.support.ClassPathXmlApplicationContext;/**
 * Hello  spring from OutOfMemory.CN
 *
 */publicclassApp{publicstaticvoid main(String[] args ){ApplicationContext appContext =newClassPathXmlApplicationContext("/spring.xml");Person person = appContext.getBean(Person.class);for(Person.Hand hand : person.getHands()){System.out.println("hand strength is "+ hand.getStrength());}}}
App类的main方法获得spring中定义的person类，然后打印每一个hand的strength属性。输出如下：
hand strength is90
## 内部类注入方式二：将内部类修改为static
对于内部类，如果没必要访问外部类，我们可以将其定义为static的，这样在spring配置文件中配置时，就不需要设置默认构造函数了。
如下修改后的Person类：
```java
package cn.outofmemory.spring;
public class Person {	
	private Hand[] hands;
	
	public Hand[] getHands() {
		return hands;
	}
	public void setHands(Hand[] hands) {
		this.hands = hands;
	}
	public static class Hand {
		private int strength;
		public int getStrength() {
			return strength;
		}
		public void setStrength(int strength) {
			this.strength = strength;
		}
		
	}
}
```
修改后的spring配置文件：
<?xml version="1.0" encoding="UTF-8"?><beansxmlns="http://www.springframework.org/schema/beans"xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xsi:schemaLocation="http://www.springframework.org/schema/beans
           http://www.springframework.org/schema/beans/spring-beans.xsd"><beanclass="cn.outofmemory.spring.Person"id="person"><propertyname="hands"><list><beanclass="cn.outofmemory.spring.Person$Hand"><propertyname="strength"value="90"/></bean></list></property></bean></beans>
App类保持不变，我们再次运行程序，依然会得到如下的输出结果：
hand strength is90
## 总结：
对于内部类的注入，要注意非静态内部类，其默认构造函数有一个参数，是其外部类的实例，记住这一点在spring中定义内部类就没有问题了。
如果内部类不需要访问外部类的实例，可以将其定义为static的，这样也就不需要额外的构造函数参数设置了。
