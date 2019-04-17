# OpenJMS安装配置及其验证测试 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年09月24日 23:37:57[boonya](https://me.csdn.net/boonya)阅读数：1995








Open JMS 安装测试



**1、内容简述**



开源的JMS服务器openJms，及怎样使用openJms来构建系统之间健全、高度可用的通讯，从而简化企业级应用的开发。openJms符合SUN的JMS API 1.1规范，支持消息队列，还支持消息传递的发布/订阅模式，本文先就系统服务的搭建及JMS的非结构化消息发送和接收进行说明。另外：ActiveMQ、mom4j也实现类jms1.1，有兴趣可以去open-open查阅他们之间特性与性能差别，一般商业使用大多采取websphere jms等。



**2、特性简介**

2-1、支持点对点、发布订阅消息模型 

2-2、可靠的消息传输 

2-3、支持同步、异步消息传输 

2-4、jdbc持久存储 

2-5、本地事物 

2-6、身份认证 

2-7、图形管理界面 

2-8、基于xml的配置文件、 

2-9、内存和数据库垃圾回收 

2-10、支持TCP, RMI, HTTP and SSL 传输协议 

2-11、可以与servlet容器结合，例如tomcat、jboss等 

2-12、支持Applet 

2-13、基于SQL92的消息过滤 

注：JMS 有五种消息类型。三种结构化或半结构化的消息类型（MapMessage、ObjectMessage 和 StreamMessage）以及两种非结构化的或自由格式的消息类型（TextMessage 和 BytesMessage）。



**3、Open JMS 安装**

 jms功能如此强大，下面我们介绍下如何安装与配置启动：

    下载Openjms、地址：[http://openjms.sourceforge.net/downloads.html](http://openjms.sourceforge.net/downloads.html)

    本文以作者本机为示例讲解，解压缩至任意目录、最好不要安装在中文目录，也请最好不要在含有特殊字符（%，&，*，@等）的路径下面，以免产生不必要的错误。

3-1、配置系统环境变量

将下载的文件解压至任意磁盘路径，如：

E:\Java_Tool_Jars\Java_UsingJar_JMS\openjms-0.7.7-beta-1

在Win7 下计算机>属性>高级系统设置>高级下面进行设置，如图：

![](https://img-blog.csdn.net/20130924232827046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




3-2、openjms目录结构简介

![](https://img-blog.csdn.net/20130924232825218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

     bin目录->unix、winodw脚本文件，启动server、停止server、启动管理界面，创建数据库脚本文件。

     doc目录->openjms全部文档

     lib目录->服务器、客户端依赖jar文件

     config->server缺省配置文件，主要在：openjms.xml中，稍后我们详细讲解

     examples->示例程序

3-3、运行openjms服务器

 window下双击bat脚本：( startup.bat启动,shutdown.bat停止)

![](https://img-blog.csdn.net/20130924233010062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


  admin.bat管理员界面:

![](https://img-blog.csdn.net/20130924233038015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



3-4、openjms测试

我们先测试下我们的openjms server是否配置成功，这里以自带示例测试；

   进入到%openjms_home%\examples\basic,双击build.bat，编译示例源文件，编译后产生一堆class文件；

   重新打开dos窗口，进入%openjms_home%\examples\basic目录，目录中已经包含运行脚本run.bat，只需执行run <filename>即可。

![](https://img-blog.csdn.net/20130924233206140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

需要注意，这里run是执行编译后的Java文件(.class文件)

![](https://img-blog.csdn.net/20130924233221875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


首先管理员界面查看服务器中没有消息队列，如图

![](https://img-blog.csdn.net/20130924233325406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

3-5、发送消息

运行sender，发送消息至目的地

run Sender queue1 3

注：queue1是openjms.xml配置的消息目的地、3表示发送多少条消息；(消息将在queue1中保存，直到被接收者取走)。

![](https://img-blog.csdn.net/20130924233340718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

查看管理界面：queue1 收到3条消息:

![](https://img-blog.csdn.net/20130924233419078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

3-6、接收消息

在dos窗口哦运行：

   run Recevier queue1 3 /*这里queueq、3和sender的对应，都是程序示例中的输入参数 *稍后章节的example示例源码讲解中会详细介绍 */

   运行结果如下：

![](https://img-blog.csdn.net/20130924233526500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20130924233548140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20130924233548140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


   观看管理界面，服务器中队列的消息已经被取走；至此，简单的openjms安装完成。





