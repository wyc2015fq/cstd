# 图解 Cookie - 文章 - 伯乐在线
原文出处： [BYSocket （@BYSocket）](http://www.bysocket.com/?p=362)
上一篇 [图解Http协议](http://blog.jobbole.com/108188/) ，这次继续Http家族中的Cookie。泥瓦匠最近看到博客园中一篇好文，如图：
[](http://www.bysocket.com/wp-content/uploads/2015/06/image1.png)![image1](http://jbcdn2.b0.upaiyun.com/2016/11/337d8e4d2679e335665376c9697fbbab.png)[](http://www.bysocket.com/wp-content/uploads/2015/06/image1.png)
这就是因为浏览器Cookie太大，导致请求时，请求头域过大造成发送失败。下面咱们就了解了解Cookie。按着以前的思路图文并茂哈，没图说个XX。
# 一、概述
首先从HTTP说起，Cookie是Http协议中那部分呢？
Cookie是什么？
自问自答：Cookie是**请求头域和响应头域的字段**。简单地说，就是伴随**请求和响应**的一组**键值对**的文本，小文本。所以称之为”Cookie“饼干。Cookie的生命来源于服务器。首先是客户端请求服务端，此时请求为第一次，无Cookie参数。这时候，服务端setCookie发送给客户端。记住，**Cookie来源自服务端**。
Cookie有什么用呢？
又自问自答：Cookie来源自服务端，当然服务于客户。就像你我的会话，文字是在我们之间传递的。所以**Cookie用于服务端和客户端的会话**。因为Http协议是**无状态**的，Cookie就是维持会话，说白了就是传递数据的额外媒介。
下面我们访问百度地址。
① 产生于**服务端**的**Response**，在**响应头域**：
![image9](http://jbcdn2.b0.upaiyun.com/2016/11/037d132a2582c9e7912cc05626b45a08.png)
**② 请求头域**是这样的：（可以在Cookie Tab页发现，和响应有一样的）
![image12](http://jbcdn2.b0.upaiyun.com/2016/11/45ced44e4432591aaf1d21ee9e20f0eb.png)
下面泥瓦匠详细介绍其Cookie在 请求和响应 的传输过程。
# 二、详细介绍Cookie 传输过程
![cookie-work](http://jbcdn2.b0.upaiyun.com/2016/11/11d67997f06077bc4f83b8eaafb89131.png)
直接上图，一一详细解释。顺便写个CookieServlet，模拟一下Cookie的一生。代码如下：

Java
```
package org.bysocket.http;
 
import java.io.IOException;
import java.io.PrintWriter;
 
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
 
@WebServlet(urlPatterns="/cookie")
public class CookieServletT extends HttpServlet
{
    private static final long serialVersionUID = 1L;
 
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp)
            throws ServletException, IOException
    {
        // 获取Cookie
        Cookie[] cookies = req.getCookies();
        for (Cookie cookie : cookies)
            System.out.println(cookie.getName() + " " + cookie.getValue());
 
        // 创建Cookie
        Cookie cookie = new Cookie("CookieName", "CookieValue");
        cookie.setMaxAge(10);
        cookie.setHttpOnly(true);
        resp.addCookie(cookie);
         
        // 响应
        PrintWriter pw = resp.getWriter();
        pw.print("<html><body><h1>Hello,Cookie!</h1></body></html>");
    }
     
}
```
### ① 客户端访问，无服务端写入的Cookie。
代码 new Cookie(“CookieName”, “CookieValue”); 可以看出服务端产生一个新的键值对Cookie，并且设置，说明**第一次请求**时，请求的请求头域**Cookie是没有**的。下面没有CookieName=CookieValue 的Cookie值。如图：
![image2](http://jbcdn2.b0.upaiyun.com/2016/11/f1041671b9839cb4250c383537d8a11d.png)
### ② 服务端的Cookie传至浏览器。
代码中 HttpServletResponse.addCookie(cookie); 这样响应就加入了刚刚那个键值对Cookie。怎么传到浏览器（客户端）呢? 同样F12下，
![image3](http://jbcdn2.b0.upaiyun.com/2016/11/2c94fb57207d2f0bc827191a447449d6.png)
从图中可得到，Cookie是通过HTTP的响应头域发送至浏览器。每个Cookie的set，都有一个对应Set-Cookie的头。还有其中的时间代表Cookie的存活时间，HttpOnly可是此Cookie只读模式。
### ③ 浏览器解析Cookie，保存至浏览器文件。
直接可以打开IE的Internet选项：
![image4](http://jbcdn2.b0.upaiyun.com/2016/11/c407cdc2522c3b209729364b2cbac42b.png)
如图，那个位置文件就是我们Cookie存的地方。既然在哪里，泥瓦匠就去找到它。
[](http://www.bysocket.com/wp-content/uploads/2015/06/image5.png)![image5](http://jbcdn2.b0.upaiyun.com/2016/11/95fde0df7052036d8063e24313c6755d.png)[](http://www.bysocket.com/wp-content/uploads/2015/06/image5.png)
打开看看，其内容就是:存放着Cookie信息和URL信息及一些关于时间的。


```
CookieName
CookieValue
localhost/servletBYSocket/
9728
 3416923392
 30449698
 3325104969
 30449698
 *
```
这样就完全搞懂了Cookie如何写入浏览器。
# ④ 客户端访问，有服务端写入的Cookie。
这样，同样的URL再次访问时，F12下：
![image6](http://jbcdn2.b0.upaiyun.com/2016/11/f3d4d8b13e5d53a73e59e45b33d86f77.png)
不多解释，看图。
### ⑤ 服务器获取
服务端这时呢？只要简单的 getCookies() 就可以获取Cookie列表了。如图，服务端控制台打印如下：
![image7](http://jbcdn2.b0.upaiyun.com/2016/11/c4de312a25c0c660eaa151ace4d9b61e.png)
**泥瓦匠记忆小抄：Cookie传输小结**
① 客户端访问，无服务端写入的Cookie
② 服务端的Cookie写入浏览器
③ 浏览器解析Cookie，保存至浏览器文件
④ 客户端访问，有服务端写入的Cookie
⑤ 服务器获取
# 四、谈Cookie的作用到XSS（跨站点脚本攻击）
Cookie没有病毒那么危险，但包含敏感信息。比如最常见的记住密码，或者一些用户经常浏览的网页数据。如图：
![u34268335753625518714fm21gp0](http://jbcdn2.b0.upaiyun.com/2016/11/b7d63bc518618500c0da9e8f9ebabfc2.jpg)
用户不希望这些泄露，甚至被攻击。但事实上存在这个攻击，究竟怎么攻击呢？我在 跨脚本攻击XSS 一文中也详细介绍并提出解决方案。
全名：Cross Site Script，中文名：跨站脚本攻击。顾名思义，是指“HTML注入”纂改了网页，插入恶意的脚本，从而在用户用浏览网页的时候，控制用户浏览器的一种攻击。一般攻击的套路如图所示：
![image_thumb71](http://jbcdn2.b0.upaiyun.com/2016/11/c227051b6241bcd3bc394ed6de13fcf1.png)
# 五、总结
回顾全文，Cookie是HTTP协议中的一种会话机制。也明白下面两个问题就好了
> 
1、What 什么是Cookie
2、How Cookie怎么用，干嘛用
