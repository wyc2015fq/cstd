# Spring Security教程（7）---- 解决UsernameNotFoundException无法被捕获的问题 - z69183787的专栏 - CSDN博客
2014年03月13日 21:23:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7733
这个教程是我在往项目中一点一点添加 Spring Security的过程的一个笔记，也是我学习 Spring Security的一个过程。
在解决这个问题之前要先说一点authentication-provider默认加载的是DaoAuthenticationProvider类。
完成了上一章的内容后在测试的时候发现在UserDetailsService中抛出的UsernameNotFoundException无法被捕获。于是找到DaoAuthenticationProvider，源码看了好几遍没有看出端倪。然后直接查看最顶级的接口AuthenticationProvider。发现它只有一个方法如下
**[java]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18359321#)[copy](http://blog.csdn.net/jaune161/article/details/18359321#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/159314/fork)
- Authentication authenticate(Authentication authentication) throws AuthenticationException;  
抛出AuthenticationException异常，而UsernameNotFoundException是AuthenticationException的子类，那问题应该就出在authenticate这个方法上了。
然后找到DaoAuthenticationProvider的父类AbstractUserDetailsAuthenticationProvider的authenticate方法，发现了这段代码。
**[java]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18359321#)[copy](http://blog.csdn.net/jaune161/article/details/18359321#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/159314/fork)
- try {  
-     user = retrieveUser(username, (UsernamePasswordAuthenticationToken) authentication);  
- } catch (UsernameNotFoundException notFound) {  
-     logger.debug("User '" + username + "' not found");  
- 
- if (hideUserNotFoundExceptions) {  
- thrownew BadCredentialsException(messages.getMessage(  
- "AbstractUserDetailsAuthenticationProvider.badCredentials", "Bad credentials"));  
-     } else {  
- throw notFound;  
-     }  
- }  
它这里有个hideUserNotFoundExceptions属性，默认是true。这样的话即便我们抛出了UsernameNotFoundException它也会转为BadCredentialsException，所以我们需要将hideUserNotFoundExceptions属性的值设为false，而在上一章中的那种配置方法是没有办法为其属性赋值的所以我们要手动注入.authentication-provider，所以配置就变成了下面的内容
**[html]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18359321#)[copy](http://blog.csdn.net/jaune161/article/details/18359321#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/159314/fork)
- <sec:authentication-manager>
- <sec:authentication-providerref="authenticationProvider"/>
- </sec:authentication-manager>
- 
- <beanid="authenticationProvider"class="org.springframework.security.authentication.dao.DaoAuthenticationProvider">
- <propertyname="hideUserNotFoundExceptions"value="false"/>
- <propertyname="userDetailsService"ref="userDetailService"/>
- <propertyname="userCache"ref="userCache"/>
- <propertyname="messageSource"ref="messageSource"/>
- <propertyname="passwordEncoder"ref="passwordEncode"/>
- <propertyname="saltSource"ref="saltSource"/>
- </bean>
- 
- 
- <!-- 配置密码加密类 -->
- <beanid="passwordEncode"class="org.springframework.security.authentication.encoding.Md5PasswordEncoder"/>
- <beanid="saltSource"class="org.springframework.security.authentication.dao.ReflectionSaltSource">
- <propertyname="userPropertyToUse"value="username"/>
- </bean>
注意：如果在authentication-provider配置中用ref指定AuthenticationProvider则authentication-provider的子元素将都不可以用。
即下面的这种配置是错误的
**[html]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18359321#)[copy](http://blog.csdn.net/jaune161/article/details/18359321#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/159314/fork)
- <sec:authentication-manager>
- <sec:authentication-providerref="authenticationProvider">
- <sec:password-encoderref="passwordEncode">
- <sec:salt-sourceuser-property="username"/>
- </sec:password-encoder>
- </sec:authentication-provider>
- </sec:authentication-manager>
所以我们的盐值加密就需要注入到AuthenticationProvider中了。
SaltSource是一个接口有两个实现类SystemWideSaltSource和ReflectionSaltSource。
SystemWideSaltSource ：只能指定固定值
ReflectionSaltSource：可以指定UserDetails的属性，这里我们用的就是它
这样的话就可以保证在抛出UsernameNotFoundException时，前台能显示出来错误信息，如下所示。
![](https://img-blog.csdn.net/20140116155430312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamF1bmUxNjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在上一章中忘了介绍如何在前台显示登录是的异常信息，在这里补上。
UsernamePasswordAuthenticationFilter认证失败后，异常信息会写到Session中，key为SPRING_SECURITY_LAST_EXCEPTION
可以通过El表达式来获取到异常的信息。
**[html]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18359321#)[copy](http://blog.csdn.net/jaune161/article/details/18359321#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/159314/fork)
- ${sessionScope.SPRING_SECURITY_LAST_EXCEPTION.message}  
- 
