# javaweb基础第一课：创建java web项目及项目结构介绍 - PeterBishop - CSDN博客





2018年12月15日 13:48:48[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：72
个人分类：[JavaWeb](https://blog.csdn.net/qq_40061421/article/category/8534452)









  这节课开始我们开始学习java web

  首先学下怎么建一个java web的项目，以idea为例，用eclipse的跟着链接做，大家跟着截图做，用idea的跟着视频做即可。



  Eclispe:

[https://jingyan.baidu.com/article/ce436649f3334e3773afd3e0.html](https://jingyan.baidu.com/article/ce436649f3334e3773afd3e0.html%EF%BC%8C%E8%B7%9F%E7%9D%80%E8%BF%99%E4%B8%AA%E5%81%9A%E5%8D%B3%E5%8F%AF%E3%80%82)

[跟着这个做即可。](https://jingyan.baidu.com/article/ce436649f3334e3773afd3e0.html%EF%BC%8C%E8%B7%9F%E7%9D%80%E8%BF%99%E4%B8%AA%E5%81%9A%E5%8D%B3%E5%8F%AF%E3%80%82)



  Idea看视频

新建项目，项目里选java，然后右边选Web Application

![](https://img-blog.csdnimg.cn/20181215134846529.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)

然后next

然后输入项目名即可



Idea里的java web项目目录结构:

![](https://img-blog.csdnimg.cn/20181215134846409.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)



src放.java文件，即源代码

web文件夹是这个web项目的根路径，web/WEB-INF其实就是/WEB-INF，因为web是根路径，所以/WEB-INF就是web/WEB-INF



WEB-INF用于存放这个web项目必要的信息，包括web.xml,jsp文件，这个项目需要的第三方jar包



web.xml，整个web项目的核心配置文件，这个后面会细细的讲。



index.jsp，这个文件的位置是web/index.jsp，是浏览器访问该项目的欢迎页面。



Web项目必须部署在服务器中，服务器我们使用apache的tomcat8，现在不提供8.0的了，所以我们用8.5版本的



链接:

[https://tomcat.apache.org/download-80.cgi](https://tomcat.apache.org/download-80.cgi)



64位点这个:

![](https://img-blog.csdnimg.cn/20181215134846425.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)



下载完成后直接解压即可。我的tomcat放在f盘，你们自己自定义位置,我用的8.0啊。。和你们统一下，我用8.5吧，暂停下，我去 下载。OK，我下载好了。



![](https://img-blog.csdnimg.cn/20181215134846528.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)



下面说下怎么配tomcat，eclipse的百度，如果eclipse不支持tomcat8，换idea



我们启动的看一下。成功了。

![](https://img-blog.csdnimg.cn/20181215134846436.png)

输出这个就代表运行成功。



下节课开始细讲各个需要用到的知识点。



