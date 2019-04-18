# spring的proxy-target-class详解 - z69183787的专栏 - CSDN博客
2018年03月07日 12:28:29[OkidoGreen](https://me.csdn.net/z69183787)阅读数：212
个人分类：[Spring-Aop & 代理](https://blog.csdn.net/z69183787/article/category/2175403)
[http://blog.csdn.net/shaoweijava/article/details/76474652](http://blog.csdn.net/shaoweijava/article/details/76474652)
proxy-target-class属性值决定是基于接口的还是基于类的代理被创建。首先说明下proxy-target-class="true"和proxy-target-class="false"的区别，为true则是基于类的代理将起作用（需要cglib库），为false或者省略这个属性，则标准的JDK 基于接口的代理将起作用。
proxy-target-class在spring事务、aop、缓存这几块都有设置，其作用都是一样的。
[html][view
 plain](http://csdnimg.cn/release/phoenix/#)[copy](http://csdnimg.cn/release/phoenix/#)
- <tx:annotation-driventransaction-manager="transactionManager"proxy-target-class="true"/>
- <aop:configproxy-target-class="true">
- <cache:annotation-drivenproxy-target-class="true"/>
下面我们就aop对proxy-target-class属性进行分析
分析之前先把相关测试类列举下：
applicationContext-test-aop.xml文件
[html][view
 plain](http://csdnimg.cn/release/phoenix/#)[copy](http://csdnimg.cn/release/phoenix/#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlns="http://www.springframework.org/schema/beans"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xmlns:context="http://www.springframework.org/schema/context"
- xmlns:aop="http://www.springframework.org/schema/aop"xmlns:tx="http://www.springframework.org/schema/tx"
- xmlns:mvc="http://www.springframework.org/schema/mvc"xmlns:p="http://www.springframework.org/schema/p"
- xsi:schemaLocation="http://www.springframework.org/schema/beans  
-                            http://www.springframework.org/schema/beans/spring-beans-3.2.xsd  
-                            http://www.springframework.org/schema/context  
-                            http://www.springframework.org/schema/context/spring-context-3.2.xsd  
-                            http://www.springframework.org/schema/aop  
-                            http://www.springframework.org/schema/aop/spring-aop-3.2.xsd">
- 
- <!-- 激活spring的注解. -->
- <context:annotation-config/>
- 
- <context:component-scanbase-package="cn.sw.study.common.test.spring.aop"/>
- 
- <aop:configproxy-target-class="true">
- <aop:aspectid="log"ref="logHandler">
- <aop:pointcutid="printLog"expression="execution(* cn.sw.study.common.test.spring.aop.service..*(..))"/>
- <aop:beforemethod="LogBefore"pointcut-ref="printLog"/>
- <aop:aftermethod="LogAfter"pointcut-ref="printLog"/>
- </aop:aspect>
- </aop:config>
- </beans>
UserService.java文件
[java][view
 plain](http://csdnimg.cn/release/phoenix/#)[copy](http://csdnimg.cn/release/phoenix/#)
- package cn.sw.study.common.test.spring.aop.service;  
- 
- /**
-  * 用户业务
-  * Created by shaowei on 2017/7/31.
-  */
- publicinterface UserService {  
- void addUser();  
- }  
UserServiceImpl.java文件
[java][view
 plain](http://csdnimg.cn/release/phoenix/#)[copy](http://csdnimg.cn/release/phoenix/#)
- package cn.sw.study.common.test.spring.aop.service;  
- 
- import org.springframework.stereotype.Service;  
- 
- /**
-  * Created by shaowei on 2017/7/31.
-  */
- @Service
- publicclass UserServiceImpl implements UserService {  
- @Override
- publicvoid addUser() {  
-         System.out.println("add user");  
-     }  
- }  
LogHandler.java文件
[java][view
 plain](http://csdnimg.cn/release/phoenix/#)[copy](http://csdnimg.cn/release/phoenix/#)
- package cn.sw.study.common.test.spring.aop;  
- 
- import org.springframework.stereotype.Component;  
- 
- /**
-  * 日志处理类
-  * Created by shaowei on 2017/7/31.
-  */
- @Component
- publicclass LogHandler  
- {  
- publicvoid LogBefore()  
-     {  
-         System.out.println("Log before method");  
-     }  
- 
- publicvoid LogAfter()  
-     {  
-         System.out.println("Log after method");  
-     }  
- }  
AopTest.java文件
[java][view
 plain](http://csdnimg.cn/release/phoenix/#)[copy](http://csdnimg.cn/release/phoenix/#)
- package cn.sw.study.common.test.spring.aop;  
- 
- import cn.sw.study.common.test.spring.aop.service.UserService;  
- import cn.sw.study.common.test.spring.aop.service.UserServiceImpl;  
- import org.junit.Test;  
- import org.springframework.context.ApplicationContext;  
- import org.springframework.context.support.ClassPathXmlApplicationContext;  
- 
- /**
-  * AOP测试类
-  * Created by shaowei on 2017/7/31.
-  */
- publicclass AopTest {  
- @Test
- publicvoid testProxyTargetClass(){  
-         ApplicationContext context = new ClassPathXmlApplicationContext("applicationContext-test-aop.xml");  
- //        UserService userService = (UserService)context.getBean("userServiceImpl");
- 
- //proxy-target-class="true"，为false时会报转换错误
-         UserServiceImpl userService = (UserServiceImpl)context.getBean("userServiceImpl");  
-         userService.addUser();  
-     }  
- }  
运行测试类，查看结果，可以正常运行，aop处理类也正常被调用
此时修改proxy-target-class="false"，则报错
java.lang.ClassCastException:com.sun.proxy.$Proxy9 cannot be cast to
cn.sw.study.common.test.spring.aop.service.UserServiceImpl
下面我们来详细的分析下proxy-target-class属性
首先查看sping的aop依赖包，下载源码文件，找到spring.handlers文件
![](https://img-blog.csdn.net/20170801082204902?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhb3dlaWphdmE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
打开文件可以看到
http\://www.springframework.org/schema/aop=org.springframework.aop.config.AopNamespaceHandler
AopNamespaceHandler类，就是spring解析<aop:configproxy-target-class="true">配置的入口，打开这个类，则可以看到init方法里注册了ConfigBeanDefinitionParser类来解析，这个类则是实际解析<aop:config proxy-target-class="true">的类
registerBeanDefinitionParser("config",new ConfigBeanDefinitionParser());
打开这个类搜索proxy-target-class，则可以看到configureAutoProxyCreator方法，这个方法则是解析proxy-target-class属性的方法
[java][view
 plain](http://csdnimg.cn/release/phoenix/#)[copy](http://csdnimg.cn/release/phoenix/#)
- privatevoid configureAutoProxyCreator(ParserContext parserContext, Element element) {  
-    AopNamespaceUtils.registerAspectJAutoProxyCreatorIfNecessary(parserContext, element);  
- }  
进入这个方法，再进入useClassProxyingIfNecessary方法则可以看到
[java][view
 plain](http://csdnimg.cn/release/phoenix/#)[copy](http://csdnimg.cn/release/phoenix/#)
- boolean proxyTargetClass =   
- Boolean.valueOf(sourceElement.getAttribute(PROXY_TARGET_CLASS_ATTRIBUTE));  
- if (proxyTargetClass) {  
-    AopConfigUtils.forceAutoProxyCreatorToUseClassProxying(registry);  
- }  
为true则调用forceAutoProxyCreatorToUseClassProxying方法，强制基于类来创建代理，从上面代码可以看出，不设置则默认为false
[java][view
 plain](http://csdnimg.cn/release/phoenix/#)[copy](http://csdnimg.cn/release/phoenix/#)
- publicstaticvoid forceAutoProxyCreatorToUseClassProxying(BeanDefinitionRegistry registry) {  
- if (registry.containsBeanDefinition(AUTO_PROXY_CREATOR_BEAN_NAME)) {  
-       BeanDefinition definition = registry.getBeanDefinition(AUTO_PROXY_CREATOR_BEAN_NAME);  
-       definition.getPropertyValues().add("proxyTargetClass", Boolean.TRUE);  
-    }  
- }  
此处可以看到，在bean定义对象中设置了proxyTargetClass属性，后面spring获取bean创建代理类的时候，会判断此属性类决定使用JdkDynamicAopProxy还是ObjenesisCglibAopProxy代理
打开DefaultAopProxyFactory类，查看createAopProxy方法
[java][view
 plain](http://csdnimg.cn/release/phoenix/#)[copy](http://csdnimg.cn/release/phoenix/#)
- public AopProxy createAopProxy(AdvisedSupport config) throws AopConfigException {  
- if (config.isOptimize() || config.isProxyTargetClass() || hasNoUserSuppliedProxyInterfaces(config)) {  
-       Class<?> targetClass = config.getTargetClass();  
- if (targetClass == null) {  
- thrownew AopConfigException("TargetSource cannot determine target class: " +  
- "Either an interface or a target is required for proxy creation.");  
-       }  
- if (targetClass.isInterface()) {  
- returnnew JdkDynamicAopProxy(config);  
-       }  
- returnnew ObjenesisCglibAopProxy(config);  
-    }  
- else {  
- returnnew JdkDynamicAopProxy(config);  
-    }  
- }  
hasNoUserSuppliedProxyInterfaces方法则说明即使你未声明proxy-target-class="true" ，但运行类没有继承接口，spring也会自动使用CGLIB代理。
总结：高版本spring自动根据运行类选择JDK或CGLIB代理，我们无需设置proxy-target-class属性，JDK动态代理是模拟接口实现的方式，cglib是模拟子类继承的方式，一般采用前者，因为前者效率高。后者不建议使用。
