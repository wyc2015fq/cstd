# Spring Security教程（8）---- 自定义决策管理器及修改权限前缀 - z69183787的专栏 - CSDN博客
2014年03月13日 21:24:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：11295
首先介绍下Spring的决策管理器，其接口为AccessDecisionManager，抽象类为AbstractAccessDecisionManager。而我们要自定义决策管理器的话一般是继承抽象类而不去直接实现接口。
在Spring中引入了投票器（AccessDecisionVoter）的概念，有无权限访问的最终觉得权是由投票器来决定的，最常见的投票器为RoleVoter，在RoleVoter中定义了权限的前缀，先看下Spring在RoleVoter中是怎么处理授权的。
**[java]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18401233#)[copy](http://blog.csdn.net/jaune161/article/details/18401233#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/160340/fork)
- publicint vote(Authentication authentication, Object object, Collection<ConfigAttribute> attributes) {  
- int result = ACCESS_ABSTAIN;  
-     Collection<? extends GrantedAuthority> authorities = extractAuthorities(authentication);  
- 
- for (ConfigAttribute attribute : attributes) {  
- if (this.supports(attribute)) {  
-             result = ACCESS_DENIED;  
- 
- // Attempt to find a matching granted authority
- for (GrantedAuthority authority : authorities) {  
- if (attribute.getAttribute().equals(authority.getAuthority())) {  
- return ACCESS_GRANTED;  
-                 }  
-             }  
-         }  
-     }  
- 
- return result;  
- }  
- 
- Collection<? extends GrantedAuthority> extractAuthorities(Authentication authentication) {  
- return authentication.getAuthorities();  
- }  
Authentication中是用户及用户权限信息，attributes是访问资源需要的权限，然后循环判断用户是否有访问资源需要的权限，如果有就返回ACCESS_GRANTED，通俗的说就是有权限。
Spring提供了3个决策管理器，至于这三个管理器是如何工作的请查看SpringSecurity源码
AffirmativeBased 一票通过，只要有一个投票器通过就允许访问
ConsensusBased 有一半以上投票器通过才允许访问资源
UnanimousBased 所有投票器都通过才允许访问
下面来实现一个简单的自定义决策管理器，这个决策管理器并没有使用投票器
**[java]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18401233#)[copy](http://blog.csdn.net/jaune161/article/details/18401233#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/160340/fork)
- publicclass DefaultAccessDecisionManager extends AbstractAccessDecisionManager {  
- 
- publicvoid decide( Authentication authentication, Object object,   
-             Collection<ConfigAttribute> configAttributes)   
- throws AccessDeniedException, InsufficientAuthenticationException{  
- 
-         SysUser user = (SysUser)authentication.getPrincipal();  
-         logger.info("访问资源的用户为"+user.getUsername());  
- 
- //如果访问资源不需要任何权限则直接通过
- if( configAttributes == null ) {  
- return ;  
-         }  
- 
-         Iterator<ConfigAttribute> ite = configAttributes.iterator();  
- //遍历configAttributes看用户是否有访问资源的权限
- while( ite.hasNext()){  
- 
-             ConfigAttribute ca = ite.next();  
-             String needRole = ((SecurityConfig)ca).getAttribute();  
- 
- //ga 为用户所被赋予的权限。 needRole 为访问相应的资源应该具有的权限。
- for( GrantedAuthority ga: authentication.getAuthorities()){  
- 
- if(needRole.trim().equals(ga.getAuthority().trim())){  
- 
- return;  
-                 }  
- 
-             }  
- 
-         }  
- 
- thrownew AccessDeniedException("");  
- 
-     }  
- }  
decide这个方法没有任何的返回值，需要在没有通过授权时抛出AccessDeniedException。
如果有访问某个资源需要同时拥有两个或两个以上权限的情况，这时候就要通过自定义AccessDecisionVoter来实现了，这个也很简单在这里就不赘述了。如果要在页面中使用hasRole()这样的表达式就需要注入WebExpressionVoter了。
在SpringSecurity中自定义权限前缀
权限的前缀默认是ROLE_，网上的很多例子是说，直接在配置文件中加上下面的配置就可以了。
**[html]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18401233#)[copy](http://blog.csdn.net/jaune161/article/details/18401233#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/160340/fork)
- <beanid="roleVoter"class="org.springframework.security.access.vote.RoleVoter">
- <propertyname="rolePrefix"value="AUTH_"></property>
- </bean>
亲测不管用的，我想应该不是我配置的问题，而是在我们配置了http auto-config="true"Spring就已经将AccessDecisionManager初始化好了，即便配置到之前也不行，因为这个初始化是Spring自己来完成的，它并没有把你配置的roleVoter注入到AccessDecisionManager中。那我们就来手动的注入AccessDecisionManager吧。
在http配置中有个access-decision-manager-ref属性，可以使我们手动注入AccessDecisionManager，下面是详细配置
**[html]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18401233#)[copy](http://blog.csdn.net/jaune161/article/details/18401233#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/160340/fork)
- <sec:httpauto-config="true"access-decision-manager-ref="accessDecisionManager">
- 
- <sec:access-denied-handlerref="accessDeniedHandler"/>
- 
- <sec:session-managementinvalid-session-url="/login.jsp"/>
- 
- <sec:intercept-urlpattern="/app.jsp"access="AUTH_GG_FBGBGG"/>
- <sec:intercept-urlpattern="/**"access="IS_AUTHENTICATED_FULLY"/>
- 
- <sec:form-loginlogin-page="/login.jsp"authentication-failure-url="/login.jsp"
- default-target-url="/index.jsp"/>
- 
- </sec:http>
- 
- <beanid="accessDecisionManager"class="org.springframework.security.access.vote.AffirmativeBased">
- <constructor-argname="decisionVoters">
- <list>
- <refbean="roleVoter"/>
- <refbean="authenticatedVoter"/>
- </list>
- </constructor-arg>
- <propertyname="messageSource"ref="messageSource"></property>
- </bean>
- 
- <beanid="roleVoter"class="org.springframework.security.access.vote.RoleVoter">
- <propertyname="rolePrefix"value=""></property>
- </bean>
- 
- <beanid="authenticatedVoter"class="org.springframework.security.access.vote.AuthenticatedVoter"/>
在这里我们就不用自定义的AccessDecisionManager了，直接用Spring的AffirmativeBased，因为Spring本身提供的这些决策管理器就已经很强大了。
配置很简单，要想修改权限的前缀只需要修改roleVoter中的rolePrefix就可以了，如果不要前缀就让它为“”。
authenticatedVoter是为了支持IS_AUTHENTICATED这种认证，authenticatedVoter提供的3种认证，分别是
IS_AUTHENTICATED_ANONYMOUSLY 允许匿名用户进入
IS_AUTHENTICATED_FULLY 允许登录用户进入
IS_AUTHENTICATED_REMEMBERED 允许登录用户和rememberMe用户进入
