# SpringMVC拦截器的使用和配置 - 零度的博客专栏 - CSDN博客
2016年04月08日 14:23:33[零度anngle](https://me.csdn.net/zmx729618)阅读数：6137
           SpringMVC中的Interceptor 拦截器也是相当重要和相当有用的，它的主要作用是拦截用户的请求并进行相应的处理。比如通过它来进行权限验证，或者是来判断用户是否登陆，或者是像12306 那样子判断当前时间是否是购票时间。
**1、定义Interceptor的实现类 **
          SpringMVC 中的Interceptor 拦截请求是通过HandlerInterceptor 来实现的。在SpringMVC 中定义一个Interceptor 非常简单，主要有两种方式：
          第一种方式是要定义的Interceptor类要实现了Spring 的HandlerInterceptor  接口，或者是这个类继承实现了HandlerInterceptor 接口的类，比如Spring 已经提供的实现了HandlerInterceptor 接口的抽象类HandlerInterceptorAdapter  ；
          第二种方式是实现Spring的WebRequestInterceptor接口，或者是继承实现了WebRequestInterceptor的类。
**2、实现HandlerInterceptor接口**
HandlerInterceptor 接口中定义了三个方法，我们就是通过这三个方法来对用户的请求进行拦截处理的。
     （1 ）preHandle (HttpServletRequest request, HttpServletResponse response, Object handle) 方法，顾名思义，该方法将在请求处理之前进行调用。SpringMVC 中的Interceptor 是链式的调用的，在一个应用中或者说是在一个请求中可以同时存在多个Interceptor 。每个Interceptor 的调用会依据它的声明顺序依次执行，而且最先执行的都是Interceptor
 中的preHandle 方法，所以可以在这个方法中进行一些前置初始化操作或者是对当前请求的一个预处理，也可以在这个方法中进行一些判断来决定请求是否要继续进行下去。该方法的返回值是布尔值Boolean 类型的，当它返回为false 时，表示请求结束，后续的Interceptor 和Controller 都不会再执行；当返回值为true 时就会继续调用下一个Interceptor 的preHandle 方法，如果已经是最后一个Interceptor 的时候就会是调用当前请求的Controller 方法。
     （2 ）postHandle (HttpServletRequest request, HttpServletResponse response, Object handle, ModelAndView modelAndView) 方法，由preHandle 方法的解释我们知道这个方法包括后面要说到的afterCompletion 方法都只能是在当前所属的Interceptor 的preHandle 方法的返回值为true 时才能被调用。postHandle
 方法，顾名思义就是在当前请求进行处理之后，也就是Controller 方法调用之后执行，但是它会在DispatcherServlet 进行视图返回渲染之前被调用，所以我们可以在这个方法中对Controller 处理之后的ModelAndView 对象进行操作。postHandle 方法被调用的方向跟preHandle 是相反的，也就是说先声明的Interceptor 的postHandle 方法反而会后执行，这和Struts2 里面的Interceptor 的执行过程有点类型。Struts2 里面的Interceptor
 的执行过程也是链式的，只是在Struts2 里面需要手动调用ActionInvocation 的invoke 方法来触发对下一个Interceptor 或者是Action 的调用，然后每一个Interceptor 中在invoke 方法调用之前的内容都是按照声明顺序执行的，而invoke 方法之后的内容就是反向的。
     （3 ）afterCompletion(HttpServletRequest request, HttpServletResponse response, Object handle, Exception ex) 方法，该方法也是需要当前对应的Interceptor 的preHandle 方法的返回值为true 时才会执行。顾名思义，该方法将在整个请求结束之后，也就是在DispatcherServlet
 渲染了对应的视图之后执行。这个方法的主要作用是用于进行资源清理工作的。
下面是一个简单的代码说明：
```java
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.springframework.web.servlet.HandlerInterceptor;
import org.springframework.web.servlet.ModelAndView;
public class SpringMVCInterceptor implements HandlerInterceptor {
	/**
	 * preHandle方法是进行处理器拦截用的，顾名思义，该方法将在Controller处理之前进行调用，SpringMVC中的Interceptor拦截器是链式的，可以同时存在
	 * 多个Interceptor，然后SpringMVC会根据声明的前后顺序一个接一个的执行，而且所有的Interceptor中的preHandle方法都会在
	 * Controller方法调用之前调用。SpringMVC的这种Interceptor链式结构也是可以进行中断的，这种中断方式是令preHandle的返
	 * 回值为false，当preHandle的返回值为false的时候整个请求就结束了。
	 */
	@Override
	public boolean preHandle(HttpServletRequest request,
			HttpServletResponse response, Object handler) throws Exception {
		// TODO Auto-generated method stub
		return false;
	}
	
	/**
	 * 这个方法只会在当前这个Interceptor的preHandle方法返回值为true的时候才会执行。postHandle是进行处理器拦截用的，它的执行时间是在处理器进行处理之
	 * 后，也就是在Controller的方法调用之后执行，但是它会在DispatcherServlet进行视图的渲染之前执行，也就是说在这个方法中你可以对ModelAndView进行操
	 * 作。这个方法的链式结构跟正常访问的方向是相反的，也就是说先声明的Interceptor拦截器该方法反而会后调用，这跟Struts2里面的拦截器的执行过程有点像，
	 * 只是Struts2里面的intercept方法中要手动的调用ActionInvocation的invoke方法，Struts2中调用ActionInvocation的invoke方法就是调用下一个Interceptor
	 * 或者是调用action，然后要在Interceptor之前调用的内容都写在调用invoke之前，要在Interceptor之后调用的内容都写在调用invoke方法之后。
	 */
	@Override
	public void postHandle(HttpServletRequest request,
			HttpServletResponse response, Object handler,
			ModelAndView modelAndView) throws Exception {
		// TODO Auto-generated method stub
		
	}
	/**
	 * 该方法也是需要当前对应的Interceptor的preHandle方法的返回值为true时才会执行。该方法将在整个请求完成之后，也就是DispatcherServlet渲染了视图执行，
	 * 这个方法的主要作用是用于清理资源的，当然这个方法也只能在当前这个Interceptor的preHandle方法的返回值为true时才会执行。
	 */
	@Override
	public void afterCompletion(HttpServletRequest request,
			HttpServletResponse response, Object handler, Exception ex)
	throws Exception {
		// TODO Auto-generated method stub
		
	}
	
}
```
**3、定义的拦截器类加到SpringMVC的拦截体系中**
```
<span style="font-size:12px;">	<mvc:interceptors>
		<!-- 使用bean定义一个Interceptor，直接定义在mvc:interceptors根下面的Interceptor将拦截所有的请求 -->
		<bean class="com.host.app.web.interceptor.AllInterceptor"/>
		<mvc:interceptor>
			<mvc:mapping path="/test/number.do"/>
			<!-- 定义在mvc:interceptor下面的表示是对特定的请求才进行拦截的 -->
			<bean class="com.host.app.web.interceptor.LoginInterceptor"/>
		</mvc:interceptor>
	</mvc:interceptors></span>
```
      由上面的示例可以看出可以利用mvc:interceptors标签声明一系列的拦截器，然后它们就可以形成一个拦截器链，拦截器的执行顺序是按声明的先后顺序执行的，先声明的拦截器中的preHandle方法会先执行，然而它的postHandle方法和afterCompletion方法却会后执行。 
        在mvc:interceptors标签下声明interceptor主要有两种方式：
（1）直接定义一个Interceptor实现类的bean对象。使用这种方式声明的Interceptor拦截器将会对所有的请求进行拦截。
（2）使用mvc:interceptor标签进行声明。使用这种方式进行声明的Interceptor可以通过mvc:mapping子标签来定义需要进行拦截的请求路径。
        经过上述两步之后，定义的拦截器就会发生作用对特定的请求进行拦截了。
**4、具体一个应用的示例**
下面通过一个例子来说明如何使用SpringMVC框架的拦截器。
现在要编写一个拦截器，拦截所有不在工作时间的请求，把这些请求转发到一个特定的静态页面，而不对它们的请求进行处理。
首先编写TimeInterceptor.Java，代码如下：
```
package com.yjde.web.interceptor;
import java.util.Calendar;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.springframework.web.servlet.ModelAndView;
import org.springframework.web.servlet.handler.HandlerInterceptorAdapter;
public class TimeInterceptor extends HandlerInterceptorAdapter {
	// 继承HandlerInterceptorAdapter类
	private int openingTime; // openingTime 属性指定上班时间
	private int closingTime; // closingTime属性指定下班时间
	private String outsideOfficeHoursPage;// outsideOfficeHoursPage属性指定错误提示页面的URL
	// 重写 preHandle()方法，在业务处理器处理请求之前对该请求进行拦截处理
	public boolean preHandle(HttpServletRequest request,
			HttpServletResponse response, Object handler) throws Exception {
		Calendar cal = Calendar.getInstance();
		int hour = cal.get(Calendar.HOUR_OF_DAY); // 获取当前时间
		if (openingTime <= hour && hour < closingTime) { // 判断当前是否处于工作时间段内
			return true;
		} else {
			response.sendRedirect(outsideOfficeHoursPage); // 返回提示页面
			return false;
		}
	}
	public void postHandle(HttpServletRequest request,
			HttpServletResponse response, Object o, ModelAndView mav)
			throws Exception {
		System.out.println("postHandle");
	}
	public void afterCompletion(HttpServletRequest request,
			HttpServletResponse response, Object o, Exception excptn)
			throws Exception {
		System.out.println("afterCompletion");
	}
	public int getOpeningTime() {
		return openingTime;
	}
	public void setOpeningTime(int openingTime) {
		this.openingTime = openingTime;
	}
	public int getClosingTime() {
		return closingTime;
	}
	public void setClosingTime(int closingTime) {
		this.closingTime = closingTime;
	}
	public String getOutsideOfficeHoursPage() {
		return outsideOfficeHoursPage;
	}
	public void setOutsideOfficeHoursPage(String outsideOfficeHoursPage) {
		this.outsideOfficeHoursPage = outsideOfficeHoursPage;
	}
}
// 可以看出，上面的代码重载了preHandle()方法，该方法在业务处理器处理请求之前被调用。在该方法中，首先获得当前的时间，判断其是否在
// openingTime和closingTime之间，如果在，返回true，这样才会调用业务控制器去处理该请求；否则直接转向一个页面，返回false，这样该请求就不会被处理。<span style="color:#7f055;"><strong>
</strong></span>
```
可以看出，上面的代码重载了preHandle()方法，该方法在业务处理器处理请求之前被调用。在该方法中，首先获得当前的时间，判断其是否在 openingTime和closingTime之间，如果在，返回true，这样才会调用业务控制器去处理该请求；否则直接转向一个页面，返回 false，这样该请求就不会被处理。
下面是在dispatcher-servlet.xml中对拦截器进行的配置，代码如下：
```java
<pre class="html" name="code"><?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:mvc="http://www.springframework.org/schema/mvc"
	xmlns:p="http://www.springframework.org/schema/p" xmlns:context="http://www.springframework.org/schema/context"
	xmlns:aop="http://www.springframework.org/schema/aop" xmlns:tx="http://www.springframework.org/schema/tx"
	xsi:schemaLocation="http://www.springframework.org/schema/beans
			http://www.springframework.org/schema/beans/spring-beans-3.0.xsd
			http://www.springframework.org/schema/context 
			http://www.springframework.org/schema/context/spring-context-3.0.xsd
			http://www.springframework.org/schema/aop 
			http://www.springframework.org/schema/aop/spring-aop-3.0.xsd
			http://www.springframework.org/schema/tx 
			http://www.springframework.org/schema/tx/spring-tx-3.0.xsd
			http://www.springframework.org/schema/mvc 
			http://www.springframework.org/schema/mvc/spring-mvc-3.0.xsd
			http://www.springframework.org/schema/context 
			http://www.springframework.org/schema/context/spring-context-3.0.xsd">
	<!--
        使Spring支持自动检测组件，如注解的Controller
        -->
	<context:component-scan base-package="com.yjde.web.controller" />
	<bean id="viewResolver"
		class="org.springframework.web.servlet.view.InternalResourceViewResolver"
		p:prefix="/WEB-INF/jsp/" p:suffix=".jsp" />
	<mvc:interceptors>
		<mvc:interceptor>
			<!--设置拦截的路径-->
			<mvc:mapping path="/login1.htm" />
			<mvc:mapping path="/login2.htm" />
			<bean class="com.yjde.web.interceptor.TimeInterceptor">
				<!--openingTime 属性指定上班时间-->
				<property name="openingTime">
					<value>12</value>
				</property>
				<!--closingTime属性指定下班时间-->
				<property name="closingTime">
					<value>14</value>
				</property>
				<!--outsideOfficeHoursPage属性指定提示页面的URL-->
				<property name="outsideOfficeHoursPage">
					<value>http://localhost:8080/SpringMVCInterceptor/jsp/outsideOfficeHours.jsp
					</value>
				</property>
			</bean>
		</mvc:interceptor>
	</mvc:interceptors>
	<bean id="messageSource"
		class="org.springframework.context.support.ResourceBundleMessageSource"
		p:basename="message">
	</bean>
</beans>
```

