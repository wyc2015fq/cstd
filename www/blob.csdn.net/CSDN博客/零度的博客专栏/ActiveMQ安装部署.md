# ActiveMQ安装部署 - 零度的博客专栏 - CSDN博客
2016年04月07日 09:11:16[零度anngle](https://me.csdn.net/zmx729618)阅读数：642标签：[activemq																[消息中间件																[jms](https://so.csdn.net/so/search/s.do?q=jms&t=blog)](https://so.csdn.net/so/search/s.do?q=消息中间件&t=blog)](https://so.csdn.net/so/search/s.do?q=activemq&t=blog)
个人分类：[JMS-ActiveMQ](https://blog.csdn.net/zmx729618/article/category/6170787)
JMS（Java Messaging Service）是Java平台上有关面向消息中间件的技术规范，它便于消息系统中的Java应用程序进行消息交换,并且通过提供标准的产生、发送、接收消息的接口简化企业应用的开发，ActiveMQ而是这个规范的一个具体实现。
JMS的队列消息传递过程如下图：
![](https://img-blog.csdn.net/20140726154958993?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2xqMTk4NjA2MDYxMTEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
JMS的主题消息传递过程如下图：
![](https://img-blog.csdn.net/20140726155031753?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2xqMTk4NjA2MDYxMTEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
ActiveMQ下载与部署。
1、  下载
到官网下载最新版本，有windows版本和linux版本的。
[http://activemq.apache.org/download.html](http://activemq.apache.org/download.html)
[](http://activemq.apache.org/download.html)
windows版本：
linux版本：apache-activemq-5.10-20140603.133406-78-bin.tar.gz
2、  部署
A、 windows下部署
ActiveMQ部署其实很简单，和所有Java一样，要跑java程序就必须先安装JDK并配置好环境变量，这个很简单。
然后解压下载的apache-activemq-5.10-20140603.133406-78-bin.zip压缩包到一个目录，得到解压后的目录结构如下图：
![](https://img-blog.csdn.net/20140726155401776?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2xqMTk4NjA2MDYxMTEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
进入bin目录，发现有win32和win64两个文件夹，这2个文件夹分别对应windows32位和windows64位操作系统的启动脚本。
![](https://img-blog.csdn.net/20140726155428234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2xqMTk4NjA2MDYxMTEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
我的实验环境是windowsXP，就进入win32目录，会看到如下目录结构。
![](https://img-blog.csdn.net/20140726155458170?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2xqMTk4NjA2MDYxMTEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
其中activemq.bat便是启动脚本，双击启动。
![](https://img-blog.csdn.net/20140726155527436?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2xqMTk4NjA2MDYxMTEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
ActiveMQ默认启动到8161端口，启动完了后在浏览器地址栏输入：[http://localhost:8161/admin](http://localhost:8161/admin)要求输入用户名密码，默认用户名密码为admin、admin，这个用户名密码是在conf/users.properties中配置的。输入用户名密码后便可看到如下图的ActiveMQ控制台界面了。
![](https://img-blog.csdn.net/20140726155339359?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2xqMTk4NjA2MDYxMTEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
