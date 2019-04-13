
# ArcSDE、ArcGis Engine、ArcGis Desktop的授权配置 - 西西吹雪 - 博客园
# [编程　&　生活      --       西西吹雪](https://www.cnblogs.com/watsonyin/)
开心工作，快乐生活  (原创文章，转载请注明出处)

### 导航
[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/watsonyin/)
[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
[联系](https://msg.cnblogs.com/send/%E8%A5%BF%E8%A5%BF%E5%90%B9%E9%9B%AA)[订阅](https://www.cnblogs.com/watsonyin/rss)![订阅](//www.cnblogs.com/images/xml.gif)[管理](https://i.cnblogs.com/)
统计
随笔 -		88文章 -		10评论 -		1793引用 -		18
公告

[ArcSDE、ArcGis Engine、ArcGis Desktop的授权配置](https://www.cnblogs.com/watsonyin/archive/2005/08/09/210745.html)
ArcSDE、ArcGis Engine、ArcGis Desktop的授权配置
（由于本人使用的并非正版本ESRI软件，所以使用比较麻烦，以下简单记录本人的一些授权方面的设置，希望对有类似情况的朋友有所参考作用。）
ESRI公司的几个产品都要授权才能使用，如果不能很好的配置的话，经常会导致某个软件不能使用。下面是本人机器上已经配置好的情况。
ArcGis Deskop9.0和ArcSDE都是通过License Manage Tools工具还实现的。请见下图：
![](https://images.cnblogs.com/cnblogs_com/watsonyin/01.jpg)
可以看到其中有两个选项，它们都对应于Windows服务中的一个同名的服务。其中ArcGis License Manager服务用于对Desktop进行授权管理，而Esri License Manager用于对ArcSDE进行授权管理。
[
](https://images.cnblogs.com/cnblogs_com/watsonyin/01.jpg)选择ArcGis License Manager，然后到Config Services页，查看其设置，如下图所示：
[
](https://images.cnblogs.com/cnblogs_com/watsonyin/01.jpg)
[
](https://images.cnblogs.com/cnblogs_com/watsonyin/01.jpg)![](https://images.cnblogs.com/cnblogs_com/watsonyin/02.jpg)
再看看Esri License Manage的设置：
[
](https://images.cnblogs.com/cnblogs_com/watsonyin/02.jpg)
[
](https://images.cnblogs.com/cnblogs_com/watsonyin/02.jpg)![](https://images.cnblogs.com/cnblogs_com/watsonyin/03.jpg)
主要有三项设置，其含义就不必多说了。请勿必保证path to imgrd.exe file和path to license file项一定要正确，本文提供界面中的设置供参考。
再来看一看ArcGis Engine的授权管理。安装好ArcGis Engine之后，在ArcGis程序组中，可以看到software Authorization程序，就是它来负责对ArcGis Engine进行授权管理了。打开该程序，界面如下：
![](https://images.cnblogs.com/cnblogs_com/watsonyin/04.jpg)
选择第二项，然后点击“下一步”，界面如下：
![](https://images.cnblogs.com/cnblogs_com/watsonyin/05.jpg)
选择授权文件，然后再点击“下一步”，就成功了。
就这么简单，但前提是您要有相应的授权文件和crack程序。
设置完毕后，如果您的ArcGis License Manager服务是启动状态的话，打开编程的IDE环境，将ArcGis Engine提供的控件添加到窗体中，您会发现，系统提示没有授权(unlicensed)。这时只要停止ArcGis License Manager服务，ArcGis Engine就可以正常使用了。
但停掉该服务，ArcGis Desktop就不能使用了，要使用的话，重新启用该服务好了。
下面顺便提供ArcGis Engine9.0的授权文件下载：
[ArcEngineDKECP100013846.rar](http://watsonyin.cnblogs.com/Files/watsonyin/ArcEngineDKECP100013846.rar)
ArcIMS的授权文件下载
[ArcIMSECP580498550.rar](http://watsonyin.cnblogs.com/Files/watsonyin/ArcIMSECP580498550.rar)
ArcGis 9.1系列授权文件
[ArcGis 9.1全套License.rar](http://watsonyin.cnblogs.com/Files/watsonyin/ArcGis%209.1全套License.rar)





posted on2005-08-09 13:07[西西吹雪](https://www.cnblogs.com/watsonyin/)阅读(...) 评论(...)[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=210745)[收藏](#)


[刷新评论](javascript:void(0);)[刷新页面](#)[返回顶部](#top)






Powered by:
博客园
Copyright © 西西吹雪
