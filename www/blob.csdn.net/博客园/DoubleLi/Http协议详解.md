# Http协议详解 - DoubleLi - 博客园






（一）初识HTTP消息头

但凡搞WEB开发的人都离不开HTTP（超文本传输协议），而要了解HTTP，除了HTML本身以外，还有一部分不可忽视的就是HTTP消息头。
做过Socket编程的人都知道，当我们设计一个通信协议时，“消息头/消息体”的分割方式是很常用的，消息头告诉对方这个消息是干什么的，消息体告诉对方怎么干。HTTP传输的消息也是这样规定的，每一个HTTP包都分为HTTP头和HTTP体两部分，后者是可选的，而前者是必须的。每当我们打开一个网页，在上面点击右键，选择“查看源文件”，这时看到的HTML代码就是HTTP的消息体，那么消息头又在哪呢？IE浏览器不让我们看到这部分，但我们可以通过截取数据包等方法看到它。
下面就来看一个简单的例子：
首先制作一个非常简单的网页，它的内容只有一行：
<html><body>hello world</body></html>
把它放到WEB服务器上，比如IIS，然后用IE浏览器请求这个页面（[http://localhost:8080/simple.htm](http://localhost:8080/simple.htm)），当我们请求这个页面时，浏览器实际做了以下四项工作：
1 解析我们输入的地址，从中分解出协议名、主机名、端口、对象路径等部分，对于我们的这个地址，解析得到的结果如下：
协议名：http
主机名：localhost
端口：8080
对象路径：/simple.htm
2 把以上部分结合本机自己的信息，封装成一个HTTP请求数据包
3 使用TCP协议连接到主机的指定端口（localhost, 8080），并发送已封装好的数据包
4 等待服务器返回数据，并解析返回数据，最后显示出来
由截取到的数据包我们不难发现浏览器生成的HTTP数据包的内容如下：
GET /simple.htm HTTP/1.1<CR>
Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, */*<CR>
Accept-Language: zh-cn<CR>
Accept-Encoding: gzip, deflate<CR>
User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 1.1.4322; .NET CLR 2.0.50727)<CR>
Host: localhost:8080<CR>
Connection: Keep-Alive<CR>
<CR>
为了显示清楚我把所有的回车的地方都加上了“<CR>”，注意最后还有一个空行加一个回车，这个空行正是HTTP规定的消息头和消息体的分界线，第一个空行以下的内容就是消息体，这个请求数据包是没有消息体的。
消息的第一行“GET”表示我们所使用的HTTP动作，其他可能的还有“POST”等，GET的消息没有消息体，而POST消息是有消息体的，消息体的内容就是要POST的数据。后面/simple.htm就是我们要请求的对象，之后HTTP1.1表示使用的是HTTP1.1协议。
第二行表示我们所用的浏览器能接受的Content-type，三四两行则是语言和编码信息，第五行显示出本机的相关系信息，包括浏览器类型、操作系统信息等，很多网站可以显示出你所使用的浏览器和操作系统版本，就是因为可以从这里获取到这些信息。
第六行表示我们所请求的主机和端口，第七行表示使用Keep-Alive方式，即数据传递完并不立即关闭连接。
服务器接收到这样的数据包以后会根据其内容做相应的处理，例如查找有没有“/simple.htm”这个对象，如果有，根据服务器的设置来决定如何处理，如果是HTM，则不需要什么复杂的处理，直接返回其内容即可。但在直接返回之前，还需要加上HTTP消息头。
服务器发回的完整HTTP消息如下：
HTTP/1.1 200 OK<CR>
Server: Microsoft-IIS/5.1<CR>
X-Powered-By: ASP.NET<CR>
Date: Fri, 03 Mar 2006 06:34:03 GMT<CR>
Content-Type: text/html<CR>
Accept-Ranges: bytes<CR>
Last-Modified: Fri, 03 Mar 2006 06:33:18 GMT<CR>
ETag: "5ca4f75b8c3ec61:9ee"<CR>
Content-Length: 37<CR>
<CR>
<html><body>hello world</body></html>
同样，我用“<CR>”来表示回车。可以看到，这个消息也是用空行切分成消息头和消息体两部分，消息体的部分正是我们前面写好的HTML代码。
消息头第一行“HTTP/1.1”也是表示所使用的协议，后面的“200 OK”是HTTP返回代码，200就表示操作成功，还有其他常见的如404表示对象未找到，500表示服务器错误，403表示不能浏览目录等等。
第二行表示这个服务器使用的WEB服务器软件，这里是IIS 5.1。第三行是ASP.Net的一个附加提示，没什么实际用处。第四行是处理此请求的时间。第五行就是所返回的消息的content-type，浏览器会根据它来决定如何处理消息体里面的内容，例如这里是text/html，那么浏览器就会启用HTML解析器来处理它，如果是image/jpeg，那么就会使用JPEG的解码器来处理。
消息头最后一行“Content-Length”表示消息体的长度，从空行以后的内容算起，以字节为单位，浏览器接收到它所指定的字节数的内容以后就会认为这个消息已经被完整接收了。




理解HTTP消息头 （二）
常见的HTTP返回码
上一篇文章里我简要的说了说HTTP消息头的格式，注意到在服务器返回的HTTP消息头里有一个“HTTP/1.1 200 OK”，这里的200是HTTP规定的返回代码，表示请求已经被正常处理完成。浏览器通过这个返回代码就可以知道服务器对所发请求的处理情况是什么，每一种返回代码都有自己的含义。这里列举几种常见的返回码。
1 403 Access Forbidden
如果我们试图请求服务器上一个文件夹，而在WEB服务器上这个文件夹并没有允许对这个文件夹列目录的话，就会返回这个代码。一个完整的403回复可能是这样的：（IIS5.1）
HTTP/1.1 403 Access Forbidden
Server: Microsoft-IIS/5.1
Date: Mon, 06 Mar 2006 08:57:39 GMT
Connection: close
Content-Type: text/html
Content-Length: 172

<html><head><title>Directory Listing Denied</title></head>
<body><h1>Directory Listing Denied</h1>This Virtual Directory does not allow contents to be listed.</body></html>
2 404 Object not found
当我们请求的对象在服务器上并不存在时，就会给出这个返回代码，这可能也是最常见的错误代码了。IIS给出的404消息内容很长，除了消息头以外还有一个完整的说明“为什么会这样”的网页。APACHE服务器的404消息比较简短，如下：
HTTP/1.1 404 Not Found
Date: Mon, 06 Mar 2006 09:03:14 GMT
Server: Apache/2.0.55 (Unix) PHP/5.0.5
Content-Length: 291
Keep-Alive: timeout=15, max=100
Connection: Keep-Alive
Content-Type: text/html; charset=iso-8859-1

<!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML 2.0//EN">
<html><head>
<title>404 Not Found</title>
</head><body>
<h1>Not Found</h1>
<p>The requested URL /notexist was not found on this server.</p>
<hr>
<address>Apache/2.0.55 (Unix) PHP/5.0.5 Server at localhost Port 8080</address>
</body></html>
也许你会问，无论是404还是200，都会在消息体内给出一个说明网页，那么对于客户端来说二者有什么区别呢？一个比较明显的区别在于200是成功请求，浏览器会记录下这个地址，以便下次再访问时可以自动提示该地址，而404是失败请求，浏览器只会显示出返回的页面内容，并不会记录此地址，要再次访问时还需要输入完整的地址。
3 401 Access Denied
当WEB服务器不允许匿名访问，而我们又没有提供正确的用户名/密码时，服务器就会给出这个返回代码。在IIS中，设置IIS的安全属性为不允许匿名访问（如下图），此时直接访问的话就会得到以下返回结果：

HTTP/1.1 401 Access Denied
Server: Microsoft-IIS/5.1
Date: Mon, 06 Mar 2006 09:15:55 GMT
WWW-Authenticate: Negotiate
WWW-Authenticate: NTLM
Connection: close
Content-Length: 3964
Content-Type: text/html

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<html dir=ltr>
……
此时浏览器上给出的提示如下图，让我们输入用户名和密码：

因返回信息中消息体较长，只取前面两行内容。注意，如果是用localhost来访问本机的IIS，因IE可以直接取得当前用户的身份，它会和服务器间直接进行协商，所以不会看到401提示。
当我们在输入了用户名和密码以后，服务器与客户端会再进行两次对话。首先客户端向服务器索取一个公钥，服务器端会返回一个公钥，二者都用BASE64编码，相应的消息如下（编码部分已经做了处理）：
GET / HTTP/1.1
Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, */*
Accept-Language: zh-cn
Accept-Encoding: gzip, deflate
User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 1.1.4322; .NET CLR 2.0.50727)
Host: 192.168.0.55:8080
Connection: Keep-Alive
Authorization: Negotiate ABCDEFG……

