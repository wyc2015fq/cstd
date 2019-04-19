# JavaWeb之Cookie详解 - 零度的博客专栏 - CSDN博客
2017年12月06日 10:09:33[零度anngle](https://me.csdn.net/zmx729618)阅读数：336
## 一、会话的概念
　　会话可简单理解为：用户开一个浏览器，点击多个超链接，访问服务器多个web资源，然后关闭浏览器，整个过程称之为一个会话。
　　有状态会话：一个同学来过教室，下次再来教室，我们会知道这个同学曾经来过，这称之为有状态会话。
## 二、会话过程中要解决的一些问题？
　　每个用户在使用浏览器与服务器进行会话的过程中，不可避免各自会产生一些数据，程序要想办法为每个用户保存这些数据。
## 三、保存会话数据的两种技术
### 3.1、Cookie
　　Cookie是客户端技术，程序把每个用户的数据以cookie的形式写给用户各自的浏览器。当用户使用浏览器再去访问服务器中的web资源时，就会带着各自的数据去。这样，web资源处理的就是用户各自的数据了。
### 3.2、Session
　　Session是服务器端技术，利用这个技术，服务器在运行时可以为每一个用户的浏览器创建一个其独享的session对象，由于session为用户浏览器独享，所以用户在访问服务器的web资源时，可以把各自的数据放在各自的session中，当用户再去访问服务器中的其它web资源时，其它web资源再从用户各自的session中取出数据为用户服务。
## 四、Java提供的操作Cookie的API
　　Java中的javax.servlet.http.Cookie类用于创建一个Cookie
10
public String getDomain()
普通方法
 获取cookie的有效域
　　response接口也中定义了一个addCookie方法，它用于在其响应头中增加一个相应的Set-Cookie头字段。 同样，request接口中也定义了一个getCookies方法，它用于获取客户端提交的Cookie。
## 五、Cookie使用范例
### 5.1、使用cookie记录用户上一次访问的时间
```
1 package gac.xdp.cookie;
 2 
 3 import java.io.IOException;
 4 import java.io.PrintWriter;
 5 import java.util.Date;
 6 import javax.servlet.ServletException;
 7 import javax.servlet.http.Cookie;
 8 import javax.servlet.http.HttpServlet;
 9 import javax.servlet.http.HttpServletRequest;
10 import javax.servlet.http.HttpServletResponse;
11 
12 /**
13  * @author gacl
14  * cookie实例：获取用户上一次访问的时间
15  */
16 public class CookieDemo01 extends HttpServlet {
17 
18     public void doGet(HttpServletRequest request, HttpServletResponse response)
19             throws ServletException, IOException {
20         //设置服务器端以UTF-8编码进行输出
21         response.setCharacterEncoding("UTF-8");
22         //设置浏览器以UTF-8编码进行接收,解决中文乱码问题
23         response.setContentType("text/html;charset=UTF-8");
24         PrintWriter out = response.getWriter();
25         //获取浏览器访问访问服务器时传递过来的cookie数组
26         Cookie[] cookies = request.getCookies();
27         //如果用户是第一次访问，那么得到的cookies将是null
28         if (cookies!=null) {
29             out.write("您上次访问的时间是：");
30             for (int i = 0; i < cookies.length; i++) {
31                 Cookie cookie = cookies[i];
32                 if (cookie.getName().equals("lastAccessTime")) {
33                     Long lastAccessTime =Long.parseLong(cookie.getValue());
34                     Date date = new Date(lastAccessTime);
35                     out.write(date.toLocaleString());
36                 }
37             }
38         }else {
39             out.write("这是您第一次访问本站！");
40         }
41         
42         //用户访问过之后重新设置用户的访问时间，存储到cookie中，然后发送到客户端浏览器
43         Cookie cookie = new Cookie("lastAccessTime", System.currentTimeMillis()+"");//创建一个cookie，cookie的名字是lastAccessTime
44         //将cookie对象添加到response对象中，这样服务器在输出response对象中的内容时就会把cookie也输出到客户端浏览器
45         response.addCookie(cookie);
46     }
47 
48     public void doPost(HttpServletRequest request, HttpServletResponse response)
49             throws ServletException, IOException {
50         doGet(request, response);
51     }
52 
53 }
```
第一次访问时这个Servlet时，效果如下所示：
点击浏览器的刷新按钮，进行第二次访问，此时就服务器就可以通过cookie获取浏览器上一次访问的时间了，效果如下：
 　　在上面的例子中，在程序代码中并没有使用setMaxAge方法设置cookie的有效期，所以当关闭浏览器之后，cookie就失效了，要想在关闭了浏览器之后，cookie依然有效，那么在创建cookie时，就要为cookie设置一个有效期。如下所示：
```
1 //用户访问过之后重新设置用户的访问时间，存储到cookie中，然后发送到客户端浏览器
2 Cookie cookie = new Cookie("lastAccessTime", System.currentTimeMillis()+"");//创建一个cookie，cookie的名字是lastAccessTime
3 //设置Cookie的有效期为1天
4 **cookie.setMaxAge(24*60*60);**5 //将cookie对象添加到response对象中，这样服务器在输出response对象中的内容时就会把cookie也输出到客户端浏览器
6 response.addCookie(cookie);
```
　　用户第一次访问时，服务器发送给浏览器的cookie就存储到了硬盘上，如下所示：
　　这样即使关闭了浏览器，下次再访问时，也依然可以通过cookie获取用户上一次访问的时间。
## 六、Cookie注意细节
- 一个Cookie只能标识一种信息，它至少含有一个标识该信息的名称（NAME）和设置值（VALUE）。
- 一个WEB站点可以给一个WEB浏览器发送多个Cookie，一个WEB浏览器也可以存储多个WEB站点提供的Cookie。
- 浏览器一般只允许存放300个Cookie，每个站点最多存放20个Cookie，每个Cookie的大小限制为4KB。
- **如果创建了一个cookie，并将他发送到浏览器，默认情况下它是一个会话级别的cookie（即存储在浏览器的内存中），用户退出浏览器之后即被删除。若希望浏览器将该cookie存储在磁盘上，则需要使用maxAge，并给出一个以秒为单位的时间。将最大时效设为0则是命令浏览器删除该cookie。**
### 6.1、删除Cookie
**注意：删除cookie时，path必须一致，否则不会删除**
```
1 package gac.xdp.cookie;
 2 
 3 import java.io.IOException;
 4 
 5 import javax.servlet.ServletException;
 6 import javax.servlet.http.Cookie;
 7 import javax.servlet.http.HttpServlet;
 8 import javax.servlet.http.HttpServletRequest;
 9 import javax.servlet.http.HttpServletResponse;
10 
11 /**
12  * @author gacl
13  * 删除cookie
14  */
15 public class CookieDemo02 extends HttpServlet {
16 
17     public void doGet(HttpServletRequest request, HttpServletResponse response)
18             throws ServletException, IOException {
19         //创建一个名字为lastAccessTime的cookie
20         Cookie cookie = new Cookie("lastAccessTime", System.currentTimeMillis()+"");
21         //将cookie的有效期设置为0，命令浏览器删除该cookie
22         cookie.setMaxAge(0);
23         response.addCookie(cookie);
24     }
25 
26     public void doPost(HttpServletRequest request, HttpServletResponse response)
27             throws ServletException, IOException {
28         doGet(request, response);
29     }
30 }
```
###  6.2、cookie中存取中文
　　要想在cookie中存储中文，那么必须使用URLEncoder类里面的encode([String]() s,
[String]() enc)方法进行中文转码，例如：
```
Cookie cookie = new Cookie("userName", URLEncoder.encode("中文, "UTF-8"));
 response.addCookie(cookie);
```
　　在获取cookie中的中文数据时，再使用URLDecoder类里面的decode([String]() s,
[String]() enc)进行解码，例如：
URLDecoder.decode(cookies[i].getValue(), "UTF-8")
**七、cookie生命周期 **
       默认情况：会话级cookie，会话结束后，cookie将被销毁 ；修改有效时间：setMaxAge，单位为秒 
> 
>0 ,指定x秒后失效，将以文件形式保存浏览器端(在硬盘中) 
=0 ,浏览器将删除cookie 
<0 ,会话级cookie，在内存中 
      浏览器处理cookie依据 ：唯一标记格式：domain + path + name ，cookie名称可以重复 
**八、路径设置 **
> 
setPath("路径") ，路径常用设置：setPath("/") 
/ ：web站点根 
/day10/：web项目根 
例如浏览器访问：http://localhost:8080/day10/a/b/c/Demo01CookieServlet 地址，默认路径：/day10/a/b/c/ 
浏览器将携带当前访问路径，以及父路径下的所有cookie,此处符合要求的路径 :
 /day10/a/b/c/   
 /day10/a/b/ 
 /day10/a/            
 /day10/          
 / 
但是，不能获得/day11目录下的cookie 
**九、域名设置domain**
        setDomain("域名") , setDomain(".itheima.com") 
        默认域名：localhost 
        二级域名例如： 
                www.itheima.com 
                bbs.itheima.com 
                v.itheima.com 
                download.itheima.com 
**十、扩展 **
  cookie只能http协议使用，false表示javascript也可以使用 
  cookie.setHttpOnly(true);
