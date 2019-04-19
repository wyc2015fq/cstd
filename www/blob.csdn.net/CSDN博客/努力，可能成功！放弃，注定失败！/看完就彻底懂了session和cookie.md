# 看完就彻底懂了session和cookie - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年07月09日 16:51:32[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：16601
## ttp 之session和cookie
- 由于HTTP协议是无状态的协议，所以服务端需要记录用户的状态时，就需要用某种机制来识具体的用户，这个机制就是Session.典型的场景比如购物车，当你点击下单按钮时，由于HTTP协议无状态，所以并不知道是哪个用户操作的，所以服务端要为特定的用户创建了特定的Session，用用于标识这个用户，并且跟踪用户，这样才知道购物车里面有几本书。这个Session是保存在服务端的，有一个唯一标识。在服务端保存Session的方法很多，内存、数据库、文件都有。集群的时候也要考虑Session的转移，在大型的网站，一般会有专门的Session服务器集群，用来保存用户会话，这个时候
 Session 信息都是放在内存的，使用一些缓存服务比如Memcached之类的来放 Session。
- **思考一下服务端如何识别特定的客户**？这个时候Cookie就登场了。每次HTTP请求的时候，客户端都会发送相应的Cookie信息到服务端。实际上大多数的应用都是用 Cookie 来实现Session跟踪的，第一次创建Session的时候，服务端会在HTTP协议中告诉客户端，需要在 Cookie 里面记录一个Session ID，以后每次请求把这个会话ID发送到服务器，我就知道你是谁了。有人问，如果客户端的浏览器禁用了 Cookie 怎么办？一般这种情况下，会使用一种叫做URL重写的技术来进行会话跟踪，即每次HTTP交互，URL后面都会被附加上一个诸如
 sid=xxxxx 这样的参数，服务端据此来识别用户。
- Cookie其实还可以用在一些方便用户的场景下，设想你某次登陆过一个网站，下次登录的时候不想再次输入账号了，怎么办？这个信息可以写到Cookie里面，访问网站的时候，网站页面的脚本可以读取这个信息，就自动帮你把用户名给填了，能够方便一下用户。这也是Cookie名称的由来，给用户的一点甜头。
所以，总结一下：
Session是在服务端保存的一个数据结构，用来跟踪用户的状态，这个数据可以保存在集群、数据库、文件中；
Cookie是客户端保存用户信息的一种机制，用来记录用户的一些信息，也是实现Session的一种方式
### Cookie： --->常见的应用场景是：自动登录的
Cookie是浏览器保存信息的一种方式，可以理解为一个文件，保存到客户端了啊，服务器可以通过响应浏览器的set-cookie的标头，得到Cookie的信息。你可以给这个文件设置一个期限，这个期限呢，不会因为浏览器的关闭而消失啊。其实大家应该对这个效果不陌生，很多购物网站都是这个做的，即使你没有买东西，他也记住了你的喜好，现在回来，会优先给你提交你喜欢的东西啊，他们也真是煞费苦心了啊。
#### Cookie的操作：
- 
添加Cookie
```cpp
```cpp
Cookie cookie = new Cookie("user", "suntao");
  cookie.setMaxAge(7*24*60*60);     // 一星期有效
  response.addCookie(cookie);
```
```
- 
获取Cookie
```
// 因为取得的是整个网页作用域的Cookie的值，所以得到的是个数组
  Cookie[] cookies = request.getCookies();
  for(int i = 0 ; i < cookies.length ; i++){
   String name = cookies[i].getName() ;
    String value = cookies[i].getValue() ;
   }
```
> 
cookie的存活期：默认为-1 
```
会话Cookie：把Cookie保存到浏览器上，当存活期为负数  
         持久Cookie：把Cookie保存到文件中，当存活期为正数  
         设置存活期：c.setMaxAge();
```
### HttpSession 会话机制 -->Servlet的会话机制的实现
创建于服务器端，保存于服务器，维护于服务器端,每创建一个新的Session,服务器端都会分配一个唯一的ID，并且把这个ID保存到客户端的Cookie中，保存形式是以JSESSIONID来保存的。
- 
|-- 通过HttpServletRequest.getSession 进行获得HttpSession对象，通过setAttribute()给会话赋值，可以通过invalidate()将其失效。
- 
|--每一个HttpSession有一个唯一的标识SessionID，只要同一次打开的浏览器通过request获取到session都是同一个。
- 
|--WEB容器默认的是用Cookie机制保存SessionID到客户端，并将此Cookie设置为关闭浏览器失效，Cookie名称为：JSESSIONID
- 
|--每次请求通过读取Cookie中的SessionID获取相对应的Session会话
- 
|--HttpSession的数据保存在服务器端，所以不要保存数据量耗资源很大的数据资源，必要时可以将属性移除或者设置为失效
- 
|--HttpSession可以通过setMaxInactiveInterval()设置失效时间(秒)或者在web.xml中配置
```xml
<session-config>
       <!--单位：分钟-->
     <session-timeout>30</session-timeout>
 </session-config>
```
- 
|--HttpSession默认使用Cookie进行保存SessionID，当客户端禁用了Cookie之后，可以通过URL重写的方式进行实现。 
- 
可以通过response.encodeURL(url) 进行实现 
- API对encodeURL的结束为，当浏览器支持Cookie时，url不做任何处理；当浏览器不支持Cookie的时候，将会重写URL将SessionID拼接到访问地址后。
要想了解内部发生了什么操作，点击以下链接[Http Session和Cookie原理](http://my.oschina.net/biezhi/blog/490242#OSC_h2_3)
文／清枫_小天（简书作者）
原文链接：http://www.jianshu.com/p/25802021be63
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
