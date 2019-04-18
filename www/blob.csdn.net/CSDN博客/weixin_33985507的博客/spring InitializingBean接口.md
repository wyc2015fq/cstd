# spring InitializingBean接口 - weixin_33985507的博客 - CSDN博客
2016年02月24日 16:12:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
最近工作需要得到sping中的每个事物需要执行的sql，称机会简单研究了一下spring的事务，项目中管理事务比较简单，用TransactionTemplate，就直接以TransactionTemplate为入口开始学习。 
TransactionTemplate的源码如下： 
public class TransactionTemplate extends DefaultTransactionDefinition 
        implements TransactionOperations, InitializingBean{ 
        . 
        . 
        . 
        } 
TransactionTemplate继承了DefaultTransactionDefinition，实现了TransactionOperations，InitializingBean接口。先研究InitializingBean接口 
InitializingBean接口为bean提供了初始化方法的方式，它只包括afterPropertiesSet方法，凡是继承该接口的类，在初始化bean的时候会执行该方法。 
测试程序如下： 
import org.springframework.beans.factory.InitializingBean; 
public class TestInitializingBean implements InitializingBean{ 
    @Override 
    public void afterPropertiesSet() throws Exception { 
        System.out.println("ceshi InitializingBean");         
    } 
    public void testInit(){ 
        System.out.println("ceshi init-method");         
    } 
} 
配置文件如下： 
    xmlns:context="http://www.springframework.org/schema/context" 
    xmlns:jdbc="http://www.springframework.org/schema/jdbc" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"         
    xmlns:p="http://www.springframework.org/schema/p" xmlns:aop="http://www.springframework.org/schema/aop" 
    xmlns:tx="http://www.springframework.org/schema/tx" 
    xsi:schemaLocation=" 
    http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-3.0.xsd"> 
Main主程序如下： 
public class Main { 
    public static void main(String[] args){ 
        ApplicationContext context = new FileSystemXmlApplicationContext("/src/main/java/com/beans.xml"); 
    } 
} 
运行Main程序，打印如下结果： 
ceshi InitializingBean   
这说明在spring初始化bean的时候，如果bean实现了InitializingBean接口，会自动调用afterPropertiesSet方法。 
问题：实现InitializingBean接口与在配置文件中指定init-method有什么不同？ 
修改配置文件，加上init-method配置，修改如下： 
    xmlns:context="http://www.springframework.org/schema/context" 
    xmlns:jdbc="http://www.springframework.org/schema/jdbc" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"         
    xmlns:p="http://www.springframework.org/schema/p" xmlns:aop="http://www.springframework.org/schema/aop" 
    xmlns:tx="http://www.springframework.org/schema/tx" 
    xsi:schemaLocation=" 
    http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-3.0.xsd"> 
在配置文件中加入init-method="testInit"。 
运行Main程序，打印如下结果： 
ceshi InitializingBean 
ceshi init-method 
由结果可看出，在spring初始化bean的时候，如果该bean是实现了InitializingBean接口，并且同时在配置文件中指定了init-method，系统则是先调用afterPropertiesSet方法，然后在调用init-method中指定的方法。 
这方式在spring中是怎么实现的？ 
通过查看spring的加载bean的源码类(AbstractAutowireCapableBeanFactory)可看出其中奥妙 
AbstractAutowireCapableBeanFactory类中的invokeInitMethods讲解的非常清楚，源码如下： 
protected void invokeInitMethods(String beanName, final Object bean, RootBeanDefinition mbd) 
            throws Throwable { 
//判断该bean是否实现了实现了InitializingBean接口，如果实现了InitializingBean接口，则只掉调用bean的afterPropertiesSet方法         
        boolean isInitializingBean = (bean instanceof InitializingBean); 
        if (isInitializingBean && (mbd == null || !mbd.isExternallyManagedInitMethod("afterPropertiesSet"))) { 
            if (logger.isDebugEnabled()) { 
                logger.debug("Invoking afterPropertiesSet() on bean with name '" + beanName + "'"); 
            } 
            if (System.getSecurityManager() != null) { 
                try { 
                    AccessController.doPrivileged(new PrivilegedExceptionAction 
                        public Object run() throws Exception { 
   //直接调用afterPropertiesSet 
                            ((InitializingBean) bean).afterPropertiesSet(); 
                            return null; 
                        } 
                    },getAccessControlContext()); 
                } catch (PrivilegedActionException pae) { 
                    throw pae.getException(); 
                } 
            }                 
            else { 
//直接调用afterPropertiesSet 
                ((InitializingBean) bean).afterPropertiesSet(); 
            } 
        } 
        if (mbd != null) { 
            String initMethodName = mbd.getInitMethodName(); 
 //判断是否指定了init-method方法，如果指定了init-method方法，则再调用制定的init-method 
            if (initMethodName != null && !(isInitializingBean && "afterPropertiesSet".equals(initMethodName)) && 
                    !mbd.isExternallyManagedInitMethod(initMethodName)) { 
//进一步查看该方法的源码，可以发现init-method方法中指定的方法是通过反射实现 
                invokeCustomInitMethod(beanName, bean, mbd); 
            } 
        } 
    } 
总结： 
1：spring为bean提供了两种初始化bean的方式，实现InitializingBean接口，实现afterPropertiesSet方法，或者在配置文件中同过init-method指定，两种方式可以同时使用 
2：实现InitializingBean接口是直接调用afterPropertiesSet方法，比通过反射调用init-method指定的方法效率相对来说要高点。但是init-method方式消除了对spring的依赖 
3：如果调用afterPropertiesSet方法时出错，则不调用init-method指定的方法。 
4：TransactionTemplate实现InitializingBean接口，主要是判断transactionManager是否已经初始化，如果没有则抛出异常。源码如下： 
     public void afterPropertiesSet() { 
        if (this.transactionManager == null) { 
            throw new IllegalArgumentException("Property 'transactionManager' is required"); 
        } 
    } 
