# Spring Security教程（9）---- 自定义AccessDeniedHandler - z69183787的专栏 - CSDN博客
2014年03月13日 21:25:06[OkidoGreen](https://me.csdn.net/z69183787)阅读数：11877
在Spring默认的AccessDeniedHandler中只有对页面请求的处理，而没有对Ajax的处理。而在项目开发是Ajax又是我们要常用的技术，所以我们可以通过自定义AccessDeniedHandler来处理Ajax请求。我们在Spring默认的AccessDeniedHandlerImpl上稍作修改就可以了。
**[java]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18403113#)[copy](http://blog.csdn.net/jaune161/article/details/18403113#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/160398/fork)
- publicclass DefaultAccessDeniedHandler implements AccessDeniedHandler {  
- 
- /* (non-Javadoc)
-      * @see org.springframework.security.web.access.AccessDeniedHandler#handle(javax.servlet.http.HttpServletRequest, javax.servlet.http.HttpServletResponse, org.springframework.security.access.AccessDeniedException)
-      */
- private String errorPage;  
- 
- //~ Methods ========================================================================================================
- 
- publicvoid handle(HttpServletRequest request, HttpServletResponse response, AccessDeniedException accessDeniedException)  
- throws IOException, ServletException {  
- boolean isAjax = ControllerTools.isAjaxRequest(request);  
- if(isAjax){  
-             Message msg = MessageManager.exception(accessDeniedException);  
-             ControllerTools.print(response, msg);  
-         }elseif (!response.isCommitted()) {  
- if (errorPage != null) {  
- // Put exception into request scope (perhaps of use to a view)
-                 request.setAttribute(WebAttributes.ACCESS_DENIED_403, accessDeniedException);  
- 
- // Set the 403 status code.
-                 response.setStatus(HttpServletResponse.SC_FORBIDDEN);  
- 
- // forward to error page.
-                 RequestDispatcher dispatcher = request.getRequestDispatcher(errorPage);  
-                 dispatcher.forward(request, response);  
-             } else {  
-                 response.sendError(HttpServletResponse.SC_FORBIDDEN, accessDeniedException.getMessage());  
-             }  
-         }  
-     }  
- 
- /**
-      * The error page to use. Must begin with a "/" and is interpreted relative to the current context root.
-      *
-      * @param errorPage the dispatcher path to display
-      *
-      * @throws IllegalArgumentException if the argument doesn't comply with the above limitations
-      */
- publicvoid setErrorPage(String errorPage) {  
- if ((errorPage != null) && !errorPage.startsWith("/")) {  
- thrownew IllegalArgumentException("errorPage must begin with '/'");  
-         }  
- 
- this.errorPage = errorPage;  
-     }  
- 
- }  
这里我们直接将异常信息通过PrintWriter输出到前台，然后在前台做统一的处理就可以了。在前台对后台消息统一处理的方法可以参考我的这篇文章[http://blog.csdn.net/jaune161/article/details/18135607](http://blog.csdn.net/jaune161/article/details/18135607)
最后在配置文件中配置下
**[html]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18403113#)[copy](http://blog.csdn.net/jaune161/article/details/18403113#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/160398/fork)
- <sec:httpauto-config="true"access-decision-manager-ref="accessDecisionManager">
- 
- <sec:access-denied-handlerref="accessDeniedHandler"/>
- 
- <sec:session-managementinvalid-session-url="/login.jsp"/>
- 
- <sec:intercept-urlpattern="/app.jsp"access="AUTH_LOGIN"/>
- <sec:intercept-urlpattern="/**"access="AUTH_GG_FBGBGG"/>
- 
- <sec:form-loginlogin-page="/login.jsp"authentication-failure-url="/login.jsp"
- default-target-url="/index.jsp"/>
- 
- </sec:http>
- 
- <!-- 自定义权限不足处理程序 -->
- <beanid="accessDeniedHandler"class="com.zrhis.system.security.RequestAccessDeniedHandler">
- <propertyname="errorPage"value="/WEB-INF/error/403.jsp"></property>
- </bean>
session-management本来计划在之前就讲的，但是准备深入讲下session-management所以就一直没有讲。今天既然提到了就简单的说下session-management最简单的配置，就是上面的配置invalid-session-url表示Session失效时跳转的连接。随后会深入讲下这个。
