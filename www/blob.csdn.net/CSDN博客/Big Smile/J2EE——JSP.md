# J2EE——JSP - Big Smile - CSDN博客
2017年01月21日 13:22:35[王啸tr1912](https://me.csdn.net/tr1912)阅读数：509
        尝试着敲SSH有一阵子了，对于JSP的理解还是不是很到位，写此博客总结和学习一下。
# 一、概念
        JSP全名为Java Server Pages，中文名叫java服务器页面，其根本是一个简化的Servlet设计，它是由Sun Microsystems公司倡导、许多公司参与一起建立的一种动态网页技术标准。JSP技术有点类似ASP技术，它是在传统的网页HTML（标准通用标记语言的子集）文件(*.htm,*.html)中插入Java程序段(Scriptlet)和JSP标记(tag)，从而形成JSP文件，后缀名为(*.jsp)。 用JSP开发的Web应用是跨平台的，既能在Linux下运行，也能在其他操作系统上运行。
         从本质上讲JSP页面也和ASP页面一样只不过基于的平台不同，而且JSP页面和后台的交互也和ASP页面是不同的。但是从地位上来讲在serverlet里面和ASP在ASP.net里面的地位是一样的。
        它实现了Html语法中的java扩展（以 <%, %>形式）。JSP与Servlet一样，是在服务器端执行的。通常返回给客户端的就是一个HTML文本，因此客户端只要有浏览器就能浏览。
# 二、基础组成
        request
request 对象是 javax.servlet.httpServletRequest类型的对象。 该对象代表了客户端的请求信息，主要用于接受通过HTTP协议传送到服务器的数据。（包括头信息、系统信息、请求方式以及请求参数等）。request对象的作用域为一次请求。
        response
response 代表的是对客户端的响应，主要是将JSP容器处理过的对象传回到客户端。response对象也具有作用域，它只在JSP页面内有效。
        session
1.什么是session：从一个客户打开浏览器并连接到服务器开始，到客户关闭浏览器离开这个服务器结束，被称为一个会话。当一个客户访问一个服务器时，可能会在这个服务器的几个页面之间反复连接，反复刷新一个页面，服务器应当通过某种办法知道这是同一个客户，这就需要session对象。
2．session对象的ID：当一个客户首次访问服务器上的一个JSP页面时，JSP引擎产生一个session对象，同时分配一个String类型的ID号，JSP引擎同时将这个ID号发送到客户端，存放在Cookie中，这样session对象和客户之间就建立了一一对应的关系。当客户再访问连接该服务器的其他页面时，不再分配给客户新的session对象，直到客户关闭浏览器后，服务器端该客户的session对象才取消，并且和客户的会话对应关系消失。当客户重新打开浏览器再连接到该服务器时，服务器为该客户再创建一个新的session对象。
3.session对象存在一定时间过期问题，所以存在session中的名值对会在一定时间后失去，可以通过更改session有效时间来避免这种情况。同时编程时尽量避免将大量有效信息存储在session中，request是一个不错的替代对象。
        application
1．什么是application:
服务器启动后就产生了这个application对象，当客户在所访问的网站的各个页面之间浏览时，这个application对象都是同一个，直到服务器关闭。但是与session不同的是，所有客户的application对象都是同一个，即所有客户共享这个内置的application对象。
2．application对象常用方法:
(1)public void setAttribute(String key,Object obj): 将参数Object指定的对象obj添加到application对象中，并为添加的对象指定一个索引关键字。
(2)public Object getAttribute(String key): 获取application对象中含有关键字的对象。
out
out 对象用于在Web浏览器内输出信息，并且管理应用服务器上的输出缓冲区。在使用 out 对象输出数据时，可以对数据缓冲区进行操作，及时清除缓冲区中的残余数据，为其他的输出让出缓冲空间。待数据输出完毕后，要及时关闭输出流。
        page
page 对象代表JSP本身，只有在JSP页面内才是合法的。 page隐含对象本质上包含当前 Servlet接口引用的变量，类似于Java编程中的 this 指针。
        config
config 对象的主要作用是取得服务器的配置信息。通过 pageContext对象的 getServletConfig() 方法可以获取一个config对象。当一个Servlet 初始化时，容器把某些信息通过 config对象传递给这个 Servlet。 开发者可以在web.xml 文件中为应用程序环境中的Servlet程序和JSP页面提供初始化参数。
        exception
java.lang.Throwable 的实例，该实例代表其他页面中的异常和错误。只有当页面是错误处理页面，即编译指令page 的isErrorPage 属性为true 时，该对象才可以使用。常用的方法有getMessage()和printStackTrace()等。
        pageContext
pageContext 对象的作用是取得任何范围的参数，通过它可以获取 JSP页面的out、request、reponse、session、application 等对象。pageContext对象的创建和初始化都是由容器来完成的，在JSP页面中可以直接使用 pageContext对象。
# 三、为什么使用
        我们应该知道的是，在JavaWeb的开发中有一个很重要的概念，就是在JavaWeb的项目中有一个盛放web页面的标准，项目中的“WebRoot”目录就是盛放web页面用的，如果把页面直接放在webroot下面的话，所有通过你的连接地址访问页面的人都可以直接通过路径访问这个下面的页面。然后再WebRoot路径下面的WEB-INF目录下则是盛放不能由连接直接访问的页面，而且必须通过一个人反射或者一个Action才可以进行访问，所以WEB-INF这个路径下面一般盛放的就是JSP这种需要解析才可以显示的页面，所以一般需要保密性比较强的网站都会用JSP的页面来进行交互。并且JSP的一些特性使得在构建一些网页的时候比纯静态的页面要简单的多，而且可以节省很多代码。
