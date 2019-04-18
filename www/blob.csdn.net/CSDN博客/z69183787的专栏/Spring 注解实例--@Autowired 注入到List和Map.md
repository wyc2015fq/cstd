# Spring 注解实例--@Autowired 注入到List和Map - z69183787的专栏 - CSDN博客
2017年12月18日 16:39:08[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1102

**[java]**[view
 plain](http://blog.csdn.net/qq_33366229/article/details/72859282#)[copy](http://blog.csdn.net/qq_33366229/article/details/72859282#)
[print](http://blog.csdn.net/qq_33366229/article/details/72859282#)[?](http://blog.csdn.net/qq_33366229/article/details/72859282#)
- package com.Autowired.ListMap;  
- 
- import org.springframework.core.annotation.Order;  
- import org.springframework.stereotype.Component;  
- /**
-  * order：把实现类排序输出 只适合List
-  * @author liu
-  *
-  */
- @Order(2)  
- @Component
- publicclass BeanImplOne implements BeanInterface {  
- 
- }  
================================
**[java]**[view
 plain](http://blog.csdn.net/qq_33366229/article/details/72859282#)[copy](http://blog.csdn.net/qq_33366229/article/details/72859282#)
[print](http://blog.csdn.net/qq_33366229/article/details/72859282#)[?](http://blog.csdn.net/qq_33366229/article/details/72859282#)
- package com.Autowired.ListMap;  
- 
- import org.springframework.core.annotation.Order;  
- import org.springframework.stereotype.Component;  
- 
- @Order(1)  
- @Component
- publicclass BeanImplTwo implements BeanInterface {  
- 
- }  
=====================================
BeanInterface只是一个接口无方法
======================================
**[java]**[view
 plain](http://blog.csdn.net/qq_33366229/article/details/72859282#)[copy](http://blog.csdn.net/qq_33366229/article/details/72859282#)
[print](http://blog.csdn.net/qq_33366229/article/details/72859282#)[?](http://blog.csdn.net/qq_33366229/article/details/72859282#)
- package com.Autowired.ListMap;  
- 
- import java.util.List;  
- import java.util.Map;  
- 
- 
- import org.springframework.beans.factory.annotation.Autowired;  
- import org.springframework.beans.factory.annotation.Qualifier;  
- import org.springframework.stereotype.Component;  
- 
- @Component
- publicclass BeanInvoke {  
- 
- @Autowired
- private List<BeanInterface> list;  
- 
- @Autowired
- private Map<String,BeanInterface> map;  
- 
- /** @Autowired默认为byType的  所以有两个相同类型的bean  
-      * 如果不使用 @Qualifier指定具体的bean就会抛出异常
-      *  private BeanInterface beaninterface;
-      */
- @Autowired
- @Qualifier("beanImplOne")  
- private BeanInterface beaninterface;  
- publicvoid say(){  
-         System.out.println("list...");  
- if(null !=list &&0!=list.size()){  
- for(BeanInterface bean :list){  
-                 System.out.println(bean.getClass().getName());  
-             }  
- 
-         }else{  
-             System.out.println("List<BeanInterface> list is null !!!!");  
-         }  
-         System.out.println();  
-         System.out.println("map...");  
- if(null !=map &&0!=map.size()){  
- for(Map.Entry<String, BeanInterface> m:map.entrySet()){  
-                   System.out.println(m.getKey()+"    "+m.getValue().getClass().getName());  
-             }  
-         }else{  
-             System.out.println("Map<String,BeanInterface> map is null !!!!");  
- 
-         }  
-         System.out.println("-------------------------");  
- if(null !=beaninterface){  
-             System.out.println(beaninterface.getClass().getName());  
-         }else{  
-             System.out.println("beaninterface is null !!!");  
-         }  
-     }  
- 
- 
- }  
------------------------------------------------------
配置文件：
**[java]**[view
 plain](http://blog.csdn.net/qq_33366229/article/details/72859282#)[copy](http://blog.csdn.net/qq_33366229/article/details/72859282#)
[print](http://blog.csdn.net/qq_33366229/article/details/72859282#)[?](http://blog.csdn.net/qq_33366229/article/details/72859282#)
- <?xml version="1.0" encoding="UTF-8"?>  
- <beans xmlns="http://www.springframework.org/schema/beans"
-     xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
-     xmlns:context="http://www.springframework.org/schema/context"
-     xsi:schemaLocation="http://www.springframework.org/schema/beans
-         http://www.springframework.org/schema/beans/spring-beans.xsd
-         http://www.springframework.org/schema/context
-         http://www.springframework.org/schema/context/spring-context.xsd" >
- 
-         <context:component-scan base-package="package com.Autowired.ListMap;"></context:component-scan>  
- 
-  </beans>  
-----------------------------------------------------
测试类：
**[java]**[view
 plain](http://blog.csdn.net/qq_33366229/article/details/72859282#)[copy](http://blog.csdn.net/qq_33366229/article/details/72859282#)
[print](http://blog.csdn.net/qq_33366229/article/details/72859282#)[?](http://blog.csdn.net/qq_33366229/article/details/72859282#)
- package com.Autowired.ListMap;  
- 
- import org.junit.Test;  
- 
- import com.imooc.test.base.UnitTestBase;  
- 
- 
- publicclass TestListMap  extends UnitTestBase{  
- public TestListMap(){  
- super("classpath*:spring-beanannotation3.xml");  
-     }  
- 
- @Test
- publicvoid test(){  
-         BeanInvoke  bean=super.getBean("beanInvoke");  
-         bean.say();  
-     }  
- }  
结果：
**[java]**[view
 plain](http://blog.csdn.net/qq_33366229/article/details/72859282#)[copy](http://blog.csdn.net/qq_33366229/article/details/72859282#)
[print](http://blog.csdn.net/qq_33366229/article/details/72859282#)[?](http://blog.csdn.net/qq_33366229/article/details/72859282#)
- 2017-6-415:38:26 org.springframework.context.support.AbstractApplicationContext prepareRefresh  
- 信息: Refreshing org.springframework.context.support.ClassPathXmlApplicationContext@58a17083: startup date [Sun Jun 0415:38:26 CST 2017]; root of context hierarchy  
- 2017-6-415:38:26 org.springframework.beans.factory.xml.XmlBeanDefinitionReader loadBeanDefinitions  
- 信息: Loading XML bean definitions from URL [file:/E:/myeclipse/workspace/Spring2/bin/spring-beanannotation3.xml]  
- 2017-6-415:38:27 org.springframework.beans.factory.annotation.AutowiredAnnotationBeanPostProcessor <init>  
- 信息: JSR-330'javax.inject.Inject' annotation found and supported for autowiring  
- list...  
- 2017-6-415:38:27 org.springframework.context.support.AbstractApplicationContext doClose  
- 信息: Closing org.springframework.context.support.ClassPathXmlApplicationContext@58a17083: startup date [Sun Jun 0415:38:26 CST 2017]; root of context hierarchy  
- com.Autowired.ListMap.BeanImplTwo  
- com.Autowired.ListMap.BeanImplOne  
- 
- map...  
- beanImplOne    com.Autowired.ListMap.BeanImplOne  
- beanImplTwo    com.Autowired.ListMap.BeanImplTwo  
- -------------------------  
- com.Autowired.ListMap.BeanImplOne  
- 
