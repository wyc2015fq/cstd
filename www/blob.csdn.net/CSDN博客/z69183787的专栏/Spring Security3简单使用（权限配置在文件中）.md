# Spring Security3简单使用（权限配置在文件中） - z69183787的专栏 - CSDN博客
2014年03月16日 08:16:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6354
　　1、权限既然写在配置文件中，那么数据库中只需要三个表即可。
　　　　1）t_user　　用户表
　　　　2）t_role　　角色表
　　　　3）t_user_role　　用户角色表　
　　2、对应的领域实体
　　　　1）用户
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
package cn.luxh.app.domain;
/**
 * 用户
 * @author Luxh
 */
public class User {
    
    private Integer id;
    /**帐号*/
    private String account;
    /**密码*/
    private String password;
    
    
    @Override
    public int hashCode() {
        return account.hashCode();
    }
    @Override
    public boolean equals(Object obj) {
        User user = (User) obj;
        return this.account.equals(user.getAccount());
    }
    
    //getter setter
    //...
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　　　2）角色
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
package cn.luxh.app.domain;
/**
 * 角色
 * @author Luxh
 */
public class Role {
    
    private Integer id;
    /**角色名称*/
    private String name;
    
    //getter setter
    //...
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　　　3）用户-角色
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
package cn.luxh.app.domain;
/**
 * 用户角色
 * @author Luxh
 */
public class UserRole {
    private Integer id;
    /**用户id*/
    private Integer userId;
    /**角色id*/
    private Integer roleId;
    
