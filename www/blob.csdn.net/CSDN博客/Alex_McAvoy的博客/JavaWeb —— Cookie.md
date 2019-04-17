# JavaWeb —— Cookie - Alex_McAvoy的博客 - CSDN博客





2019年01月11日 22:23:05[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：29








# 【概述】

由于 HTTP 协议是一种无状态协议，即当客户向服务器发送请求，服务器接受请求并作出回应后，该连接就结束了，而服务器不保存相关的信息，为了弥补这一缺点，HTTP 提供了 Cookie 机制。

当浏览器访问 WEB 服务器的某个资源时，WEB 服务器在 HTTP 响应消息头中附带一个小文本文件，传送给浏览器，这个小文本文件就是 Cookie，一旦 WEB 浏览器保存了某个 Cookie，那么它在以后每次访问该 WEB 服务器时，都会在 HTTP 请求头中将这个 Cookie 回传给WEB 服务器。

一个 Cookie 只能标识一种信息，浏览器将 Cookie 以 " key#value " 形式保存到客户端的某个指定目录中，一个 WEB 站点则可以给一个 WEB 浏览器发送多个 Cookie，一个 WEB 浏览器也可存储多个 WEB 站点提供的 Cookie，浏览器一般允许存放 300 个 Cookie，每个站点最多存 20 个，每个 Cookie 的大小限制在 4KB

Cookie 的底层实现原理：WEB 服务器通过在 HTTP 响应消息中增加 Set-Cookie 响应头字段将 Cookie 信息发送给浏览器，浏览器则通过在 HTTP 请求消息中增加 Cookie 请求头字段将 Cookie 回传给 WEB 服务器

![](https://img-blog.csdnimg.cn/2019020323060715.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

# 【常用方法】

Servlet API 中提供了一个 javax.servlet.Cookie 类来封装 Cookie 信息，其包含生成 Cookie 信息和提取 Cookie 信息的各个属性的方法。
- Cookie(String name,String value)：构造器
- String getName()：获取 Cookie 的 Name
- void setValue(String value)：设置 Cookie 的 Value
- String getValue()：获取 Cookie 的 Value
- void setMaxAge(int age)：以秒为单位设置 Cookie 的最大时效
- String getMaxAge()：获取 Cookie 的最大时效
- void setPath()：设置 Cookie 的存储路径
- String getPath(String cookieName)：获取 Cookie 的存储路径

# 【获取与写入】

除 Cookie 类的方法之外，HttpServletResponse 接口的 addCookie() 的方法，可用于在发送给浏览器的 HTTP 响应消息中增加一个 Set-Cookie 响应头字段，该方法并不修改之前指定的任何 Set-Cookie 报头，而是创建新的报头；HttpServletRequest 接口的 getCookie() 的方法，可用于从 HTTP 请求消息的 Cookie 请求头字段中读取所有的 Cookie 项。
- Cookie[] getCookies()：返回请求中发送的所有 cookie 对象
- void addCookie(Cookie cookie)：向客户端写入 Cookie 信息

```
<!-- 获取Cookie -->
<%
    Cookie[] cookies=request.getCookies();//从request中获取cookie对象集合
    if(cookies!=null&&cookies.length>1){
       for(Cookie cookie:cookies){//遍历所有Cookie
            //获取Cookie的name与value
            out.print(cookie.getName()+":"+cookie.getValue());
            out.print("<br>");
       }
    }
%>
<!-- 创建一个Cookie对象并传给客户端 -->
<%
    Cookie cookie=new Cookie("name","test");//创建Cookie对象
    response.addCookie(cookie);//将Cookie传给客户端
%>
```

# 【最大时效】

当创建一个 Cookie 后，默认情况下，其是一个会话级别的 Cookie，称为**会话 Cookie**，其存储在浏览器的内存中，生存周期为浏览器的生存周期，当用户退出浏览器后会自动被删除。

若希望浏览器将 Cookie 存储到磁盘上，则需要使用 setMaxAge() 方法给出一个最大时效，此时浏览器会将 Cookie 存到硬盘上，此时 Cookie 称为**持久 Cookie。**

存储在硬盘上的 Cookie 可以在不同的浏览器进程间共享，当关闭浏览器后再次打开浏览器，这些 Cookie 依然有效，直到超过设定的过期时间。

若将最大时效设为 0 就是命令浏览器删除该 Cookie，若为负数表示不存储该 Cookie

```
<%
    Cookie cookie=new Cookie("name","test");//创建Cookie对象
    cookie.setMaxAge(30);//设置最大时效为30秒
    response.addCookie(cookie);//将Cookie传给客户端
%>
```

# 【作用范围】

Cookie 仅可作用当前目录和当前目录的子目录，一般用 setPath() 方法将 Cookie 的作用范围设为 WEB 站点或当前 WEB 应用。

其中，" / " 代表 WEB 站点，通过 request 对象的 getContextPath() 方法获取的路径为当前 WEB 应用

```
<%
    Cookie cookie=new Cookie("name","test");//创建Cookie对象
    cookie.setPath(request.getContextPath());//设置Cookie作用范围为当前WEB应用
    response.addCookie(cookie);//将Cookie传给客户端
%>
```

#  【Cookie 的写入】

向浏览器写入一个 Cookie 对象一般分为以下 4 个 步骤：
- 创建一个 Cookie 对象
- 设置 Cookie 的最大时效
- 设置 Cookie 的作用范围
- 将 Cookie 传送给客户端

```
<%
    Cookie cookie=new Cookie("name","test");//创建Cookie对象
    cookie.setMaxAge(2*60);//设置Cookie最大时效
    cookie.setPath(request.getContextPath());//设置Cookie作用范围
    response.addCookie(cookie);//将Cookie传给客户端
%>
```