HTTP/1.1 401 Access Denied
Server: Microsoft-IIS/5.1
Date: Mon, 06 Mar 2006 09:20:53 GMT
WWW-Authenticate: Negotiate HIJKLMN……
Content-Length: 3715
Content-Type: text/html

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<html dir=ltr>
……
客户端拿到公钥之后使用公钥对用户名和密码进行加密码，然后把加密以后的结果重新发给服务器：
GET / HTTP/1.1
Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, */*
Accept-Language: zh-cn
Accept-Encoding: gzip, deflate
User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 1.1.4322; .NET CLR 2.0.50727)
Host: 192.168.0.55:8080
Connection: Keep-Alive
Authorization: Negotiate OPQRST……
这样，如果验证通过，服务器端就会把请求的内容发送过来了，也就是说禁止匿名访问的网站会经过三次请求才可以看到页面。但因为客户端浏览器已经缓存了公钥，用同一个浏览器窗口再次请求这个网站上的其它页面时就可以直接发送验证信息，从而一次交互就可以完成了。
4 302 Object Moved
用过ASP的人都知道ASP中页面重定向至少有Redirect和Transfer两种方法。二的区别在于Redirect是客户端重定向，而Transfer是服务器端重定向，那么它们具体是如何通过HTTP消息头实现的呢？
先来看一下Transfer的例子：
例如ASP文件1.asp只有一行
<% Server.Transfer "1.htm" %>
HTML文件1.htm也只有一行：
<p>this is 1.htm</p>
如果我们从浏览器里请求1.asp，发送的请求是：
GET /1.asp HTTP/1.1
Accept: */*
Accept-Language: zh-cn
Accept-Encoding: gzip, deflate
User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 1.1.4322; .NET CLR 2.0.50727)
Host: localhost:8080
Connection: Keep-Alive
Cookie: ASPSESSIONIDACCTRTTT=PKKDJOPBAKMAMBNANIPIFDAP
注意请求的文件确实是1.asp，而得到的回应则是：
HTTP/1.1 200 OK
Server: Microsoft-IIS/5.1
Date: Mon, 06 Mar 2006 12:52:44 GMT
X-Powered-By: ASP.NET
Content-Length: 20
Content-Type: text/html
Cache-control: private

