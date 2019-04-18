# Spring Security教程（5）---- 国际化配置及UserCache - z69183787的专栏 - CSDN博客
2014年03月13日 21:22:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4555
这一章是为了给后面的讲解打基础的，主要介绍下国际化的配置及UserCache的配置及使用
国际化配置
**[html]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18356061#)[copy](http://blog.csdn.net/jaune161/article/details/18356061#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/159023/fork)
- <!-- 定义上下文返回的消息的国际化 -->
- <beanid="messageSource"
- class="org.springframework.context.support.ReloadableResourceBundleMessageSource">
- <propertyname="basename"
- value="classpath:config/messages_zh_CN"/>
- </bean>
basename中配置的是消息文件的路径
在spring-security-core-3.2.0.M1.jar包中的org.springframework.security下可以找到国际化文件，可以直接拿来，这个类也可以用在项目中
**[java]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18356061#)[copy](http://blog.csdn.net/jaune161/article/details/18356061#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/159023/fork)
- @Autowired
- private MessageSource messageSource;  
这样就可以在类中引如MessageSource使用了，MessageSource提供了下面三个方法
**[html]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18356061#)[copy](http://blog.csdn.net/jaune161/article/details/18356061#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/159023/fork)
- String getMessage(String code, Object[] args, String defaultMessage, Locale locale);  
- String getMessage(String code, Object[] args, Locale locale) throws NoSuchMessageException;  
- String getMessage(MessageSourceResolvable resolvable, Locale locale) throws NoSuchMessageException;  
比如我们在property文件中定义了如下消息
**[java]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18356061#)[copy](http://blog.csdn.net/jaune161/article/details/18356061#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/159023/fork)
- UserDetails.isLocked=用户已被锁定  
- UserDetails.userNotFound=用户{0}不存在  
然后使用getMessage方法
getMessage("UserDetails.isLocked",null,null)  //用户已被锁定
getMessage("UserDetails.isLocked",new Object[]{“admin”},null)  //用户admin不存在
UserCache配置，通过ecahe实现
**[java]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18356061#)[copy](http://blog.csdn.net/jaune161/article/details/18356061#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/159023/fork)
- <!-- 启用用户的缓存功能 -->  
- <bean id="userCache"
- class="org.springframework.security.core.userdetails.cache.EhCacheBasedUserCache">  
-     <property name="cache" ref="userEhCache" />  
- </bean>  
- 
- <bean id="userEhCache"class="org.springframework.cache.ehcache.EhCacheFactoryBean">  
-     <property name="cacheName" value="userCache" />  
-     <property name="cacheManager" ref="cacheManager" />  
- </bean>  
- 
- <bean id="cacheManager"
- class="org.springframework.cache.ehcache.EhCacheManagerFactoryBean" />  
ehcache.xml
**[html]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18356061#)[copy](http://blog.csdn.net/jaune161/article/details/18356061#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/159023/fork)
- <cache
- name="userCache"
- maxElementsInMemory="100"
- eternal="false"
- timeToIdleSeconds="600"
- timeToLiveSeconds="3600"
- overflowToDisk="true"
- />
注入ecache
**[java]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18356061#)[copy](http://blog.csdn.net/jaune161/article/details/18356061#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/159023/fork)
- @Autowired
- private UserCache userCache;  
这样在程序中就可以通过
this.userCache.getUserFromCache(username);获取到缓存中的用户对象
用户对象为UserDetails类型
