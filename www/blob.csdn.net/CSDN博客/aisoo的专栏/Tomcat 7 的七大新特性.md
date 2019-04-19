# Tomcat 7 的七大新特性 - aisoo的专栏 - CSDN博客
2010年12月15日 13:02:00[aisoo](https://me.csdn.net/aisoo)阅读数：459标签：[tomcat																[servlet																[session																[outofmemoryerror																[servlets																[csrf](https://so.csdn.net/so/search/s.do?q=csrf&t=blog)](https://so.csdn.net/so/search/s.do?q=servlets&t=blog)](https://so.csdn.net/so/search/s.do?q=outofmemoryerror&t=blog)](https://so.csdn.net/so/search/s.do?q=session&t=blog)](https://so.csdn.net/so/search/s.do?q=servlet&t=blog)](https://so.csdn.net/so/search/s.do?q=tomcat&t=blog)
Tomcat 7引入了许多新功能，并对现有功能进行了增强。很多文章列出了Tomcat 7的新功能，但大多数并没有详细解释它们，或指出它们的不足，或提供代码示例。本文将明确描述Tomcat 7中七个最显著的特征和新增的功能，并对其作出评论，而不是仅仅列出新的功能。本文还提供了代码例子以方便你可以对其有更好的理解。
本文分为两个部分，分别是“Tomcat 7的新特性”和“Tomcat 7增强的功能”。
Tomcat 7新特性
1、使用随机数去防止跨站脚本攻击；
2、改变了安全认证中的jessionid的机制，防止session攻击；
3、内存泄露的侦测和防止；
4、在war文件外使用别名去存储静态内容；
Tomcat 7的增强功能
5、对Servlet 3.0,JSP 2.2和JSP-EL 2。2的支持；
6、更容易将Tomcat内嵌到应用去中去，比如JBoss；
7、异步日志记。
根据Mark Thomas，Tomcat 7委员会的经理的说法，Tomcat 7最显著的三个特征是Servlet 3.0，内存检测泄露和增强的安全特性。
Tomcat 7的例子程序中，包含了Eclipse的工程文件和Ant的构建文件，以方便去构建war文件。其中Eclipse工程文件有例子代码描述了Tomcat 7的一些新特性。
下面逐一开始介绍。
Tomcat 7新特性
1、使用随机数去防止跨站请求伪造攻击。 
Wikipedia将跨站请求伪造攻击(Cross Site Request forgery，CSRF)定义为：“一种影响Web应用的恶意攻击。CSRF让用户当进入一个可信任的网页时，被强行执行恶意代码。
经典的防止CSRF攻击的方法是使用随机数的方式，Wikipedia中定义为“利用随机或伪随机数嵌入到认证协议中，以确保旧的不能在以后的重放攻击中被利用。”
Tomcat 7中有一个servlet过滤器，用于将随机数存储在用户每次请求处理后的seesion会话中。这个随机数，必须作为每次请求中的一个参数。 Servlet过滤器然后检查在请求中的这个随机数是否与存储在用户session中的随机数是一样的。如果它们是相同的，该请求是判断来自指定的网站。如果它们是不同的，该请求被认为是从其他网站发出并且会被拒绝。
这个servlet过滤器是十分简单的，下面是从Tomcat 源代码CsrfPreventionFilter文档中摘录的片段：
Java代码：
public class CsrfPreventionFilter extends FilterBase { public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain) throws IOException, ServletException { String previousNonce = req.getParameter(Constants.CSRF_NONCE_REQUEST_PARAM); String expectedNonce = (String) req.getSession(true).getAttribute(Constants.CSRF_NONCE_SESSION_ATTR_NAME); if (expectedNonce != null && !expectedNonce.equals(previousNonce)) { res.sendError(HttpServletResponse.SC_FORBIDDEN); return; } String newNonce = generateNonce(); req.getSession(true).setAttribute(Constants.CSRF_NONCE_SESSION_ATTR_NAME, newNonce); 所以每个URL地址中都有一个从用户session中提取的随机数，下面是使用的JSTL例子：
在以前，JSTL中构造链接可以这样：
< c:url var="url" value="/show" > < c:param name="id" value="0" / > < /c:url > < a href="${show}" >Show< /a > 而现在可以这样：
< c:url var="url" value="/show" > < c:param name="id" value="0" / > < c:param name="org.apache.catalina.filters.CSRF_NONCE" value="${session.org.apache.catalina.filters.CSRF_NONCE}" / > < /c:url > 具体的例子可以参考Tomcat 7自带例子中的演示，这个过滤器可以在web.xml中进行配置，配置后，所有访问如[http://localhost:8080/Tomcat7demo/csrf/](http://localhost:8080/Tomcat7demo/csrf/) 的都必须带上参数，不带上参数的话会出现403禁止访问错误。
当然这种方法的缺点就是所有的链接都必须带上这个随机数。
2、改变了安全认证中的jessionid的机制，防止session攻击。
Session劫持攻击通常是以下的情况：
1 、恶意攻击者先访问一个网页，由于cookie是以jsession id的方式存储在浏览器中的，即使攻击者不登陆，他可以伪造一个带有jsession id的地址，把它发给受害者，比如：[http://example.com/login?JESSIONID=qwerty](http://example.com/login?JESSIONID=qwerty))；
2、 受害者点这个带有jsessionid的链接，提示输入验证信息之后就登陆系统；
3 、攻击者现在使用这个带jsessionid的链接，以受害者的身份登陆进系统了。
对于攻击者来说，将jsessionid加在url中以及通过一个恶意表单发送出去是很容易的事，对于session劫持攻击的更详细描述，请参考Acros Security组织的白皮书“Session Fixation Vulnerability in Web-based Applications”。
Tomcat 7对此的解决方案是一个补丁，它在验证后改变了jsessionid。这个补丁主要是应用在Tomcat 7中，当然在Tomcat 5和6中也可以使用但只是有些不同。
根据Mark Thomas说的，应用了Tomcat 7的这个补丁后：
• Tomcat默认情况下安全性不再变得脆弱，因为验证后会话发生了变化
• 如果用户改变了默认设置(比如应用程序不能处理变化了的session id),风险也会降到最小，因为在Servlet 3中，可以禁止在url中进行会话跟踪。
而在Tomcat 5和Tomcat 6中，应用了补丁后：
• 能阻止session劫持攻击，因为能让Tomcat在验证后改变session id。
• 如果应用程序不能处理变化了的session id，可以通过写自定义的过滤器去检查request.isRequestedSessionIdFromURL()和其返回的结果，以降低风险。
以上这些改变都是Tomcat在幕后所做的，开发者根本不用去理会。
3、内存泄露的侦测和防止
开发者在部署他们写的程序到生产环境上时，经常会遇到Pemgen错误：OutOfMemoryError。这是由于内存泄露而引起的。通常开发者是通过增大permgen内存的大小去解决或者就是重新启动Tomcat。
Tomcat 7包含了一个新的特性，它通过把不能垃圾回收的引用对象移走的方法，能解决一些Permgen内存泄露的问题。这个特性对程序员部署应用程序在他们的开发环境中是十分方便的，因为程序员在开发环境中为了节省时间一般不重新启动Tomcat就能部署新的war文件。在生产环境中，最好的建议还是停掉Tomcat,然后清除work下面的目录文件并且重新部署应用。
当然，内存泄露检测和防止这个特性现在还不是很完善，还是有的情况Tomcat不能检测内存泄露和修复之的，所以对于生产环境，最好的的办法还是停掉Tomcat，然后清除work下面的目录文件并且重新部署应用。
Mark Thomas解析应用程序或者库程序在如下情况下会触发内存泄露：
• JDBC驱动的注册
• 一些日志框架
• 在ThreadLocals中保存了对象但没有删除它们
• 启动了线程但没停止
而 Java API 存在内存泄漏的地方包括：
1、使用 javax.imageio API ( Google Web Toolkit会用到)
2、使用 java.beans.Introspector.flushCaches()
3、使用 XML 解析器
4、使用 RMI 远程方法调用
5、从 Jar 文件中读取资源
4、在war文件外使用别名去存储静态内容
Web应用程序需要静态资源文件，比如象CSS，Javascript和视频文件、图片文件等。通常都把它们打包放在war文件中，这将增加了WAR文件的大小并且导致很多重复的加载静态资源。一个比较好的解决方法是使用Apache HTTP服务器去管理这些静态文件资源，下面是一个apache httpd.conf文件的配置摘录：
< Directory "/home/avneet/temp/static" > Order allow,deny Allow from all < /Directory > Alias /static "/home/avneet/temp/static" 以上的设置，使得访问[http://localhost/static](http://localhost/static)时，能访问到放在/home/avneet/temp/static下的资源。
允许使用新的aliases属性，指出静态文件资源的位置，可以通过使用Classloader.getResourceAsStream('/static/...')或者在链接中嵌入的方法让Tomcat去解析绝对路径，下面是一个在context.xml中配置的例子：
< ?xml version="1.0" encoding="UTF-8"? > < Context path="/Tomcat7demo" aliases="/static=/home/avneet/temp/static" > < /Context > 假设/home/avneet/temp/static这个文件夹存放有一张图片bg.png，如果war文件以Tomcat7demo的名字部署，那么可以通过以下三个方式去访问这张图片
1.直接访问 [http://localhost:8080/Tomcat7demo/static/bg.png](http://localhost:8080/Tomcat7demo/static/bg.png)
2.在HTML链接中访问：< img src="/Tomcat7demo/static/bg.png" / >
3.通过JAVA代码访问： ByteArrayInputStream bais = (ByteArrayInputStream)getServletContext().getResourceAsStream("/static/bg.png");
使用aliases的好处是可以代替Apache的httpd.conf的设置，并且可以在servlet容器范围内访问，并且不需要Apache。
Tomcat 7的增强特性
5、对Servlet 3.0,JSP 2.2和JSP-EL 2。2的支持
Servlet 3的增强特性有：
• 可以在POJO或者过滤器filters中使用annotations注释(在web.xml中不再需要再进行设置了)
• 可以将web.xml分块进行管理了。也就是说，用户可以编写多个xml文件，而最终在web.xml中组装它们，这将大大降低web.xml的复杂性增强可读性。比如， struts.jar和spring-mvc.jar每一个都可以有一个web-fragment.xml。开发者不再需要在web.xml中去配置它们了，在web-fragment.xml中的jar文件会自动加载，并且struts/spring-mvc servlets和filters也会自动装配设置。
• 异步处理web的请求----这个特性在Tomcat 6 中已经有了，现在在Tomcat 7中以Servlet 3标准规范化了，能让使用异步I/O的web应用程序可以移植到不同的web容器中。异步处理使用非阻塞I/O，每次的HTTP连接都不需要对应一个线程。更少的线程可以为更多的连接提供服务。这对于需要长时间计算处理才能返回结果的情景来说是很有用的，比如产生报表，Web Servce调用等。
• 安全的增强---Servlet 3.0现在使用SSL 去加强了会话session的跟踪，代替了原来的cookie和URL重写。
6、更容易将Tomcat内嵌到应用去中去
Tomcat 7现在可以嵌入到应用程序中去，并可以通过程序去动态设置和启动。象在CATALINA_HOME/conf/server.xml中的很多配置，现在都可以用程序动态去设置了。在Tomcat 7前，Tomcat 6提供了一个嵌入类，它能方便地去配置Tomcat。但在Tomcat 7中，这个类已被废弃了。这个新的Tomcat 7的类，使用了几个默认的配置元素，并提供了一个更容易和简单的方法去嵌入Tomcat。
下面是CATALINA_HOME/conf/server.xml中的一些相关属性和配置：
< Server > < Service > < Connector port="8080 > < Engine > < Host appBase="/home/avneet/work/Tomcat7demo/dist" / > < /Engine > < /Connector > < /Service > < /Server > 我们可以通过程序去进行动态设置了：
final String CATALINA_HOME = "/home/avneet/work/temp/Tomcat7demo/"; Tomcat Tomcat = new Tomcat(); Tomcat.setBaseDir( CATALINA_HOME ); Tomcat.setPort( 8080 ); Tomcat.addWebapp("/Tomcat7demo", CATALINA_HOME + "/webapps/Tomcat7demo.war"); Tomcat.start(); System.out.println("Started Tomcat"); Tomcat.getServer().await(); //Keeps Tomcat running until it is shut down //Webapp Tomcat7demo accessible at [http://localhost:8080/Tomcat7demo/](http://localhost:8080/Tomcat7demo/) 7. 异步日志记录
Tomcat 7现在包括了一个异步日志记录器(AsyncFileHandler)。AsyncFileHandler继承了FileHandler类并能代替FileHandler。使用AsyncFileHandler,时，只需要在CATALINA_HOME/conf/logging.properties中把FileHandler全部替换为AsyncFileHandler就可以了。要注意的是异步日志不能跟log4一起工作。
当有日志发向AsyncFileHandler时，日志被加入到队列中(java.util.concurrent.LinkedBlockingDeque)并且方法调用的信息会马上返回不需要等待I/O写到磁盘中。当类加载器加载AsyncFileHandler时，会有一个单独的线程启动，这个线程会从队列中读取日志信息并且写到磁盘中去。
这种方法的好处是如果I/O速度很慢(比如日志要保存在远端的设备上)时，记录日志的请求和处理过程不会显得很慢。
AsyncFileHandler使用生产者和消费者的关系原理，在队列中存储日志信息。队列默认大小为10000。为了预防队列溢出，默认是丢弃最后的信息。默认的队列大小和溢出的设置都可以通过启动参数进行设置。
关于Tomcat 7的示例程序
Tomcat 7的自带程序例子有两个servlets，一个是演示了如何采用随机数的办法防止CSRF攻击，另外一个是描述了使用aliases。更新一下web/META-INF/context.xml，指出图片的绝对路径即可顺利运行。
通过ant运行build.xml去将它们部署到Tomcat 7中，使用如下两个地址访问：
• [http://localhost:8080/Tomcat7demo/csrf/](http://localhost:8080/Tomcat7demo/csrf/)
• [http://localhost:8080/Tomcat7demo/alias/](http://localhost:8080/Tomcat7demo/alias/)
Tomcat 7 的七大新特性(本教程仅供研究和学习，不代表JAVA中文网观点)
