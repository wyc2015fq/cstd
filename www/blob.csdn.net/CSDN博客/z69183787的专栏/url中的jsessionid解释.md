# url中的jsessionid解释 - z69183787的专栏 - CSDN博客
2013年12月18日 20:00:09[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2181
（1） 
这是一个保险措施 
因为Session默认是需要Cookie支持的 
但有些客户浏览器是关闭Cookie的 
这个时候就需要在URL中指定服务器上的session标识,也就是5F4771183629C9834F8382E23BE13C4C 
用一个方法(忘了方法的名字)处理URL串就可以得到这个东西 
这个方法会判断你的浏览器是否开启了Cookie,如果他认为应该加他就会加上去 
（2） 
链接1：wapbrowse.jsp?curAlbumID=9 ； 
链接2:wapbrowse.jsp;jsessionid=5AC6268DD8D4D5D1FDF5D41E9F2FD960?curAlbumID=9； 
这两个链接是从模拟器运行时生成的source中拷贝过来的，两个链接都是指向wapbrowse.jsp,链接1由于不包含jsessionid,所以在wapbrowse.jsp中变量为null,通过链接2打开wapbrowse.jsp可以正常访问session 变量 
（3） 
URL重写功能,为了防止一些用户把Cookie禁止而无法使用session而设置的功能.jsessionid后面的一长串就是你服务器上的session的ID号,这样无需cookie也可以使用session. 
（4） 
http本身是无session的，无法跟踪客户端的信息，换句话说：http协议不管是谁联接自己。 
为了实现session，必须有浏览器支持。浏览器可以用cookie存储session，这是最通用的做法。 
但是，如果我自己写一个完全符合http协议的浏览器，但是不配合服务器的session要求，那么服务器就无法产生session。 
好在现在的浏览器都支持session要求，即使关闭了cookie，浏览器也会向服务器传递sessionid，这个id是存储在浏览器的内存空间中的，不保存在硬盘cookie中。 
（5） 
sessionid是作为一个临时cookie放在浏览器端的。 
session的具体信息放在服务器端。 
每次浏览器发出的请求，都会在http header里 带上 sessionid来标识自己。 
既然用Struts，顺便再把JSTL用上， 
下面一个非常有用的标签： 
清单 12. 操作的语法 
    var="name" scope="scope"> 
  ... 
URL 重写是由 操作自动执行的。如果 JSP 容器检测到一个存储用户当前会话标识的 cookie，那么就不必进行重写。但是，如果不存在这样的 cookie，那么 生成的所有 URL 都会被重写以编码会话标识。注：如果在随后的请求中存在适当的 cookie，那么 将停止重写 URL 以包含该标识。 
参考：http://www-900.ibm.com/developerWorks/cn/java/j-jstl0318/index.shtml 
（6） 
方法一：url中紧跟servlet/jsp文件名加;jsessionid=sessionId，其中sessionId由HttpSession.getId()得到，如http://localhost:8080/aaa/bbb.jsp;jsessionid=saldjfsdflsaeir234?para=1¶2=2 
方法二：在application(ServletContext)里保存一个session管理器HashMap:sessionId---sessionRef，这样可以在所有的servlet/jsp里调用，这需要在url里将sessionId以参数形式传递，如http://localhost:8080/aaa/bbb.jsp?sessionId=saldjfsdflsaeir234?para=1¶2=2，在服务器端用request.getParameter("sessionId")获取 
（7） 
session是在服务器端保存。服务器根据url请求中的session_id来查找对应的session。 
以一个bbs为例，网站需要根据每个请求url获取用户的信息，如果以cookie方式，用户信息全部是存放在cookie中的，这样可能会不安全；如果以session方式，用户信息可以存放在服务器端，服务器只要从http请求中得到session_id，就可以得到存放在session中的用户信息了，这样安全性比较高。session在服务器中的表现方式依服务器而定，可能是写到临时文件中，也可能直接放在内存中。 
服务器从http请求中得到session_id的方式有两种：cookie和url重写。如果客户端启用cookie，那么session_id可以保存在cookie中；如果禁用cookie，就用url重写方式，在url中添加.jsessionid=xxxxx参数部分，服务器会试图从url中得到.jsessionid参数作为session_id. 
（8） 
cookie 是保存在客户端的文本格式数据，session是客户端登录到应用，由服务器为该客户端建立的唯一的标识，可以在session里面保存该客户端的数据比如说用户帐号。 
一般cookie可以用来保存你的登录帐号和密码，在你登录到应用服务上，自动添加到登录界面或直接发送到服务器上进行登录，这就是你经常能在论坛上看到的你的登录信息保存一年的选项 的实现方式 
（9） 
在http的报文格式里面cookie和session是在同一个包文位置上的 
如果ie发现包文里面包含cookie/session的信息的话，他会根据安全级别来决定是否保存相关信息，比如，如果安全机制允许使用cookie那么ie将把cookie的信息保存到临时文件里面，每次在请求服务器文件的时候会把收到的session的信息加入到请求的报文里面，这就是session保存信息的原理。如果安全机制不允许使用cookie的话，虽然ie收到了cookie和session的信息，那么cookie的信息不会被写入临时文件，当ie再次请求服务器文件的时候，也不会把收到的session的信息加入到请求报文里面，服务器就无法知道session的信息了。 
