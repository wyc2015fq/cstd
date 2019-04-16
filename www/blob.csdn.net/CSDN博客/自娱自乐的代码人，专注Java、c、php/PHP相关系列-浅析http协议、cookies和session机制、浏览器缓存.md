# PHP相关系列 - 浅析http协议、cookies和session机制、浏览器缓存 . - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年07月31日 18:37:06[initphp](https://me.csdn.net/initphp)阅读数：2547








                
作者：[benwin](http://www.phpben.com/)
来源：http://www.phpben.com/?post=77









最近几天在复习http协议中headers，cookies、session、缓存等相关知识，发现些新知识点。

这篇文章注重结合PHP去理解这些内容，也就是比较注重实践部分。

**一、            http headers          **

![](http://www.phpben.com/uploadfile/13396553380.99971400.jpg)

**NO1：**对于web应用，用户群在客户端 （各种浏览器）点击任何一个连接向服务器发送http请求，这过程肯定需要3次握手，建立连接，服务器响应返回数据。

每次请求都有头部和实体部分，先看下面笔者监听QQ空间的headers，QQ空间的原因是它头部内容比较全

- Request Headers：  
- 
- GET http://user.qzone.qq.com/445235728 HTTP/1.1  
- 
- Host: user.qzone.qq.com  
- 
- Connection: keep-alive  
- 
- Cache-Control: max-age=0  
- 
- User-Agent: Mozilla/5.0 (Windows NT 6.1) AppleWebKit/535.11 (KHTML, like Gecko) Chrome/17.0.963.56 Safari/535.11  
- 
- Accept:text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8  
- 
- Referer: http://qzone.qq.com/  
- 
- Accept-Encoding:gzip,deflate,sdch  
- 
- Accept-Language: zh-CN,zh;q=0.8  
- 
- Accept-Charset: GBK,utf-8;q=0.7,*;q=0.3  
- 
- Cookie:o_cookie=445235728;(省略很多……)  
- 
- If-Modified-Since: Wed, 13 Jun 2012 01:32:19 GMT  
- 
- -----------------  
- 
- Response Headers：  
- 
- HTTP/1.1 200 OK  
- 
- Connection:close  
- 
- Server: QZHTTP-2.34.0  
- 
- Date: Wed, 13 Jun 2012 02:59:31 GMT  
- 
- Content-Encoding: gzip  
- 
- Set-Cookie:login_time=61F0EEA02D704B1DBCF25166A74941B24F4BE24B205C466F;PATH=/; DOMAIN=qzone.qq.com  
- 
- Set-Cookie:Loading=Yes;expires=Wed,13-Jun-201216:00:00GMT;PATH=/;DOMAIN=qzone.qq.com X-UA-Compatible: IE=Edge Last-Modified: Wed, 13 Jun 2012 02:59:31 GMT  
- 
- Cache-Control: max-age=0, no-transform  
- 
- Content-Type: text/html;charset=utf-8  
- 
- Transfer-Encoding: chunked  




客户端向服务端发请求headers和服务端响应客户端headers图：

![](http://www.phpben.com/uploadfile/13396553720.06804400.jpg)

通过图片可以看出：

1、   客户端请求headers包含了请求行和一些头域。

请求行：请求的方法 统一资源标识器（URL）协议版本 ------这三者用空格分开，最后换行回车(\r\n) 例如：GET http://user.qzone.qq.com/445235728 HTTP/1.1

各种头域：这些头域都是有关键字和键值成对组合，最后换行回车(\r\n)结束，这些头域告诉服务器应该怎么去响应以及本身一些信息。

2、   服务器响应 

状态行：协议版本 响应状态 状态描述 ------这三者用空格分开，最后换行回车(\r\n) 例如：HTTP/1.1 200 OK

各种头域：这些头域也是有关键字和键值成对组合，最后换行回车(\r\n)结束，这些头域告诉客户端应该怎么去响应以及本身一些信息。



**NO2**

这里就不一一说每个头域的概念和作用，想了解的请看：[http://www.phpben.com/?post=34](http://www.phpben.com/?post=34) 现在介绍几个认为重要、在一些网站上的测试数据、以及请求返回各头域php代码实现

测试时间：2012.6.14前

测试对象：csdn 、cnbeta 、cnblos、腾讯（QQ空间、朋友网、新闻网）、新浪（微博、主页）、人人网、百度、淘宝、优酷、土豆这些网站

(1)     Connection头域：这个头域只有http/1.1才有，默认是keep-alive值表示长连接，这样的话就不用每请求一个资源比如图片，css文件，js文件都要和服务器进行3此握手连接，这个在一定程度上弥补了http没状态的一个缺陷，减少连接服务器的时间。

查看测试网站Connection头域发现腾讯QQ空间、腾讯新闻网、新浪主页和微博，优酷和土豆Connection：close；除了这些其他的都是Connection：keep-alive

**为什么?**

1、connection: keep-alive 能正常使用的一个前提条件是还要提供content-length的头域告诉客户端正文的长度。那计算正文长度是个问题，对于多内容，集群服务器来说不是件易事。腾讯和新浪，优酷的这些都很难计算，对与工程师来说之间关闭了（默认是打开的）。

2、老服务器端不支持，对于腾讯，新浪这些老油条，服务器集群很庞大，难免有些老旧的不支持长连接的，为了一些兼容性问题，直接关闭了

Ps：这两点原因未求证过！^-^

用php headers(“Connection:keep-alive”);

(2)     Content-Encoding头域

Content-Encoding文档的编码（Encode）方法.

上述网站出了cnbeta不用gzip压缩，优酷用deflate，其余都是。这也透漏一个重要信息，那就phper要掌握压缩gzip传输技术。

Php可以通过mod_gzip模块来实现。代码：ob_start("ob_gzhandler");

(3)     Server头域暴漏服务器重要的安全信息。

Csdn：Server:nginx/0.7.68  ------------版本都暴露

腾讯QQ空间：Server:QZHTTP-2.34.0--------某位tx朋友透漏这是内部自己开发的服务器，这个可够安全

新浪微博：Server:apache -------------这个没暴漏版本

凤凰网：Server: nginx/0.8.53

人人网：Server:nginx/1.2.0

淘宝网：Tengine ---------这是淘宝内部技术团队开发的web服务器，基于Nginx

cnblogs博客园：Server:Microsoft-IIS/7.5

腾讯朋友网：Tencent/PWS ---------腾讯内部开发

腾讯新闻网：Server:squid/3.1.18

优酷网：Server: fswww1-----------是不是内部就不清楚，至少笔者不知道什么来的^_^

土豆网：Tengine/1.2.3

百度：server: BWS/1.0 ---------应该也是百度内部自己开发的服务器

很明显Server头域是返回服务器的信息，但也可以说暴漏信息，面对这个问题，大公司就自己开发基于自己功能的内部服务器。

(4)     X-Powered-By头域可供修改，基于安全则可以修改

X-Powered-By头域反应什么语言什么版本执行后台程序。这个可以同个header函数修改

header("X-Powered-By:acb");

(5)     Cache-control、expires、last-modified等重要头域

Cache-control:指定请求和响应遵循的缓存机制。在请求消息或响应消息中设置Cache-Control并不会修改另一个消息处理过程中的缓存处理过程。请求时的缓存指令包括no-cache、no-store、max-age、max-stale、min-fresh、only-if-cached，响应消息中的指令包括public、private、no-cache、no-store、no-transform、must-revalidate、proxy-revalidate、max-age。

Php代码实现：header("cache-control: abc");abc是上述指令值一个或多个，多个用’,’分开

Expires:告诉浏览器指明应该在什么时候认为文档已经过期，从而不再缓存它。代码实现：header("Expires:". date('D, d M Y H:i:s \G\M\T', time()+10));--------这个是把时间截转化成格林时区字符串给expires头域，这个显示时间会比中国北京时间少8个小时，东8区的实现：header("Expires:". date('r', time()+10))

last-modified：这个是服务器返回给浏览器，浏览器下次请求则把该值赋给if-modified-since头域传给服务器，服务器就可以根据此值判断是否有改变，有则继续运行下去，否者返回304 not modified。Php设置expires头域一样。

代码：

- <?php 
- if(isset($_SERVER['HTTP_IF_MODIFIED_SINCE']) && (time()-strtotime($_SERVER['HTTP_IF_MODIFIED_SINCE']) < 10)) {  
- 
-   header("HTTP/1.1 304 Not Modified");  
- 
-      exit;  
- 
-    }  
- 
- header("Last-Modified: " . date('D, d M Y H:i:s \G\M\T', time()) );或者header("Last-Modified: " . date('r', time()) );
  
- ?> 




前者是格林时间格式，后者是中国时间。需要注意的就是php.ini 的时区prc则用后则，否者前者。笔者曾经试过在时区是prc的情况下用了前者，导致time()-strtotime($_SERVER['HTTP_IF_MODIFIED_SINCE']) <0永远成立，因为是负值。

**注意：**当请求页面有session_start()的时候，则不管是否有expires、cache-control、last-modified设置，则返回给客户端Cache-Control头域为Cache-Control:no-store, no-cache, must-revalidate Expires头域 Expires:Thu, 19 Nov 1981 08:52:00 GMT。这个问题烦了笔者2天，都以为php.ini 或是apache的问题。最后竟然是session_start()的问题。

**二、             浏览器缓存动态**

前面介绍了http headers几个告诉浏览器如何处理缓存。但不同浏览器处理各种头域的方式不同，以下就是笔者。

Ps：各个浏览器监听http headers的方法可以查看：[http://www.phpben.com/?post=76](http://www.phpben.com/?post=76)

(1)   header(“cache-control: no-store”)
||IE9|Google17.0|`Firefox11`|Maxthon3|
|----|----|----|----|----|
|点击刷新键|重发请求，返回200状态|重发请求，返回200状态|重发请求，返回200状态|重发请求，返回200状态|
|地址栏回车|重发请求，返回200状态|重发请求，返回200状态|重发请求，返回200状态|重发请求，返回200状态|
|点击后退键|同上|同上|同上|同上|



(2)   header(“cache-control: no-cache”)
||IE9|Google17.0|`Firefox11`|Maxthon3|
|----|----|----|----|----|
|点击刷新键|重发请求，返回200状态|重发请求，返回200状态|重发请求，返回200状态|重发请求，返回200状态|
|地址栏回车|重发请求，返回200状态|重发请求，返回200状态|重发请求，返回200状态|重发请求，返回200状态|
|点击后退键|同上|From cache|From cache|同上|



(3)   header(“cache-control:bublic”)
||IE9|Google17.0|`Firefox11`|Maxthon3|
|----|----|----|----|----|
|点击刷新键|重发请求，返回200状态|重发请求，返回200状态|重发请求，返回200状态|重发请求，返回200状态|
|地址栏回车|from cache|重发请求，返回200状态|重发请求，返回200状态|重发请求，返回200状态|
|点击后退键|From cache|From cache|From cache|同上|

(4)   header("cache-control:private"); header("cache-control: must-revalidate ")
||IE9|Google17.0|`Firefox11`|Maxthon3|
|----|----|----|----|----|
|点击刷新键|重发请求，返回200状态|重发请求，返回200状态|重发请求，返回200状态|重发请求，返回200状态|
|地址栏回车|除第一次外都是from cache|重发请求，返回200状态|重发请求，返回200状态|重发请求，返回200状态|
|点击后退键|From cache|From cache|From cache|同上|

(5)   header("cache-control:max-age=num");num是秒数
||IE9|Google17.0|`Firefox11`|Maxthon3|
|----|----|----|----|----|
|点击刷新键|重发请求，返回200状态|重发请求，返回200状态|重发请求，返回200状态|重发请求，返回200状态|
|地址栏回车|秒数<num from cache|重发请求，返回200状态|重发请求，返回200状态|重发请求，返回200状态|
|点击后退键|From cache|From cache|From cache|同上|

(6)   header("Expires:". date('D, d M Y H:i:s \G\M\T', time()+num)); num是秒数
||IE9|Google17.0|`Firefox11`|Maxthon3|
|----|----|----|----|----|
|点击刷新键|重发请求，返回200状态|重发请求，返回200状态|重发请求，返回200状态|重发请求，返回200状态|
|地址栏回车|秒数<num from cache|重发请求，返回200状态|重发请求，返回200状态|重发请求，返回200状态|
|点击后退键|From cache|From cache|From cache|同上|

(7)   if(isset($_SERVER['HTTP_IF_MODIFIED_SINCE']) && (time()-strtotime($_SERVER['HTTP_IF_MODIFIED_SINCE']) < **num**)) {

  header("HTTP/1.1 304 Not Modified");

     exit;

   } header("Last-Modified: " . date('D, d M Y H:i:s \G\M\T', time()) );
||IE9|Google17.0|`Firefox11`|Maxthon3|
|----|----|----|----|----|
|点击刷新键|秒数<num 304 not modified|秒数<num 304 not modified|秒数<num 304 not modified|重发请求，返回200状态|
|地址栏回车|from cache|秒数<num 304 not modified|秒数<num 304 not modified|重发请求，返回200状态|
|点击后退键|From cache|From cache|From cache|同上|



结论：

1、   刷新对于任何浏览器且不管是什么cache-control，都会重新请求，一般返回是200，除非Last-Modified设置

2、   后退键除非no-cache; no-store外都是使用缓存

3、   Cache-control:no-store 在浏览器中任何操作都重新提交请求，包括后退

4、   遨游3的缓存很差

5、   IE9 的缓存很强，所以用ie9调试的时候尽可能点刷新而不是在地址栏回车



鉴于这种情况，对于不同的应用（有些要缓存，有些经常更新）对于不同的国家各种浏览器份额，而哪种缓存方式。中国IE比较多，加上360浏览器的加入（用IE内核），那就要主要参照IE浏览器。

但笔者还是比较喜欢header("Last-Modified: " . date('D, d M Y H:i:s \G\M\T', time()) );这种方式。结合起来connection:keep-alive能让缓存技术更成熟。

**注意**：

1、   也许你会问，用Cache-control:no-store或Cache-control:no-store，但调试页面还是没原来的缓存。然后清除浏览器缓存关掉重启浏览器， 缓存还在。这是因为你的web应用用了文件缓存如ecshop常出现这种情况，这种情况就要进web应用后台删除文件缓存。

2、   调试的时候尽可能不要在地址栏回车，特别是IE，google还好一点，但是要知道这次的测试只是各个浏览器中的一个版本，所以调试的时候尽可能点刷新按钮。

3、   但在cache-control:max-age=num 和expires 一起使用的时候，前者级别比较高，浏览器会忽略expires的设置。（上面没给出测试内容）

**三、             Session和cookies**

Session 、cookies是程序员永远讨论的话题之一。

**1、   简单说一下cookies、session**

(1)     Cookies是保存在客户端的小段文本，随客户端点每一个请求发送该url下的所有cookies到服务器端。比如在谷歌浏览器下，打开ww.abc.com下的两个文件，a.php包含cookies1和cookies2，b.php包含了cookies3和cookies4，那么在a.php或b.php 点任意一个连接（当然是ww.abc.com服务器上的），浏览器就会把cookies1~4这4个cookies发送给服务器。但是如果在IE9有打开一个包含cookies5的c.php，哪门在google浏览器点击连接是不会发送cookies5的。

(2)     Session则保存服务器段，通过唯一的值sessionID来区别每一个用户。SessionID随每个连接请求发送到服务器，服务器根据sessionID来识别客户端，再通过session 的key获取session值。SessionID传回服务器的实现方式可以通过cookies和url回写来实现。

**注意**：

1、   同一个浏览器打开同一个文件，如a.php ，或同时有设置session的两个文件a.php、b.php sessionID则只有一个。（时间上不能是打开a.php 关闭浏览器再打开b.php）

2、   不同浏览器在同一时间打开同意文件的sessionID也不一样

3、   sessionID是服务器生成的不规则唯一字符串，如：

PHPSESSID=05dbfffd3453b7be02898fdca4fcd82b;------ PHPSESSID可以通过php.ini中session.name来改变，所以笔者在监听一些大型网站的时候查不出PHPSESSID，这是一个安全因素。

(3)     cookies、session在php中的主要相关参数

(1)     session.save_handler = ”files”

默认以文件方式存取session数据，如果想要使用自定义的处理器来存取session数据，比如数据库，用”user”。

(2)     session.use_cookies = 1 前面说到sessionID用cookies来实现，这里就是，1表示用cookies

(3)     session.use_trans_sid = 0 上面是用cookies来实现sessionID，这里值若是1则使用url回写方式，级别比session.use_cookies高

(4)     session.use_only_cookies = 0 值为1则sessionID只可以用cookies实现，级别比前两个高

(5)     session.cache_expire =180  session 缓存过期的秒数

(6)     session.gc_maxlifetime = 1440

设定保存的session文件生存期，超过此参数设定秒数后，保存的数据将被视为’垃圾’并由垃圾回收程序清理。判断标准是最后访问数据的时间(对于FAT文件系统是最后刷新数据的时间)。如果多个脚本共享同一个session.save_path目录但session.gc_maxlifetime不同，将以所有session.gc_maxlifetime指令中的最小值为准。

(4)     图说cookie 、ssession

php代码如下

- session_start();  
- 
- $_SESSION['favcolor'] = 'green';  
- 
- $_SESSION['animal']   = 'cat';  
- 
- $_SESSION['time']     = time();  
- 
- setcookie("cookie1","www.phpben.com",time()+3600*10);  
- 
- setcookie("cookie2","www.phpben.com",time()+3600*10);  




图片：

![](http://www.phpben.com/uploadfile/13396554200.39043500.jpg)

结论：

1、 第一次请求是没用cookies的，而第二次有PHPSESSID和两个cookies是因为服务器第一请求返回这个三个cookies。

2、第二次请求比第一次多返回PHPSESSID这个cookies，在第二次则没有了，直到session过期后重新设置。

**2、   **;**浏览器关掉cookies，session是否可以正常运行？**

前面提及sessionID的时候有两种方式。

(1)     cookies 方式，在session.use_trans_sid=0 and session.use_cookies = 1的情况下使用。这种方法是每次浏览器端点每个请求，都把sessionID发送到服务器。

(2)     url回写，session.use_only_cookies = 0 and session.use_trans_sid=1的情况下，服务器会忽略session.use_trans_sid，在浏览器发hhtp请求后，服务器会在返回页面内容中每个连接后面加上PHPSESSID=05dbfffd3453b7be02898fdca4fcd82b （在php.ini没改session.name，默认是PHPSESSID），这样就算客户端的浏览器禁止了cookies，一样能实现session功能。

这里来个测试：

在1.php文件代码：

- <?php  
- 
- echo'Welcome to page #1<br/>';  
- 
- session_start();  
- 
- $_SESSION['favcolor'] = 'green';  
- 
- $_SESSION['animal']   = 'cat';  
- 
- $_SESSION['time']     = time();  
- 
- 
- 
- // Works if session cookie was accepted
- 
- echo'<br /><a href="2.php">page 1 (这个href中没SID参数)</a><br/>';  
- 
- 
- 
- // Or maybe pass along the session id, if needed
- 
- echo'<br /><a href="2.php?' . SID . '">page 2 (这个href中有SID参数)</a><br/>';  
- 
- ?>  




在2.php文件代码：

- <?php  
- 
- session_start();  
- 
- 
- 
- echo'Welcome to page #2<br />';  
- 
- 
- 
- echo $_SESSION['favcolor'],'<br/>'; // green
- 
- echo $_SESSION['animal'],'<br/>';   // cat
- 
- echo date('Y m d H:i:s', $_SESSION['time']),'<br/>';  
- 
- 
- 
- // You may want to use SID here, like we did in page1.php
- 
- echo'<br /><a href="1.php">return page 1</a>';  
- 
- ?>  


情景1：没禁用浏览器的cookies（用cookies实现session），则在2.php能正常输出

![](http://www.phpben.com/uploadfile/13396554610.45636600.jpg)

情景2：禁用用浏览器的cookies且在php.ini开启session.use_trans_sid=1，通过1.php第一连接过去显示不了session的值，但第二个连接则正常显示。（说明url回写正常运行）

![](http://www.phpben.com/uploadfile/13396555160.13661700.jpg)



