# Servlet 使用Filter HttpSessionListener HttpSessionAttributeListener实现webSession管理 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年11月04日 16:58:08[boonya](https://me.csdn.net/boonya)阅读数：2337








前言：Servlet的使用逐渐被淡化了，但是它是基于Http方式请求的扩展和延伸。很多移动开发都有监听、事件等的处理，这篇文章就看看web端的session监听处理吧。

#### 1.web.xml配置



```
<?xml version="1.0" encoding="UTF-8"?>
<web-app xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xmlns="http://java.sun.com/xml/ns/javaee"
	xsi:schemaLocation="http://java.sun.com/xml/ns/javaee http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd"
	version="2.5">
	<context-param>
		<param-name>contextConfigLocation</param-name>
		<param-value>classpath:applicationContext.xml</param-value>
	</context-param>
	<context-param>
		<param-name>log4jConfigLocation</param-name>
		<param-value>classpath:log4j.properties</param-value>
	</context-param>
	<listener>
		<listener-class>org.springframework.web.util.Log4jConfigListener</listener-class>
	</listener>
	<filter>
		<filter-name>encodingFilter</filter-name>
		<filter-class>org.springframework.web.filter.CharacterEncodingFilter</filter-class>
		<init-param>
			<param-name>encoding</param-name>
			<param-value>utf-8</param-value>
		</init-param>
	</filter>
	<filter-mapping>
		<filter-name>encodingFilter</filter-name>
		<url-pattern>/*</url-pattern>
	</filter-mapping>
	<filter>
		<filter-name>sessionFilter</filter-name>
		<filter-class>com.wlyd.wms.util.syswatch.SessionFilter</filter-class>
	</filter>
	<filter-mapping>
		<filter-name>sessionFilter</filter-name>
		<url-pattern>/*</url-pattern>
	</filter-mapping>
	<session-config>
		<session-timeout>30</session-timeout>
	</session-config>
	<listener>
		<listener-class>com.wlyd.wms.util.syswatch.WebSessionListener</listener-class>
	</listener>
	<listener>
		<listener-class>org.springframework.web.util.Log4jConfigListener</listener-class>
	</listener>
	<listener>
		<listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
	</listener>
	<listener>
		<listener-class>org.springframework.web.context.request.RequestContextListener</listener-class>
	</listener>
	<servlet>
		<servlet-name>springMvc</servlet-name>
		<servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
		<init-param>
			<param-name>contextConfigLocation</param-name>
			<param-value>classpath:Spring-mvc.xml</param-value>
		</init-param>
		<load-on-startup>1</load-on-startup>
	</servlet>
	<servlet-mapping>
		<servlet-name>springMvc</servlet-name>
		<url-pattern>*.html</url-pattern>
	</servlet-mapping>
	<servlet-mapping>
		<servlet-name>springMvc</servlet-name>
		<url-pattern>/admin</url-pattern>
	</servlet-mapping>
	<error-page>
		<error-code>404</error-code>
		<location>/404.jsp</location>
	</error-page>
	<error-page>
		<error-code>500</error-code>
		<location>/500.jsp</location>
	</error-page>
	<error-page>
		<error-code>400</error-code>
		<location>/500.jsp</location>
	</error-page>
	<!-- 默认首页定义 -->
	<welcome-file-list>
		<welcome-file>admin</welcome-file>
	</welcome-file-list>
</web-app>
```



#### 2.SessionFiler实现

SessionFilter.java




