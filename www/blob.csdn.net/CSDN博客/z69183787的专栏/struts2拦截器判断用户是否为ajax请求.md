# struts2拦截器判断用户是否为ajax请求 - z69183787的专栏 - CSDN博客
2014年03月13日 20:10:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1411
      现在的网站，应该绝大多数用jquery作为基础类库了。假设有这样的需求，在用struts2做权限管理的时候，我们可以拦截用户请求的资源，然后判断该用户是否有相应资源的权限。如果没有就返回一个字符串，让其跳转相应的页面，问题来了。如果是ajax请求呢，用户是不会跳转的，虽然用户的操作被拦截，但是用户却不知道。所以我们可以在拦截成功的时候，响应一个字符，但是必须先判断出，用户的当前请求是否ajax请求。
这段代码只是针对jquery的ajax请求。 如果经常用firebug进行调试的话，会发现jquery的ajax都会向后退发送一个X-Requested-With报头，其值为XMLHttpRequest。所以我们可以利用这个特点来判断。
**[java]**[view
 plain](http://blog.csdn.net/k10509806/article/details/6412918#)[copy](http://blog.csdn.net/k10509806/article/details/6412918#)
- privateboolean isAjaxRequest(HttpServletRequest request) {  
-     String header = request.getHeader("X-Requested-With");  
- if (header != null && "XMLHttpRequest".equals(header))  
- returntrue;  
- else
- returnfalse;  
- }  
