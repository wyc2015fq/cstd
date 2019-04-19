# Spring的﻿﻿IntializingBean和Init-method的应用 - 零度的博客专栏 - CSDN博客
2016年04月06日 10:47:48[零度anngle](https://me.csdn.net/zmx729618)阅读数：967
#### InitializingBean
Spirng的InitializingBean为bean提供了定义初始化方法的方式。InitializingBean是一个接口，它仅仅包含一个方法：afterPropertiesSet()。在spring 初始化后，执行完所有属性设置方法(即setXxx)将自动调用 afterPropertiesSet(), 在配置文件中无须特别的配置， 但此方式增加了bean对spring 的依赖，应该尽量避免使用
Spirng的InitializingBean为bean提供了定义初始化方法的方式。InitializingBean是一个接口，它仅仅包含一个方法：afterPropertiesSet()。
public interface InitializingBean
{
    public abstract void afterPropertiesSet()
        throws Exception;
}
Bean实现这个接口，在afterPropertiesSet()中编写初始化代码：
package research.spring.beanfactory.ch4;import org.springframework.beans.factory.InitializingBean;publicclass LifeCycleBean implements InitializingBean{publicvoid afterPropertiesSet() throws Exception {System.out.println("LifeCycleBean initializing...");}}
在xml配置文件中并不需要对bean进行特殊的配置：
xml version="1.0" encoding="UTF-8"?>DOCTYPE beans PUBLIC "-//SPRING//DTD BEAN//EN""http://www.springframework.org/dtd/spring-beans.dtd"><beans><bean name="lifeBean" class="research.spring.beanfactory.ch4.LifeCycleBean">bean>beans>
编写测试程序进行测试：
package research.spring.beanfactory.ch4;import org.springframework.beans.factory.xml.XmlBeanFactory;import org.springframework.core.io.ClassPathResource;publicclass LifeCycleTest {publicstaticvoid main(String[] args) {XmlBeanFactory factory=new XmlBeanFactory(new ClassPathResource(
"research/spring/beanfactory/ch4/context.xml"));factory.getBean("lifeBean");}}
    运行上面的程序我们会看到：“LifeCycleBean initializing...”，这说明bean的afterPropertiesSet已经被Spring调用了。
    Spring在设置完一个bean所有的合作者后，会检查bean是否实现了InitializingBean接口，如果实现就调用bean的afterPropertiesSet方法。
装配**bean的合作者**
查看**bean是否实现**InitializingBean**接口**
调用**afterPropertiesSet**方法
#### init-method
    Spring虽然可以通过InitializingBean完成一个bean初始化后对这个bean的回调，但是这种方式要求bean实现 InitializingBean接口。一但bean实现了InitializingBean接口，那么这个bean的代码就和Spring耦合到一起了。通常情况下我不鼓励bean直接实现InitializingBean，可以使用Spring提供的init-method的功能来执行一个bean 子定义的初始化方法。
写一个java class，这个类不实现任何Spring的接口。定义一个没有参数的方法init()。
package research.spring.beanfactory.ch4;publicclass LifeCycleBean{publicvoid init(){System.out.println("LifeCycleBean.init...");}}
在Spring中配置这个bean：
xml version="1.0" encoding="UTF-8"?>DOCTYPE beans PUBLIC "-//SPRING//DTD BEAN//EN""http://www.springframework.org/dtd/spring-beans.dtd"><beans><bean name="lifeBean" class="research.spring.beanfactory.ch4.LifeCycleBean"
 init-method="init">bean>beans>
当Spring实例化lifeBean时，你会在控制台上看到” LifeCycleBean.init...”。
Spring要求init-method是一个无参数的方法，如果init-method指定的方法中有参数，那么Spring将会抛出java.lang.NoSuchMethodException
init-method指定的方法可以是public、protected以及private的，并且方法也可以是final的。
init-method指定的方法可以是声明为抛出异常的，就像这样：
       final protected void init() throws Exception{
           System.out.println("init method...");
           if(true) throw new Exception("init exception");
    }
如果在init-method方法中抛出了异常，那么Spring将中止这个Bean的后续处理，并且抛出一个org.springframework.beans.factory.BeanCreationException异常。
InitializingBean和init-method可以一起使用，Spring会先处理InitializingBean再处理init-method。
org.springframework.beans.factory.support. AbstractAutowireCapableBeanFactory完成一个Bean初始化方法的调用工作。 AbstractAutowireCapableBeanFactory是XmlBeanFactory的超类，再 AbstractAutowireCapableBeanFactory的invokeInitMethods方法中实现调用一个Bean初始化方法：
**org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.java:**
//……//在一个bean的合作者设备完成后，执行一个bean的初始化方法。
protectedvoid invokeInitMethods(String beanName, Object bean, RootBeanDefinition mergedBeanDefinition)
 throws Throwable 
{
//判断bean是否实现了InitializingBean接口
if (bean instanceof InitializingBean) 
{if (logger.isDebugEnabled()) 
{
   logger.debug("Invoking afterPropertiesSet() on bean with name ‘"+ beanName +"‘");
}
//调用afterPropertiesSet方法
   ((InitializingBean) bean).afterPropertiesSet();
}
//判断bean是否定义了init-method
if(mergedBeanDefinition!=null&&mergedBeanDefinition.getInitMethodName() !=null) 
{
    //调用invokeCustomInitMethod方法来执行init-method定义的方法
    invokeCustomInitMethod(beanName, bean, mergedBeanDefinition.getInitMethodName());
}
}
//执行一个bean定义的init-method方法p
rotectedvoid invokeCustomInitMethod(String beanName, Object bean, String initMethodName)throws Throwable 
{
if (logger.isDebugEnabled()) 
logger.debug("Invoking custom init method ‘"+ initMethodName +"‘ on bean with name ‘"+ beanName +"‘");
}
//使用方法名，反射Method对象
Method initMethod = BeanUtils.findMethod(bean.getClass(), initMethodName, null);
if (initMethod ==null) 
{
thrownew NoSuchMethodException(
"Couldn‘t find an init method named ‘"+ initMethodName +"‘ on bean with name ‘"+ beanName +"‘");
}
//判断方法是否是public
if (!Modifier.isPublic(initMethod.getModifiers())) {//设置accessible为true，可以访问private方法。                     initMethod.setAccessible(true);}
try {
//反射执行这个方法initMethod.invoke(bean, (Object[]) null);
}catch (InvocationTargetException ex) 
{throw ex.getTargetException();}}//………..
    通过分析上面的源代码我们可以看到，init-method是通过反射执行的，而afterPropertiesSet是直接执行的。所以 afterPropertiesSet的执行效率比init-method要高，不过init-method消除了bean对Spring依赖。在实际使用时我推荐使用init-method。
    需要注意的是Spring总是先处理bean定义的InitializingBean，然后才处理init-method。如果在Spirng处理InitializingBean时出错，那么Spring将直接抛出异常，不会再继续处理init-method。
    如果一个bean被定义为非单例的，那么afterPropertiesSet和init-method在bean的每一个实例被创建时都会执行。单例 bean的afterPropertiesSet和init-method只在bean第一次被实例时调用一次。大多数情况下 afterPropertiesSet和init-method都应用在单例的bean上。
