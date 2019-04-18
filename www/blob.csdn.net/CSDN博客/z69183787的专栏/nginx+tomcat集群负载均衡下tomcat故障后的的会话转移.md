# nginx+tomcat集群负载均衡下tomcat故障后的的会话转移 - z69183787的专栏 - CSDN博客
2015年08月28日 11:24:06[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1396
最近自己尝试着在linux下面进行开发和学习，想想即使对于一般的企业应用，做一个小的集群也是有必要的，能防止单服务器down掉之后的尴尬，所以就在学习nginx+tomcat配置一个小型的集群，google之下也算搞定，然后自然涉及到session共享之类的问题，依旧想google之，前人经验后人乘凉，搜索后发现最多的文章的解决方式就是开启tomcat的session复制，还有比较多的情况是使用memcached
 session manager 这个开源框架，这样就能在某个tomcat故障时候防止用户会话失效，这两者都要对tomcat配置文件什么的进行一些不大不小的改动，而且在我尝试之后发现，开启tomcat session复制尤其不可取，尤其在session中内容比较多的情况下（当然，session中放太多东西本身就不应该..），效率很是问题，也有人说在nginx下用iphash，将同一ip会话粘在同一个tomcat服务器上，但是如果在这台tomcat服务器down掉的情况下，却没有给出一个比较好的会话转移解决方法。（或者我没搜到?） 
额。前面罗嗦一通是前提，我的一个程序中的解决方法，不必像开启tomcat的session复制或者像memcached session mamager这样对tomcat的配置文件进行修改，只需要代码实现即可。
 想法如下： 
用memcached 存储用户信息，将会话的sessionId作为key，放了一个map用来存储信息, 
这样在会话中如果所粘合的tomcat down掉，nginx将此次会话转发给其他服务器时候，将存储在memcached中的信息复制回来即可。像下面这样： 
request--sessionFilter（会话转移filter）--powerFilter(权限验证filter)--action-- 
首先我在登录的时候做了如下操作：初始化信息 
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- SessionCacheService.initSessionCache(sessionId,map);  
因为采取iphash策略，用户会话是粘合在同一服务器上的，那么当这台服务器down掉后的处理: 
用request.getRequestedSessionId();获取上一会话的sessionId去memcached中找,找到后将信息复制到此会话中. 
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
然后我的powerFilter 权限验证filter如下: 
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
看的出来我是用cache里面的信息来判断会话失效的，放弃使用HttpSession了，只用了它的ID，当然，这只是我的个人做法，要使用session存储信息的话，就是类似于将将memcached作为一个session信息的备份了，添加两个listener保证session和memcached同步即可，我的做法： 
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
这个是监听session属性变化的listener,保证当session内容变化时候能同步到cache中 
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
， 
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
不过据我所知，如果客户端禁用cookie的话，request.getRequestedSessionId()可能就有问题了，，不过。。不管啦！ 
这是我的一个解决方法，贴出来与大家分享。觉得这么做最大的好处就是和服务器无关,像session复制和memcached session manager这样都是依赖tomcat服务器。 
毕业一年，还只是新手,这也是我在iteye的第一贴。。如果这么做有什么错误的话，请指正，我会虚心学习，请言论不要过激。。谢谢。 
还有，看了精华贴 【晒晒大家年龄和近期愿望吧】  心中真是感慨万千，感觉天朝上学的时间真是太长了，刚毕业没多久，就觉得时间已经不够用。同时祝帖子里面的朋友愿望都能实现(话说最多的愿望是找个女朋友?)!
