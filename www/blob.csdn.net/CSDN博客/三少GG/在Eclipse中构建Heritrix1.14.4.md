# 在Eclipse中构建Heritrix1.14.4 - 三少GG - CSDN博客
2010年09月29日 00:47:00[三少GG](https://me.csdn.net/scut1135)阅读数：1799标签：[eclipse																[compiler																[java																[网络爬虫																[jar](https://so.csdn.net/so/search/s.do?q=jar&t=blog)](https://so.csdn.net/so/search/s.do?q=网络爬虫&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=compiler&t=blog)](https://so.csdn.net/so/search/s.do?q=eclipse&t=blog)
个人分类：[Web网站设计](https://blog.csdn.net/scut1135/article/category/762263)

### [在Eclipse中构建Heritrix1.14.4](http://extjs2.javaeye.com/blog/699751)
[http://extjs2.javaeye.com/blog/699751](http://extjs2.javaeye.com/blog/699751)
在Eclipse中构建Heritrix 
这里采用的是Heritrix 1.14.4（2010年5月10日的版本 目前来看是最新版本）   **jdk环境1.6 **
1.首先从http://sourceforge.net/projects/archive-crawler/ 中下载 
heritrix-1.14.4.zip 
heritrix-1.14.4-src.zip 
2.在Eclipse 中创建一个java project的工程,分别对 
heritrix-1.14.4.zip 
heritrix-1.14.4-src.zip 进行解压。 
3.将heritrix-1.14.4-src.zip解压中的src/java 中的 com、org、st三个文件夹复制到工程src下。 
4.将heritrix-1.14.4-src.zip解压中src中conf文件夹复制到项目根目录。 
5.将heritrix-1.14.4-src.zip解压中lib文件夹复制到项目根目录。 
6.将heritrix-1.14.4-src.zip解压中src/resources/org/archive/util中tlds-alpha-by-domain.txt文件复制到工程中org.archive.util包下。**(直接粘贴到工程里，自动复制到源文件中）**
7.将heritrix-1.14.4.zip解压中webapps文件夹复制到项目根目录。 
===========下面跳过===========================
如果文件夹名称不是webapps 需要在Heritrix.java中进行相应的更改。 
Java代码 
- /** 
-  * @throws IOException 
-  * @return Returns the directory under which reside the WAR files 
-  * we're to load into the servlet container. 
-  */
- publicstatic File getWarsdir()    
- throws IOException {    
- return getSubDir("webapps");    
- }    
===========================================
8.配置文件进行修改,找到conf下heritrix.properties文件 
Java代码 
- //设置用户密码  
- heritrix.cmdline.admin = admin:admin    
- //设置端口  
- heritrix.cmdline.port = 8080
9.对工程进行jar包引入，将lib下面所有的jar包引入工程。 
10.在工程中找到org.archive.crawler.Heritrix.java右键选运行方式配置 
选择Classpath 
选择User Entries -- Advanced 
选择Add Folders 将conf文件夹添加进去。 
点击Run 开始运行 
Java代码 
- 05:22:32.875 EVENT  Starting Jetty/4.2.23
- 05:22:32.937 WARN!! Delete existing temp dir C:/DOCUME~1/ADMINI~1/LOCALS~1/Temp/Jetty_127_0_0_1_8080__ for WebApplicationContext[/,jar:file:/D:/workspace/jcjcd/heritrixDemo/webapps/admin.war!/]    
- 05:22:33.062 EVENT  Started WebApplicationContext[/,Heritrix Console]    
- 05:22:33.156 EVENT  Started SocketListener on 127.0.0.1:8080
- 05:22:33.156 EVENT  Started org.mortbay.jetty.Server@1f6f0bf
- Heritrix version: @VERSION@    
到目前我们已经完成Heritrix在Eclipse下的配置。 
问题解决：
1. "【8】：在window->Preference->Java->Compiler中，将Compiler compliance level 设为5.0，也就是JDK1.5兼容的语法。目的是使Heritrix编译通过。否则Eclipse可能不认assert这个关键字。"  但出现以下错误： 
The method XXXXXX   of type XXXXXXXXX must override a superclass method 
那是因为你的Compiler 是jdk1.5，只要把它改为 1.6就可以了  方法:1. window ->preferences... -> java -> Compiler 
                                                                                              2. Compiler compliance level : 6.0 3.Apply 4. ok 5.完成 
2. 在浏览器地址栏输入：http://127.0.0.1:8080/即可登录WUI，输入用户名：admin密码：admin，即可登录设置抓取任务.
========================================================================
========================================================================
第二种设置方法：**!!!**
[http://www.360doc.com/content/10/0913/18/2793979_53385587.shtml](http://www.360doc.com/content/10/0913/18/2793979_53385587.shtml)
Heritrix eclipse安装、配置（原创）
摘抄： 如果利用heritrix-1.14.4.zip 安装配置
**如果顺利，就可以运行了。系统自带脚本的运行如下：**
           将cmd定位到 F:/heritrix/bin下，执行 "heritrix --admin=admin:admin" 命令，即可启动 heritrix，
     有一点需要注意，heritrix默认使用8080端口，要保证系统端口没有冲突。之后便可以访问[http://127.0.0.1:8080](http://127.0.0.1:8080/)或[http://localhost:8080/](http://localhost:8080/)使用 heritrix 提供的WUI，即Web管理端。并且使用"admin/admin"登录。
引用：
讲完了通过命令行方式启动的Heritrix，当然要讲一下如何在Eclipse中配置Heritrix的开发环境，因为可能需要对代码进行调试，甚至修改一些它的源代码，来达到所需要的效果。下面来研究一下Heritrix的下载包。
============================================================
=============================================================
附录：
[http://www.javaeye.com/topic/410475](http://www.javaeye.com/topic/410475)
【1】：先来认识下什么是Hertirex吧！
是IA的开放源代码，可扩展的，基于整个Web的，归档网络爬虫工程
   Heritrix工程始于2003年初，IA的目的是开发一个特殊的爬虫，对网上的
   资源进行归档，建立网络数字图书馆，在过去的6年里，IA已经建立了400TB的数据。
   IA期望他们的crawler包含以下几种：
   宽带爬虫：能够以更高的带宽去站点爬。
   主题爬虫：集中于被选择的问题。
   持续爬虫：不仅仅爬更当前的网页还负责爬日后更新的网页。
   实验爬虫：对爬虫技术进行实验，以决定该爬什么，以及对不同协议的爬虫      爬行结果进行分析的。
   Heritrix的主页是[http://crawler.archive.org](http://crawler.archive.org/)
  Heritrix是一个爬虫框架，可加如入一些可互换的组件。
  它的执行是递归进行的，主要有以下几步：
     1。在预定的URI中选择一个。
     2。获取URI
     3。分析，归档结果
     4。选择已经发现的感兴趣的URI。加入预定队列。
     5。标记已经处理过的URI
 Heritrix主要有三大部件：范围部件，边界部件，处理器链
    范围部件：主要按照规则决定将哪个URI入队。
    边界部件：跟踪哪个预定的URI将被收集，和已经被收集的URI，选择下一个   URI，剔除已经处理过的URI。
    处理器链：包含若干处理器获取URI，分析结果，将它们传回给边界部件
Heritrix的其余部件有：
   WEB管理控制台：大多数都是单机的WEB应用，内嵌JAVA HTTP 服务器。
   操作者可以通过选择Crawler命令来操作控制台。
   Crawler命令处理部件：包含足够的信息创建要爬的URI。
   Servercache（处理器缓存）存放服务器的持久信息，能够被爬行部件随时查到，包括IP地址，历史记录，机器人策略。
处理器链：
            预取链：主要是做一些准备工作，例如，对处理进行延迟和重新处理，否决随后的操作。
            提取链：主要是获得资源，进行DNS转换，填写请求和响应表单
            抽取链：当提取完成时，抽取感兴趣的HTML，JavaScript，通常那里有新的也适合的URI，此时URI仅仅被发现，不会被评估
            写链：存储爬行结果，返回内容和抽取特性，过滤完存储。
            提交链：做最后的维护，例如，测试那些不在范围内的，提交给边界部件
 Heritrix 1.0.0包含以下关键特性：
              1.用单个爬虫在多个独立的站点一直不断的进行递归的爬。
              2。从一个提供的种子进行爬，收集站点内的精确URI，和精确主机。
              3。主要是用广度优先算法进行处理。
              4。主要部件都是高效的可扩展的
              5。良好的配置，包括：
                  a。可设置输出日志，归档文件和临时文件的位置
                  b。可设置下载的最大字节，最大数量的下载文档，和最大的下载时间。
                  c。可设置工作线程数量。
                  d。可设置所利用的带宽的上界。
                  e。可在设置之后一定时间重新选择。
                  f。包含一些可设置的过滤机制，表达方式，URI路径深度选择等等。
Heritrix的局限：
               1。单实例的爬虫，之间不能进行合作。
               2。在有限的机器资源的情况下，却要复杂的操作。
               3。只有官方支持，仅仅在Linux上进行了测试。
               4。每个爬虫是单独进行工作的，没有对更新进行修订。                      
               5。在硬件和系统失败时，恢复能力很差。
               6。很少的时间用来优化性能。
【2】：配置在我们的工程当中：
(1):新建一个工程：Hertirex.(no web project).
(2):在这里下载最新的Hertirex吧：[http://sourceforge.net/project/showfiles.php?group_id=73833&package_id=73980](http://sourceforge.net/project/showfiles.php?group_id=73833&package_id=73980)
【3】:heritrix-1.14.3.zip  和  heritrix-1.14.3-src.zip。
（1）。先下载heritrix-1.14.3-src.zip和heritrix-1.14.3.zip。
（2）解压heritrix-1.14.3.jar到/heritrix-1.14.3/heritrix-1.14.3。解压得到的文件夹结构如图：![](http://images.cnblogs.com/cnblogs_com/hejycpu/1.jpg)
【3】：把以上目录的所有文件拷贝到heritrix-1.14.3-src/heritrix-1.14.3src/java文件夹下。目的是将java文件夹配置成一个完整的工程目录。
【4】：把/heritrix-1.14.3-src/heritrix-1.14.3/lib文件夹 拷贝到/heritrix-1.14.3-src/heritrix-1.14.3/src/java文件夹下。把lib下的jar包放入工程内部是为了便于管理。
【5】：把heritrix-1.14.3-src/heritrix-1.14.3/src/webapps文件夹拷贝到/heritrix-1.14.3-src/heritrix-1.14.3/src/java文件夹下。此时工程目录基本上完整了。得到的效果如图：
![](http://images.cnblogs.com/cnblogs_com/hejycpu/22.jpg)
【6】：在Eclipse中新建一个Java Project，选择从现有源代码建立，源代码的路径指向/heritrix-1.14.3-src/heritrix-1.14.3/src/java。命名为Heritrix，点击ok。
【7】：这时在package explorer中应该会出现大量的编译错误标记。原因是没有把对应的jar包导入项目。在Java Build Path的Library选项卡中，把项目lib文件夹下的jar包全部加入Build Path。
【8】：在window->Preference->Java->Compiler中，将Compiler compliance level 设为5.0，也就是JDK1.5兼容的语法。目的是使Heritrix编译通过。否则Eclipse可能不认assert这个关键字。
【9】：用鼠标选中**/Heritrix/org/archive/crawler/Heritrix.java**，右键选择Run As Java Application，Console应该是没有输出的。原因是没有提供运行的参数（用户名/口令）。 一种方法是，修改项目根文件夹下的heritrix.properties文件，设置“heritrix.cmdline.admin = admin:admin”。正常运行时的输出如下：
Heritrix version: 1.14.3
【10】：另外一种设定用户名和密码的方式是，在Run Dialog中的Arguments参数选项卡中，输入-admin=admin:admin，单击Apply，再单击Run即可运行。
【11】：运行Heritrix，在浏览器中输入[http://localhost:8080/](http://localhost:8080/)，用户名/密码为“admin/admin"

