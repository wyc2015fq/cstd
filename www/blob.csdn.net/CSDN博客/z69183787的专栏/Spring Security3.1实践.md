# Spring Security3.1实践 - z69183787的专栏 - CSDN博客
2014年03月13日 21:00:29[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2126
   本例子是我在spring MVC3.0.5的基础上进行修改的，用的Spring Security3.1.2。
          数据库：mysql，开发工具：myeclipse8.6,tomcat6.0。
# 1.收集资料
[http://blog.csdn.net/k10509806/article/details/6369131](http://blog.csdn.net/k10509806/article/details/6369131)
[http://www.cnblogs.com/wenxiu/archive/2011/01/22/1942084.html](http://www.cnblogs.com/wenxiu/archive/2011/01/22/1942084.html)
[http://ootabc.iteye.com/blog/688213](http://ootabc.iteye.com/blog/688213)
[http://wenku.baidu.com/view/abf23846336c1eb91a375d83.html](http://wenku.baidu.com/view/abf23846336c1eb91a375d83.html)
[http://www.cnblogs.com/zhangliang0115/archive/2012/04/02/2429584.html](http://www.cnblogs.com/zhangliang0115/archive/2012/04/02/2429584.html)
# 2.数据库建表
采用基于角色-资源-用户权限管理设计。
## 2.1.用户表    test_user
|**字段名**|**标示符**|**类型**|**有无空值**|**主键**|**备注**|
|----|----|----|----|----|----|
|ID|ID|varchar(36)|NO|Y|编号|
|USERNAME|用户名|Varchar(30)|NO|||
|PASSWORD|密码|varchar(36)|NO|||
|STATUS|状态|tinyint|||0开启、1关闭|
## 2.2.角色表    test_role
|**字段名**|**标示符**|**类型**|**有无空值**|**主键**|**备注**|
|----|----|----|----|----|----|
|ID|ID|varchar(36)|NO|Y|编号|
|NAME|角色名|varchar(50)||||
## 2.3.资源表    test_resource
|**字段名**|**标示符**|**类型**|**有无空值**|**主键**|**备注**|
|----|----|----|----|----|----|
|ID|ID|varchar(36)|NO|Y|编号|
|NAME|资源名称|varchar(50)||||
|URL|地址|Varchar(50)||||
|TYPE|类型|Tinyint||||
## 2.4.用户角色表   test_user_role
|**字段名**|**标示符**|**类型**|**有无空值**|**主键**|**备注**|
|----|----|----|----|----|----|
|ID|ID|varchar(36)|NO|Y|编号|
|UID|用户ID|varchar(36)||||
|RID|角色ID|varchar(36)||||
## 2.5.角色资源表   test_role_resource
|**字段名**|**标示符**|**类型**|**有无空值**|**主键**|**备注**|
|----|----|----|----|----|----|
|ID|ID|varchar(36)|NO|Y|编号|
|RSID|资源ID|varchar(36)||||
|RID|角色ID|varchar(36)||||
# 3.梳理资料，整理思路
## 3.1.Spring Security3.1的2种常见方式
Ø  用户信息和权限存储于数据库，而资源和权限的对应采用硬编码配置。
Ø  细分角色和权限，并将角色、用户、资源、权限均都存储于数据库中。并且自定义过滤器，代替原来的FilterSecurityInterceptor过滤器；并分别实现AccessDecisionManager、UserDetailsService和InvocationSecurityMetadataSourceService，并在配置文件中进行相应配置。
Ø发现两者不可结合使用，会有问题。
# 4.代码整理
接下来开始着手代码编写，不管是两种实现方式中的哪种方式，个人感觉都需要把加载用户信息放在一个类里面管理，直观方便，结构清晰，不要用在配置文件直接写sql语句。
## 4.1.资源和权限对应写在配置文件中
1、     web.xml配置
     a)     启动时加载Spring的jdbc和security配置文件。
     b)      配置spring的servlet过滤器，使其能够识别Spring Controller。
     c)     加载Spring Security过滤器链代理，它按照顺序执行spring的权限过滤器。
     d)     其他业务加载，比如：log4j，字符集编码过滤器，session超时等。
Xml代码  ![收藏代码](http://aokunsang.iteye.com/images/icon_star.png)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <web-appversion="2.5"
- xmlns="http://java.sun.com/xml/ns/javaee"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://java.sun.com/xml/ns/javaee   
-     http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd">
- 
- <context-param>
- <param-name>webAppRootKey</param-name>
- <param-value>springMvc</param-value>
- </context-param>
- 
- <!-- Listener log4jConfigLocation -->
- <listener>
- <listener-class>org.springframework.web.util.Log4jConfigListener</listener-class>
- </listener>
- 
- <context-param>
- <param-name>contextConfigLocation</param-name>
- <param-value>
-         classpath:/module/applicationContext-jdbc.xml,  
-         classpath:/module/applicationContext-security.xml  
- </param-value>
- </context-param>
- 
- <!-- Log4j -->
- <context-param>
- <param-name>log4jConfigLocation</param-name>
- <param-value>classpath:/config/log4j.properties</param-value>
- </context-param>
- 
- <listener>
- <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
- </listener>
- 
- <!-- 权限过滤器链 -->
- <filter>
- <filter-name>springSecurityFilterChain</filter-name>
- <filter-class>org.springframework.web.filter.DelegatingFilterProxy</filter-class>
- </filter>
- 
- <filter-mapping>
- <filter-name>springSecurityFilterChain</filter-name>
- <url-pattern>/*</url-pattern>
- </filter-mapping>
- 
- <servlet>
- <servlet-name>springmvc</servlet-name>
- <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
- <init-param>
- <param-name>contextConfigLocation</param-name>
- <param-value>classpath:/module/applicationContext-servlet.xml</param-value>
- </init-param>
- <load-on-startup>1</load-on-startup>
- </servlet>
- 
- <servlet-mapping>
- <servlet-name>springmvc</servlet-name>
- <url-pattern>/</url-pattern>
- </servlet-mapping>
- 
- <!-- Spring 刷新Introspector防止内存泄露 -->
- <listener>
- <listener-class>
-             org.springframework.web.util.IntrospectorCleanupListener  
- </listener-class>
- </listener>
- 
- <!--  获取Spring Security session的生命周期-->
- <listener>
- <listener-class>
-             org.springframework.security.web.session.HttpSessionEventPublisher   
- </listener-class>
- </listener>
- 
- <!-- session超时定义,单位为分钟 -->
- <session-config>
- <session-timeout>20</session-timeout>
- </session-config>
- 
- 
- <filter>
- <filter-name>encodingFilter</filter-name>
- <filter-class>org.springframework.web.filter.CharacterEncodingFilter</filter-class>
- </filter>
- 
- <filter-mapping>
- <filter-name>encodingFilter</filter-name>
- <url-pattern>/*</url-pattern>
- </filter-mapping>
- 
- <welcome-file-list>
- <welcome-file>index.jsp</welcome-file>
- </welcome-file-list>
- </web-app>
  2、  application-security.xml文件的配置。application-servlet.xml配置不懂的参考spring MVC3.0.5搭建全程。
Xml代码  ![收藏代码](http://aokunsang.iteye.com/images/icon_star.png)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beans:beansxmlns="http://www.springframework.org/schema/security"
- xmlns:beans="http://www.springframework.org/schema/beans"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-3.0.xsd  
-                     http://www.springframework.org/schema/security http://www.springframework.org/schema/security/spring-security-3.1.xsd">
- 
- <httppattern="/resources/**"security="none"></http>
- <httppattern="/user/login"security="none"></http>
- 
- <httpauto-config="true"
- use-expressions="true"
- access-denied-page="/user/denied">
-   <!--   
-     default-target-url       指定了从登录页面登录后进行跳转的页面  
-     always-use-default-target   true表示登录成功后强制跳转  
-     authentication-failure-url  表示验证失败后进入的页面  
-     login-processing-url       设置验证登录验证地址，如果不设置，默认是j_spring_security_check  
-     username-parameter,password-parameter     设置登录用户名和密码的请求name，默认：j_username,j_password  
- default-target-url="/user/home"
-    -->
- <form-loginlogin-page="/user/login"
- always-use-default-target="true"
- authentication-failure-url="/user/login?error=1"
- login-processing-url="/logincheck"
- authentication-success-handler-ref="successHandler"/>
- 
- <intercept-urlpattern="/user/myjsp"access="hasRole('ROLE_USER')"/>
- <intercept-urlpattern="/user/admin"access="hasRole('ROLE_ADMIN')"/>
- 
- <logoutlogout-url="/logout"logout-success-url="/user/login"/>
-     <!--   
-          error-if-maximum-exceeded 后登陆的账号会挤掉第一次登陆的账号   
-          session-fixation-protection  防止伪造sessionid攻击,用户登录成功后会销毁用户当前的session。  
-     -->
- <session-managementinvalid-session-url="/user/timedout"session-fixation-protection="none">
- <concurrency-controlmax-sessions="1"error-if-maximum-exceeded="true"/>
- </session-management>
- <!-- <custom-filter ref="mySecurityFilter" before="FILTER_SECURITY_INTERCEPTOR"/> -->
- </http>
- 
- <authentication-manageralias="authManager">
- <authentication-provideruser-service-ref="userServiceDetail">
-         <!--<jdbc-user-servicedata-source-ref="dataSource"
- authorities-by-username-query=""
- group-authorities-by-username-query=""/> -->
- <password-encoderhash="md5">
- <salt-sourceuser-property="username"/><!-- 盐值  [添加这个属性后，加密密码明文为:"密码明文{盐值}"] -->
- </password-encoder>
- 
- </authentication-provider>
- </authentication-manager>
- 
- </beans:beans>
问题：
    我自己写了个User实现UserDetails，发现同一个账号可以同时登陆，也就是说concurrency-control没有起到作用，参考了一下资料后，重写一下User的hashcode，equals方法就行了。【后来发现的问题，附件自己添加】
    详细参考：[http://flashing.iteye.com/blog/823666](http://flashing.iteye.com/blog/823666)
Java代码  ![收藏代码](http://aokunsang.iteye.com/images/icon_star.png)
- @Override
- publicint hashCode() {  
- return username.hashCode();  
- }  
- 
- @Override
- publicboolean equals(Object obj) {  
-     User user = (User)obj;  
- returnthis.username.equals(user.getUsername());  
- }  
解析：
     a、use-expressions
如：hasRole(‘ROLE_ADMIN’或hasIpAddress(‘127.0.0.1’))等，看不懂的可以参考下面链接。
[http://static.springsource.org/spring-security/site/docs/3.0.7.RELEASE/reference/el-access.html](http://static.springsource.org/spring-security/site/docs/3.0.7.RELEASE/reference/el-access.html)
[http://hougbin.iteye.com/blog/1526980](http://hougbin.iteye.com/blog/1526980)
[http://kongcodecenter.iteye.com/blog/1320021](http://kongcodecenter.iteye.com/blog/1320021)
   b、<password-encoder  hash=”md5”>
其属性hash就是加密的方法是什么?常用的可能是md5和sha吧。
主要说下<salt-source user-property=’username’>盐值：不加这个属性，spring验证密码时，直接用MD5加密后的值，与我们自己写的实现了UserDetailsService接口的类中loadUsersByUsername(String username)方法返回的UserDetails中的密码进行比较；如果加了这个属性并且设置user-property=’username’[不知道能不能设置其他值，或许也可以设置password，没有尝试]，加密前的明文就成为”密码明文{盐值}”，这里的盐值为用户名。
   c、remember-me的实现策略参考下面：
[http://static.springsource.org/spring-security/site/docs/3.0.x/reference/remember-me.html](http://static.springsource.org/spring-security/site/docs/3.0.x/reference/remember-me.html)
[http://blog.csdn.net/small_love/article/details/6641316](http://blog.csdn.net/small_love/article/details/6641316)
[http://xyz20003.iteye.com/blog/223282](http://xyz20003.iteye.com/blog/223282)
   d、UserDetailsService可以通过手工设置几个用户的权限:
Java代码  ![收藏代码](http://aokunsang.iteye.com/images/icon_star.png)
- <user-service id="userDetailsService">  
-     <user name="jimi" password="jimispassword" authorities="ROLE_USER, ROLE_ADMIN" />  
-     <user name="bob" password="bobspassword" authorities="ROLE_USER" />  
-   </user-service>  
     或者通过属性文件读取；
    <user-service id="userDetailsService" properties="users.properties"/>
     属性 文件内容格式为: username=password,grantedAuthority[,grantedAuthority][,enabled|disabled]
Java代码  ![收藏代码](http://aokunsang.iteye.com/images/icon_star.png)
- jimi=jimispassword,ROLE_USER,ROLE_ADMIN,enabled  
- bob=bobspassword,ROLE_USER,enabled  
   e、UserDetailsService实现类
Java代码  ![收藏代码](http://aokunsang.iteye.com/images/icon_star.png)
- /**
-  * @description  项目实现的用户查询服务,将用户信息查询出来(用于实现用户的认证)
-  * @author aokunsang
-  * @date 2012-8-15
-  */
- publicclass MyUserDetailServiceImpl implements UserDetailsService {  
- 
- private UserService userService;  
- 
- @Override
- public UserDetails loadUserByUsername(String username)  
- throws UsernameNotFoundException {  
- 
-         System.out.println("---------MyUserDetailServiceImpl:loadUserByUsername------正在加载用户名和密码，用户名为："+username);  
- 
-         User user = userService.loadUserByUserName(username);  
- if(user==null){  
- thrownew UsernameNotFoundException("用户名没有找到!");  
-         }  
- 
- boolean enabled = true;                //是否可用
- boolean accountNonExpired = true;        //是否过期
- boolean credentialsNonExpired = true;     
- boolean accountNonLocked = true;    
- 
-         Set<GrantedAuthority> authorities = new HashSet<GrantedAuthority>();  
- //如果你使用资源和权限配置在xml文件中，如：<intercept-url pattern="/user/admin" access="hasRole('ROLE_ADMIN')"/>；
- //并且也不想用数据库存储，所有用户都具有相同的权限的话，你可以手动保存角色(如：预订网站)。
- //authorities.add(new SimpleGrantedAuthority("ROLE_USER"));
- 
-         List<Role> roles = userService.findUserRolesByUsername(username);  
- for(Role role : roles){  
-             GrantedAuthority ga = new SimpleGrantedAuthority(role.getName());  
-             authorities.add(ga);      
-         }  
- returnnew org.springframework.security.core.userdetails.User(  
-                 user.getUserName(),  
-                 user.getPassWord(),   
-                 enabled,   
-                 accountNonExpired,   
-                 credentialsNonExpired,   
-                 accountNonLocked,   
-                 authorities);  
-     }  
- /**
-      * @param userService the userService to set
-      */
- publicvoid setUserService(UserService userService) {  
- this.userService = userService;  
-     }  
- 
- }  
## 4.2.资源和配置文件存储在数据库中
需要自己手动写一个拦截器，提供查询数据库中的资源权限，提供验证用户是否具有访问URL地址的权限（3个类）。
        1、AbstractSecurityInterceptor继承类，同时实现Filter接口。
Java代码  ![收藏代码](http://aokunsang.iteye.com/images/icon_star.png)
- /**
-  * @description 一个自定义的filter，
-  *  必须包含authenticationManager,accessDecisionManager,securityMetadataSource三个属性，
-         我们的所有控制将在这三个类中实现
-  * @author aokunsang
-  * @date 2012-8-15
-  */
- publicclass MyFilterSecurityInterceptor extends AbstractSecurityInterceptor  
- implements Filter {  
- 
- private FilterInvocationSecurityMetadataSource fisMetadataSource;  
- 
- /* (non-Javadoc)
-      * @see org.springframework.security.access.intercept.AbstractSecurityInterceptor#getSecureObjectClass()
-      */
- @Override
- public Class<?> getSecureObjectClass() {  
- return FilterInvocation.class;  
-     }  
- 
- @Override
- public SecurityMetadataSource obtainSecurityMetadataSource() {  
- return fisMetadataSource;  
-     }  
- 
- @Override
- publicvoid destroy() {}  
- 
- @Override
- publicvoid doFilter(ServletRequest request, ServletResponse response,  
-             FilterChain chain) throws IOException, ServletException {  
- //super.beforeInvocation(fi);源码  
- //1.获取请求资源的权限  
- //执行Collection<ConfigAttribute> attributes = SecurityMetadataSource.getAttributes(object);  
- //2.是否拥有权限  
- //this.accessDecisionManager.decide(authenticated, object, attributes);  
-         System.out.println("------------MyFilterSecurityInterceptor.doFilter()-----------开始拦截器了....");  
-         FilterInvocation fi = new FilterInvocation(request, response, chain);  
-         InterceptorStatusToken token = super.beforeInvocation(fi);  
- try {  
-             fi.getChain().doFilter(fi.getRequest(), fi.getResponse());  
-         } catch (Exception e) {  
-             e.printStackTrace();  
-         }finally{  
- super.afterInvocation(token,null);  
-         }  
-         System.out.println("------------MyFilterSecurityInterceptor.doFilter()-----------拦截器该方法结束了....");  
-     }  
- 
- @Override
- publicvoid init(FilterConfig config) throws ServletException {  
- 
-     }  
- 
- 
- publicvoid setFisMetadataSource(  
-             FilterInvocationSecurityMetadataSource fisMetadataSource) {  
- this.fisMetadataSource = fisMetadataSource;  
-     }  
- }  
        2、FilterInvocationSecurityMetadataSource实现类
Java代码  ![收藏代码](http://aokunsang.iteye.com/images/icon_star.png)
- /**
-  * @description  资源源数据定义，将所有的资源和权限对应关系建立起来，即定义某一资源可以被哪些角色访问
-  * @author aokunsang
-  * @date 2012-8-15
-  */
- publicclass MySecurityMetadataSource implements FilterInvocationSecurityMetadataSource {  
- 
- private UserService userService;  
- /* 保存资源和权限的对应关系  key-资源url  value-权限 */
- private Map<String,Collection<ConfigAttribute>> resourceMap = null;   
- private AntPathMatcher urlMatcher = new AntPathMatcher();  
- 
- public MySecurityMetadataSource(UserService userService) {  
- this.userService = userService;  
-         loadResourcesDefine();  
-     }  
- 
- @Override
- public Collection<ConfigAttribute> getAllConfigAttributes() {  
- returnnull;  
-     }  
- 
- privatevoid loadResourcesDefine(){  
-         resourceMap = new HashMap<String,Collection<ConfigAttribute>>();  
- //      Collection<ConfigAttribute> configAttributes1 = new ArrayList<ConfigAttribute>() ;
- //      ConfigAttribute configAttribute1 = new SecurityConfig("ROLE_ADMIN");
- //      configAttributes1.add(configAttribute1);
- //      resourceMap.put("/leftmenu.action", configAttributes1);
- 
-         System.out.println("MySecurityMetadataSource.loadResourcesDefine()--------------开始加载资源列表数据--------");  
-         List<Role> roles = userService.findAllRoles();  
- for(Role role : roles){  
-             List<Resource> resources = userService.findResourcesByRoleName(role.getName());  
- for(Resource resource : resources){  
-                 Collection<ConfigAttribute> configAttributes = null;  
-                 ConfigAttribute configAttribute = new SecurityConfig(role.getName());  
- if(resourceMap.containsKey(resource.getUrl())){  
-                     configAttributes = resourceMap.get(resource.getUrl());  
-                     configAttributes.add(configAttribute);  
-                 }else{  
-                     configAttributes = new ArrayList<ConfigAttribute>() ;  
-                     configAttributes.add(configAttribute);  
-                     resourceMap.put(resource.getUrl(), configAttributes);  
-                 }  
-             }  
-         }  
-     }  
- /* 
-      * 根据请求的资源地址，获取它所拥有的权限
-      */
- @Override
- public Collection<ConfigAttribute> getAttributes(Object obj)  
- throws IllegalArgumentException {  
- //获取请求的url地址
-         String url = ((FilterInvocation)obj).getRequestUrl();  
-         System.out.println("MySecurityMetadataSource:getAttributes()---------------请求地址为："+url);  
-         Iterator<String> it = resourceMap.keySet().iterator();  
- while(it.hasNext()){  
-             String _url = it.next();  
- if(_url.indexOf("?")!=-1){  
-                 _url = _url.substring(0, _url.indexOf("?"));  
-             }  
- if(urlMatcher.match(_url,url))  
- return resourceMap.get(_url);  
-         }  
- returnnull;  
-     }  
- 
- @Override
- publicboolean supports(Class<?> arg0) {  
-         System.out.println("MySecurityMetadataSource.supports()---------------------");  
- returntrue;  
-     }  
- 
- }  
       3、AccessDecisionManager实现类
Java代码  ![收藏代码](http://aokunsang.iteye.com/images/icon_star.png)
- /**
-  * @description  访问决策器，决定某个用户具有的角色，是否有足够的权限去访问某个资源 ;做最终的访问控制决定
-  * @author aokunsang
-  * @date 2012-8-16
-  */
- publicclass MyAccessDescisionManager implements AccessDecisionManager {  
- 
- /**
-      * @description 认证用户是否具有权限访问该url地址
-      * 
-      */
- @Override
- publicvoid decide(Authentication authentication, Object obj,  
-             Collection<ConfigAttribute> configAttributes) throws AccessDeniedException,InsufficientAuthenticationException {  
-         System.out.println("MyAccessDescisionManager.decide()------------------验证用户是否具有一定的权限--------");  
- if(configAttributes==null) return;  
-         Iterator<ConfigAttribute> it = configAttributes.iterator();  
- while(it.hasNext()){  
-             String needRole = it.next().getAttribute();  
- //authentication.getAuthorities()  用户所有的权限
- for(GrantedAuthority ga:authentication.getAuthorities()){  
- if(needRole.equals(ga.getAuthority())){  
- return;  
-                 }  
-             }  
-         }  
- thrownew AccessDeniedException("--------MyAccessDescisionManager：decide-------权限认证失败！");  
-     }  
- 
- /**
-      * 启动时候被AbstractSecurityInterceptor调用，决定AccessDecisionManager是否可以执行传递ConfigAttribute。
-      */
- @Override
- publicboolean supports(ConfigAttribute configAttribute) {  
-         System.out.println("MyAccessDescisionManager.supports()------------角色名："+configAttribute.getAttribute());  
- returntrue;  
-     }  
- 
- /**
-      * 被安全拦截器实现调用，包含安全拦截器将显示的AccessDecisionManager支持安全对象的类型
-      */
- @Override
- publicboolean supports(Class<?> clazz) {  
-         System.out.println("MyAccessDescisionManager.supports()--------------------------------");  
- returntrue;  
-     }  
- 
- }  
补充：还可以实现AuthenticationFailureHandler和AuthenticationSuccessHandler这两个接口，可以做一些验证失败和成功后的业务逻辑操作。(注意实现了这两个接口后，需要手动跳转路径)，在<form-login>里面可配置。
        4、修改配置文件。
Xml代码  ![收藏代码](http://aokunsang.iteye.com/images/icon_star.png)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beans:beansxmlns="http://www.springframework.org/schema/security"
- xmlns:beans="http://www.springframework.org/schema/beans"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-3.0.xsd  
-                     http://www.springframework.org/schema/security http://www.springframework.org/schema/security/spring-security-3.1.xsd">
- 
- <httppattern="/resources/**"security="none"></http>
- <httppattern="/user/login"security="none"></http>
- 
- <httpauto-config="true"access-denied-page="/user/denied">
-   <!--   
-     default-target-url       指定了从登录页面登录后进行跳转的页面  
-     always-use-default-target   true表示登录成功后强制跳转  
-     authentication-failure-url  表示验证失败后进入的页面  
-     login-processing-url       设置验证登录验证地址，如果不设置，默认是j_spring_security_check  
-     username-parameter,password-parameter     设置登录用户名和密码的请求name，默认：j_username,j_password  
- default-target-url="/user/home"
-    -->
- <form-loginlogin-page="/user/login"
- always-use-default-target="true"
- authentication-failure-url="/user/login?error=1"
- login-processing-url="/logincheck"
- authentication-success-handler-ref="successHandler"/>
- 
- <logoutlogout-url="/logout"logout-success-url="/user/login"/>
-     <!--   
-          error-if-maximum-exceeded 后登陆的账号会挤掉第一次登陆的账号   
-          session-fixation-protection  防止伪造sessionid攻击,用户登录成功后会销毁用户当前的session。  
-     -->
- <session-managementinvalid-session-url="/user/timedout"session-fixation-protection="none">
- <concurrency-controlmax-sessions="1"error-if-maximum-exceeded="true"/>
- </session-management>
- <custom-filterref="mySecurityFilter"before="FILTER_SECURITY_INTERCEPTOR"/>
- </http>
- 
- <authentication-manageralias="authManager">
- <authentication-provideruser-service-ref="userServiceDetail">
-         <!--<jdbc-user-servicedata-source-ref="dataSource"
- authorities-by-username-query=""
- group-authorities-by-username-query=""/> -->
- <password-encoderhash="md5">
- <salt-sourceuser-property="username"/><!-- 盐值  [添加这个属性后，加密密码明文为:"密码明文{盐值}"] -->
- </password-encoder>
- 
- </authentication-provider>
- </authentication-manager>
- 
- 
- <!-- 登录失败后业务处理 -->
- <beans:beanid="failureHandler"class="com.aokunsang.security.LoginAuthenticationFailureHandler"></beans:bean>
- <!-- 登录成功业务处理 -->
- <beans:beanid="successHandler"class="com.aokunsang.security.LoginAuthenticationSuccesssHandler">
- <beans:propertyname="defaultUrl"value="/user/admin"></beans:property><!-- 可变换登录成功后的路径，验证用户是否拥有该权限 -->
- </beans:bean>
- 
- <!-- 自定义过滤器  -->
- <beans:beanid="mySecurityFilter"class="com.aokunsang.security.MyFilterSecurityInterceptor">
- <beans:propertyname="accessDecisionManager"ref="accessDescisionManager"></beans:property>
- <beans:propertyname="fisMetadataSource"ref="securityMetadataSource"></beans:property>
- <beans:propertyname="authenticationManager"ref="authManager"></beans:property>
- </beans:bean>
- 
- <beans:beanid="securityMetadataSource"class="com.aokunsang.security.MySecurityMetadataSource">
- <beans:constructor-argname="userService"ref="userService"></beans:constructor-arg>
- </beans:bean>
- 
- <beans:beanid="accessDescisionManager"class="com.aokunsang.security.MyAccessDescisionManager"></beans:bean>
- 
- <beans:beanid="userServiceDetail"class="com.aokunsang.security.MyUserDetailServiceImpl">
- <beans:propertyname="userService">
- <beans:refbean="userService"/>
- </beans:property>
- </beans:bean>
- </beans:beans>
## 4.3.替换form-login配置，实现自己的业务逻辑
如果想在登录之前做一些业务逻辑操作，比如：检查验证码的正确性（这个操作肯定要在验证用户名密码之前操作了）。那么我们自己继承UsernamePasswordAuthenticationFilter类，替换form-login里面的配置，完成检查验证码的操作；这里还需要注意一点，我们还需要实现一个未登录的切点（配置AuthenticationProcessingFilterEntryPoint或者LoginUrlAuthenticationEntryPoint），也就是没登录的都跳转到这个页面，相当于<form-login>中的login-page属性。
这里面的配置注意两点就行：
       1、在<http>中添加未登录切点配置entry-point-ref属性；
       2、去掉<form-login>，添加<custom-filter ref="XXXXFilter" position="FORM_LOGIN_FILTER"/>  
 说明：详细使用方法以及用户账户登录数控制<session-management><concurrency-control></session-management>(单点登录)可参考另一博客[http://aokunsang.iteye.com/blog/1944111](http://aokunsang.iteye.com/blog/1944111)
[http://blog.csdn.net/k10509806/article/details/6436987](http://blog.csdn.net/k10509806/article/details/6436987)
[http://hi.baidu.com/youxitou/item/de0fb00e76e15095a2df43cd](http://hi.baidu.com/youxitou/item/de0fb00e76e15095a2df43cd)
## 4.4.补充问题汇总
          在项目中使用spring Security3.1时，发现抛出的UsernameNotFoundException异常信息，总是打印出Bad credentials。如果我想得到比如：用户不存在，等信息，需要在xml中做设置。
Xml代码  ![收藏代码](http://aokunsang.iteye.com/images/icon_star.png)
- <!-- 使用该类主要解决例如UsernameNotFoundException抛出的异常全部显示Bad credentials[详细参考AbstractUserDetailsAuthenticationProvider:authenticate()];  
-              注意：如果通过用户名已经查询到用户信息(密码错误)，此时抛出异常依然为Bad credentials[详细参考DaoAuthenticationProvider:additionalAuthenticationChecks()]  
-    -->
- <beans:beanid="daoAuthenticationProvider"class="org.springframework.security.authentication.dao.DaoAuthenticationProvider">
- <beans:propertyname="userDetailsService"ref="userServiceDetail"></beans:property>
- <beans:propertyname="passwordEncoder"ref="md5Encoder"></beans:property>
- <beans:propertyname="hideUserNotFoundExceptions"value="false"/><!-- 【关键】没有这个将不能准确地报告异常(全部报告异常为：Bad credentials) -->
- </beans:bean>
- 
- <beans:beanid="md5Encoder"class="org.springframework.security.authentication.encoding.Md5PasswordEncoder"></beans:bean>
- 
- <authentication-manageralias="authManager">
- <authentication-providerref="daoAuthenticationProvider"></authentication-provider>
- </authentication-manager>
- 
# 5.附录
## 5.1.默认请求参数说明
|默认值|说明|可设置|
|----|----|----|
|j_username|请求用户名|<from_login/>中username_parameter属性|
|j_password|请求密码|<from_login/>中password_parameter属性|
|j_spring_security_check|Post请求验证路径|<from_login/>中login_processing_url属性|
|_spring_security_remember_me|“记住我”的请求name|暂无|
|sessionScope['SPRING_SECURITY_LAST_USERNAME']|Session中保存的最后一次登录的用户名|暂无|
