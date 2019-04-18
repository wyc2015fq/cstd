# HttpClient设置HTTP请求头Header - z69183787的专栏 - CSDN博客
2015年01月21日 11:00:36[OkidoGreen](https://me.csdn.net/z69183787)阅读数：98718
用Firebug对POST的数据进行监控 **请求 HTTP头** 信息,得到如下内容:
Java代码  ![收藏代码](http://greatwqs.iteye.com/images/icon_star.png)
- Accept  application/json, text/javascript, */*  
- Accept-Encoding gzip, deflate  
- Accept-Language en-us,en;q=0.5
- Cache-Control   no-cache  
- Content-Length  432
- Content-Type    application/x-www-form-urlencoded; charset=UTF-8
- Host    www.huaxixiang.com  
- Pragma  no-cache  
- Proxy-Connection    keep-alive  
- Refere   http://www.huaxixiang.com/CrossPriceDetail.action
- User-Agent  Mozilla/5.0 (Windows NT 5.1; rv:11.0) Gecko/20100101 Firefox/11.0
- X-Requested-With    XMLHttpRequest  
用HttpClient模仿浏览器访问页面,加载URL的HTML信息,为了良好的加载网站的信息,不被限制.
为了说明请求头的信息添加了一个小测试项目LoginTest,添加页面index.jsp,添加主要代码打印Http Header的JSP页面.
主要打印Http Header信息.
**1. index.jsp**
Java代码  ![收藏代码](http://greatwqs.iteye.com/images/icon_star.png)
- out.println("Protocol: " + request.getProtocol());   
- out.println("Scheme: " + request.getScheme());   
- out.println("Server Name: " + request.getServerName() );   
- out.println("Server Port: " + request.getServerPort());   
- out.println("Protocol: " + request.getProtocol());   
- out.println("Server Info: " + getServletConfig().getServletContext().getServerInfo());   
- out.println("Remote Addr: " + request.getRemoteAddr());  
- out.println("Remote Host: " + request.getRemoteHost());   
- out.println("Character Encoding: " + request.getCharacterEncoding());   
- out.println("Content Length: " + request.getContentLength());   
- out.println("Content Type: "+ request.getContentType());   
- out.println("Auth Type: " + request.getAuthType());   
- out.println("HTTP Method: " + request.getMethod());   
- out.println("Path Info: " + request.getPathInfo());   
- out.println("Path Trans: " + request.getPathTranslated());   
- out.println("Query String: " + request.getQueryString());   
- out.println("Remote User: " + request.getRemoteUser());   
- out.println("Session Id: " + request.getRequestedSessionId());   
- out.println("Request URI: " + request.getRequestURI());   
- out.println("Servlet Path: " + request.getServletPath());   
- out.println("Accept: " + request.getHeader("Accept"));   
- out.println("Host: " + request.getHeader("Host"));   
- out.println("Referer : " + request.getHeader("Referer"));   
- out.println("Accept-Language : " + request.getHeader("Accept-Language"));   
- out.println("Accept-Encoding : " + request.getHeader("Accept-Encoding"));   
- out.println("User-Agent : " + request.getHeader("User-Agent"));   
- out.println("Connection : " + request.getHeader("Connection"));   
- out.println("Cookie : " + request.getHeader("Cookie"));   
- out.println("Created : " + session.getCreationTime());   
- out.println("LastAccessed : " + session.getLastAccessedTime());   
**2. 使用IE浏览器加载**[**http://127.0.0.1:8080/LoginTest/index.jsp**](http://127.0.0.1:8080/LoginTest/index.jsp)**返回内容如下:**
Java代码  ![收藏代码](http://greatwqs.iteye.com/images/icon_star.png)
- Protocol: HTTP/1.1
- Scheme: http   
- Server Name: 127.0.0.1
- Server Port: 8080
- Protocol: HTTP/1.1
- Server Info: Apache Tomcat/6.0.18
- Remote Addr: 127.0.0.1
- Remote Host: 127.0.0.1
- Character Encoding: null
- Content Length: -1
- Content Type: null
- Auth Type: null
- HTTP Method: GET   
- Path Info: null
- Path Trans: null
- Query String: null
- Remote User: null
- Session Id: E2C384C095E34AD355684EB554517FB1   
- Request URI: /LoginTest/index.jsp   
- Servlet Path: /index.jsp   
- Accept: */*   
- Host: 127.0.0.1:8080
- Referer : null
- Accept-Language : en-us   
- Accept-Encoding : gzip, deflate   
- User-Agent : Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; .NET CLR 1.1.4322; .NET CLR 2.0.50727; InfoPath.3; .NET CLR 3.0.4506.2152; .NET CLR 3.5.30729; .NET4.0C; .NET4.0E)   
- Connection : Keep-Alive   
- Cookie : JSESSIONID=E2C384C095E34AD355684EB554517FB1   
- Created : 1322294859981
- LastAccessed : 1322294859981
**3.  后面使用HttpClient不设置header信息加载**[**http://127.0.0.1:8080/LoginTest/index.jsp**](http://127.0.0.1:8080/LoginTest/index.jsp)**信息如下:**
Java代码  ![收藏代码](http://greatwqs.iteye.com/images/icon_star.png)
- Protocol: HTTP/1.1
- Scheme: httpServer   
- Name: 127.0.0.1
- Server Port: 8080
- Protocol: HTTP/1.1
- Server Info: Apache Tomcat/6.0.18
- Remote Addr: 127.0.0.1
- Remote Host: 127.0.0.1
- Character Encoding: null
- Content Length: -1
- Content Type: null
- Auth Type: null
- HTTP Method: GET  
- Path Info: null
- Path Trans: null
- Query String: null
- Remote User: null
- Session Id: null
- Request URI: /LoginTest/index.jspServlet   
- Path: /index.jsp  
- Accept: null
- Host: 127.0.0.1:8080
- Referer : null
- Accept-Language : null
- Accept-Encoding : null
- User-Agent : Apache-HttpClient/4.1.1 (java 1.5)  
- Connection : Keep-Alive  
- Cookie : null
- Created : 1322293843369
- LastAccessed : 1322293843369
**分析:** 由于这里纯粹加载页面,没有动用CookieStore自动管理Cookie,在上面没有能显示Cookie,SessionID的信息,区别于浏览器的的User-Agent,Cookie,SessionID,Accept,Accept-Language,Accept-Encoding等信息都没有进行设置.
对于爬取网站在HttpClient中设置Host,Referer,User-Agent,Connection,Cookie和爬取的频率和入口Url有讲究.
**4. 考虑设置HttpClient的Header信息代码:**
Java代码  ![收藏代码](http://greatwqs.iteye.com/images/icon_star.png)
- HashMap<String, String> headers = new HashMap<String, String>();  
- headers.put("Referer", p.url);  
- headers.put("User-Agent", "Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.2.6) Gecko/20100625
- 
- Firefox/3.6.6 Greatwqs");  
- headers.put("Accept","text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");  
- headers.put("Accept-Language","zh-cn,zh;q=0.5");  
- headers.put("Host","www.yourdomain.com");  
- headers.put("Accept-Charset","ISO-8859-1,utf-8;q=0.7,*;q=0.7");  
- headers.put("Referer", "http://www.yourdomian.com/xxxAction.html");  
- HttpRequestBase httpget = ......  
- httpget.setHeaders(headers);  
**5. 由新的HttpClient执行[http://127.0.0.1:8080/LoginTest/index.jsp](http://127.0.0.1:8080/LoginTest/index.jsp)得到的HTML信息如下:**
Java代码  ![收藏代码](http://greatwqs.iteye.com/images/icon_star.png)
- Protocol: HTTP/1.1
- Scheme: http  
- Server Name: www.yourdomain.com  
- Server Port: 80
- Protocol: HTTP/1.1
- Server Info: Apache Tomcat/6.0.18
- Remote Addr: 127.0.0.1
- Remote Host: 127.0.0.1
- Character Encoding: null
- Content Length: -1
- Content Type: null
- Auth Type: null
- HTTP Method: GET  
- Path Info: null
- Path Trans: null
- Query String: null
- Remote User: null
- Session Id: null
- Request URI: /LoginTest/index.jsp  
- Servlet Path: /index.jsp  
- Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
- Host: www.yourdomain.com  
- Referer : http://www.yourdomian.com/xxxAction.html
- Accept-Language : zh-cn,zh;q=0.5
- Accept-Encoding : null
- User-Agent : Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.2.6) Gecko/20100625 Firefox/3.6.6
- 
- Greatwqs  
- Connection : Keep-Alive  
- Cookie : null
- Created : 1322294148709
- LastAccessed : 1322294148709