<p>this is 1.htm</p>
不难看出，通过Server.Transfer语句服务器端已经做了页面重定向，而客户端对此一无所知，表面上看上去得到的就是1.asp的结果。
如果把1.asp的内容改为：
<% Response.Redirect "1.htm" %>
再次请求1.asp，发送的请求没有变化，得到的回应却变成了：
HTTP/1.1 302 Object moved
Server: Microsoft-IIS/5.1
Date: Mon, 06 Mar 2006 12:55:57 GMT
X-Powered-By: ASP.NET
Location: 1.htm
Content-Length: 121
Content-Type: text/html
Cache-control: private

<head><title>Object moved</title></head>
<body><h1>Object Moved</h1>This object may be found <a HREF="">here</a>.</body>
注意HTTP的返回代码由200变成了302，表示这是一个重定向消息，客户端需要根据消息头中Location字段的值重新发送请求，于是就有了下面一组对话：
GET /1.htm HTTP/1.1
Accept: */*
Accept-Language: zh-cn
Accept-Encoding: gzip, deflate
User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 1.1.4322; .NET CLR 2.0.50727)
Host: localhost:8080
Connection: Keep-Alive
If-Modified-Since: Thu, 02 Mar 2006 06:50:13 GMT
If-None-Match: "b224758ec53dc61:9f0"
Cookie: ASPSESSIONIDACCTRTTT=PKKDJOPBAKMAMBNANIPIFDAP

HTTP/1.1 200 OK
Server: Microsoft-IIS/5.1
X-Powered-By: ASP.NET
Date: Mon, 06 Mar 2006 12:55:57 GMT
Content-Type: text/html
Accept-Ranges: bytes
Last-Modified: Mon, 06 Mar 2006 12:52:32 GMT
ETag: "76d85bd51c41c61:9f0"
Content-Length: 20

<p>this is 1.htm</p>
很明显，两种重定向方式虽然看上去结果很像，但在实现原理上有很大的不同。
5 500 Internal Server Error
500号错误发生在服务器程序有错误的时候，例如，ASP程序为
<% if %>
显然这个程序并不完整，于是得到的结果为：
HTTP/1.1 500 Internal Server Error
Server: Microsoft-IIS/5.1
Date: Mon, 06 Mar 2006 12:58:55 GMT
X-Powered-By: ASP.NET
Content-Length: 4301
Content-Type: text/html
Expires: Mon, 06 Mar 2006 12:58:55 GMT
Set-Cookie: ASPSESSIONIDACCTRTTT=ALKDJOPBPPKNPCNOEPCNOOPD; path=/
Cache-control: private

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<html dir=ltr>
……
服务器发送了500号错误，并且后面通过HTML的方式说明了错误的原因。


理解HTTP消息头 （三）

（三） 客户端发送的内容
这一次主要来观察HTTP消息头中客户端的请求，从中找到一些有意思的内容。

1 HTTP_REFERER
写两个简单的网页：
a.htm：
<a href=b.htm>to page b</a>
b.htm：
haha
内容很简单，就是网页A中有一个到B的链接。把它们放到IIS上，并访问网页A，从中再点击到B的链接，于是看到了B页的“haha”。那么这两次请求有什么不同吗？观察它们所发送的HTTP消息头，最明显的区别就是访问B页时比访问A页时多了一行：
Referer: [http://localhost/a.htm](http://localhost/a.htm)
这一行就表示，用户要访问的B页是从A页链接过来的。
服务器端要想取得这个值也是很容易的，以ASP为例，只需要写一句
<% =Request.ServerVariables("HTTP_REFERER") %>
就可以了。
一些网站通过HTTP_REFERER来做安全验证，判断用户是不是从允许的页面链接来的，而不是直接从浏览器上打URL或从其他页面链接过来，这样可以从一定程度上防止网页被做非法使用。但从上述原理来看，想要骗过服务器也并不困难，只要手工构造输入的HTTP消息头就可以了，其他常用的手段还有通过HOSTS文件伪造域名等。
除了超链接以外，还有其他几种方式会导致HTTP_REFERER信息被发送，如：
内联框架：<iframe src=b.asp></iframe>
框架集：<frameset><frame src=b.asp></frameset>
表单提交：<form action=b.asp><input type=submit></form>
SCRIPT引用：<script src=b.asp></script>
CSS引用：<link rel=stylesheet type=text/css href=b.asp>
XML数据岛：<xml src=b.asp></xml>
而以下形式不会发送HTTP_REFERER：
script转向：<script>location.href="b.asp"</script>
script开新窗口：<script>window.open("b.asp");</script>
META转向：<meta http-equiv="refresh" content="0;URL=b.asp">
引入图片：<img src=b.asp>

2 COOKIE
COOKIE是大家都非常熟悉的了，通过它可以在客户端保存用户状态，即使用户关闭浏览器也能继续保存。那么客户端与服务器端是如何交换COOKIE信息的呢？没错，也是通过HTTP消息头。
首先写一个简单的ASP网页：
<%
Dim i
i =  Request.Cookies("key")
Response.Write i
Response.Cookies("key") = "haha"
Response.Cookies("key").Expires = #2007-1-1#
%>
第一次访问此网页时，屏幕上一片白，第二次访问时，则会显示出“haha”。通过阅读程序不难发现，屏幕上显示的内容实际上是COOKIE的内容，而第一次访问时还没有设置COOKIE的值，所以不会有显示，第二次显示的是第一次设置的值。那么对应的HTTP消息头应该是什么样的呢？
第一次请求时没什么不同，略过
第一次返回时消息内容多了下面这一行：
Set-Cookie: key=haha; expires=Sun, 31-Dec-2006 16:00:00 GMT; path=/
很明显，key=haha表示键名为“key”的COOKIE的值为“haha”，后面是这则COOKIE的过期时间，因为我用的中文操作系统的时区是东八区，2007年1月1日0点对应的GMT时间就是2006年12月31日16点。
第二次再访问此网页时，发送的内容多了如下一行：
Cookie: key=haha
它的内容就是刚才设的COOKIE的内容。可见，客户端在从服务器端得到COOKIE值以后就保存在硬盘上，再次访问时就会把它发送到服务器。发送时并没有发送过期时间，因为服务器对过期时间并不关心，当COOKIE过期后浏览器就不会再发送它了。
如果使用IE6.0浏览器并且禁用COOKIE功能，可以发现服务器端的set-cookie还是有的，但客户端并不会接受它，也不会发送它。有些网站，特别是在线投票网站通过记录COOKIE防止用户重复投票，破解很简单，只要用IE6浏览器并禁用COOKIE就可以了。也有的网站通过COOKIE值为某值来判断用户是否合法，这种判断也非常容易通过手工构造HTTP消息头来欺骗，当然用HOSTS的方式也是可以欺骗的。

3 SESSION
HTTP协议本身是无状态的，服务器和客户端都不保证用户访问期间连接会一直保持，事实上保持连接是HTTP1.1才有的新内容，当客户端发送的消息头中有“Connection: Keep-Alive”时表示客户端浏览器支持保持连接的工作方式，但这个连接也会在一段时间没有请求后自动断开，以节省服务器资源。为了在服务器端维持用户状态，SESSION就被发明出来了，现在各主流的动态网页制做工具都支持SESSION，但支持的方式不完全相同，以下皆以ASP为例。
当用户请求一个ASP网页时，在返回的HTTP消息头中会有一行：
Set-Cookie: ASPSESSIONIDCSQCRTBS=KOIPGIMBCOCBFMOBENDCAKDP; path=/
服务器通过COOKIE的方式告诉客户端你的SESSIONID是多少，在这里是“KOIPGIMBCOCBFMOBENDCAKDP”，并且服务器上保留了和此SESSIONID相关的数据，当同一用户再次发送请求时，还会把这个COOKIE再发送回去，服务器端根据此ID找到此用户的数据，也就实现了服务器端用户状态的保存。所以我们用ASP编程时可以使用“session("name")=user”这样的方式保存用户信息。注意此COOKIE内容里并没有过期时间，这表示这是一个当关闭浏览器时立即过期的COOKIE，它不会被保存到硬盘上。这种工作方式比单纯用COOKIE的方式要安全很多，因为在客户端并没有什么能让我们修改和欺骗的值，唯一的信息就是SESSIONID，而这个ID在浏览器关闭时会立即失效，除非别人能在你浏览网站期间或关闭浏览器后很短时间内知道此ID的值，才能做一些欺骗活动。因为服务器端判断SESSION过期的方式并不是断开连接或关闭浏览器，而是通过用户手工结束SESSION或等待超时，当用户关闭浏览器后的一段时间里SESSION还没有超时，所以这时如果知道了刚才的SESSIONID，还是可以欺骗的。因此最安全的办法还是在离开网站之前手工结束SESSION，很多网站都提供“Logout”功能，它会通过设置SESSION中的值为已退出状态或让SESSION立即过期从而起到安全的目的。
SESSION和COOKIE的方式各有优缺点。SESSION的优点是比较安全，不容易被欺骗，缺点是过期时间短，如果用过在超过过期时间里没有向服务器发送任何信息，就会被认为超过过期了；COOKIE则相反，根据服务器端设置的超时时间，可以长时间保留信息，即使关机再开机也可能保留状态，而安全性自然大打折扣。很多网站都提供两种验证方式相结合，如果用户临时用这台电脑访问此访问则需要输入用户名和密码，不保存COOKIE；如果用户使用的是自己的个人电脑，则可以让网站在自己硬盘上保留COOKIE，以后访问时就不需要重新输入用户名和密码了。

4 POST
浏览器访问服务器常用的方式有GET和POST两种，GET方式只发送HTTP消息头，没有消息体，也就是除了要GET的基本信息之外不向服务器提供其他信息，网页表单（FROM）的默认提交方式就是用GET方式，它会把所有向服务器提交的信息都作为URL后面的参数，如a.asp?a=1&b=2这样的方式。而当要提交的数据量很大，或者所提交内容不希望别人直接看到时，应该使用POST方式。POST方式提交的数据是作为HTTP消息体存在的，例如，写一个网页表单：
<form method=post>
<input type=text name=text1>
<input type=submit>
</form>
访问此网页，并在表单中填入一个“haha”，然后提交，可以看到此次提交所发送的信息如下：
POST /form.asp HTTP/1.1
Accept: */*
Referer: [http://localhost:8080/form.asp](http://localhost:8080/form.asp)
Accept-Language: zh-cn
Content-Type: application/x-www-form-urlencoded
Accept-Encoding: gzip, deflate
User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0; InfoPath.1; .NET CLR 1.1.4322; .NET CLR 2.0.50727)
Host: localhost:8080
Content-Length: 10
Connection: Keep-Alive
Cache-Control: no-cache
Cookie: key=haha; ASPSESSIONIDCSQCRTBS=LOIPGIMBLMNOGCOBOMPJBOKP
text1=haha
前面关键字从“GET”变为了“POST”，Content-Type变成了“application/x-www-form-urlencoded”，后面内容并无大变化，只是多了一行：Content-Length: 10，表示提交的内容的长度。空行后面是消息体，内容就是表单中所填的内容。注意此时发送的内容只是“Name=Value”的形式，表单上其他的信息不会被发送，所以想直接从服务器端取得list box中所有的list item是办不到的，除非在提交前用一段script把所有的item内容都连在一起放到一个隐含表单域中。
如果是用表单上传文件，情况就要复杂一些了，首先是表单声明中要加上一句话：enctype='multipart/form-data'，表示这个表单将提交多段数据，并用HTML：input type=file来声明一个文件提交域。
表单内容如下：
<form method=post enctype='multipart/form-data'>
<input type=text name=text1>
<input type=file name=file1>
<input type=submit>
</form>
我们为text1输入文字：hehe，为file1选择文件haha.txt，其内容为“ABCDEFG”，然后提交此表单。提交的完全信息为：
POST /form.asp HTTP/1.1
Accept: */*
Referer: [http://localhost:8080/form.asp](http://localhost:8080/form.asp)
Accept-Language: zh-cn
Content-Type: multipart/form-data; boundary=---------------------------7d62bf2f9066c
Accept-Encoding: gzip, deflate
User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0; InfoPath.1; .NET CLR 1.1.4322; .NET CLR 2.0.50727)
Host: localhost:8080
Content-Length: 337
Connection: Keep-Alive
Cache-Control: no-cache
Cookie: key=haha; ASPSESSIONIDCSQCRTBS=LOIPGIMBLMNOGCOBOMPJBOKP
-----------------------------7d62bf2f9066c
Content-Disposition: form-data; name="text1"
hehe
-----------------------------7d62bf2f9066c
Content-Disposition: form-data; name="file1"; filename="H:/Documents and Settings/Administrator/桌面/haha.txt"
Content-Type: text/plain
ABCDEFG
-----------------------------7d62bf2f9066c--

显然这个提交的信息要比前述的复杂很多。Content-Type变成了“multipart/form-data”，后面还多了一个boundary，此值是为了区分POST的内容的区段用的，只要在内容中遇到了此值，就表示下面要开始一个新的区段了，每个区段的内容相对独立。如果遇到的是此值后面连着两个减号，则表示全部内容到此结束。每个段也分为段头和段体两部分，用空行隔开，每段都有自己的类型和相关信息。如第一区段是text1的值，它的名称是“text1”，值为“hehe”。第二段是文件内容，段首里表明了此文件域的名称“file1”和此文件在用户磁盘上的位置，后面就是文件的内容。
如果我们想要自己写一个上传文件组件来接收HTML表单传送的文件数据，那么最核心的任务就是解析此数据包，从中取得需要的信息。



理解HTTP消息头 （四）
服务器返回的消息
服务器返回的HTTP消息也分为消息头和消息体两部分。前面连载的第二篇里已经介绍了返回消息中常见返回代码的含义。对于非正常的返回代码的处理比较简单，只要照着要求去做就好了，而对于正常的返回代码（200），其处理方式就多种多样了。

1 Content-Type
Content-Type是返回消息中非常重要的内容，它标识出这个返回内容的类型，其值为“主类型/子类型”的格式，例如最常见的就是text/html，它的意思是说返回的内容是文本类型，这个文本又是HTML格式的。原则上浏览器会根据Content-Type来决定如何显示返回的消息体内容。常见的内容类型有：
text/html HTML文本
image/jpeg JPG图片
image/gif GIF图片
application/xml XML文档
audio/x-mpegurl MP3文件列表，如果安装了Winamp，则可以直接把它当面M3U文件来打开
更多的内容类型可以在注册表“HKCR/MIME/Database/Content Type”下看到
对于IE6浏览器来说，如果Content-Type中的类型和实际的消息体类型不一致，那么它会根据内容中的类型来分析实际应该是什么类型，对于JPG、GIF等常用图片格式都可以正确的识别出来，而不管Content-Type中写的是什么。
如果Content-Type中指定的是浏览器可以直接打开的类型，那么浏览器就会直接打开其内容显示出来，如果是被关联到其它应用程序的类型，这时就要查找注册表中关于这种类型的注册情况，如果是允许直接打开而不需要询问的，就会直接调出这个关联的应用程序来打开这个文件，但如果是不允许直接打开的，就会询问是否打开。对于没有关联到任何应用程序的类型，IE浏览器不知道它该如何打开，此时IE6就会把它当成XML来尝试打开。
2 Content-Disposition
如果用AddHeader的方法在HTTP消息头中加入Content-Disposition段，并指定其值为“attachment”，那么无论这个文件是何类型，浏览器都会提示我们下载此文件，因为此时它认为后面的消息体是一个“附件”，不需要由浏览器来处理了。例如，在ASP.Net中写入如下语句：
Response.AddHeader("Content-Disposition: attachment");
请求此页面是得到的结果如：
HTTP/1.1 200 OK
Server: Microsoft-IIS/5.1
Date: Thu, 23 Mar 2006 07:54:53 GMT
Content-Disposition: attachment
Cache-Control: private
Content-Type: text/html; charset=utf-8
……
也就是说，通过AddHeader函数可以为HTTP消息头加入我们自定义的内容。使用这种方法可以强制让浏览器提示下载文件，即使这个文件是我们已知的类型，基于是HTML网页。如果想要让用户下载时提示一个默认的文件名，只需要在前面一句话后加上“filename=文件名”即可。例如：
Response.AddHeader("Content-Disposition: attachment; filename=mypage.htm");
3 Content-Type与Content-Disposition
如果把Content-Type和Content-Disposition结合在一起使用会怎么样呢？
打开一个网页时，浏览器会首先看是否有Content-Disposition: attachment这一项，如果有，无论Content-Type的值是什么，都会提示文件下载。
如果指定了filename，就会提示默认的文件名为此文件名。注意到在IE6中除了“保存”按扭外还有“打开”按扭，此时打开文件的类型是由在filename中指定的文件扩展名决定的，例如让filename=mypic.jpg，浏览器就会查找默认的图片查看器来打开此文件。
如果没有指定filename，那么浏览器就根据Content-Type中的类型来决定文件的类型，例如Content-Type类型为image/gif，那么就会去查找默认的看GIF图片的工具，并且设置此文件的名字为所请求的网页的主名（不带扩展名）加上对应于此文件类弄扩展名，例如请求的mypage.aspx，就会自动变成mypage.gif。如果并没有指定Content-Type值，那么就默认它为“text/html”，并且保存的文件名就是所请求的网页文件名。
但如果没有指定Content-Disposition，那么就和前面关于Content-Type中所讨论的情况是一样的了。

4 Cache
返回消息中的Cache用于指定网页缓存。我们经常可以看到这样的情况，打开一个网页时速度不快，但再次打开时就会快很多，原因是浏览器已经对此页面进行了缓存，那么在同一浏览器窗口中再次打开此页时不会重新从服务器端获取。网页的缓存是由HTTP消息头中的“Cache-control”来控制的，常见的取值有private、no-cache、max-age、must-revalidate等，默认为private。其作用根据不同的重新浏览方式分为以下几种情况：
（1） 打开新窗口
如果指定cache-control的值为private、no-cache、must-revalidate，那么打开新窗口访问时都会重新访问服务器。而如果指定了max-age值，那么在此值内的时间里就不会重新访问服务器，例如：
Cache-control: max-age=5
表示当访问此网页后的5秒内再次访问不会去服务器
（2） 在地址栏回车
如果值为private或must-revalidate（和网上说的不一样），则只有第一次访问时会访问服务器，以后就不再访问。如果值为no-cache，那么每次都会访问。如果值为max-age，则在过期之前不会重复访问。
（3） 按后退按扭
如果值为private、must-revalidate、max-age，则不会重访问，而如果为no-cache，则每次都重复访问
（4） 按刷新按扭
无论为何值，都会重复访问

当指定Cache-control值为“no-cache”时，访问此页面不会在Internet临时文章夹留下页面备份。
另外，通过指定“Expires”值也会影响到缓存。例如，指定Expires值为一个早已过去的时间，那么访问此网时若重复在地址栏按回车，那么每次都会重复访问：
Expires: Fri, 31 Dec 1999 16:00:00 GMT

在ASP中，可以通过Response对象的Expires、ExpiresAbsolute属性控制Expires值；通过Response对象的CacheControl属性控制Cache-control的值，例如：
Response.ExpiresAbsolute = #2000-1-1# ' 指定绝对的过期时间，这个时间用的是服务器当地时间，会被自动转换为GMT时间
Response.Expires = 20  ' 指定相对的过期时间，以分钟为单位，表示从当前时间起过多少分钟过期。
Response.CacheControl = "no-cache" 
Expires值是可以通过在Internet临时文件夹中查看临时文件的属性看到的，如：


HTTP请求：GET与POST方法的区别

 HTTP 定义了与服务器交互的不同方法，最基本的方法是 GET 和 POST。事实上 GET 适用于多数请求，而保留 POST 仅用于更新站点。根据 HTTP 规范，GET 用于信息获取，而且应该是 安全的和 幂等的。所谓安全的意味着该操作用于获取信息而非修改信息。换句话说，GET 请求一般不应产生副作用。幂等的意味着对同一 URL 的多个请求应该返回同样的结果。完整的定义并不像看起来那样严格。从根本上讲，其目标是当用户打开一个链接时，她可以确信从自身的角度来看没有改变资源。比如，新闻站点的头版不断更新。虽然第二次请求会返回不同的一批新闻，该操作仍然被认为是安全的和幂等的，因为它总是返回当前的新闻。反之亦然。POST 请求就不那么轻松了。POST 表示可能改变服务器上的资源的请求。仍然以新闻站点为例，读者对文章的注解应该通过 POST 请求实现，因为在注解提交之后站点已经不同了（比方说文章下面出现一条注解）；

在FORM提交的时候，如果不指定Method，则默认为GET请求，Form中提交的数据将会附加在url之后，以?分开与url分开。字母数字字符原样发送，但空格转换为“+“号，其它符号转换为%XX,其中XX为该符号以16进制表示的ASCII（或ISO Latin-1）值。GET请求请提交的数据放置在HTTP请求协议头中，而POST提交的数据则放在实体数据中；

GET方式提交的数据最多只能有1024字节，而POST则没有此限制。
HTTP 状态码
200 - 服务器成功返回网页 
404 - 请求的网页不存在 
503 - 服务器超时 
以下是 HTTP 状态码的完整列表。您也可以访问 HTTP 状态码上的 W3C 页以了解更多信息。

1xx 状态码

表示临时响应并需要请求者继续执行操作的状态码。

100（继续） 请求者应当继续提出请求。服务器返回此代码表示已收到请求的第一部分，正在等待其余部分。 
101（切换协议） 请求者已要求服务器切换协议，服务器已确认并准备切换。

2xx 状态码

表示成功处理了请求的状态码。

200（成功） 服务器已成功处理了请求。通常，这表示服务器提供了请求的网页。如果针对您的 robots.txt 文件显示此状态码，则表示 Googlebot 已成功检索到该文件。 
201（已创建） 请求成功并且服务器创建了新的资源。 
202（已接受） 服务器已接受请求，但尚未处理。 
203（非授权信息） 服务器已成功处理了请求，但返回的信息可能来自另一来源。 
204（无内容） 服务器成功处理了请求，但没有返回任何内容。 
205（重置内容） 服务器成功处理了请求，但没有返回任何内容。与 204 响应不同，此响应要求请求者重置文档视图（例如，清除表单内容以输入新内容）。 
206（部分内容） 服务器成功处理了部分 GET 请求。

3xx 状态码

要完成请求，需要进一步操作。通常，这些状态码用来重定向。建议您在每次请求中使用重定向不要超过 5 次。您可以使用网站管理员工具查看一下 Googlebot 在抓取重定向网页时是否遇到问题。诊断下的网络抓取页列出了由于重定向错误导致 Googlebot 无法抓取的网址。

300（多种选择） 针对请求，服务器可执行多种操作。服务器可根据请求者 (user-agent) 选择一项操作，或提供操作列表供请求者选择。 
301（永久移动） 请求的网页已永久移动到新位置。服务器返回此响应（对 GET 或 HEAD 请求的响应）时，会自动将请求者转到新位置。您应使用此代码告诉 Googlebot 某个网页或网站已永久移动到新位置。 
302（临时移动） 服务器目前从不同位置的网页响应请求，但申请人应当继续使用原有位置来响应以后的请求。此代码与响应 GET 和 HEAD 请求的 301 代码类似，会自动将请求者转到不同的位置，但不应使用此代码来告诉 Googlebot 页面或网站已经移动，因为 Googlebot 要继续抓取原来的位置并编制索引。 
303（查看其他位置） 请求者应当对不同的位置使用单独的 GET 请求来检索响应时，服务器返回此代码。对于除 HEAD 之外的所有请求，服务器会自动转到其他位置。 
304（未修改） 自从上次请求后，请求的网页未修改过。服务器返回此响应时，不会返回网页内容。

如果网页自请求者上次请求后再也没有更改过，您应当将服务器配置为返回此响应（称为 If-Modified-Since HTTP 标头）。由于服务器可以告诉 Googlebot 自从上次抓取后网页没有变更，因此可节省带宽和开销。

305（使用代理） 请求者只能使用代理访问请求的网页。如果服务器返回此响应，还表示请求者应当使用代理。 
307（临时重定向） 服务器目前从不同位置的网页响应请求，但请求者应当继续使用原有位置来响应以后的请求。此代码与响应 GET 和 HEAD 请求的 <a href=answer.py?answer=>301</a> 代码类似，会自动将请求者转到不同的位置，但您不应使用此代码来告诉 Googlebot 某个网页或网站已经移动，因为 Googlebot 会继续抓取原有位置并编制索引。

4xx 状态码

这些状态码表示请求可能出错，这妨碍了服务器的处理。

400（错误请求） 服务器不理解请求的语法。 
401（身份验证错误） 此页要求授权。您可能不希望将此网页纳入索引。如果您的 Sitemap 中列出该网页，您可以将其删除。但如果您将其保留在您的 Sitemap 中，我们就不会抓取或索引该网页（尽管该网页将继续保持错误状态在此处列出）。如果我们将其作为搜索抓取的一部分抓取，您可以在我们的网站管理员信息中查阅其原因。 
403（禁止） 服务器拒绝请求。如果您在 Googlebot 尝试抓取您网站上的有效网页时看到此状态码（可以在 Google 网站管理员工具<strong>诊断</strong>下的<strong>网络抓取< /strong>页面上看到此信息），可能是您的服务器或主机拒绝 Googlebot 访问。 
404（未找到） 服务器找不到请求的网页。例如，对于服务器上不存在的网页经常会返回此代码。

如果您的网站上没有 robots.txt 文件，而您在 Google 网站管理员工具"诊断"标签的 robots.txt 页上看到此状态码，那么这是正确的状态码。但是，如果您有 robots.txt 文件而又看到此状态码，则说明您的 robots.txt 文件可能命名错误或位于错误的位置（该文件应当位于顶级域，名为 robots.txt）。

如果对于 Googlebot 尝试抓取的网址看到此状态码（在"诊断"标签的 HTTP 错误页面上），则表示 Googlebot 追踪的可能是另一个页面的无效链接（是旧链接或输入有误的链接）。

405（方法禁用） 禁用请求中指定的方法。

406（不接受） 无法使用请求的内容特性响应请求的网页。 
407（需要代理授权） 此状态码与 401 类似，但指定请求者必须授权使用代理。如果服务器返回此响应，还表示请求者应当使用代理。 
408（请求超时） 服务器等候请求时发生超时。 
409（冲突） 服务器在完成请求时发生冲突。服务器必须在响应中包含有关冲突的信息。服务器在响应与前一个请求相冲突的 PUT 请求时可能会返回此代码，以及两个请求的差异列表。 
410（已删除） 请求的资源永久删除后，服务器返回此响应。该代码与 404（未找到）代码相似，但在资源以前存在而现在不存在的情况下，有时会用来替代 404 代码。如果资源已永久删除，您应当使用 301 指定资源的新位置。 
411（需要有效长度） 服务器不接受不含有效内容长度标头字段的请求。 
412（未满足前提条件） 服务器未满足请求者在请求中设置的其中一个前提条件。 
413（请求实体过大） 服务器无法处理请求，因为请求实体过大，超出服务器的处理能力。 
414（请求的 URI 过长） 请求的 URI（通常为网址）过长，服务器无法处理。 
415（不支持的媒体类型） 请求的格式不受请求页面的支持。 
416（请求范围不符合要求） 如果页面无法提供请求的范围，则服务器会返回此状态码。 
417（未满足期望值） 服务器未满足"期望"请求标头字段的要求。

5xx 状态码

这些状态码表示服务器在处理请求时发生内部错误。这些错误可能是服务器本身的错误，而不是请求出错。

500（服务器内部错误） 服务器遇到错误，无法完成请求。 
501（尚未实施） 服务器不具备完成请求的功能。例如，服务器无法识别请求方法时则会返回此代码。 
502（错误网关） 服务器作为网关或代理，从上游服务器收到无效响应。 
503（服务不可用） 服务器目前无法使用（由于超载或停机维护）。通常，这只是暂时状态。 
504（网关超时） 服务器作为网关或代理，但是没有及时从上游服务器收到请求。 
505（HTTP 版本不受支持） 服务器不支持请求中所用的 HTTP 协议版本。









