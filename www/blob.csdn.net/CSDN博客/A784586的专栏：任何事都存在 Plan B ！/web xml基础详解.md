# web.xml基础详解 - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年06月22日 20:57:33[QuJack](https://me.csdn.net/A784586)阅读数：365








**web.xml基础详解**

**Javaweb中经常遇到的**web.xml配置**，所以做个基础整理。**

//以一个XML头开始,这个头声明可以使用的XML版本并给出文件的字符编码

//DOCYTPE声明必须立即出现在此头之后。这个声明告诉服务器适用的servlet规范的版本（如2.2或2.3）

//并指定管理此文件其余部分内容的语法的DTD(Document Type Definition，文档类型定义)

**<!DOCTYPE web-app PUBLIC**

**"-//Sun Microsystems, Inc.//DTD Web Application 2.3//EN"**

**"http://java.sun.com/dtd/web-app_2_3.dtd"*****>***

//部署描述符文件的顶层（根）元素为web-app,XML 元素不仅是大小写敏感;出现在其他元素中的次序敏感;

//servlet元素必须出现在所有servlet-mapping元素之前。请注意，所有这些元素都是可选的;

<web-app>

<display-name>scs_service</display-name>




<context-param>

<param-name>rootConfigPath</param-name>

<param-value>WEB-INF/../../conf</param-value>

</context-param>




<listener>

<listener-class>com.sohu.scs.imgprocess.web.ImgProcessListener</listener-class>

</listener>




<servlet>

<servlet-name>ImgProcessServlet</servlet-name>

<servlet-class>com.sohu.scs.imgprocess.web.ImgProcessServlet</servlet-class>

</servlet>




<servlet-mapping>

<servlet-name>ImgProcessServlet</servlet-name>

<url-pattern>/*</url-pattern>

</servlet-mapping>




</web-app>

--------------------------------------

XML头必须是文件中的第一项，DOCTYPE声明必须是第二项，而web- app元素必须是第三项。在web-app元素内，元素的次序：所有这些元素都是可选的：（保证可移植，请保证顺序，否则服务器可能拒绝执行web应用）

l icon icon元素指出IDE和GUI工具用来表示Web应用的一个和两个图像文件的位置。

l display-name display-name元素提供GUI工具可能会用来标记这个特定的Web应用的一个名称。

l description description元素给出与此有关的说明性文本。

l context-param context-param元素声明应用范围内的初始化参数。

l filter 过滤器元素将一个名字与一个实现javax.servlet.Filter接口的类相关联。

l filter-mapping 一旦命名了一个过滤器，就要利用filter-mapping元素把它与一个或多个servlet或JSP页面相关联。

l listener servlet API的版本2.3增加了对事件监听程序的支持，事件监听程序在建立、修改和删除会话或servlet环境时得到通知。Listener元素指出事件监听程序类。

l servlet 在向servlet或JSP页面制定初始化参数或定制URL时，必须首先命名servlet或JSP页面。Servlet元素就是用来完成此项任务的。

l servlet-mapping 服务器一般为servlet提供一个缺省的。但是，常常会更改这个URL，以便servlet可以访问初始化参数或更容易地处理相对URL。在更改缺省URL时，使用servlet-mapping元素。

l session-config 如果某个会话在一定时间内未被访问，服务器可以抛弃它以节省内存。可通过使用HttpSession的setMaxInactiveInterval方法明确设置单个会话对象的超时值，或者可利用session-config元素制定缺省超时值。

l mime-mapping 如果Web应用具有想到特殊的文件，希望能保证给他们分配特定的MIME类型，则mime-mapping元素提供这种保证。

l welcom-file-list welcome-file-list元素指示服务器在收到引用一个目录名而不是文件名的URL时，使用哪个文件。

l error-page error-page元素使得在返回特定HTTP状态代码时，或者特定类型的异常被抛出时，能够制定将要显示的页面。

l taglib taglib元素对标记库描述符文件（Tag
 Libraryu Descriptor file）指定别名。此功能使你能够更改TLD文件的位置，而不用编辑使用这些文件的JSP页面。

l resource-env-ref resource-env-ref元素声明与资源相关的一个管理对象。

l resource-ref resource-ref元素声明一个资源工厂使用的外部资源。

l security-constraint security-constraint元素制定应该保护的URL。它与login-config元素联合使用

l login-config 用login-config元素来指定服务器应该怎样给试图访问受保护页面的用户授权。它与sercurity-constraint元素联合使用。

l security-role security-role元素给出安全角色的一个列表，这些角色将出现在servlet元素内的security-role-ref元素的role-name子元素中。分别地声明角色可使高级IDE处理安全信息更为容易。

l env-entry env-entry元素声明Web应用的环境项。

l ejb-ref ejb-ref元素声明一个EJB的主目录的引用。

l ejb-local-ref ejb-local-ref元素声明一个EJB的本地主目录的应用。




参考：

[http://www.cnblogs.com/chinafine/archive/2010/09/02/1815980.html](http://www.cnblogs.com/chinafine/archive/2010/09/02/1815980.html)









