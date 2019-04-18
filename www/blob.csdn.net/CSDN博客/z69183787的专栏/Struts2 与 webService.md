# Struts2 与 webService - z69183787的专栏 - CSDN博客
2013年03月21日 16:01:01[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1287
主要目标是让struts2不对webservice进行拦截，因此写一个类来继续struts2的过滤器org.apache.struts2.dispatcher.FilterDispatcher
以下是自己写的过滤器类
[java][view
 plain](http://blog.csdn.net/sword51/article/details/5709252#)[copy](http://blog.csdn.net/sword51/article/details/5709252#)
- /**
-  * FilterDispetor.java
-  * com.sword.actions
-  *
-  * Function： TODO 
-  *
-  *   ver     date           author
-  * ──────────────────────────────────
-  *           2010-7-2       lidl
-  *
-  * Copyright (c) 2010, TNT All Rights Reserved.
- */
- package com.sword.actions;  
- import java.io.IOException;  
- import javax.servlet.FilterChain;  
- import javax.servlet.ServletException;  
- import javax.servlet.ServletRequest;  
- import javax.servlet.ServletResponse;  
- import javax.servlet.http.HttpServletRequest;  
- import org.apache.struts2.dispatcher.FilterDispatcher;  
- /**
-  * ClassName:FilterDispetor
-  * Function: TODO ADD FUNCTION
-  * Reason:   TODO ADD REASON
-  *
-  * @author   lidl
-  * @version  
-  * @since    Ver 1.1
-  * @Date     2010-7-2       下午12:58:37
-  *
-  * @see      
-  */
- @SuppressWarnings("deprecation")  
- publicclass FilterDispetor extends FilterDispatcher {  
- @Override
- publicvoid doFilter(ServletRequest req, ServletResponse res,  
-             FilterChain chain) throws IOException, ServletException {  
- 
- // TODO Auto-generated method stub
-         HttpServletRequest request=(HttpServletRequest) req;  
-         String uri=request.getRequestURI();  
-         System.out.println("******************************:"+uri);  
- if(uri.indexOf("RemoteMachine")!=-1||uri.indexOf("/Servlet/")!=-1||uri.indexOf("Service")!=-1){  
-             System.out.println("FilterDispetor.doFilter():true");  
-             chain.doFilter(req, res);  
-         }else{  
- super.doFilter(req, res, chain);  
-         }  
-     }  
- }  
"RemoteMachine",为自己写的webservice名，"Servlet"为axis 要使用的
然后 修改 web.xml struts2 相关过分
[java][view
 plain](http://blog.csdn.net/sword51/article/details/5709252#)[copy](http://blog.csdn.net/sword51/article/details/5709252#)
- <filter>  
-   <filter-name>struts2</filter-name>  
-   <filter-class>    
-            com.sword.actions.FilterDispetor    
-   </filter-class>  
- </filter>  
- <filter-mapping>  
-   <filter-name>struts2</filter-name>  
-   <url-pattern>/*</url-pattern>  
- </filter-mapping>  
