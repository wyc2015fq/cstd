# XP下IIS5运行MVC站点 - weixin_33985507的博客 - CSDN博客
2012年04月04日 22:36:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
在XP下，IIS5是如何运行MVC站点的呢，在2003下的IIS6里很容易就可以运行MVC，但是IIS5中需要配置一下扩展名才行
选选择“默认网站”右键选择“属性”，出现下面界面后，选择“主目录”，然后选择“配置”
![image](https://images.cnblogs.com/cnblogs_com/lori/201204/20120404103202759.png)
点击“配置”后出现下面界面
![image](https://images.cnblogs.com/cnblogs_com/lori/201204/201204042222583702.png)
可以添加几个你在MVC路由中配置的扩展名，可以是aspx,asp,html,shtml等等
![image](https://images.cnblogs.com/cnblogs_com/lori/201204/201204042236043888.png)
在可执行文件中选择.net4.0的aspnet_isapi.dll文件即可。
