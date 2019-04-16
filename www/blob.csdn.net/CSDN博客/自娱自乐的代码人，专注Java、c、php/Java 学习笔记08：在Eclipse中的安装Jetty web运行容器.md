# Java 学习笔记08：在Eclipse中的安装Jetty web运行容器 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年11月21日 13:11:49[initphp](https://me.csdn.net/initphp)阅读数：1850
所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)









在eclipse中直接可以安装Jetty容器


- 首先打开eclipse中的“帮助” - “安装新软件”

![](https://img-my.csdn.net/uploads/201211/21/1353474532_7173.jpg)


- 在Work with中输入：http://run-jetty-run.googlecode.com/svn/trunk/updatesite，选择1.3版本

![](https://img-my.csdn.net/uploads/201211/21/1353474597_6178.jpg)



- 点击下一步，之后Eclipse会检查，一些前置依赖，没有它会自动下载，下载完成之后如下图。

![](https://img-my.csdn.net/uploads/201211/21/1353474655_8701.jpg)



- 继续点击下一步，并且同意

![](https://img-my.csdn.net/uploads/201211/21/1353474694_7956.jpg)



- 点击完成，开始安装，需要等待一些时间。

![](https://img-my.csdn.net/uploads/201211/21/1353474741_3423.jpg)



- 重启eclipse

![](https://img-my.csdn.net/uploads/201211/21/1353474781_4504.jpg)



- 点击项目，运行配置，使用Jetty容器，配置项目参数
![](https://img-my.csdn.net/uploads/201211/21/1353474865_1190.jpg)



- 假如提示“ Must specify at least one scan interval seconds”，则需要设置时间多少秒



- ![](https://img-my.csdn.net/uploads/201211/21/1353474955_9623.jpg)



- 最后，你可以运行了！