    //getter setter
    //...
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　3、配置文件
　　　　在web.xml文件中加上如下内容：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<!-- SpringSecurity权限框架 -->  
  <filter>  
        <filter-name>springSecurityFilterChain</filter-name>  
        <filter-class>  
            org.springframework.web.filter.DelegatingFilterProxy  
        </filter-class>  
   </filter>  
    <filter-mapping>  
        <filter-name>springSecurityFilterChain</filter-name>  
        <url-pattern>/*</url-pattern>  
    </filter-mapping>  
 <!--  获取Spring Security session的生命周期-->  
    <listener>  
        <listener-class>  
         org.springframework.security.web.session.HttpSessionEventPublisher   
        </listener-class>  
    </listener>  
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　　　当然配置spring监听器的时候得把springsecurity的权限配置文件给加载进去：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<!-- 配置Spring监听器 -->
    <listener>
        <listener-class>
            org.springframework.web.context.ContextLoaderListener
        </listener-class>
    </listener>
    <context-param>
        <param-name>contextConfigLocation</param-name>
        <param-value>classpath:applicationContext.xml,classpath:application-security.xml</param-value>
    </context-param>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　　　权限配置文件内容如下：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<?xml version="1.0" encoding="UTF-8"?>
<beans:beans xmlns="http://www.springframework.org/schema/security"
    xmlns:beans="http://www.springframework.org/schema/beans" 
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://www.springframework.org/schema/beans 
                        http://www.springframework.org/schema/beans/spring-beans-3.2.xsd
                        http://www.springframework.org/schema/security 
                        http://www.springframework.org/schema/security/spring-security-3.1.xsd">
    <!-- 登录页面不拦截 -->
    <http pattern="/login" security="none" />
    <!-- 资源文件不拦截 -->
    <http pattern="/resources/**" security="none" />
    
    <http auto-config="true" use-expressions="true" access-denied-page="/denied">
        <!-- default-target-url 指定了从登录页面登录后进行跳转的页面 always-use-default-target true表示登录成功后强制跳转 
            authentication-failure-url 表示验证失败后进入的页面 login-processing-url 设置验证登录验证地址，如果不设置，默认是j_spring_security_check 
            username-parameter,password-parameter 设置登录用户名和密码的请求name，默认：j_username,j_password 
            default-target-url="/user/home" -->
        <form-login login-page="/login"
            always-use-default-target="true"
            authentication-success-handler-ref="successHandler"
            authentication-failure-handler-ref="failureHandler" />
        
        <!-- 管理员和普通用户可以访问 -->
        <intercept-url pattern="/index" access="hasAnyRole('ROLE_ADMIN','ROLE_USER')" />
        <!-- 管理员和普通用户可以访问 -->
        <intercept-url pattern="/common" access="hasAnyRole('ROLE_ADMIN','ROLE_USER')" />
        <!-- 只有管理员可以访问 -->
        <intercept-url pattern="/admin" access="hasAnyRole('ROLE_ADMIN')" />
        <!-- 退出后到登陆页面 -->
        <logout logout-success-url="/login" />
        <!-- error-if-maximum-exceeded 后登陆的账号会挤掉第一次登陆的账号 
            session-fixation-protection 
            防止伪造sessionid攻击. 用户登录成功后会销毁用户当前的session.   
                创建新的session,并把用户信息复制到新session中. -->
        <session-management invalid-session-url="/login?error=3"
            session-fixation-protection="none">
            <concurrency-control max-sessions="1"
                error-if-maximum-exceeded="true" expired-url="/login?error=2" /><!-- 阻止第二次登录 -->
        </session-management>
    </http>
    <authentication-manager alias="authenticationManager">
        <authentication-provider user-service-ref="UserDetailsService"/>
    </authentication-manager>
    <beans:bean id="UserDetailsService" class="cn.luxh.app.security.UserDetailsServiceImpl" />
    
    <!-- 登录成功业务处理 -->
    <beans:bean id="successHandler"
        class="cn.luxh.app.security.LoginAuthenticationSuccessHandler">
        <beans:property name="url" value="/index"></beans:property>  
    </beans:bean>
    
    <!-- 登录失败业务处理 -->
    <beans:bean id="failureHandler" class="cn.luxh.app.security.LoginAuthenticationFailureHandler"/>
    
</beans:beans>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　4、权限配置文件中用到的类
　　　　1）UserDetailsServiceImpl
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
package cn.luxh.app.security;
import java.util.Collection;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.core.GrantedAuthority;
import org.springframework.security.core.authority.SimpleGrantedAuthority;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.core.userdetails.UsernameNotFoundException;
import cn.luxh.app.domain.Role;
import cn.luxh.app.domain.User;
import cn.luxh.app.persistence.RoleMapper;
import cn.luxh.app.persistence.UserMapper;
public class UserDetailsServiceImpl implements UserDetailsService{
    
    private static Logger log = LoggerFactory.getLogger(UserDetailsServiceImpl.class);
    
    @Autowired
    private UserMapper userMapper;
    
    @Autowired
    private RoleMapper roleMapper;
    
    /**
     * @param account 登录帐号
     */
    public UserDetails loadUserByUsername(String account)
            throws UsernameNotFoundException {
        log.info("登录账号："+account);
        org.springframework.security.core.userdetails.User userDetails = null;
        User user = userMapper.selectByAccount(account);
        
        //账号密码错误，可以在这里手动抛出异常，让验证失败处理器AuthenticationFailureHandler进行处理
        
        Collection<GrantedAuthority> grantedAuthorities = getGrantedAuthorities(user);  
        boolean enables = true;  
        boolean accountNonExpired = true;  
        boolean credentialsNonExpired = true;  
        boolean accountNonLocked = true; 
        userDetails = new org.springframework.security.core.userdetails.User(user.getAccount(), user.getPassword(), enables, accountNonExpired, credentialsNonExpired, accountNonLocked, grantedAuthorities);  
        return userDetails;
    }
    
