# java-web之servlet基础知识梳理(一) - 苝花向暖丨楠枝向寒 - CSDN博客

2018年02月03日 00:02:21[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：75


![](https://img-blog.csdn.net/20180202234959170?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQwMjQ3MjYz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

自己写的servlet extends HttpServlet  。 HttpServlet 在GenericServlet的基础上又增加了一些 对浏览器转发方式的 各种处理方法，如doGet()、doPut()、doPost()、doDelete() 等等。还重写了service 方法。

通过 super.getServletConfig()  可以获取到config 对象。  通过config.getServletContext()方法可以获取ServletContext()对象。

ServletConfig ： 

(1) 在Servlet的配置文件中，可以使用一个或多个<init-param>标签为servlet配置一些初始化参数。

(2) 当servlet配置了初始化参数后，web容器在创建servlet实例对象时，会自动将这些初始化参数封装到ServletConfig对象中，并在调用servlet的init方法时，将ServletConfig对象传递给servlet。进而，程序员通过ServletConfig对象就可以得到当前servlet的初始化参数信息。

ServletContext：

(1) 域对象,在整个项目范围内有效; （四大域之一）

(2) WEB容器在启动时，它会为每个WEB应用程序都创建一个对应的ServletContext对象,它代表当前web应用;

(3) 一个WEB应用对应一个ServletContext对象;

(4) 一个WEB应用下有多个Servlet程序;

(5) 所有的servlet程序都共享同一个ServletContext对象;

**Servlet四大域对象**

ServletContext ： 全局，将属性放入SevletContext域 任何一个项目内的Servlet都可以获取。 

HttpSession 域 ：为每一个用户都创建一个Session对象，存于服务端，用来存储用户信息跟踪用户的操作状态。内部是map结构

ServletRequest 域 ：客户端发送过来一次请求，就创建一个 ServletRequest 对象，将请求封装进去。 各个请求之间信息不能相互获取。

PageContext 域 ：范围是 当前页面，包含页面信息，能够对其他域对象进行操作。

关于cookie和session

产生原因：比如购物车系统，买了 冰箱，将信息放入request中，传给了购物车Servlet。当我们结算的时候去结算Servlet，由于request 信息不互通。 无法拿到订单信息。 所以出现了回话技术。

区别：

**Cookie是客户端技术**

程序把每个用户的数据以cookie的形式写给用户各自的浏览器;

cookie

当用户使用浏览器再去访问服务器中的web资源时，就会带着各自的数据去。

这样，web资源处理的就是用户各自的数据了;

作为客户端技术缺点是:安全性不高

![](https://img-blog.csdn.net/20180208183017277)


**Session是服务器端技术**

session数据将保存在服务器端;

利用这个技术，服务器在运行时可以为每一个用户的浏览器创建一个其独享的session对象，

由于session为用户浏览器独享，所以用户在访问服务器的web资源时，可以把各自的数据放在各自的session中，当用户再去访问服务器中的其它web资源时，其它web资源再从用户各自的session中取出数据为用户服务;

![](https://img-blog.csdn.net/20180208182659677)

HttpServletRequest：将客户端发来的请求封装到request 中。

* 请求

*** 请求行**

				* 请求方式

					* get和post区别

				* 请求路径

				* 协议版本

					* HTTP/1.1版本

*** 请求头**

*** if-modefied-since		和last-modefied 完成控制缓存。**

**				* referer				记住当前网页的来源**

**				* **user-agent			查看浏览器的版本

*** 空行**

*** 请求体**

				* 封装post请求参数列表

请求头信息

|referer|记住当前网页的来源|
|----|----|
|user-agent|查看浏览器的版本|
|if-modefied-since|和last-modefied 完成控制缓存|
get和post区别：

**1 GeET 请求信息放在了请求行里，因此能在URL地址栏里看见,因此不安全**

**  POST: 请求信息方法在请求体力,因此相对是安全;**

**2 GET：传输的数据大小受限制，一般不超2KB**

** POST: 传输的数据大小不收限制;**

**3 GET是从服务器上获取数据,传输速度快，post是向服务器传送数据。**

HttpServletResponse：

*** 响应行**

	* 协议版本

	* 状态码

			* 200		一起ok

			* 302		完成重定向操作

			* 304		if-modefied-since和last-modefied一起控制缓存

			* 404		找不到资源

			* 500		程序内部错误

			* 状态码描述

* 响应头

			* Location		    和302完成重定向

			* last-modefied	

			* refresh		       页面定时刷新

			* Content-Dispostion	文件下载

					禁用浏览器的缓存。

 * 空行

 * 响应体

			* 展示用户的数据

响应头信息

|location|和302一起来完成重定向的操作|
|----|----|
|refresh|页面的定时刷新|
|last-modefied|和 if-modefied-since和304状态码一起来控制缓存|

tomcat 启动  只会读取一次web.xml 以及 servlet中的init 方法，因此修改这些需要重启服务器。doget(),dopost() 等 以及jsp、html网页，只要浏览器刷新一次 就会重新读取一次，因此不需重启服务器。

