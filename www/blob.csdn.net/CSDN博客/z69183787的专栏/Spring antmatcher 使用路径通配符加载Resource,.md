# Spring antmatcher 使用路径通配符加载Resource, - z69183787的专栏 - CSDN博客
2015年10月07日 09:41:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4019
Spring提供了强大的Ant模式通配符匹配，从同一个路径能匹配一批资源。
Ant路径通配符支持"?"、"*"、"**"，注意通配符匹配不包括目录分隔符“/”。
“?”:匹配一个字符，如"config?.xml"可匹配"config1.xml".
“*”：匹配零个或多个字符串，如“com/*/config.xml”将匹配“cn/feng/config.xml”，但不匹配匹配“com/config.xml”(因为这里匹配的是字符串，如果是目录的话则可以)；而“com/config-*.xml”将匹配“com/config-dao.xml”；
"**"：匹配路径中的零个或多个目录。如“com/**/config.xml”将匹配“com/config.xml”，也匹配“com/feng/spring/config.xml”；而“com/feng/config-**.xml”将匹配“com/feng/config-dao.xml”，即把“**”当做两个“*”处理。
Spring在加载类路径资源时除了提供前缀“classpath:”的来支持加载一个Resource，还提供一个前缀“classpath*:”来支持加载所有匹配的类路径Resource。
Spring提供了ResourcePatternResolver接口来加载多个Resource.
**[java]**[view
 plain](http://blog.csdn.net/howlaa/article/details/8492289#)[copy](http://blog.csdn.net/howlaa/article/details/8492289#)
- package com.feng.spring.chapter2.helloworld;  
- 
- import java.io.IOException;  
- 
- import org.springframework.core.io.Resource;  
- import org.springframework.core.io.ResourceLoader;  
- 
- publicinterface ResourcePatternResolver extends ResourceLoader{  
-     String CLASSPATH_ALL_URL_PREFIX = "classpath*:";  
-     Resource[] getResources(String locationPattern) throws IOException;//添加了此方法用来接收多个Resource
- }  
一、"classpath":用于加载类路径(包括jar包)中的一个且仅一个资源;对于多个匹配的也只返回一个。如果需要多个匹配的则考虑"classpath*."前缀。
**[java]**[view
 plain](http://blog.csdn.net/howlaa/article/details/8492289#)[copy](http://blog.csdn.net/howlaa/article/details/8492289#)
- package com.feng.spring.chapter2.helloworld;  
- 
- 
- import java.io.IOException;  
- 
- import org.junit.Test;  
- import org.springframework.core.io.Resource;  
- import org.springframework.core.io.support.PathMatchingResourcePatternResolver;  
- 
- 
- publicclass HelloTest {  
- @Test
- publicvoid testClasspathPrefix()throws IOException{  
-             ResourcePatternResolver resolver = (ResourcePatternResolver) new PathMatchingResourcePatternResolver();  
- //只加载一个绝对匹配Resource,且通过ResourceLoader.getResource进行加载
-             Resource resources=resolver.getResource("classpath:META-INF/INDEX.LIST");  
-              Assert.assertEquals(1, resources.length);  
- //只加载一个匹配的Resource,且通过ResourceLoader.getResource进行加载
-              resources = resolver.getResource("classpath:META-INF/*.LIST");  
-              Assert.assertTrue(resources.length == 1);   
-         }  
- }  
二、"classpath*":
用于加载类路径(包括jar包)中所有的匹配的资源。
**[java]**[view
 plain](http://blog.csdn.net/howlaa/article/details/8492289#)[copy](http://blog.csdn.net/howlaa/article/details/8492289#)
- package com.feng.spring.chapter2.helloworld;  
- 
- 
- import java.io.IOException;  
- 
- import javax.annotation.Resource;  
- 
- import junit.framework.Assert;  
- 
- import org.junit.Test;  
- import org.springframework.core.io.support.PathMatchingResourcePatternResolver;  
- 
- 
- publicclass HelloTest {  
- @Test
- publicvoid testClasspathAsteriskPrefix()throws IOException{  
-             ResourcePatternResolver resolver = (ResourcePatternResolver) new PathMatchingResourcePatternResolver();  
- 
- //将加载多个绝对匹配的所有Resource
- //将首先通过ClassLoader.getResource("META-INF")加载非模式路径部分
- //然后进行遍历模式匹配
-             Resource[] resources = (Resource[]) resolver.getResources("classpath*:META-INF/INDEX.LIST");   
-             Assert.assertTrue(resources.length > 1);    
- //将加载多个模式匹配的Resource
-             resources = (Resource[]) resolver.getResources("classpath*:META-INF/*.LIST");  
-              Assert.assertTrue(resources.length > 1);     
-         }  
- }  
带通配符的classpath使用“ClassLoader”的“Enumeration<URL> getResources(String name)”方法来查找通配符之前的资源，然后通过模式匹配来获取匹配的资源。如“classpath:META-INF/*.LIST”将首先加载通配符之前的目录“META-INF”，然后再遍历路径进行子路径匹配从而获取匹配的资源。
三、"file":加载一个或多个系统中的Resource。如:"file:D/*.txt"将返回D盘下的所有txt文件。
四、无前缀：通过ResourceLoader实现加载一个资源。
ApplicationContext提供的getResource方法将获取资源委托给ResourcePatternResolver实现，默认使用PathMatingResourcePatternResolver.
