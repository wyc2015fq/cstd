# Spring Security教程（10）---- 自定义登录成功后的处理程序及修改默认验证地址 - z69183787的专栏 - CSDN博客
2014年03月13日 21:25:52[OkidoGreen](https://me.csdn.net/z69183787)阅读数：18742
form-login配置中的authentication-success-handler-ref可以让手动注入登录成功后的处理程序，需要实现AuthenticationSuccessHandler接口。
**[html]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18445003#)[copy](http://blog.csdn.net/jaune161/article/details/18445003#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/161680/fork)
- <sec:form-loginlogin-page="/login.jsp"
- login-processing-url="/login.do"
- authentication-failure-url="/login.jsp"
- authentication-success-handler-ref="authenticationSuccessHandler"
- />
springSecurity默认的登录用户验证路径为：j_spring_security_check，这个路径是可以通过login-processing-url来自己定义，比如我的就定义成了login.do。
然后在前台登录页面中改下form中的action就可以了。
配置文件，注意这里的defaultTargetUrl，本来这个是在form-login中，配置的但是如果我们自己定义登录成功后的处理程序后它就不起作用了，所以这个跳转也需要我们在自定义程序中处理。
**[html]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18445003#)[copy](http://blog.csdn.net/jaune161/article/details/18445003#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/161680/fork)
- <beanid="authenticationSuccessHandler"class="com.zrhis.system.security.SimpleLoginSuccessHandler">
- <propertyname="defaultTargetUrl"value="/WEB-INF/app.jsp"></property>
- <propertyname="forwardToDestination"value="true"></property>
- </bean>
SimpleLoginSuccessHandler，这个类主要处理登录后的处理，我处理的是登录后记录用户的IP地址和登录时间，代码如下 
**[java]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18445003#)[copy](http://blog.csdn.net/jaune161/article/details/18445003#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/161680/fork)
- package com.zrhis.system.security;  
- 
- import java.io.IOException;  
- import java.util.Date;  
- 
- import javax.servlet.ServletException;  
- import javax.servlet.http.HttpServletRequest;  
- import javax.servlet.http.HttpServletResponse;  
- 
- import org.apache.commons.logging.Log;  
- import org.apache.commons.logging.LogFactory;  
- import org.springframework.beans.factory.InitializingBean;  
- import org.springframework.beans.factory.annotation.Autowired;  
- import org.springframework.dao.DataAccessException;  
- import org.springframework.security.core.Authentication;  
- import org.springframework.security.web.DefaultRedirectStrategy;  
- import org.springframework.security.web.RedirectStrategy;  
- import org.springframework.security.web.authentication.AuthenticationSuccessHandler;  
- import org.springframework.transaction.annotation.Propagation;  
- import org.springframework.transaction.annotation.Transactional;  
- import org.springframework.util.StringUtils;  
- 
- import com.zrhis.base.exception.InitializationException;  
- import com.zrhis.system.bean.SysUsers;  
- import com.zrhis.system.repository.SysUsersRepository;  
- 
- publicclass SimpleLoginSuccessHandler implements AuthenticationSuccessHandler,InitializingBean {  
- 
- protected Log logger = LogFactory.getLog(getClass());  
- 
- private String defaultTargetUrl;  
- 
- privateboolean forwardToDestination = false;  
- 
- private RedirectStrategy redirectStrategy = new DefaultRedirectStrategy();  
- 
- @Autowired
- private SysUsersRepository sysUsersRepository;  
- 
- /* (non-Javadoc)
-      * @see org.springframework.security.web.authentication.AuthenticationSuccessHandler#onAuthenticationSuccess(javax.servlet.http.HttpServletRequest, javax.servlet.http.HttpServletResponse, org.springframework.security.core.Authentication)
-      */
- @Override
- @Transactional(readOnly=false,propagation= Propagation.REQUIRED,rollbackFor={Exception.class})  
- publicvoid onAuthenticationSuccess(HttpServletRequest request,  
-             HttpServletResponse response, Authentication authentication)  
- throws IOException, ServletException {  
- 
- this.saveLoginInfo(request, authentication);  
- 
- if(this.forwardToDestination){  
-             logger.info("Login success,Forwarding to "+this.defaultTargetUrl);  
- 
-             request.getRequestDispatcher(this.defaultTargetUrl).forward(request, response);  
-         }else{  
-             logger.info("Login success,Redirecting to "+this.defaultTargetUrl);  
- 
- this.redirectStrategy.sendRedirect(request, response, this.defaultTargetUrl);  
-         }  
-     }  
- 
- @Transactional(readOnly=false,propagation= Propagation.REQUIRED,rollbackFor={Exception.class})  
- publicvoid saveLoginInfo(HttpServletRequest request,Authentication authentication){  
-         SysUsers user = (SysUsers)authentication.getPrincipal();  
- try {  
-             String ip = this.getIpAddress(request);  
-             Date date = new Date();  
-             user.setLastLogin(date);  
-             user.setLoginIp(ip);  
- this.sysUsersRepository.saveAndFlush(user);  
-         } catch (DataAccessException e) {  
- if(logger.isWarnEnabled()){  
-                 logger.info("无法更新用户登录信息至数据库");  
-             }  
-         }  
-     }  
- 
- public String getIpAddress(HttpServletRequest request){    
-         String ip = request.getHeader("x-forwarded-for");    
- if (ip == null || ip.length() == 0 || "unknown".equalsIgnoreCase(ip)) {    
-             ip = request.getHeader("Proxy-Client-IP");    
-         }    
- if (ip == null || ip.length() == 0 || "unknown".equalsIgnoreCase(ip)) {    
-             ip = request.getHeader("WL-Proxy-Client-IP");    
-         }    
- if (ip == null || ip.length() == 0 || "unknown".equalsIgnoreCase(ip)) {    
-             ip = request.getHeader("HTTP_CLIENT_IP");    
-         }    
- if (ip == null || ip.length() == 0 || "unknown".equalsIgnoreCase(ip)) {    
-             ip = request.getHeader("HTTP_X_FORWARDED_FOR");    
-         }    
- if (ip == null || ip.length() == 0 || "unknown".equalsIgnoreCase(ip)) {    
-             ip = request.getRemoteAddr();    
-         }    
- return ip;    
-     }  
- 
- publicvoid setDefaultTargetUrl(String defaultTargetUrl) {  
- this.defaultTargetUrl = defaultTargetUrl;  
-     }  
- 
- publicvoid setForwardToDestination(boolean forwardToDestination) {  
- this.forwardToDestination = forwardToDestination;  
-     }  
- 
- /* (non-Javadoc)
-      * @see org.springframework.beans.factory.InitializingBean#afterPropertiesSet()
-      */
- @Override
- publicvoid afterPropertiesSet() throws Exception {  
- if(StringUtils.isEmpty(defaultTargetUrl))  
- thrownew InitializationException("You must configure defaultTargetUrl");  
- 
-     }    
- 
- }  
其中getIpAddress方法摘自网络，如有雷同纯属必然。
实现InitializingBean，在afterPropertiesSet中我们验证defaultTargetUrl是否为空，如果为空就抛出异常，因为这个地址是必须的。可以根据自己的情况来选择要不要加验证。
如果实现InitializingBean在程序启动是Spring在创建完这个类并注入属性后会自动执行afterPropertiesSet，所以我们的一些初始化的操作也是可以在这里完成的。
onAuthenticationSuccess是主要的接口这个是登录成功后Spring调用的方法，而我们的跳转和保存用户信息都是在这里完成的。
RedirectStrategy是Spring提供的一个客户端跳转的工具类。使用它可以支持“/index.jsp”这种地址，同时可以保证服务器跳转和客户端跳转的路径一致。
加入我们的项目名为my ，项目访问地址为http://localhost:8080/my
现在要使用客户端跳转到 "/login.jsp" 如果是response.sendRedirect 会直接跳转到http://localhost:8080/login.jsp
而使用redirectStrategy.sendRedirect则会跳转到http://localhost:8080/my/login.jsp
在redirectStrategy中，'/'代表的是项目根目录而不是服务器根目录。
