# Windows下JDK1.6.0+Tomcat6.0的安装配置  - 战斗蜗牛的专栏 - CSDN博客





2010年01月13日 16:14:00[vbskj](https://me.csdn.net/vbskj)阅读数：520标签：[tomcat																[jdk																[windows																[myeclipse																[eclipse																[installer](https://so.csdn.net/so/search/s.do?q=installer&t=blog)
个人分类：[java](https://blog.csdn.net/vbskj/article/category/643636)






## [Windows下JDK1.6.0+Tomcat6.0的安装配置](http://www.cnblogs.com/coralsea/archive/2008/03/07/1095411.html)


**1.****安装JDK**
这是进行JSP开发的重要一步，也是安装JSP引擎（Tomcat、Resin、Weblogic等）的前提，
一般的JSP引擎是由Java语言编写的。

本人使用的JDK版本是1.6.0，平台是Windows，安装文件可到Sun公司的网站上下载
（[https://sdlc3e.sun.com/ECom/EComActionServlet/DownloadPage:~:com.sun.
sunit.sdlc.content.DownloadPageInfo;jsessionid=033BE12AFE722B242F5AADE1
2A5114EB;jsessionid=033BE12AFE722B242F5AADE12A5114EB](https://sdlc3e.sun.com/ECom/EComActionServlet/DownloadPage:~:com.sun.sunit.sdlc.content.DownloadPageInfo;jsessionid=033BE12AFE722B242F5AADE12A5114EB;jsessionid=033BE12AFE722B242F5AADE12A5114EB)）。

在Windows平台上安装JDK比较简便，运行安装文件jdk-6-nb-5_5-win.exe，一直单击
【Next】按钮就完成了。

安装完成后，需要在系统的环境变量加入安装JDK的路径，例如：JDK安装到C:" jdk1.6.0,
那么在环境变量加入JAVA_HOME变量，变量值为C:" jdk1.6.0。这样JDK的安装就完成了。

**2.****安装Tomcat**

这里使用的Tomcat是6.0版本，平台为Windows，安装之前必须完成上述的JDK的安装。安装
文件可以到Tomcat网站上下载（[http://tomcat.apache.org/download-60.cgi#6.0.16](http://tomcat.apache.org/download-60.cgi#6.0.16)）注意
下载版本为：Windows Service Installer。下载后直接运行apache-tomcat-6.0.16.exe，一直
单击【Next】按钮就完成了。安装时，它会自动找到你的JDK安装路径。

运行Tomcat，打开“开始”——“运行”，输入cmd并回车，进入Tomcat的bin目录，启动命
令为startup，关闭命令为shutdown。启动后，打开浏览器，在地址栏输入[http://localhost:8080/](http://localhost:8080/)，
测试Tomcat是否安装正常，如果正常运行，会显示下图：![](http://images.cnblogs.com/cnblogs_com/coralsea/1.jpg)



**3.****安装Eclipse**

Eclipse是个开源的IDE，首先下载Eclipse的安装文件，这里用的是Eclipse3.3.2版本（

[http://eclipse.cdpa.nsysu.edu.tw/downloads/drops/R-3.3.2-200802211800/winPlatform.php#EclipseSDK](http://eclipse.cdpa.nsysu.edu.tw/downloads/drops/R-3.3.2-200802211800/winPlatform.php#EclipseSDK)），下载后直接运行eclipse.exe,第一次
运行会自动寻找JDK并完成相应的配置，选择workspace的路径，然后进入Eclipse的主界面。
可以下载中文的语言包插件。

**4.****安装myEclipse**

从[http://www.myeclipseide.com](http://www.myeclipseide.com/)下载myEclipse，

运行MyEclipse_6.0.1GA_E3.3.1_Installer.exe，先点“next”,再接受协议，再点“next”
，选择好Eclipse的目录之后，再点“next”,选择myEclipse的安装目录之后，继续“next”,再
“next”->"install"->“next”->"done"。到此完成myEclipse的安装了。

**5.****配置myEclipse**

运行eclipse.exe，同样，将检测到更新。确定，然后再重新启动Eclipse。

1）添加jdk路径

Menu（主菜单）->Window（窗口）->Preferences（首选项）->java->Installed JREs
（已安装的jre），点Add（添加）。


![](http://images.cnblogs.com/cnblogs_com/coralsea/2.jpg)
    Browse jdk的路径，点OK

![](http://images.cnblogs.com/cnblogs_com/coralsea/3.jpg)
    2）设置Server

菜单栏中选择“窗口(window)”－－“首选项...(preference)” ——“MyEclipse”－－
“ Servers”－－“Tomcat”——Tomcat6.x 

将Tomcat Server设置为“Enable”，并将“Tomcat Home Directory”设置为Tomcat6.0
的安装目录，其他目录选项将会自动生成。


![](http://images.cnblogs.com/cnblogs_com/coralsea/4.jpg)
    接着选择Tomcat6.x ——JDK，在Tomcat JDK name那选择之前添加的jdk。


![](http://images.cnblogs.com/cnblogs_com/coralsea/6.jpg)
3)配置默认编码

仍然是菜单栏中的“窗口(window)”－－“首选项...(preference)”——“常规(General)”－－
“内容类型(Content Types)”－－“文本(text)” 。

然后在“缺省编码(Default encoding)”中输入“UTF-8”，点击“更新(Update)”将“Java类文件
(Java Properties File)”及其他字符编码均“更新(Update)”为“UTF-8”。


**![](http://images.cnblogs.com/cnblogs_com/coralsea/5.jpg)6.****新建项目**

主菜单->文件->新建->web project


![](http://images.cnblogs.com/cnblogs_com/coralsea/7.jpg)
右键src——new——包

右键包——new——java类

右键WebRoot——New——JSP

**![](http://images.cnblogs.com/cnblogs_com/coralsea/8.jpg)7.****发布设置**

右键项目名称——MyElipse——Add and Remove Project Deployments，点Add

![](http://images.cnblogs.com/cnblogs_com/coralsea/9.jpg)
在Server里选择刚才配置好的Tomcat6.x

![](http://images.cnblogs.com/cnblogs_com/coralsea/10.jpg)
然后鼠标选中Tomcat6.x，点“Redeploy”进行发布，以后工程更新了，也在这里进行发布更新。


**![](http://images.cnblogs.com/cnblogs_com/coralsea/11.jpg)8.****启动web****服务**

![](http://images.cnblogs.com/cnblogs_com/coralsea/12.jpg)
查看运行结果
![](http://images.cnblogs.com/cnblogs_com/coralsea/13.jpg)](https://so.csdn.net/so/search/s.do?q=eclipse&t=blog)](https://so.csdn.net/so/search/s.do?q=myeclipse&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=jdk&t=blog)](https://so.csdn.net/so/search/s.do?q=tomcat&t=blog)




