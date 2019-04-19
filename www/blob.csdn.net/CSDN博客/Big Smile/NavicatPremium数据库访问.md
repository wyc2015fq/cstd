# NavicatPremium数据库访问 - Big Smile - CSDN博客
2016年02月28日 23:26:11[王啸tr1912](https://me.csdn.net/tr1912)阅读数：619
评教的时候接触了Navicat这个软件，一直用到了现在，感觉还不错，现在来分享一下。
# 一、软件简介
        这个软件从名字来看有些晦涩，但是它确实是访问和操作数据库的利器，对了它就是一款数据库访问软件，访问数据库的速度绝对是比原有的软件要快的。主界面如图：
![](https://img-blog.csdn.net/20160228225623955?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
从界面就可以看出来它有很多的功能，包括了数据表的访问，数据库的管理，数据视图的建立，数据模型的建立，数据库的备份等功能。
二、数据库的连接
        它的第一个优点就在于这个软件可以访问各种现在主流的数据库，sqlserver，oracle，MySQL等。![](https://img-blog.csdn.net/20160228230104221?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


然后就是他的连接方式也是很简单的，点击进入相应的数据库，就可以看到响应的配置，现在以SqlServer来举例子：
![](https://img-blog.csdn.net/20160228231010780?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
输入自己命名的连接名和相应的连接字符串，就可以进行连接了，这个时候我们就可以看到相应服务器上的所有数据库了。
![](https://img-blog.csdn.net/20160228231807510?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
        打开这个软件的时候，默认所有的库都是不连接的，如第一张图，Loacl就是有颜色的，是已经连接上的，而**考试服务器**就是灰色的，没有连接上，这个设定的好处就在于他能快速的打开软件来进行设置，也可以让自己选择想要连接的数据库，避免不必要的资源浪费。
# 三、优缺点
**        优点**：这款软件最强大的地方就是可以随时访问不同厂家的数据库，并且启动异常的快速，浏览数据非常的方便，也可以执行查询语句等基本的数据库操作，也可以设计数据库，非常的方便。所以这款软件的最方便的地方就在于数据查询浏览和轻微的数据操作，尤其是远程的情况下要管理多个不同的数据库的情况。
![](https://img-blog.csdn.net/20160228232052310?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
方便的数据查询界面。
**        缺点：**缺点就是和数据服务不是连接的，数据更新需要手动更新，并且也用户的权限也可能遭到限制，又些数据库官方软件中有的详细功能他是没有的，备份数据库的方式单一等缺点。
但是对于我们来说，做项目看数据库的话足矣。
# 四、总结
        这款软件的轻量性和快速性都可以让你的数据访问更加的高效便捷，多数据库的管理也是可以的，在适当的时候要用用这个软件。
