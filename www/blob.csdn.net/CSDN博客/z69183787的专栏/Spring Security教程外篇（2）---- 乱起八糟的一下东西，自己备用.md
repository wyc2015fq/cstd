# Spring Security教程外篇（2）---- 乱起八糟的一下东西，自己备用 - z69183787的专栏 - CSDN博客
2014年03月13日 21:25:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：9404
intercept-url中access配置
IS_AUTHENTICATED_ANONYMOUSLY 允许匿名用户进入
IS_AUTHENTICATED_FULLY 允许登录用户进入
IS_AUTHENTICATED_REMEMBERED 允许登录用户和rememberMe用户进入
支持表达式
**[html]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18405071#)[copy](http://blog.csdn.net/jaune161/article/details/18405071#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/160542/fork)
- <sec:httpauto-config="true"access-decision-manager-ref="accessDecisionManager"
- use-expressions="true">
表达式在access中要写成hasRole(xxx)，如果access中任然写ROLE_USER这中形式的话会报下面的错误
Unsupported configuration attributes: [AUTH_GG_FBGBGG, AUTH_GG_FBGBGG]
目前Spring已经不提供下载地址都是Maven地址，但从Spring的Maven仓库中还可以下载到
仓库地址 http://maven.springframework.org/
Spring下载：http://maven.springframework.org/release/org/springframework/spring/
SpringData：http://maven.springframework.org/release/org/springframework/data/
SpringSecurity：http://maven.springframework.org/release/org/springframework/security/
如何获取到Session中的UserDeatils;
在页面中可以使用
**[html]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18405071#)[copy](http://blog.csdn.net/jaune161/article/details/18405071#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/160542/fork)
- <p>用户名：<sec:authenticationproperty="name"/></p>
- <p>用户名：<sec:authenticationproperty="principal.username"/></p>
- <p>用户姓名：<sec:authenticationproperty="principal.name"/></p>
- <p>用户名：${SPRING_SECURITY_CONTEXT.authentication.principal.username}</p>
- <p>用户姓名：${SPRING_SECURITY_CONTEXT.authentication.principal.name} </p>
principal为UserDeatils，在el表达式中不能直接通过name字段来获取用户名即
${SPRING_SECURITY_CONTEXT.authentication.name}是错误的
在后台程序中可以通过SecurityContextHolder来获取
**[java]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18405071#)[copy](http://blog.csdn.net/jaune161/article/details/18405071#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/160542/fork)
- UserDetails user = (UserDetails)SecurityContextHolder.getContext().getAuthentication().getPrincipal();  
- 