```java
package com.wlyd.wms.util.syswatch;

import java.io.IOException;

import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import com.wlyd.wms.persistence.beans.EsUser;
import com.wlyd.wms.util.OperationSession;
import com.wlyd.wms.util.Tools;
import com.wlyd.wms.util.ehcache.EhcacheUtil;
import com.wlyd.wms.util.finals.Const;

/**
 * @author arrow 登录超时过滤器
 */
public class SessionFilter implements Filter {

	public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain) throws IOException, ServletException {
		// 由于 session 属于 HTTP 范畴，故需要向下转型成 HttpServletRequest 类型
		HttpServletRequest req = (HttpServletRequest) request;
		HttpServletResponse res = (HttpServletResponse) response;
		String path = req.getServletPath();
		if (!path.matches((String) EhcacheUtil.get(Const.NO_INTERCEPTOR_PATH))) {
			String baseUrl = Tools.getBaseUrl();
			EsUser user = null;
			if (path.contains("/admin/")) {
				user = (EsUser) OperationSession.getSession(req, Const.SESSION_BACK_USER);
				if (user == null) {
					res.sendRedirect(baseUrl + "/admin.html");
					return;
				}else{
					OperationSession.putSession(req, Const.SESSION_BACK_USER, user);
				}
			}
		}
		chain.doFilter(request, response);
	}

	@Override
	public void init(FilterConfig filterConfig) throws ServletException {
		// TODO Auto-generated method stub

	}

	@Override
	public void destroy() {
		// TODO Auto-generated method stub

	}
}
```



#### 3.SessionListener实现

WebSessionListener.java



```java
package com.wlyd.wms.util.syswatch;

import java.util.Date;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import javax.servlet.http.HttpSession;
import javax.servlet.http.HttpSessionAttributeListener;
import javax.servlet.http.HttpSessionBindingEvent;
import javax.servlet.http.HttpSessionEvent;
import javax.servlet.http.HttpSessionListener;
import org.apache.log4j.Logger;
import com.wlyd.wms.persistence.beans.EsUser;
import com.wlyd.wms.util.DateUtil;
import com.wlyd.wms.util.finals.Const;

/**
 * 监听所有会话的创建和失效,供后期监控及分析用
 * @author arrow
 *
 */
public class WebSessionListener implements HttpSessionListener,HttpSessionAttributeListener {
	
	public static final Map<String, HttpSession> allsession = new ConcurrentHashMap<String, HttpSession>();//session
	
	Logger logger = Logger.getLogger(HihSoftSessionListener.class);
	
	//新建一个会话时候触发也可以说是客户端第一次和服务器交互时候触发
	@Override
	public void sessionCreated(HttpSessionEvent e) {
		logger.info("有客户端向服务器请求新的会话"+e.getSession().getId());
	}
	// 销毁一个session时触发此操作   一般来说只有某个按钮触发进行销毁 或者配置定时销毁
	@Override
	public void sessionDestroyed(HttpSessionEvent e) {
		Const.context.remove(e.getSession().getId());
		allsession.remove(e.getSession().getId());
		logger.info("session会话："+e.getSession().getId()+"超时,自动销毁.");
	}
	// 在session中添加对象时触发此操作
	@Override
	public void attributeAdded(HttpSessionBindingEvent se) {
		// TODO Auto-generated method stub
		if(se.getName()==Const.SESSION_BACK_USER){
			EsUser u=(EsUser) se.getSession().getAttribute(se.getName());
			SessionControl sc=new SessionControl();
			sc.setCreateDate(DateUtil.date2String(new Date(), "yyyy-MM-dd HH:mm:ss"));
			sc.setLoginName(u.getEsLoginName());//登录账号
			sc.setSessionId(se.getSession().getId());//会话id
			sc.setLastIp(u.getEsLastIp());//客户端IP
			sc.setLastAddress(u.getEsLastAddress());//客户端地址
			Const.context.put(se.getSession().getId(), sc);
			logger.info("有用户session会话发生："+se.getName()+"当前用户会话数:"+Const.context.size());
		}
		allsession.put(se.getSession().getId(), se.getSession());
		logger.info("有session会话发生："+se.getName()+"当前会话总数:"+allsession.size());
	}
	// 修改、删除session中添加对象时触发此操作  笼统的说就是调用 removeAttribute这个方法时候会触发的
	@Override
	public void attributeRemoved(HttpSessionBindingEvent se) {
		// TODO Auto-generated method stub
		if(se.getName()==Const.SESSION_BACK_USER){
			Const.context.remove(se.getSession().getId());
			logger.info("有用户session会话："+se.getName()+"被修改或被删除");
		}
		allsession.remove(se.getSession().getId());
		logger.info("有其他session会话："+se.getName()+"被修改或被删除");
	}
	 //在Session属性被重新设置时
	@Override
	public void attributeReplaced(HttpSessionBindingEvent se) {
		// TODO Auto-generated method stub
		if(se.getName()==Const.SESSION_BACK_USER){
			// 用户在操作系统过程中只要有请求就更新session
			SessionControl originalSession=Const.context.get(se.getSession().getId());
			if(originalSession!=null){
				originalSession.setCreateDate(DateUtil.date2String(new Date(), "yyyy-MM-dd HH:mm:ss"));
				Const.context.put(se.getSession().getId(), originalSession);
			}
		}
		allsession.put(se.getSession().getId(), se.getSession());
	}
}
```

