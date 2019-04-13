
# MyEclipse2014配置Tomcat开发JavaWeb程序JSP以及Servlet - 无界 - CSDN博客

置顶2014年03月23日 12:42:51[21aspnet](https://me.csdn.net/21aspnet)阅读数：50918个人分类：[Java																](https://blog.csdn.net/21aspnet/article/category/1877583)



**1.安装准备**
1).下载安装MyEclipse2014，这已经是最新版本。
![](https://img-blog.csdn.net/20140323085615203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

2).下载Tomcat
官网：[http://tomcat.apache.org/](http://tomcat.apache.org/)
我们选择8.0：[http://tomcat.apache.org/download-80.cgi](http://tomcat.apache.org/download-80.cgi)
在windows下选择64位解压版：[http://mirror.bit.edu.cn/apache/tomcat/tomcat-8/v8.0.3/bin/apache-tomcat-8.0.3-windows-x64.zip](http://mirror.bit.edu.cn/apache/tomcat/tomcat-8/v8.0.3/bin/apache-tomcat-8.0.3-windows-x64.zip)
![](https://img-blog.csdn.net/20140323091946796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
下载好以后在本地解压即可。

**2.下载Java，配置JDK**
参考：[Java开发环境的搭建以及使用eclipse创建项目](http://blog.csdn.net/unix21/article/details/18813173)

**3.配置MyEclipse中的JRE**
**Myeclipse菜单--Window--Preferences**
进入偏好设置
![](https://img-blog.csdn.net/20140323090431828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**Window--Preferences--Java--Installed JREs--Add**
注意，在MyEclipse2014中默认自带的是jdk7。如果需要其他jdk可以自行配置。
需要选择Standard VM:
![](https://img-blog.csdn.net/20140323090447796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

选择我们使用前面配置好的非MyEclipse2014自带的jdk7的安装路径C:\Program Files\Java\jdk1.7.0_51
![](https://img-blog.csdn.net/20140323090503375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

勾选新增的jdk
![](https://img-blog.csdn.net/20140323090514046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**Window--Preferences--Java--Compiler**

设置Java编译器的版本
![](https://img-blog.csdn.net/20140323101940046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**4.为MyEclipse配置Tomcat**
**Window--Preferences--MyEclipse--Servers--Tomcat**
选择Tomcat版本，我们这里选择Tomcat 8.x，注意首先要勾选Tomcat 8.x server 为Enable，不然不起作用！
在Tomcat home directory目录选择之前解压的tomcat解压包即可
![](https://img-blog.csdn.net/20140323091355171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

再配置Tomcat的jdk为我们之前配置好的jdk
![](https://img-blog.csdn.net/20140323092136593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

不过也可以在这里Add需要的JDK:
![](https://img-blog.csdn.net/20140323101555187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

还有一个需要注意的地方，我们需要修改。
Window -> Perferences -> Java -> Editor -> Content Assist，在右边最下面一栏找到 auto-Activation ，下面有三个选项，找到第二个“Auto activation triggers for Java：”选项
在其后的文本框中会看到一个“.”存在。这表示：只有输入“.”之后才会有代码提示和自动补全，我们要修改的地方就是这里。把该文本框中的“.”换掉，换成“abcdefghijklmnopqrstuvwxyz.搜索”，这样，你在Eclipse里面写Java代码就可以做到按“abcdefghijklmnopqrstuvwxyz.@”中的任意一个字符都会有代码提示。
![](https://img-blog.csdn.net/20140323092146828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**5.新建Java web项目**
**MyEclipse的菜单栏--File--Web Project**
新建一个web工程
![](https://img-blog.csdn.net/20140323102411687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

Project name 填上自己的项目名称，例如HelloWorld。
需要选择Java EE版本以及Target runtime为之前配置好的Tomcat8：
![](https://img-blog.csdn.net/20140323102201828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下一步
![](https://img-blog.csdn.net/20140323102425515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这里最好勾选自动生成web.xml，如果需要修改web根目录名称可以修改Content directory为所需即可
![](https://img-blog.csdn.net/20140323102434875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

建好的项目
![](https://img-blog.csdn.net/20140323102811156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

项目上右键弹出属性对话框，修改文本编码为UTF-8：
![](https://img-blog.csdn.net/20140323102821546?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**6.JSP输出当前时间**
打开index.jsp中，注意需要用右键Open with “MyEclipse JSP Editor”方式以纯代码的方式打开JSP文件，不然会默认用Visual 的可视化方式打开。
![](https://img-blog.csdn.net/20140323103048843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在index.jsp中书写代码：

```python
<body>
    当前时间：<br>
    <%
    Date data=new Date();
    out.write(data.toLocaleString());
    %>
  </body>
```
启动Tomcat：
![](https://img-blog.csdn.net/20140323103059843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在Console窗口可以查看输出信息：
![](https://img-blog.csdn.net/20140323104107953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

部署代码到Tomcat中：
![](https://img-blog.csdn.net/20140323103111125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

部署好了会显示--Successfully deploy
![](https://img-blog.csdn.net/20140323103124984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

查看服务器信息，可以点击下面的Servers窗口：
![](https://img-blog.csdn.net/20140323104137328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

查看运行结果，可以在下面的Web Browser窗口输入网址即可查看：
![](https://img-blog.csdn.net/20140323104438171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

也可以在浏览器中输入网址一样查看：
![](https://img-blog.csdn.net/20140323104450562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**7.Myeclipse的自动编译和Tomcat的自动部署**
在tomcat的配置文件conf/server.xml
```python
<Host name="localhost"  appBase="webapps"
            unpackWARs="true" autoDeploy="true">
```
如果autoDeploy="true"那么tomcat就会自动部署，所谓自动部署就是不需要重启tomcat就可以自动检测应用程序的 /WEB-INF/lib 和 /WEB-INF/classes目录的变化，自动装载新的应用程序，我们可以在不重起 tomcat的情况下改变应用程序。
**Window--Preferences--General--workspace**
默认是勾选Build automatically的，这样myeclipse会自动编译发布到tomcat，而tomcat的配置文件conf/server.xmlautoDeploy="true"，也就是说，用redeploy的情况几乎没有。但是myeclipse不太稳定，有的时候，不能自动发布，必须要redeploy。所以当你重启tomcat后还是得不到你的预期结果时候，考虑下redeploy。
![](https://img-blog.csdn.net/20140323123316203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下面是去掉勾选Build automatically以后修改JSP文件，点“保存”就不会自动编译到tomcat中：
![](https://img-blog.csdn.net/20140323123342984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**8.Servlet输出当前时间**
在src目录右键新建一个servelet文件：
![](https://img-blog.csdn.net/20140323110731593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

需要填写包名package和servlet名Name，默认继承自httpServlet，默认勾选doGet和doPost等方法都不要改变：
![](https://img-blog.csdn.net/20140323110744750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

勾选自动在web.xml文件中生成Map文件：
![](https://img-blog.csdn.net/20140323110756000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

web,xml文件中自动生成的代码：

```python
<servlet>
    <servlet-name>Timeprint</servlet-name>
    <servlet-class>com.mc.demo.Timeprint</servlet-class>
  </servlet>
  <servlet-mapping>
    <servlet-name>Timeprint</servlet-name>
    <url-pattern>/servlet/Timeprint</url-pattern>
  </servlet-mapping>
```
可以修改url-pattern为需要的格式：
![](https://img-blog.csdn.net/20140323110816578?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在servlet文件中myeclipse已经自动为我们生成了部分代码，我们只需要加上我们需要的代码。
增加输出当前时间的代码
```python
SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");//设置日期格式
		out.println(df.format(new Date()));// new Date()为获取当前系统时间
```
注意，如果出现红叉叉说明需要引入对应的包文件，在鼠标光标停留在代码上的时候会自动弹出提示框，点击inport导入对应的包即可：
![](https://img-blog.csdn.net/20140323110831078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

完整的doGet代码如下：

```python
public void doGet(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		response.setContentType("text/html");
		PrintWriter out = response.getWriter();
		out.println("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">");
		out.println("<HTML>");
		out.println("  <HEAD><TITLE>A Servlet</TITLE></HEAD>");
		out.println("  <BODY>");
		SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");//设置日期格式
		out.println(df.format(new Date()));// new Date()为获取当前系统时间
		out.println("  </BODY>");
		out.println("</HTML>");
		out.flush();
		out.close();
	}
```
页头已经自动增加了自动导入的包：
```python
import java.text.SimpleDateFormat;
import java.util.Date;
```

点击保存编译好的文件或者jsp文件已经自动保存到到tomcat中对应的文件夹下：
在tomcat的webapps目录下可以看到生成的Timeprint.class文件已经是最新，
输入地址http://localhost:8080/HelloWorld/servlet/Timeprint可以在Web Browser中看到servlet已经输出当前时间了，可以对比二者。
![](https://img-blog.csdn.net/20140323112314828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



