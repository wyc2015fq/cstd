# session超时，处理ajax请求 - z69183787的专栏 - CSDN博客
2015年06月12日 16:07:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1477
    项目使用的是spring-security安全框架，当session超时时，如果不是ajax请求，很简单就能实现跳到指定的页面。但是ajax请求就会有问题。session超时的时候，点击到ajax请求就会弹出一些页面源码文件。
    由于一直都在忙其他的这个问题一直放着。最近有时间了，就把这个问题给解决了。
    首先建了个拦截器，来判断session超时。用户登录后会保存用户信息在一个session里，在session的监听里，session超时会销毁保存在session里的用户信息，而拦截器就通过session里是否有用户信息来判断session超时。（我总觉得这种方法不怎么好。不知还有什么更好的办法。）
   拦截器是spring-mvc的拦截器，在拦截器里判断是不是ajax请求：
**[java]**[view
 plain](http://blog.csdn.net/zuxianghaung/article/details/7270489#)[copy](http://blog.csdn.net/zuxianghaung/article/details/7270489#)
- publicboolean preHandle(HttpServletRequest request,  
-              HttpServletResponse response, Object handler) throws Exception  
-      {  
- if (request.getSession().getAttribute("user") == null)//判断session里是否有用户信息
-             {  
- if (request.getHeader("x-requested-with") != null
-                      && request.getHeader("x-requested-with")  
-                              .equalsIgnoreCase("XMLHttpRequest"))//如果是ajax请求响应头会有，x-requested-with；
-              {  
-                  response.setHeader("sessionstatus", "timeout");//在响应头设置session状态
- returnfalse;  
-              }  
- 
-          }  
- returntrue;  
-      }  
这样，如果session超时，而且是ajax请求，就会在响应头里，sessionstatus有一个timeout；
再用一个全局的方法来处理，session超时要跳转的页面。
jquery 可以用$.ajaxSetup 方法，ext也有类似的方法
**[javascript]**[view
 plain](http://blog.csdn.net/zuxianghaung/article/details/7270489#)[copy](http://blog.csdn.net/zuxianghaung/article/details/7270489#)
- //全局的ajax访问，处理ajax清求时sesion超时
-          $.ajaxSetup({   
-              contentType:"application/x-www-form-urlencoded;charset=utf-8",   
-              complete:function(XMLHttpRequest,textStatus){   
- var sessionstatus=XMLHttpRequest.getResponseHeader("sessionstatus"); //通过XMLHttpRequest取得响应头，sessionstatus，
- if(sessionstatus=="timeout"){   
- //如果超时就处理 ，指定要跳转的页面
-                                          window.location.replace("${path}/common/login.do");   
-                                  }   
-                       }   
-              }   
-            });  
- 
