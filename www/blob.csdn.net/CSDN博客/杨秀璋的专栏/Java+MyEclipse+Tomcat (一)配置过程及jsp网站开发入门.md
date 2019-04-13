
# Java+MyEclipse+Tomcat (一)配置过程及jsp网站开发入门 - 杨秀璋的专栏 - CSDN博客

2015年05月05日 16:53:53[Eastmount](https://me.csdn.net/Eastmount)阅读数：10641


这些年免费帮同学或同学的朋友做过的毕设还是有一些了，我的博客基本都是基于我做过的项目相关的，而里面又有三分之一都是自己做的毕设或别人的毕设的做后感。毕设其中就包括MFC图像处理、.Net安全软件、C\#聊天软件字典软件、Android百度地图随手拍、php网站、.Net图书管理系统等等。其实我回过头来想想啊！这些知识对我的帮助和提高还是非常大的，虽然比不上公司实际上线项目，但是帮帮同学对我挺好的，当然很大程度也是害了一些同学，但他们很多都从事了非软件相关的专业。
很显然，大部分的毕设都是基于XXX系统关于数据库的增删改查，包括JSP(Java)、PHP、ASP(C\#)，前端包括JavaScript、Ajax、Html、CSS等知识。写这篇文章也标志着我要做一个Java相关的网站，我也就简单写写几篇入门的知识吧！因为我真的不是很擅长Java，没有系统的学过，只是做过Android相关的APP，没学的原因很大程度是由于以前J2EE\Android都非常难配置吧！
文章主要讲述Java+MyEclipse+Tomcat配置过程及实现第一个Jsp网站的入门知识，并没有涉及Spring、Struts等框架知识，希望文章对你有所帮助。
## 一. 配置Tomcat
由于前面配置过Android的JDK，所以我此时从Tomcat开始进行配置。我下载的apache-tomcat-7.0.59.exe，下面是安装过程，通常是8080端口：
![](https://img-blog.csdn.net/20150505150810310)
下面是绑定Java Virtual Machine：
![](https://img-blog.csdn.net/20150505150927171)
建议安装到非C盘目录下，因为如果需要手动修改或配置一些参数更容易些。安装后运行Tomcat，在“General”点击“Start”启动。如下图我Tomcat安装在F盘目录下：
![](https://img-blog.csdn.net/20150505152327708)![](https://img-blog.csdn.net/20150505151619035)
然后在F:\tomcat\Tomcat 7.0\webapps\test目录下新建一个index.jsp文件（xml中默认寻找运行index.jsp文件），代码如下：
```python
<html>
<body>
<h1>第一个tomcat运行程序!!!</h1>
</body>
</html>
```
调用浏览器访问，运行结果如下所示，这也标志这Tomcat安装成功。
![](https://img-blog.csdn.net/20150505153208026)



## 二. 配置MyEclipse
安装MyEclipse，下面是免费下载地址，按照里面的PDF配置激活MyEclipse。
[http://pan.baidu.com/s/1bnuEcnd](http://pan.baidu.com/s/1bnuEcnd)
运行MyEclipse，我的工作空间安装目录是F:\MyEclipse\Workspaces\MyEclipse 10。新建Web Project，项目名称为FirstWeb。
![](https://img-blog.csdn.net/20150505161131966)
项目的目录结构如下图所示，它有一个默认的index.jsp：
![](https://img-blog.csdn.net/20150505161631643)
此时需要绑定MyEclipse和Tomcat，点击
“MyEclipse”》“Preferences”》“Servers”》“Tomcat”选择“Tomcat 7.x”，并且选中Tomcat的安装目录，我的是F盘，如下图所示。

![](https://img-blog.csdn.net/20150505162132427)
PS：此时可能会出现配置中Apply不能点击，好像点击OK也能配置成功。
有的还需要绑定或检查JDK，如下图所示：
![](https://img-blog.csdn.net/20150505162939882)
部署项目连接Tomcat如下图所示：
![](https://img-blog.csdn.net/20150507030015086)
Tomcat和MyEclipse配置并绑定成功后，运行程序可能会报错：
严重: StandardServer.await: create[localhost:8005]:
java.net.BindException: Address already in use: JVM_Bind
![](https://img-blog.csdn.net/20150505163503654)
那是因为你的端口被占用，你需要把Tomcat关闭和其进程杀死。因为你以后运行MyEclipse后，直接运行就可以，不需要手动运行Tomcat。运行如下所示，再选择Tomcat 7.x：
![](https://img-blog.csdn.net/20150505163734964)
运行结果如下图所示：
![](https://img-blog.csdn.net/20150505164342173)

![](https://img-blog.csdn.net/20150505164028764)
其默认index.jsp代码如下图所示：
```python
<%@ page language="java" import="java.util.*" pageEncoding="ISO-8859-1"%>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <base href="<%=basePath%>">
    <title>My JSP 'index.jsp' starting page</title>
	<meta http-equiv="pragma" content="no-cache">
	<meta http-equiv="cache-control" content="no-cache">
	<meta http-equiv="expires" content="0">    
	<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
	<meta http-equiv="description" content="This is my page">
	<!--
	<link rel="stylesheet" type="text/css" href="styles.css">
	-->
  </head>
  <body>
    This is my JSP page. <br>
  </body>
</html>
```
最后实现的网站效果如下图所示，我后面的文章也将简单介绍一些技术。希望文章对大家有所帮助，如果有错误或不足之处，还请海涵~
![](https://img-blog.csdn.net/20150505164836748?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



PS：文章开头讲了这么多，我只是提醒你，如果你也是大学软件工程或计算机的一员，很多时候你都需要做一些自己喜欢的东西来提升自己；而且帮别人做东西也是能提高的，但这个度需要你自己把握！
说到这里我想到了火车上一位高中朋友对我说的话:"我们其实还是很优秀的,至少能从家乡贵州考出来读大学,在生活中我们会遇到很多人有困难需要帮助,很可能这个对你来说只是举手之劳,而对他却为难整个家庭.这时你要帮助,你帮助他又不会少块肉.尤其是对陌生人的那种无偿帮助!"是啊!在编程当中我们也会遇到各种各样的困难,而且很可能这种困难对你来说非常的简单,但是别人确实百思不得其解!这种感觉我也经常遇到,怎么都不会的别人一点就通.所以当别人不会的问你时,你也应该...
（By:Eastmount 2015-5-5 下午5点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/)）

