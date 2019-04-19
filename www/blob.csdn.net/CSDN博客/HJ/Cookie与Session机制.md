# Cookie与Session机制 - HJ - CSDN博客
2018年03月18日 19:18:01[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：110
**Cookie与Session机制**
**一、Cookie机制**
**1.Cookie机制简述**
        Cookie是通过在客户端记录信息确定用户身份。Cookie是一小段文本信息。客户端请求服务器，如果服务器需要记录该用户状态，就是用response向客户端颁发一个Cookie，客户端浏览器会把Cookie保存起来。当浏览器再请求该网站时，浏览器把请求的网址连同该Cookie一同提交给服务器，服务器检查该Cookie，以此来辨认用户状态。服务器还可以根据需要修改Cookie的内容；
**        注意，Cookie功能需要浏览器的支持。如果浏览器不支持Cookie（如大部分手机中的浏览器）或者把Cookie禁用了，Cookie功能就会失效；**
**2.记录用户访问次数**
        Java中把Cookie封装成了javax.servlet.http.Cookie类。每个Cookie都是该Cookie类的对象。服务器通过操作Cookie类对象对客户端Cookie进行操作。通过request.getCookie()获取客户端提交的所有Cookie（以Cookie[]数组形式返回），通过response.addCookie(Cookie cookie)向客户端设置Cookie；
        Cookie对象使用key-value属性对的形式保存用户状态，一个Cookie对象保存一个属性对，一个request或者response同时使用多个Cookie。因为Cookie类位于包javax.servlet.http.*下面，所以JSP中需要import该类；
**3.Cookie的有效期和修改删除**
        Cookie的maxAge决定着Cookie的有效期，单位为秒（Second）。Cookie中通过getMaxAge()方法与setMaxAge(int maxAge)方法来读写maxAge属性。     
        如果maxAge属性为**正数**，则表示该Cookie会在maxAge秒之后自动失效。浏览器会将maxAge为正数的**Cookie持久化**，即写到对应的Cookie文件中。无论客户关闭了浏览器还是电脑，只要在maxAge秒之前，登录网站时该Cookie仍然有效。
        如果maxAge为**负数**，则表示该Cookie仅在本浏览器窗口以及本窗口打开的子窗口内有效，关闭窗口后该Cookie即失效。maxAge为负数的Cookie，为临时性Cookie，不会被持久化，Cookie信息不会被写到Cookie文件中，只是保存在浏览器内存中，因此关闭浏览器该Cookie就消失了。Cookie默认的maxAge值为-1。
        如果maxAge为**0**，则表示删除该Cookie。Cookie机制中没有提供删除Cookie的方法，因此通过设置该Cookie即时失效实现删除Cookie的效果。失效的Cookie会被浏览器从Cookie文件或者内存中删除；
        response对象提供的Cookie操作方法只有一个添加操作add(Cookie cookie)。想要修改Cookie只能使用一个同名的Cookie来覆盖原来的Cookie，达到修改的目的。删除时只需要把maxAge修改为0即可。注意修改、删除Cookie时，新建的Cookie除value、maxAge之外的所有属性，例如name、path、domain等，都要与原Cookie完全一样。否则，浏览器将视为两个不同的Cookie不予覆盖，导致修改、删除失败；
        注意：从客户端读取Cookie时，包括maxAge在内的其他属性都是不可读的，也不会被提交。浏览器提交Cookie时只会提交name与value属性。maxAge属性只被浏览器用来判断Cookie是否过期；
**4.Cookie的域名和路径**
        正常情况下，同一个一级域名下的两个二级域名也不能交互使用Cookie，因为二者的域名并不严格相同，如果想同一个一级域名下的所有的二级域名都可以使用该Cookie，需要设置Cookie的domain参数。读者可以修改本机**C:\WINDOWS\system32\drivers\etc**下的hosts文件来配置多个临时域名，然后使用setCookie.jsp程序来设置跨域名Cookie验证domain属性。注意，domain参数必须以点(“.”)开始。
        另外，name相同但domain不同的两个Cookie是两个不相同的Cookie。如果想要两个域名完全不同的网站共有Cookie，可以生成两个Cookie，domain属性分别为两个域名，输出到客户端；
        domain属性决定运行访问Cookie的域名，而path属性决定允许访问Cookie的路径(ContextPath)。注意，页面只能获取它属于的Path的Cookie。例如/session/test/a.jsp不能获取到路径为/session/abc/的Cookie。使用时一定要注意；
**5.Cookie的安全属性**
        HTTP协议不仅是无状态的，而且是不安全的。使用HTTP协议的数据不经过任何加密就直接在网络上传播，有被截获的可能。使用和HTTP协议传输很机密的内容是一种隐患。如果不希望Cookie在HTTP等非安全协议中传输，可以设置Cookie的secure属性为true。浏览器只会在HTTPS和SSL等安全协议中传输此类Cookie。注意，secure属性并不能对Cookie内容加密，因而不能保证绝对的安全性。如果需要高安全性，需要在程序中对Cookie内容加密、解密，以防泄密；
**二、Session机制**
**1.Session机制简述**
        Session是通过在服务器端记录信息确定用户身份。Session是另一种记录客户状态的机制，不同的Session机制是服务器把客户端信息以某种形式记录在服务器上。客户端浏览器再次访问时只需要从该Session中查找该客户的状态就可以了；
        如果说Cookie机制是通过检查客户身上的“通行证”来确定客户身份的话，那么Session机制就是通过检查服务器上的“客户明细表”来确认客户身份。Session相当于程序在服务器上建立的一份客户档案，客户来访的时候只需要查询客户档案表就可以了；
        Session对应的类为javax.servlet.http.HttpSession类。每个来访者对应一个Session对象，所有该客户的状态信息都保存在这个Session对象里。Session对象是在客户端第一次请求服务器的时候创建的。Session也是一种key-value的属性对，通过getAttribute(String key)和setAttribute(Stringkey,Object value)方法读写客户状态细信息。Servlet里通过request.getSession()方法获取该客户的Session；
    request还可以使用getSession(boolean create)来获取Session。区别是如果该客户的Session不存在，request.getSession()方法会返回**null**，而getSession(true)会先创建Session再将Session返回。Servlet中必须使用request来编程式获取HttpSession对象，而JSP中内置了Session隐藏对象，可以直接使用。如果使用声明了<%@page session=”false” %>，则Session隐藏对象不可用；
        当多个客户端执行程序时，服务器会保存多个客户端的Session。获取Session的时候也不需要声明获取谁的Session。**Session机制决定了当前客户只会获取到自己的Session，而不会获取到别人的Session。各客户的Session也彼此独立，互不可见；**
**        注意：Session的使用比Cookie方便，但是过多的Session存储在服务器内存中，会对服务器造成压力；**
**2.Session的生命周期**
        Session保存在服务器端，为了获得更高的存取速度，服务器一般把Session放在内存里。每个用户都会有一个独立的Session。如果Session内容过于复杂，当大量客户访问服务器时可能会导致内存溢出。因此，Session里的信息应该尽量精简。Session在用户第一次访问服务器的时候自动创建。需要注意只有访问JSP、Servlet等程序时才会创建Session，只访问HTMP、IMAGE等静态资源并不会创建Session。如果尚未生成Session，也可以使用request.getSession(true)强制生成Session；
        为了防止内存溢出，服务器会把长时间没有活跃的Session从内存删除，这个时间就是Session的超时时间。如果超过了超时时间没有访问过服务器，Session就自动失效了。Session的超时时间为maxInactiveInterval属性，可以通过对应的getMaxInactiveInterval()获取，通过setMaxInactiveInterval(longinterval)修改。Session的超时时间也可以在web.xml中修改。另外，通过调用Session的invalidate()方法可以使Session失效；
**3.Session对浏览器的要求**
        虽然Session保存在服务器，对客户端是透明的，它的正常运行仍然需要客户端浏览器的支持。这是因为Session需要使用Cookie作为识别标志，因此服务器向客户端浏览器发送一个名为JSESSIONID的Cookie，它的值为该Session的id（也就是HttpSession.getId()的返回值）。Session依据该Cookie来识别是否为同一用户；
        该Cookie为服务器自动生成的，它的maxAge属性一般为-1，表示仅当前浏览器内有效，并且各浏览器窗口间不共享，关闭浏览器就会失效。因此同一机器的两个浏览器窗口访问服务器时，会生成两个不同的Session。但是由于浏览器窗口内的链接、脚本等打开的新窗口（也就是说不是双击桌面浏览器图标等打开的窗口）除外。这类子窗口会共享父窗口的Cookie，因此会共享一个Session；
**4.URL地址重写**
        如果客户端浏览器将Cookie功能禁用或者不支持Cookie的话，Java Web提供了另一种解决方案：URL地址重写。URL地址重写的原理是将该用户的Session的id信息重写到URL地址中。服务器能够解析重写后的URL获取Session的id。这样即使客户端不支持Cookie，也可以使用Session来记录用户状态。HttpServletResponse类提供了encodeURL(String url)来实现URL地址重写；
**5.Session中禁止使用Cookie**
        打开项目sessionWeb的WebRoot目录下的META-INF文件夹（跟WEB-INF文件夹同级，如果没有则创建），打开context.xml（如果没有则创建），编辑如下内容：
                <?xmlversion=’1.0’ encoding=’UTF-8’?>
                <Contextpath=”/sessionWeb” cookies=”false”>
                </Context>
        或者修改Tomact全局的conf/context.xml，修改内容如下：
                <Contextcookies=”false”>
                    <!--...中间代码略-->
                </Context>
        部署后Tomcat便不会自动生成名为JSESSIONID的Cookie，Session也不会以Cookie为识别标志，而仅仅以重写后的URL地址为识别标志了；
        注意：该配置只是禁止Session使用Cookie作为识别标志，并不能阻止其他的Cookie读写。也就是说服务器不会自动维护名JSESSIONID的Cookie了，但是程序中仍然可以读写其他的Cookie；
