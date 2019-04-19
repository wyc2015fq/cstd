# Web开发问题记录 - =朝晖= - 博客园
# [Web开发问题记录](https://www.cnblogs.com/dhcn/p/7100180.html)
     1、先说一个CSS的:CSS中带有中文（比如字体定义）的属性定义最好放在该选择器定义诸项的最后一条，为什么----编码格式问题。
     2、其实自己也可以用自己写的DispatcherServlet+jsp:include动作来做完全透明的公共模版+子页面的组合，当然这个还需要一些公共的约定问题。
     3、关于绝对URL的问题，在服务器端的绝对URL是以发布目录为根目录(最简单的样例:Servlet映射的URL样例)，也就是前面已经包括了WebApp的AppName,而客户端使用的域绝对URL是以/AppName/起头的。
     4、CSS也有编码格式命令：
```
@CHARSET "UTF-8";
```
5、一段得到客户IP的代码(zt from [http://lijie250.javaeye.com/blog/251616](http://lijie250.javaeye.com/blog/251616))：
```
public String getIpAddr(HttpServletRequest request) {       
      String ip = request.getHeader("x-forwarded-for");       
      if(ip == null || ip.length() == 0 || "unknown".equalsIgnoreCase(ip)) {       
         ip = request.getHeader("Proxy-Client-IP");       
     }       
      if(ip == null || ip.length() == 0 || "unknown".equalsIgnoreCase(ip)) {       
         ip = request.getHeader("WL-Proxy-Client-IP");       
      }       
     if(ip == null || ip.length() == 0 || "unknown".equalsIgnoreCase(ip)) {       
          ip = request.getRemoteAddr();       
     }       
     return ip;       
}
```

