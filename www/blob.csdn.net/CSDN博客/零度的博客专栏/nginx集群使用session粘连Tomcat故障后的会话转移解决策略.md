# nginx集群使用session粘连Tomcat故障后的会话转移解决策略 - 零度的博客专栏 - CSDN博客
2016年10月28日 14:21:15[零度anngle](https://me.csdn.net/zmx729618)阅读数：3202
       即使对于一般的企业应用，做一个小的集群也是有必要的，能防止单服务器down掉之后的尴尬，所以就在学习nginx+tomcat配置一个小型的集群，google之下也算搞定，然后自然涉及到session共享之类的问题，依旧想google之，前人经验后人乘凉，搜索后发现最多的文章的解决方式就是开启tomcat的session复制，还有比较多的情况是使用memcached
 session manager 这个开源框架，这样就能在某个tomcat故障时候防止用户会话失效，这两者都要对tomcat配置文件什么的进行一些不大不小的改动，而且在我尝试之后发现，开启tomcat session复制尤其不可取，尤其在session中内容比较多的情况下（当然，session中放太多东西本身就不应该..），效率很是问题，也有人说在nginx下用iphash，将同一ip会话粘在同一个tomcat服务器上，但是如果在这台tomcat服务器down掉的情况下，却没有给出一个比较好的会话转移解决方法。（或者我没搜到?）
对于Seeion粘粘，有Tomcat服务器出现故障的问题解决方法，不必像开启tomcat的session复制或者像memcached session mamager这样对tomcat的配置文件进行修改，只需要代码实现即可。 想法如下： 用memcached 存储用户信息，将会话的sessionId作为key，放了一个map用来存储信息，这样在会话中如果所粘合的tomcat
 down掉，nginx将此次会话转发给其他服务器时候，将存储在memcached中的信息复制回来即可。像下面这样： 
request-->sessionFilter（会话转移filter）--> powerFilter(权限验证filter)-->action-->view 
        首先我在登录的时候做了如下操作：初始化信息 
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- SessionCacheService.initSessionCache(sessionId,map);  
![](http://static.blog.csdn.net/images/save_snippets_01.png)
因为采取iphash策略，用户会话是粘合在同一服务器上的，那么当这台服务器down掉后的处理：用request.getRequestedSessionId();可以获取上一会话的sessionId去memcached中找，找到后将信息复制到此会话中。 
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- /**
-  * Created by IntelliJ IDEA.
-  * User: bjslogin[at]gmail.com
-  * Date: 11-8-2
-  * Time: 上午10:28
-  * 设置nginx为iphash,如果发生故障转移,即会将session信息转移至新的服务器
-  */
- publicclass SessionFilter implements Filter {  
- private Logger logger = Logger.getLogger(getClass());  
- 
- publicvoid init(FilterConfig filterConfig) throws ServletException {  
- 
-     }  
- 
- publicvoid doFilter(ServletRequest servletRequest, ServletResponse servletResponse, FilterChain filterChain) throws IOException, ServletException {  
-         HttpServletRequest request = (HttpServletRequest)servletRequest;  
-         String url = request.getRequestURI();  
- for(String c : PowerFilter.noFilterPages){  
- if(url.indexOf(c) > 0){  
-                 filterChain.doFilter(servletRequest,servletResponse);  
- return;  
-             }  
-         }  
- 
-         String requestedSessionId = request.getRequestedSessionId();  
-         HttpSession session = request.getSession();  
- if(requestedSessionId.equals(session.getId())){  
-             filterChain.doFilter(servletRequest,servletResponse);  
- return;  
-         }  
-         Map<String,Object> _map = SessionCacheService.getSessionCache(session.getId());  
- if(_map == null){  
-             Map<String,Object> map = SessionCacheService.getSessionCache(requestedSessionId);  
- if(map == null){  
-                 filterChain.doFilter(servletRequest,servletResponse);  
- return;  
-             }  
-             logger.info("开始转移...");  
-             SessionCacheService.initSessionCache(session.getId(),map);  
-         }  
- 
-         filterChain.doFilter(servletRequest,servletResponse);  
-     }  
- 
- publicvoid destroy() {  
- 
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
        然后我的powerFilter 权限验证filter如下： 
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- /**
-  * Created by IntelliJ IDEA.
-  * User: bjslogin[at]gmail.com
-  * Date: 11-8-1
-  * Time: 下午2:23
-  * 权限验证filter
-  */
- publicclass PowerFilter implements Filter {  
- private Logger logger = Logger.getLogger(getClass());  
- private FilterConfig filterConfig;  
- private ApplicationContext ctx = null;  
- protectedstaticfinal String sessionErrorPage = "/sessionError.jsp";  
- protectedstaticfinal String noPermissionPage = "/noPermission.jsp";  
- protectedstaticfinal String[] noFilterPages = {"login.action","logout.action"};  
- 
- publicvoid init(FilterConfig filterConfig) throws ServletException {  
- this.filterConfig = filterConfig;  
-     }  
- 
- publicvoid doFilter(ServletRequest servletRequest, ServletResponse servletResponse, FilterChain filterChain) throws IOException, ServletException {  
-         HttpServletRequest request = (HttpServletRequest)servletRequest;  
-         String url = request.getRequestURI();  
-         logger.info("【"+request.getRemoteHost()+"[at]"+new Date()+"】"+url);  
- 
- for(String c : noFilterPages){  
- if(url.indexOf(c) > 0){  
-                 filterChain.doFilter(servletRequest,servletResponse);  
- return;  
-             }  
-         }  
- 
-         HttpSession session = request.getSession();  
-         Map<String,Object> map = SessionCacheService.getAndRefreshSessionCache(session.getId());  
- if(map == null){  
-             HttpServletResponse response = (HttpServletResponse)servletResponse;  
-             response.sendRedirect(request.getContextPath() + sessionErrorPage);  
- return;  
-         }  
- 
-         LoginService loginService = (LoginService)getBean(LoginService.class);  
- boolean flag = loginService.checkPrivate(request.getSession().getId(),StringUtils.substringAfter(url,request.getContextPath() + "/"));  
- if(!flag){  
-             HttpServletResponse response = (HttpServletResponse)servletResponse;  
-             response.sendRedirect(request.getContextPath() + noPermissionPage);  
- return;  
-         }  
- 
-         filterChain.doFilter(servletRequest,servletResponse);  
-     }  
- 
- publicvoid destroy() {  
- 
-     }  
- 
- public Object getBean(Class clazz) {  
- if (ctx == null) {  
-             ctx = WebApplicationContextUtils.getRequiredWebApplicationContext(filterConfig.getServletContext());  
-         }  
- return ctx.getBean(clazz);  
-     }  
- 
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
        看的出来我是用cache里面的信息来判断会话失效的，放弃使用HttpSession了，只用了它的ID，当然，这只是我的个人做法，要使用session存储信息的话，就是类似于将memcached作为一个session信息的备份了，添加两个listener保证session和memcached同步即可，代码如下： 
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- /**
-  * Created by IntelliJ IDEA.
-  * User: bjslogin[at]gmail.com
-  * Date: 11-8-1
-  * Time: 上午11:07
-  * session失效时同时干掉缓存
-  */
- publicclass SessionCacheListener implements HttpSessionListener{  
- 
- publicvoid sessionCreated(HttpSessionEvent httpSessionEvent) {  
- // do nothing
- // 登陆时才初始化sessionCache内容
-     }  
- 
- //session失效时同时移除缓存内容
- publicvoid sessionDestroyed(HttpSessionEvent httpSessionEvent) {  
-         HttpSession session = httpSessionEvent.getSession();  
-         SessionCacheService.removeSessionCache(session.getId());  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
         其次，这个是监听session属性变化的listener，保证当session内容变化时候能同步到cache中： 
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- /**
-  * Created by IntelliJ IDEA.
-  * User: bjslogin[at]gmail.com
-  * Date: 11-8-1
-  * Time: 上午11:28
-  * 监听session属性变化,同步cache
-  */
- publicclass SessionCacheAttributeListener implements HttpSessionAttributeListener {  
- 
- publicvoid attributeAdded(HttpSessionBindingEvent httpSessionBindingEvent) {  
-         HttpSession session = httpSessionBindingEvent.getSession();  
-         String key = httpSessionBindingEvent.getName();  
-         Object value = httpSessionBindingEvent.getValue();  
-         SessionCacheService.putSessionCacheAttribute(session.getId(), key, value);  
-     }  
- 
- publicvoid attributeRemoved(HttpSessionBindingEvent httpSessionBindingEvent) {  
-         HttpSession session = httpSessionBindingEvent.getSession();  
-         String key = httpSessionBindingEvent.getName();  
-         SessionCacheService.removeSessionCacheAttribute(session.getId(), key);  
-     }  
- 
- publicvoid attributeReplaced(HttpSessionBindingEvent httpSessionBindingEvent) {  
-         attributeAdded(httpSessionBindingEvent);  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
        这样就能保证session与cache内容同步了，只需要在SessionFilter的SessionCacheService.initSessionCache(session.getId(),map);方法之前将cache中的内copy到session中即可，下一步的powerFIlter判断用户信息就不会出现问题。 
         在我本机nginx+3个tomcat测试中，将正在被访问的tomcat人为down掉，确实是能保证会话平滑转移的，我的nginx的集群配置方式： 
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- upstream localhost {    
-       ip_hash;  
-       server localhost:8060;    
-       server localhost:8070;    
-       server localhost:8080;    
-      }  
- 
- server {    
-             listen       8090;    
-             server_name  localhost;       
-             location / {    
-               proxy_connect_timeout   3;    
-               proxy_send_timeout      30;    
-               proxy_read_timeout      30;    
-               proxy_pass http://localhost;  
-             }   
- }     
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
      不过据我所知，如果客户端禁用cookie的话，request.getRequestedSessionId()可能就有问题了！ 然后这是其中一个解决方法，这么做最大的好处就是和服务器无关，像session复制和memcached session manager这样都是依赖tomcat服务器。 
