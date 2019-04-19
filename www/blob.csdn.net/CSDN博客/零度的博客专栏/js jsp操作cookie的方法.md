# js/jsp操作cookie的方法 - 零度的博客专栏 - CSDN博客
2016年05月10日 11:29:09[零度anngle](https://me.csdn.net/zmx729618)阅读数：5005
最近项目用到了自动登录功能，使用到了cookie。
一、cookie简介    浏览器与WEB服务器之间是使用HTTP协议进行通信的；而HTTP协议是无状态协议。也就是说，当某个用户发出页面请求时，WEB服务器只是简单的进行 响应，然后就关闭与该用户的连接。因此当一个请求发送到WEB服务器时，无论其是否是第一次来访，服务器都会把它当作第一次来对待，这样的不好之处可想而 知。为了弥补这个缺陷，Netscape开发出了cookie这个有效的工具来保存某个用户的识别信息，     它是一种WEB服务器通过浏览器在访问者的硬盘上存储信息的手段。     它是服务器发送给浏览器的体积很小的纯文本信息。    
 定义：cookie是Web浏览器存储的少量命名数据，它与某个特定的网页和网站关联在一起。     cookie实际上和某一网站和网页关联的，保存用户一定信息的文本文件。
二、cookie的属性     除名字与值外，每个cookie有四个可选属性：     1.expires：指定cookie的生存期。默认情况下cookie是暂时的，浏览器关闭就失效。     2.path：它指定了与cookie关联在一起的网页。默认是在和当前网页同一目录的网页中有效。         如果把path设置为"/"，那么它对该网站的所有网页都可见了。     3.domain：设定cookie有效的域名，         如果把path设置为"/"，把domain设为".sohu.com"，那么
 A.sohu.com和B.sohu.com         的所有网页都能够访问此cookie。     4.secure：布尔值，它指定了网络上如何传输cookie。默认情况下，cookie是不安全的，         可以通过一个不安全的，普通的HTTP协议传输；若设置cookie为安全的，那么它将         只在浏览器和服务器通过HTTPS或其它的安全协议连接在一起时才被传输。
三、cookie的操作     cookie可以用javascipt来操作，也可以用JSP来操作。     下面给大家我写的几个例子，相信大家一看就明白了：     1.javascript 操作：    
    <script language="javascript"> 
    //设置cookie，cookie有效期时间未GMT时间（距1970年1月1日临时的毫秒） 
    //例如可以设置setCookie("password","12345",(3600000*24*180)),180有效 
    function setCookie (name, value, expires) {  
var expdate = new Date(); 
expdate.setTime(expdate.getTime() + expires); 
document.cookie = name + "=" + escape(value) + 
        "; expires=" + expires.toGMTString() +  "; path=/"; 
    } 
    //根据cookie名，取得cookie值 
    function getCookie(name) {   
      var search;  
      search = name + "=" 
      offset = document.cookie.indexOf(search) 
      if (offset != -1) { 
        offset += search.length ; 
        end = document.cookie.indexOf(";", offset) ; 
        if (end == -1) 
          end = document.cookie.length; 
        return unescape(document.cookie.substring(offset, end)); 
      } 
      else 
        return ""; 
    } 
    //删除某一cookie 
    function deleteCookie(name) {    
      var expdate = new Date(); 
      expdate.setTime(expdate.getTime() - (3600 *24* 1000 * 1)); 
      setCookie(name, "", expdate); 
    } 
    //检查是否存在此cookie 
    function checkCookie(cookieName,cookieValue){ 
if (getCookie(cookieName)==cookieValue){ 
  return true; 
}else{ 
  return false; 
} 
    } 
    </script> 
    2.jsp 操作：     java中有个Cookie类：javax.servlet.http.Cookie 
    //读取cookie的通用类，以Cookie[] 做参数传个构造函数； 
package com.test; 
import javax.servlet.http.*; 
public class ComCookie { 
private Cookie[] cookies; 
private Cookie cookie; 
public ComCookie(Cookie[] cs){ 
  cookies = cs;  
} 
/** 
  * 通过cookieName，取得cookieValue，如果没有此cookie则返回默认值 
  * @param cookieName 
  * @param defaultValue 
  * @return 
  */ 
public String getCookieValue(String cookieName,String defaultValue) { 
  for(int i=0; i< cookies.length; i++) { 
    Cookie cookie = cookies[i]; 
    if (cookieName.equals(cookie.getName())) 
   return(cookie.getValue()); 
  } 
  return(defaultValue); 
} 
/** 
  * 类方法，通过cookieName，取得cookieValue 
  * @param cookies 
  * @param cookieName 
  * @param defaultValue 
  * @return 
  */ 
public static String getCookieValue(Cookie[] cookies, 
     String cookieName, 
     String defaultValue) { 
    for(int i=0; i < cookies.length; i++) { 
          Cookie cookie = cookies[i]; 
          if (cookieName.equals(cookie.getName())) 
              return(cookie.getValue()); 
  } 
  return(defaultValue); 
} 
} 
JSP中：(以下内容来源于网络) 
JSP 是使用如下的语法格式来创建cookie的： 　　Cookie cookie_name =new Cookie("Parameter","Value"); 　　例如：Cookie newCookie =new Cookie("username","waynezheng"); response.addCookie(newCookie); 解释：JSP是调用Cookie对象相应的构造函数Cookie(name,value)用合适的名字和值来创建Cookie，然后 Cookie可以通过HttpServletResponse的addCookie方法加入到Set-Cookie应答头，本例中Cookie对象有两个
 字符串参数：username,waynezheng。注意，名字和值都不能包含空白字符以及下列字符： @ : ;? , " / [ ] ( ) = 
　　处理Cookie的属性 看到这里，有的朋友又要问了：我光知道如何创建Cookie有什么用呀？是呀，光知道如何创建Cookie而不知道怎么使用是不够的。在JSP中，程 序是通过cookie.setXXX设置各种属性，用cookie.getXXX读出cookie的属性，现把Cookie的主要属性，及其方法列于下， 供大家参考：  读取客户端的Cookie
类型 方法名 方法解释 
String getComment() 返回cookie中注释,如果没有注释的话将返回空值. 
String  getDomain() 返回cookie中Cookie适用的域名. 使用getDomain() 方法可以指示浏览器把Cookie返回给同 一域内的其他服务器,而通常Cookie只返回给与发送它的服务器名字完全相同的服务器。注意域名必须以点开始（例如.yesky.com） 
int  getMaxAge()  返回Cookie过期之前的最大时间，以秒计算。  
String getName()  返回Cookie的名字。名字和值是我们始终关心的两个部分，笔者会在后面详细介绍 getName/setName。  
String getPath() 返回Cookie适用的路径。如果不指定路径，Cookie将返回给当前页面所在目录及其子目录下 的所有页面。 
boolean getSecure() 如果浏览器通过安全协议发送cookies将返回true值，如果浏览器使用标准协议则返回false值。 
String  getValue() 返回Cookie的值。笔者也将在后面详细介绍getValue/setValue。  
int getVersion()  返回Cookie所遵从的协议版本。  
void setComment(String purpose) 设置cookie中注释。 
void setDomain(String pattern) 设置cookie中Cookie适用的域名 
void setMaxAge(int expiry)  以秒计算，设置Cookie过期时间。 
void setPath(String uri) 指定Cookie适用的路径。  
void  setSecure(boolean flag) 指出浏览器使用的安全协议，例如HTTPS或SSL。  
void setValue(String newValue) cookie创建后设置一个新的值。  
void setVersion(int v) 设置Cookie所遵从的协议版本。 　　  
在Cookie发送到客户端前，先要创建一个Cookie，然后用addCookie方法发送一个HTTP Header。  JSP将调用request.getCookies()从客户端读入Cookie，getCookies()方法返回一个HTTP请求头中  的内容对应的Cookie对象数组。你只需要用循环访问该数组的各个元素,调用getName方法检查各  个Cookie的名字，至找到目标Cookie，然后对该Cookie调用getValue方法取得与指定名字关联的值 。
例如 
<% 
　String userName=request.getParameter("username");//从提交的HTML表单中获取，用户名 
　Cookie theUsername=new Cookie("username",userName);//以"username",userName值/对创建一个Cookie
　response.addCookie(theUsername); 
%> 
.............. 
<% 
　Cookie myCookie[]=request.getCookies();//创建一个Cookie对象数组 
　for(int n=0;n=cookie.length-1;i++);//设立一个循环，来访问Cookie对象数组的每一个元素 
　Cookie newCookie= myCookie[n]; 
　if(newCookie.getName().equals("username")); //判断元素的值是否为username中的值 
　　{%> 
　　　你好,<%=newCookie.getValue()%>!//如果找到后，向他问好 
　　<%} 
%> 
　　设置Cookie的存在时间，及删除Cookie 在JSP中，使用setMaxAge(int expiry)方法来设置Cookie的存在时间， 
参数expiry应是一个整数。正值表示cookie将在这么多秒以后失效。 注意这个值是cookie将要存在的最大时间， 
而不是cookie现在的存在时间。 负值表示当浏览器关闭时，Cookie将会被删除。零值则是要删除该Cookie。 如： 　　 
<% 
　Cookie deleteNewCookie=new Cookie("newcookie",null); 
　deleteNewCookie.setMaxAge(0); 
　deleteNewCookie.setPath("/"); 
　response.addCookie(deleteNewCookie); 
%>
