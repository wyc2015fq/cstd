# Spring Security教程（14）---- Logout和SessionManager - z69183787的专栏 - CSDN博客
2014年03月16日 08:16:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2065
Logout的配置很简单，只需要在http中加入下面的配置就可以了
**[html]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18736687#)[copy](http://blog.csdn.net/jaune161/article/details/18736687#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/169857/fork)
- <sec:logoutinvalidate-session="true"logout-url="/logout"
- logout-success-url="/login.jsp"/>
invalidate-session是否销毁Session
logout-url logout地址
logout-success-url logout成功后要跳转的地址
Session管理中最简单的配置方法是
**[html]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18736687#)[copy](http://blog.csdn.net/jaune161/article/details/18736687#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/169857/fork)
- <sec:session-managementinvalid-session-url="/login.jsp"/>
意思就是Session失效时跳转到login.jsp
配置同一事件，只能有一个用户登录系统。
网上有的例子是这样配置的
**[html]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18736687#)[copy](http://blog.csdn.net/jaune161/article/details/18736687#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/169857/fork)
- <sec:session-managementinvalid-session-url="/login.jsp">
- <sec:concurrency-controlerror-if-maximum-exceeded="true"
- max-sessions="1"expired-url="/login.jsp"/>
- </sec:session-management>
但是这种配置在3.2版本中不管用
在3.2版本中需要这样配置
首先在web.xml中加入一下配置
**[html]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18736687#)[copy](http://blog.csdn.net/jaune161/article/details/18736687#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/169857/fork)
- <listener>
- <listener-class>org.springframework.security.web.session.HttpSessionEventPublisher</listener-class>
- </listener>
然后修改applicationContext-security.xml
**[html]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18736687#)[copy](http://blog.csdn.net/jaune161/article/details/18736687#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/169857/fork)
- <sec:httpaccess-decision-manager-ref="accessDecisionManager"
- entry-point-ref="authenticationEntryPoint">
- 
- <sec:access-denied-handlerref="accessDeniedHandler"/>
- 
- <sec:logoutinvalidate-session="true"logout-url="/logout"
- logout-success-url="/login.jsp"/>
- 
- <sec:session-managementsession-authentication-strategy-ref="concurrentSessionControlStrategy"/>
- 
- <sec:remember-me
- authentication-success-handler-ref="authenticationSuccessHandler"
- data-source-ref="dataSource"
- user-service-ref="userDetailService"
- />
- 
- 
- <sec:custom-filterref="filterSecurityInterceptor"before="FILTER_SECURITY_INTERCEPTOR"/>
- <sec:custom-filterref="captchaAuthenticaionFilter"position="FORM_LOGIN_FILTER"/>
- <sec:custom-filterref="concurrencyFilter"position="CONCURRENT_SESSION_FILTER"/>
- </sec:http>
- 
- <beanid="captchaAuthenticaionFilter"class="com.zrhis.system.security.CaptchaAuthenticationFilter">
- <propertyname="authenticationManager"ref="authenticationManager"/>
- <propertyname="authenticationFailureHandler"ref="authenticationFailureHandler"/>
- <propertyname="authenticationSuccessHandler"ref="authenticationSuccessHandler"/>
- <propertyname="filterProcessesUrl"value="/login.do"/>
- <propertyname="sessionAuthenticationStrategy"ref="concurrentSessionControlStrategy"/>
- </bean>
- 
- <beanid="authenticationSuccessHandler"class="com.zrhis.system.security.SavedRequestLoginSuccessHandler">
- <propertyname="defaultTargetUrl"value="/index.jsp"/>
- <propertyname="forwardToDestination"value="true"/>
- <propertyname="alwaysUseDefaultTargetUrl"value="false"/>
- </bean>
- <beanid="authenticationFailureHandler"class="org.springframework.security.web.authentication.SimpleUrlAuthenticationFailureHandler">
- <propertyname="defaultFailureUrl"value="/login.jsp"/>
- </bean>
- 
- <beanid="authenticationEntryPoint"class="org.springframework.security.web.authentication.LoginUrlAuthenticationEntryPoint">
- <propertyname="loginFormUrl"value="/login.jsp"/>
- </bean>
- 
- <beanid="concurrencyFilter"class="org.springframework.security.web.session.ConcurrentSessionFilter">
- <constructor-argname="sessionRegistry"ref="sessionRegistry"/>
- <constructor-argname="expiredUrl"value="/sessionOut.jsp"/>
- </bean>
- 
- <beanid="concurrentSessionControlStrategy"
- class="org.springframework.security.web.authentication.session.ConcurrentSessionControlStrategy">
- <constructor-argname="sessionRegistry"ref="sessionRegistry"/>
- <propertyname="maximumSessions"value="1"></property>
- </bean>
- 
- <beanid="sessionRegistry"class="org.springframework.security.core.session.SessionRegistryImpl"/>
