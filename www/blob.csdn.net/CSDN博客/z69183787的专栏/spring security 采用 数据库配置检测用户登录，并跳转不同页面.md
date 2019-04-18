# spring security 采用 数据库配置检测用户登录，并跳转不同页面 - z69183787的专栏 - CSDN博客
2014年03月16日 08:16:52[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3044
applicationContext-security.xml
```java
<?xml version="1.0" encoding="UTF-8"?>
<beans:beans xmlns="http://www.springframework.org/schema/security"
	xmlns:beans="http://www.springframework.org/schema/beans" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xsi:schemaLocation="http://www.springframework.org/schema/beans
           http://www.springframework.org/schema/beans/spring-beans-3.1.xsd
           http://www.springframework.org/schema/security
           http://www.springframework.org/schema/security/spring-security-3.1.xsd">
	<global-method-security pre-post-annotations="enabled" />
	
	<http pattern="/images/**" security="none" />
	<http pattern="/widgets/**" security="none" />
	<http pattern="/css/**" security="none" />
	<http pattern="/style/**" security="none" />
	<http pattern="**/*.js" security="none" />
	<http pattern="**/*.swf" security="none" />
	<http pattern="/login.jsp" security="none" />
	<http pattern="/session-timeout.jsp" security="none"/>
	<http pattern="/session-expired.jsp" security="none"/>
	<http auto-config="true" use-expressions="true">		
		<!-- <intercept-url pattern="/**" access="isAuthenticated()" /> -->
		<intercept-url pattern="/**" access="permitAll" />
		<form-login login-page='/login.jsp' 
			 authentication-success-handler-ref="authSuccessHandlerImpl"
			 authentication-failure-handler-ref="authFailureHandlerImpl"/>
        
		<session-management invalid-session-url="/session-timeout.jsp">
            <concurrency-control max-sessions="1" expired-url="/session-expired.jsp"/>
        </session-management>
        	
		<logout logout-success-url="/login.jsp" delete-cookies="JSESSIONID"/>
	</http>
	
	<authentication-manager>
       <authentication-provider ref="daoAuthenticationProvider" />
    </authentication-manager>
	
	<beans:bean id="daoAuthenticationProvider"  class="org.springframework.security.authentication.dao.DaoAuthenticationProvider">
		<beans:property name="passwordEncoder" ref="passwordEncoder" />
		<beans:property name="userDetailsService" ref="userService" />
	</beans:bean>
	<beans:bean id="passwordEncoder" class="org.springframework.security.authentication.encoding.Md5PasswordEncoder"/>
	
	<beans:bean id="authSuccessHandlerImpl" class="com.wonders.stpt.security.service.AuthenticationSuccessHandlerImpl">
    </beans:bean>
    
    <beans:bean id="authFailureHandlerImpl" class="com.wonders.stpt.security.service.AuthenticationFailureHandlerImpl">
    </beans:bean>
</beans:beans>
```
web.xml
```
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE web-app PUBLIC
 "-//Sun Microsystems, Inc.//DTD Web Application 2.3//EN"
 "http://java.sun.com/dtd/web-app_2_3.dtd" >
<web-app>
	<display-name>STPT_QUERY</display-name>
	<context-param>
		<param-name>contextConfigLocation</param-name>
		<param-value>classpath*:/applicationContext.xml</param-value>
	</context-param>
	<filter>
		<filter-name>springSecurityFilterChain</filter-name>
		<filter-class>org.springframework.web.filter.DelegatingFilterProxy</filter-class>
	</filter>
	<filter>
		<filter-name>characterEncodingFilter</filter-name>
		<filter-class>org.springframework.web.filter.CharacterEncodingFilter</filter-class>
		<init-param>
			<param-name>encoding</param-name>
			<param-value>UTF-8</param-value>
		</init-param>
	</filter>
	<filter>
		<filter-name>struts2</filter-name>
		<filter-class>org.apache.struts2.dispatcher.ng.filter.StrutsPrepareAndExecuteFilter</filter-class>
	</filter>
	<filter-mapping>
		<filter-name>springSecurityFilterChain</filter-name>
		<url-pattern>/*</url-pattern>
	</filter-mapping>
	<filter-mapping>
		<filter-name>characterEncodingFilter</filter-name>
		<url-pattern>/*</url-pattern>
	</filter-mapping>
	<filter-mapping>
		<filter-name>struts2</filter-name>
		<url-pattern>*.action</url-pattern>
	</filter-mapping>
	<filter-mapping>
		<filter-name>struts2</filter-name>
		<url-pattern>*.jsp</url-pattern>
	</filter-mapping>
	<listener>
		<listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
	</listener>
	<listener>
		<listener-class>org.springframework.security.web.session.HttpSessionEventPublisher</listener-class>
	</listener>
	<!-- <listener>
		<listener-class>com.wonders.stpt.framework.web.listener.MyBatisInitListener</listener-class>
	</listener> -->
	
	<session-config>
		<session-timeout>120</session-timeout>
	</session-config>
	<welcome-file-list>
        <welcome-file>login.jsp</welcome-file>
    </welcome-file-list>
	<taglib>
		<taglib-uri>/struts-tags</taglib-uri>
		<taglib-location>/WEB-INF/tlds/struts-tags.tld</taglib-location>
	</taglib>
</web-app>
```
登陆失败 AuthenticationFailureHandlerImpl
```java
package com.wonders.stpt.security.service;
import java.io.IOException;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.springframework.security.core.AuthenticationException;
import org.springframework.security.web.authentication.AuthenticationFailureHandler;
public class AuthenticationFailureHandlerImpl
  implements AuthenticationFailureHandler
{
  public void onAuthenticationFailure(HttpServletRequest request, HttpServletResponse response, AuthenticationException exception)
    throws IOException, ServletException
  {
    response.sendRedirect(request.getContextPath() + "/login.jsp?login_error=1");
  }
}
```
登陆成功：AuthenticationSuccessHandlerImpl
```java
package com.wonders.stpt.security.service;
import com.wonders.stpt.security.domain.SecurityLog;
import java.io.IOException;
import javax.annotation.Resource;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.springframework.security.core.Authentication;
import org.springframework.security.web.authentication.AuthenticationSuccessHandler;
public class AuthenticationSuccessHandlerImpl
  implements AuthenticationSuccessHandler
{
  @Resource
  private SecurtiyLogService securityLogService;
  public void onAuthenticationSuccess(HttpServletRequest request, HttpServletResponse response, Authentication authentication)
    throws IOException, ServletException
  {
    SecurityLog log = new SecurityLog(request.getRemoteAddr(), "登入", 
      authentication.getName());
    this.securityLogService.save(log);
    response.sendRedirect(request.getContextPath() + "/query.jsp");
  }
  public SecurtiyLogService getSecurityLogService() {
    return this.securityLogService;
  }
  public void setSecurityLogService(SecurtiyLogService securityLogService) {
    this.securityLogService = securityLogService;
  }
  public static String getIpAddr(HttpServletRequest request) {
    String ip = request.getHeader("x-forwarded-for");
    if ((ip == null) || (ip.length() == 0) || ("unknown".equalsIgnoreCase(ip))) {
      ip = request.getHeader("Proxy-Client-IP");
    }
    if ((ip == null) || (ip.length() == 0) || ("unknown".equalsIgnoreCase(ip))) {
      ip = request.getHeader("WL-Proxy-Client-IP");
    }
    if ((ip == null) || (ip.length() == 0) || ("unknown".equalsIgnoreCase(ip))) {
      ip = request.getRemoteAddr();
    }
    return ip;
  }
}
```
user类 需要重写 equals方法
```java
package com.wonders.stpt.security.domain;
import java.util.Collection;
import java.util.HashSet;
import java.util.Set;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.Id;
import javax.persistence.Table;
import javax.persistence.Transient;
import org.hibernate.annotations.Cache;
import org.hibernate.annotations.CacheConcurrencyStrategy;
import org.hibernate.annotations.GenericGenerator;
import org.springframework.security.core.GrantedAuthority;
import org.springframework.security.core.userdetails.UserDetails;
@Entity
@Cache(usage=CacheConcurrencyStrategy.READ_WRITE)
@Table(name="SEC_USER", uniqueConstraints={@javax.persistence.UniqueConstraint(columnNames={"USER_NAME"})})
public class User
  implements UserDetails
{
  private static final long serialVersionUID = -6642778955506920404L;
  private String id;
  private String username;
  private String realName;
  private String password;
  private String address;
  private String phoneNumber;
  private Boolean enabled;
  @Id
  @GeneratedValue(generator="generator")
  @GenericGenerator(name="generator", strategy="guid")
  @Column(name="ID", unique=true, nullable=false, length=32)
  public String getId()
  {
    return this.id;
  }
  public void setId(String id) {
    this.id = id;
  }
  @Column(name="USER_NAME", unique=true, nullable=false, length=50)
  public String getUsername() {
    return this.username;
  }
  public void setUsername(String username) {
    this.username = username;
  }
  @Column(name="REAL_NAME", length=100)
  public String getRealName() {
    return this.realName;
  }
  public void setRealName(String realName) {
    this.realName = realName;
  }
  @Column(name="PASSWORD", nullable=false)
  public String getPassword() {
    return this.password;
  }
  public void setPassword(String password) {
    this.password = password;
  }
  @Column(name="PHONE_NUMBER", length=20)
  public String getPhoneNumber() {
    return this.phoneNumber;
  }
  public void setPhoneNumber(String phoneNumber) {
    this.phoneNumber = phoneNumber;
  }
  @Column(name="IS_ENABLED", precision=1, scale=0)
  public boolean isEnabled() {
    return this.enabled.booleanValue();
  }
  public void setEnabled(Boolean enabled) {
    this.enabled = enabled;
  }
  @Column(name="ADDRESS", length=200)
  public String getAddress() {
    return this.address;
  }
  public void setAddress(String address) {
    this.address = address;
  }
  @Transient
  public Collection<GrantedAuthority> getAuthorities() {
    Set authoritys = new HashSet(0);
    return authoritys;
  }
  @Transient
  public boolean isAccountNonExpired() {
    return true;
  }
  @Transient
  public boolean isAccountNonLocked() {
    return true;
  }
  @Transient
  public boolean isCredentialsNonExpired() {
    return true;
  }
  public boolean equals(Object obj)
  {
    if (this == obj)
      return true;
    if (obj == null)
      return false;
    if (getClass() != obj.getClass())
      return false;
    User other = (User)obj;
    if (this.id == null) {
      if (other.id != null)
        return false;
    } else if (!this.id.equals(other.id))
      return false;
    return true;
  }
}
```
user 检测类 实现指定接口，达到通过数据库检测目的：
```java
package com.wonders.stpt.security.service;
import com.wonders.stpt.framework.persist.HibernateDao;
import com.wonders.stpt.framework.service.AbstractBaseService;
import com.wonders.stpt.security.domain.User;
import com.wonders.stpt.security.persist.UserDao;
import com.wonders.stpt.security.utils.SecurityUtils;
import org.apache.commons.codec.digest.DigestUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.dao.DataAccessException;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.core.userdetails.UsernameNotFoundException;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
@Service
public class UserService extends AbstractBaseService<User, String>
  implements UserDetailsService
{
  private UserDao userDao;
  protected HibernateDao<User, String> getDao()
  {
    return this.userDao;
  }
  public void delete(String id)
  {
    this.userDao.delete(id);
  }
  @Transactional(readOnly=true)
  public UserDetails loadUserByUsername(String username) throws UsernameNotFoundException, DataAccessException
  {
    User user = this.userDao.loadUserByUsername(username);
    if (user == null) {
      throw new UsernameNotFoundException("用户:" + username + "没有找到!");
    }
    return user;
  }
  @Autowired
  public void setUserDao(UserDao userDao) {
    this.userDao = userDao;
  }
  @Transactional
  public void changePassword(User user, String password) {
    user.setPassword(DigestUtils.md5Hex(password));
    this.userDao.save(user);
  }
  public void save(User user)
  {
    if (user.getPassword().equals(SecurityUtils.getCurrentUserPassword()))
      getDao().save(user);
    else
      changePassword(user, user.getPassword());
  }
}
```
