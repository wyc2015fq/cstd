# Cookie实现同域，跨域单点登录 - 零度的博客专栏 - CSDN博客
2016年05月10日 11:32:30[零度anngle](https://me.csdn.net/zmx729618)阅读数：5834
个人分类：[Session/Cookie](https://blog.csdn.net/zmx729618/article/category/6221899)

**[javascript]**[view plain](http://blog.csdn.net/smartsmile2012/article/details/37650939#)[copy](http://blog.csdn.net/smartsmile2012/article/details/37650939#)[print](http://blog.csdn.net/smartsmile2012/article/details/37650939#)[?](http://blog.csdn.net/smartsmile2012/article/details/37650939#)
- Cookie 同域单点登录  
- 
- 最近在做一个单点登录的系统整合项目，之前我们使用控件实现单点登录(以后可以介绍一下)。但现在为了满足客户需求，在不使用控件情况下实现单点登录，先来介绍一下单点登录。  
- 
- 单点登录：多个不同系统整合到统一加载个平台，用户在任何一个系统登录后，可以访问这个统一加载上的所有系统。登录之后，用户的权限和信息不再受某个系统的限制，即使某个系统出现故障(包括统一加载平台)，其他系统还是能正常使用的。这就需要用户权限等信息保存到客户端，不受服务器的限制。  
- 
- 在cookie相关文档信息中，提到cookie是不能跨域访问的，但是在二级域名是可以共享cookie的。这样就是我们的项目有了局限性，必须将多个系统的域名统一，作为二级域名，统一平台提供使用主域名。这样就可以实现cookie的单点登录了。  
- 
- 在这里介绍一下cookie：  
- 
- 1.cookie是一个存储在客户端的字符串属性,可以用它对当前网页的cookie进行读,写,增.删等操作;javascript能够用document对象的cookie属性对cookie进行操作;  
- 
- 2.cookie的四个可选属性:  
- 
- 2.1 cookie的生存期属性:expires;默认情况下,cookie只在浏览器会话期存在.退出浏览器就丢失;可以用expires设置时间;退出浏览器后就不会丢失并存为客户端浏览器的cookie文件;过了时间后cookie失效,还会自动删除cookie文件.  
- 
- 2.2 path属性:默认情况下,在同一个目录下文件可以调用;  
- 例如:http://hanj.com/c1/1.html设置的cookie可以被http://hanj.com/c1/2.html调用.但不能被http://hanj.com/c2/目录下的文件调用;
- 但如把path属性设成"/";则在http://hanj.com/下的所有文件都可调用此cookie.
- 
- 2.3 domain属性:例如设成".hanj.com"则在.hanj.com下的所有服务器下的文件都可以调用cookie.  
- 
- 2.4 安全属性:默认情况下为false;用http协议不安全传输;true:用https等协议安全传输.  
- 
- 3.cookie的局限性:  
- 
- 浏览器最多保存300个cookie;为单个web服务器的最多只能保存20个cookie;每个cookie不能超过4000个字节.  
- 
- 单点登录实现环境：  
- 
- 统一平台域名：www.hanj.com  
- 
- 子系统1：a.hanj.com  
- 
- 子系统2：b.hanj.com  
- 
- 子系统3：c.hanj.com  
- 
- 统一加载平台和各子系统都是不同的服务器，统一加载平台提供登录认证服务，在统一加载平台认证系统上登录后，用户都可以被其他的系统识别。  
- 
- 
- 
- /**
- 函数名称：getCookie
- 函数功能：获取指定名称的cookie的值
- 输入参数：需要测试的字符串
- 返回参数：
- */
- function getSSOCookie()  
- {  
- var arrStr = document.cookie.split("; ");  
- for(var i = 0;i < arrStr.length;i ++){  
- var temp = arrStr[i].split("=");  
- if(temp[0] == "sso") {  
- return unescape(temp[1]);  
-         }  
-     }  
- return"";  
- }  
- /**
- 函数名称：addCookie
- 函数功能：添加cookie
- 输入参数：需要测试的字符串
- 返回参数：
- */
- function addSSOCookie(objValue)  
- {  
- var str = "sso" + "=" + escape(objValue);  
- if(true){//为0时不设定过期时间，浏览器关闭时cookie自动消失
-         str += "; path=/";  
-     }  
-     document.cookie = str;  
- }  
- 
- /**
- 函数名称：delCookie
- 函数功能：删除cookie
- 输入参数：需要测试的字符串
- 返回参数：
- */
- function delCookie()  
- {//为了删除指定名称的cookie，可以将其过期时间设定为一个过去的时间
- var date =  new  Date();  
-     date.setTime(date.getTime() - 10000);  
-     document.cookie = "sso" + "=a; expires=" + date.toGMTString()+"; path=/";  
- }  
- 
- 
- 用户在统一加载平台认证系统认证通过后，使用addSSOCookie，用户权限信息保存到了cookie中，其他平台通过调用getSSOCookie，取得用户信息。这样用户就可以不再受平台限制，而实现自由访问各个系统了。  
- 
- 在addSSOCookie方法中，没有设置cookie的失效时间，这样在浏览器关闭后，cookie就自动消失。注意：这样cookie的有效性只能在同一浏览器进程，如果重新打开了一个浏览器进程，cookie信息是获取不到的，也就是单点登录只能在同一个浏览器进程有效。如果想在不用浏览器进程中共享cookie信息，那就设置失效时间，如下：  
- 
- function addCookie(objValue,objHours){//添加cookie
- var str = "sso"+ "=" + escape(objValue);  
- if(objHours > 0){//为0时不设定过期时间，浏览器关闭时cookie自动消失
- var date = new Date();  
- var ms = objHours*3600*1000;  
-                 date.setTime(date.getTime() + ms);  
-                 str += "; expires=" + date.toGMTString()+"; path=/; domain=.hanj.com";  
-             }  
-             document.cookie = str;  
-         }  
- 
- 
- 这样cookie在指定的时间后失效。但这样安全性不能保证，如果时间设置太短，用户在使用中，可能cookie就失效了，需要重新登录。如果时间过长，用户在下次访问，或电脑重起访问，cookie还在有效期中，有可能别其他人使用。cookie不能准确的删除，存在安全隐患。  
```java
Cookie 同域单点登录
 	 
最近在做一个单点登录的系统整合项目，之前我们使用控件实现单点登录(以后可以介绍一下)。但现在为了满足客户需求，在不使用控件情况下实现单点登录，先来介绍一下单点登录。
单点登录：多个不同系统整合到统一加载个平台，用户在任何一个系统登录后，可以访问这个统一加载上的所有系统。登录之后，用户的权限和信息不再受某个系统的限制，即使某个系统出现故障(包括统一加载平台)，其他系统还是能正常使用的。这就需要用户权限等信息保存到客户端，不受服务器的限制。
在cookie相关文档信息中，提到cookie是不能跨域访问的，但是在二级域名是可以共享cookie的。这样就是我们的项目有了局限性，必须将多个系统的域名统一，作为二级域名，统一平台提供使用主域名。这样就可以实现cookie的单点登录了。
在这里介绍一下cookie：
1.cookie是一个存储在客户端的字符串属性,可以用它对当前网页的cookie进行读,写,增.删等操作;javascript能够用document对象的cookie属性对cookie进行操作;
2.cookie的四个可选属性:
2.1 cookie的生存期属性:expires;默认情况下,cookie只在浏览器会话期存在.退出浏览器就丢失;可以用expires设置时间;退出浏览器后就不会丢失并存为客户端浏览器的cookie文件;过了时间后cookie失效,还会自动删除cookie文件.
2.2 path属性:默认情况下,在同一个目录下文件可以调用;
例如:http://hanj.com/c1/1.html设置的cookie可以被http://hanj.com/c1/2.html调用.但不能被http://hanj.com/c2/目录下的文件调用;
但如把path属性设成"/";则在http://hanj.com/下的所有文件都可调用此cookie.
2.3 domain属性:例如设成".hanj.com"则在.hanj.com下的所有服务器下的文件都可以调用cookie.
2.4 安全属性:默认情况下为false;用http协议不安全传输;true:用https等协议安全传输.
3.cookie的局限性:
浏览器最多保存300个cookie;为单个web服务器的最多只能保存20个cookie;每个cookie不能超过4000个字节.
单点登录实现环境：
统一平台域名：www.hanj.com
子系统1：a.hanj.com
子系统2：b.hanj.com
子系统3：c.hanj.com
统一加载平台和各子系统都是不同的服务器，统一加载平台提供登录认证服务，在统一加载平台认证系统上登录后，用户都可以被其他的系统识别。
 
/**
函数名称：getCookie
函数功能：获取指定名称的cookie的值
输入参数：需要测试的字符串
返回参数：
*/
function getSSOCookie()
{
    var arrStr = document.cookie.split("; ");
    for(var i = 0;i < arrStr.length;i ++){
        var temp = arrStr[i].split("=");
        if(temp[0] == "sso") {
          return unescape(temp[1]);
        }
    }
    return "";
}
/**
函数名称：addCookie
函数功能：添加cookie
输入参数：需要测试的字符串
返回参数：
*/
function addSSOCookie(objValue)
{
    var str = "sso" + "=" + escape(objValue);
    if(true){//为0时不设定过期时间，浏览器关闭时cookie自动消失
        str += "; path=/";
    }
    document.cookie = str;
}
/**
函数名称：delCookie
函数功能：删除cookie
输入参数：需要测试的字符串
返回参数：
*/
function delCookie()
{//为了删除指定名称的cookie，可以将其过期时间设定为一个过去的时间
    var date =  new  Date();
    date.setTime(date.getTime() - 10000);
    document.cookie = "sso" + "=a; expires=" + date.toGMTString()+"; path=/";
}
 
用户在统一加载平台认证系统认证通过后，使用addSSOCookie，用户权限信息保存到了cookie中，其他平台通过调用getSSOCookie，取得用户信息。这样用户就可以不再受平台限制，而实现自由访问各个系统了。
在addSSOCookie方法中，没有设置cookie的失效时间，这样在浏览器关闭后，cookie就自动消失。注意：这样cookie的有效性只能在同一浏览器进程，如果重新打开了一个浏览器进程，cookie信息是获取不到的，也就是单点登录只能在同一个浏览器进程有效。如果想在不用浏览器进程中共享cookie信息，那就设置失效时间，如下：
function addCookie(objValue,objHours){//添加cookie
            var str = "sso"+ "=" + escape(objValue);
            if(objHours > 0){//为0时不设定过期时间，浏览器关闭时cookie自动消失
                var date = new Date();
                var ms = objHours*3600*1000;
                date.setTime(date.getTime() + ms);
                str += "; expires=" + date.toGMTString()+"; path=/; domain=.hanj.com";
            }
            document.cookie = str;
        }
 
这样cookie在指定的时间后失效。但这样安全性不能保证，如果时间设置太短，用户在使用中，可能cookie就失效了，需要重新登录。如果时间过长，用户在下次访问，或电脑重起访问，cookie还在有效期中，有可能别其他人使用。cookie不能准确的删除，存在安全隐患。
```
**[javascript]**[view plain](http://blog.csdn.net/smartsmile2012/article/details/37650939#)[copy](http://blog.csdn.net/smartsmile2012/article/details/37650939#)[print](http://blog.csdn.net/smartsmile2012/article/details/37650939#)[?](http://blog.csdn.net/smartsmile2012/article/details/37650939#)
- Cookie跨域单点登录  
- 为了快速、简单的实现这一功能，首先想到就是通过JS操作Cookie并让两个不同域的cookie能够相互访问，这样就可达到了上述的效果，具体实现过程大致可分以下两个步骤：  
- 
- １、在Ａ系统下成功登录后，利用JS动态创建一个隐藏的iframe，通过iframe的src属性将Ａ域下的cookie值作为  
- get参数重定向到Ｂ系统下b.aspx页面上；  
- 
- var _frm = document.createElement("iframe");  
- _frm.style.display="none";   
- _frm.src="http://b.com/b.jsp?test_cookie=xxxxx";   
- document.body.appendChild(_frm);     
- 
- 
- 2、在Ｂ系统的b.aspx页面中来获取Ａ系统中所传过来的cookie值，并将所获取到值写入cookie中，这样就简单的实现了cookie跨域的访问；　不过这其中有个问题需要注意，就是在IE浏览器下这样操作不能成功，需要在b.aspx页面中设置P3P HTTP Header就可以解决了（具体詳細信息可以参考:http://www.w3.org/P3P/)，P3P设置代码为：
- /*
- *也可以在html加入标记
- <meta http-equiv="P3P" content='CP="IDC DSP COR CURa ADMa  OUR IND PHY ONL COM STA"'>  
- */
- Response.AppendHeader("P3P", "CP='IDC DSP COR CURa ADMa  OUR IND PHY ONL COM STA'");  
