# tomcat7+nginx+memcached实现负载均衡和session共享(windows) - 零度的博客专栏 - CSDN博客
2016年10月28日 12:34:11[零度anngle](https://me.csdn.net/zmx729618)阅读数：789
tomcat7 下 memcached分布式session 解决方案：
   1、nginx安装与配置 
          a、nginx下载：http://download.csdn.net/detail/ajian11/8282983
          b、nginx安装：
将nginx的程序copy到C盘，(window下权限问题)
进入window的cmd窗口，cd到nginx目录，使用“start nginx.exe ”，如果任务管理器中出现两个nginx.exe说明安装成功
           c、nginx配置：
              找到 nginx.conf，该文件为nginx的请求分发配置文件，打开nginx.conf做如下修改：
 　　　（1）在http {…} - server{…} - location / {…}中加入一行：“proxy_pass  http://127.0.0.1;”
　　　　修改后如下：   
               location / {  
                      root   html;  
                      index  index.html index.htm;  
proxy_pass    [http://127.0.0.1](http://127.0.0.1);
}  
        (2)在http {…}中加入以下代码：
               #设定负载均衡的服务器列表 分别对应两台tomcat的端口
               upstream 127.0.0.1 {   
                          server 127.0.0.1:8080 weight=1;  
                          server 127.0.0.1:8081 weight=2;  
                 }
 2、 memcached
下载地址：[http://download.csdn.net/detail/ajian11/8283017](http://download.csdn.net/detail/ajian11/8283017)
         安装：cd到memcached 目录，使用“memcached.exe -d install ”
 3、tomcat配置
Copy两份tomcat用于模拟两台服务器， 
删除环境变量中的CATALINA_HOME和CATALINA_BASE  
分别修改安装目录下的conf子目录中的server.xml文件：
         a.修改http访问端口（默认为 8080端口），将8080修改为tomcat唯一的，其他tomcat不在使用的端口号。此处所设的端口号即是以后访问web时所用的端口号。
         b.修改Shutdown端口（默 认为8005端口），将8005修改为唯一的的端口号，例如8055。
         c.修改8009端口，将8009修改为没有在使用的端口号，例如8099（注 意：N个文件中对应的端口号要都不一样）
4、配置session 共享
用到的jar包memcached-session-manager-tc7-1.6.3和spymemcached-2.7.3.jar等
          1、memcached的jar下载：http://download.csdn.net/detail/ajian11/8283079
          2、把上述jar包放到tomcat/lib 目录下,修改每个tomcat的配置文件tomcat\conf\context.xml
　　   在<context>...</context>标签之间加入如下代码：
```
<Manager  
className="de.javakaffee.web.msm.MemcachedBackupSessionManager"  
memcachedNodes="n1:127.0.0.1:11211"  
sticky="false"  
sessionBackupAsync="false"  
lockingMode="uriPattern:/path1|/path2"  
requestUriIgnorePattern=".*\.(ico|png|gif|jpg|css|js)$"  
transcoderFactoryClass="de.javakaffee.web.msm.serializer.kryo.KryoTranscoderFactory"/>
```
5、测试
新建一个cluster.jsp文件
```
<%@ page contentType="text/html; charset=UTF-8" %> 
< %@ page import="java.util.*" %> 
< html><head><title>Cluster Test</title></head> 
< body> 
< % 
   //HttpSession session = request.getSession(true); 
   System.out.println(session.getId()); 
   out.println("<br> SESSION ID:" + session.getId()+"<br>");   
   // 如果有新的请求，则添加session属性 
   String name = request.getParameter("name"); 
   if (name != null && name.length() > 0) { 
      String value = request.getParameter("value"); 
      session.setAttribute(name, value); 
   }   
     out.print("<b>Session List:</b>");   
     Enumeration<String> names = session.getAttributeNames(); 
     while (names.hasMoreElements()) { 
         String sname = names.nextElement();  
         String value = session.getAttribute(sname).toString(); 
         out.println( sname + " = " + value+"<br>"); 
         System.out.println( sname + " = " + value); 
    } 
 %> 
< /body> 
< /html>
```
在浏览器中输入[localhost:8080/cluster/cluster.jsp](http://www.cnblogs.com/fish-king/admin/clustertest.jsp) 
 重复刷新发现session的值不变如下所示，session共享成功。
SESSION ID:ID:C881A23C90BA8D57BA1A69DB069355D0-n1.jmv-1