#### 4.其他基础bean

BaseBean.java



```java
package com.wlyd.wms.util.base;

import java.io.Serializable;
import java.util.Date;

import com.wlyd.wms.persistence.beans.EsUser;
import com.wlyd.wms.util.DateUtil;
import com.wlyd.wms.util.Tools;

public class BaseBean implements Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = -1727605156609983257L;

	private Integer createUser;// 创建人

	private String createDate;// 创建时间

	private Integer updateUser;// 更新人

	private String updateDate;// 更新时间

	private String lastIp;// 最后登录ip

	private String lastAddress;// 最后登录地址

	private String lastLoginDate;// 最后登录时间

	private EsUser createUserObj;// 创建人

	private EsUser updateUserObj;// 更新人

	private Integer disabled;
	
	private Date modifyTime;//修改时间
	
	private Integer modifier;//修改人
	
	private Date createTime;//创建时间
	
	private Integer creator;//创建人
	
	@SuppressWarnings("unused")
	private String strModifyTime;//修改日期格式化
	@SuppressWarnings("unused")
	private String strCreateTime;//创建日期格式化
	
	private String modifierName;//修改人名称
	private String creatorName;//创建人名称
	
	public String getStrModifyTime() {
		if(modifyTime !=null){
		return DateUtil.getDateFormat(modifyTime);
		}else{
			return "";
		}
	}

	public String getStrCreateTime() {
		if(createTime !=null){
		return DateUtil.getDateFormat(createTime);
		}else{
			return "";
		}
	}

	public String getModifierName() {
		return modifierName;
	}

	public String getCreatorName() {
		return creatorName;
	}

	public Integer getCreateUser() {
		return createUser;
	}

	public void setCreateUser(Integer createUser) {
		this.createUser = createUser;
	}

	public Integer getUpdateUser() {
		return updateUser;
	}

	public void setUpdateUser(Integer updateUser) {
		this.updateUser = updateUser;
	}

	public String getCreateDate() {
		if(Tools.isEmpty(createDate)){
			return "";
		}
		if(createDate.endsWith(".0")){
			return Tools.isEmpty(createDate) ? "" : createDate.substring(0, createDate.length() - 2);
		}
		return createDate;
	}

	public void setCreateDate(String createDate) {
		this.createDate = createDate;
	}

	public String getUpdateDate() {
		if(Tools.isEmpty(updateDate)){
			return "";
		}
		if(updateDate.endsWith(".0")){
			return Tools.isEmpty(updateDate) ? "" : updateDate.substring(0, updateDate.length() - 2);
		}
		return updateDate;
	}

	public void setUpdateDate(String updateDate) {
		this.updateDate = updateDate;
	}

	public String getLastIp() {
		return lastIp;
	}

	public void setLastIp(String lastIp) {
		this.lastIp = lastIp;
	}

	public String getLastAddress() {
		return lastAddress;
	}

	public void setLastAddress(String lastAddress) {
		this.lastAddress = lastAddress;
	}

	public String getLastLoginDate() {
		return lastLoginDate;
	}

	public void setLastLoginDate(String lastLoginDate) {
		this.lastLoginDate = lastLoginDate;
	}

	public EsUser getCreateUserObj() {
		return createUserObj;
	}

	public void setCreateUserObj(EsUser createUserObj) {
		this.createUserObj = createUserObj;
	}

	public EsUser getUpdateUserObj() {
		return updateUserObj;
	}

	public void setUpdateUserObj(EsUser updateUserObj) {
		this.updateUserObj = updateUserObj;
	}

	public Integer getDisabled() {
		return disabled;
	}

	public void setDisabled(Integer disabled) {
		this.disabled = disabled;
	}

	public Date getModifyTime() {
		return modifyTime;
	}

	public void setModifyTime(Date modifyTime) {
		this.modifyTime = modifyTime;
	}

	public Integer getModifier() {
		return modifier;
	}

	public void setModifier(Integer modifier) {
		this.modifier = modifier;
	}

	public Date getCreateTime() {
		return createTime;
	}

	public void setCreateTime(Date createTime) {
		this.createTime = createTime;
	}

	public Integer getCreator() {
		return creator;
	}

	public void setCreator(Integer creator) {
		this.creator = creator;
	}

}
```