    /**
     * 根据用户获取该用户拥有的角色
     * @param user
     * @return
     */
    private Set<GrantedAuthority> getGrantedAuthorities(User user) {
        Set<GrantedAuthority> grantedAuthorities = new HashSet<GrantedAuthority>();  
        List<Role> roles = roleMapper.selectByUserId(user.getId()); 
        if(roles != null) {
            for(Role role : roles) {  
                grantedAuthorities.add(new SimpleGrantedAuthority(role.getName()));
            }  
        }
        return grantedAuthorities;  
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　　　UserMapper和RoleMapper是我使用MyBatis访问数据库的接口。
　　　　2）LoginAuthenticationSuccessHandler
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
package cn.luxh.app.security;
import java.io.IOException;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.security.core.Authentication;
import org.springframework.security.web.authentication.AuthenticationSuccessHandler;
/**
 * 登录验证成功处理器
 * @author Luxh
 */
public class LoginAuthenticationSuccessHandler implements AuthenticationSuccessHandler{
    
    private static Logger log = LoggerFactory.getLogger(LoginAuthenticationSuccessHandler.class);
    
    //登录验证成功后需要跳转的url
    private String url;
    
    public void onAuthenticationSuccess(HttpServletRequest request,
            HttpServletResponse response, Authentication authentication) throws IOException,
            ServletException {
        log.info("登录验证成功："+request.getContextPath()+url);
        //response.sendRedirect(request.getContextPath()+url);
        request.getRequestDispatcher(url).forward(request, response);
    }
    
    public void setUrl(String url) {
        this.url = url;
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　　　3）LoginAuthenticationFailureHandler
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
package cn.luxh.app.security;
import java.io.IOException;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.springframework.security.core.AuthenticationException;
import org.springframework.security.web.authentication.AuthenticationFailureHandler;
/**
 * 登录验证出错处理
 * @author Luxh
 */
public class LoginAuthenticationFailureHandler implements AuthenticationFailureHandler {
    @Override
    public void onAuthenticationFailure(HttpServletRequest request,
            HttpServletResponse response, AuthenticationException ae)
            throws IOException, ServletException {
        
        //根据AuthenticationException异常的类型
        //进行出错业务逻辑处理
        //...
        
        response.sendRedirect(request.getContextPath()+"/login");
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　5、SpringMVC的Controller
　　　　1）LoginController
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
package cn.luxh.app.controller;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
@Controller
public class LoginController {
    
    private static Logger log = LoggerFactory.getLogger(LoginController.class);
    
    /**
     * 登录入口
     */
    @RequestMapping(value={"/","/login"})
    public String login(@RequestParam(required=false) String error) {
        log.info("login......");
        if("1".equals(error)) {
            log.info("验证失败！");
        }else if("2".equals(error)) {
            log.info("你的帐号已登录，不允许重复登陆！");
        }else if("3".equals(error)) {
            log.info("会话超时!");
        }
        return "login";
    }
    
    /**
     * 没有权限访问跳转url
     */
    @RequestMapping(value="/denied")
    public String denied(){
        log.info("denied......");
        return "denied";
    }
    
    /**
     * 超时跳转url
     */
    @RequestMapping(value="/timeout")
    public String timedout(){
        log.info("timeout......");
        return "timedout";
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　　　2）IndexController
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
package cn.luxh.app.controller;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
@Controller
public class IndexController {
    
    private static Logger log = LoggerFactory.getLogger(IndexController.class);
    
    /**
     * 管理员和普通用户可以访问
     */
    @RequestMapping(value="/index")
    public String index(){
        log.info("index.......");
        return "index";
    }
    
    /**
     * 管理员和普通用户可以访问
     */
    @RequestMapping(value="/common")
    public String myJsp(){
        log.info("common.......");
        return "common";
    }
    
    /**
     * 管理员可以访问
     */
    @RequestMapping(value="/admin")
    public String admin(){
        log.info("admin.......");
        return "admin";
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
 　　6、源码，不含jar包
　　　　spring3.2.2+springsecurity3.1.3+myBatis3.2.2
[http://files.cnblogs.com/luxh/app3.rar](http://files.cnblogs.com/luxh/app3.rar)
