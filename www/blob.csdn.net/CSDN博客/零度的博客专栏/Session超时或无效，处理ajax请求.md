# Session超时或无效，处理ajax请求 - 零度的博客专栏 - CSDN博客
2016年05月30日 17:39:18[零度anngle](https://me.csdn.net/zmx729618)阅读数：2110
       项目使用的是spring-security安全框架，当session超时时，如果不是ajax请求，很简单就能实现跳到指定的页面。但是ajax请求就会有问题。session超时的时候，点击到ajax请求就会弹出一些页面源码文件。
       由于一直都在忙其他的这个问题一直放着。最近有时间了，就把这个问题给解决了。
       首先建了个拦截器，来判断session超时。用户登录后会保存用户信息在一个session里，在session的监听里，session超时会销毁保存在session里的用户信息，而拦截器就通过session里是否有用户信息来判断session超时。此时需要注意登陆的时候将用户信息写入session，超时需要在监听器中将session销毁（我总觉得这种方法不怎么好。不知还有什么更好的办法。）
       拦截器是spring-mvc的拦截器，在拦截器里判断是不是ajax请求：
```
package org.nercita.bcp.system.util;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.springframework.web.servlet.HandlerInterceptor;
import org.springframework.web.servlet.ModelAndView;
public class CustomSessionHandlerInterceptor implements HandlerInterceptor {
	@Override
	public boolean preHandle(HttpServletRequest request,
            HttpServletResponse response, Object handler) throws Exception
            
    {
	//	System.out.println(LogInfoService.getLoginUserName());
	//	System.out.println(request.getSession().getAttribute("SPRING_SECURITY_CONTEXT"));
		if (request.getHeader("x-requested-with") != null
                    && request.getHeader("x-requested-with")
                    .equalsIgnoreCase("XMLHttpRequest")){//如果是ajax请求响应头会有，x-requested-with；
            
			if (request.getSession().getAttribute("user") == null){//判断session里是否有用户信息
				
                            response.setHeader("sessionstatus", "timeout");//在响应头设置session状态
                            return false;
                         }
            
                }
                return true;
    }
	@Override
	public void postHandle(HttpServletRequest request,
			HttpServletResponse response, Object handler,
			ModelAndView modelAndView) throws Exception {     
		
	}
	@Override
	public void afterCompletion(HttpServletRequest request,
			HttpServletResponse response, Object handler, Exception ex)
			throws Exception {
		
	}
}
```
       这样，如果session超时，而且是ajax请求，就会在响应头里，sessionstatus有一个timeout；再用一个全局的方法来处理，session超时要跳转的页面。
jquery 可以用$.ajaxSetup 方法，ext也有类似的方法
```
//全局的ajax访问，处理ajax清求时sesion超时
 $.ajaxSetup({ 
     contentType:"application/x-www-form-urlencoded;charset=utf-8", 
     complete:function(XMLHttpRequest,textStatus){ 
             var sessionstatus=XMLHttpRequest.getResponseHeader("sessionstatus"); //通过XMLHttpRequest取得响应头，sessionstatus，
             if(sessionstatus=="timeout"){ 
                         //如果超时就处理 ，指定要跳转的页面
                         window.location.replace("${ctx}/login.jsp"); 
             } 
               
     } 
  });
```
[](http://blog.csdn.net/z69183787/article/details/46472807#)[](http://blog.csdn.net/z69183787/article/details/46472807#)[](http://blog.csdn.net/z69183787/article/details/46472807#)[](http://blog.csdn.net/z69183787/article/details/46472807#)[](http://blog.csdn.net/z69183787/article/details/46472807#)[](http://blog.csdn.net/z69183787/article/details/46472807#)