SessionControl.java





```java
package com.wlyd.wms.util.syswatch;

import com.wlyd.wms.util.base.BaseBean;


/**
 * @author arrow
 * session会话监控
 */
public class SessionControl extends BaseBean{

	/**
	 * 
	 */
	private static final long serialVersionUID = 8215808163531153788L;
	
	private String sessionId;//会话id
	
	private String loginName;//登录账号
	
	public String getSessionId() {
		return sessionId;
	}

	public void setSessionId(String sessionId) {
		this.sessionId = sessionId;
	}

	public String getLoginName() {
		return loginName;
	}

	public void setLoginName(String loginName) {
		this.loginName = loginName;
	}
}
```


OperationSession.java





```java
package com.wlyd.wms.util;

import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;

import com.wlyd.wms.util.syswatch.HihSoftSessionListener;

/**
 * 封装session会话
 * @author arrow
 *
 */
public class OperationSession{
	/**
	 * 把信息存入session
	 * @param request
	 * @param attributeKey
	 * @param obj
	 */
	public static void putSession(final HttpServletRequest request,
			final String attributeKey, final Object obj) {
		request.getSession().setAttribute(attributeKey, obj);
	}

	/**
	 * 从session中取得信息
	 * @param request
	 * @param attributeKey
	 * @return
	 */
	public static Object getSession(final HttpServletRequest request,
			final String attributeKey) {
		return request.getSession().getAttribute(attributeKey);
	}

	/**
	 * 从request或session中移除信息
	 * @param request
	 * @param key
	 */
	public static void remove(HttpServletRequest request, String key) {
		request.removeAttribute(key);
		request.getSession().removeAttribute(key);
	}
	/**
	 * 杀死客户端session会话
	 * @param request
	 * @param key
	 */
	public static void kill(String key) {
		Map<String, HttpSession> context=HihSoftSessionListener.allsession;
		HttpSession session=(HttpSession) context.get(key);
		session.invalidate();
	}
}
```


Const.java





```java
package com.wlyd.wms.util.finals;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import org.springframework.context.ApplicationContext;
import com.wlyd.wms.util.syswatch.SessionControl;

/**
 * 常量配置类
 * 
 * @author arrow
 */

public class Const {

	public static final String SESSION_BACK_USER = "sessionBackUser";// 后台账户
	public static final String SESSION_SECURITY_CODE = "sessionSecCode";// 验证码
	public static final String NO_INTERCEPTOR_PATH = "NO_INTERCEPTOR_PATH";// 不对匹配该值的访问路径拦截
	public static final Map<String, SessionControl> context = new ConcurrentHashMap<String, SessionControl>();// 用户session会话
	public static final String WEB_SECURE_KEY = "!#&*990abYwd3q_@!#_+&^$45&*";
	public static final String AUTO_LOGIN_COOKIE_NAME = "login_token";
	public static ApplicationContext APPLICATION_CONTEXT;// spring容器
}
```

注：HashMap是非线程安全的，我这里使用了线程安全的ConcurrentHashMap替代HashMap。








