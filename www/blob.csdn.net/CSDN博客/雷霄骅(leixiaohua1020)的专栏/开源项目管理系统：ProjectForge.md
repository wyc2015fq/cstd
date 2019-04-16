# 开源项目管理系统：ProjectForge - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年12月27日 12:27:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：38
个人分类：[开源项目](https://blog.csdn.net/leixiaohua1020/article/category/1652801)










ProjectForge是一个Java编写的开源的基于Web的项目管理解决方案。包括：工时表(Timesheet)、费用管理、项目甘特图，控制和管理工程分解结构。集成JiRA做为问题管理系统。

![](https://img-blog.csdn.net/20131126213531859)

使用的时候发现，ProjectForge好像对IE内核的浏览器支持不佳，对其他内核的浏览器支持还是不错的。



**ProjectForge体验**

ProjectForge的安装极其简单，几乎什么都不用做。下载独立运行的版本后，得到一个exe文件，运行后出现如下界面。

![](https://img-blog.csdn.net/20131221163413390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

单击“Start”之后，就可以开始准备启动服务器了。第一次运行的时候，会出现安装界面，要求设置ProjectForge安装路径。

![](https://img-blog.csdn.net/20131221163417687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

安装完成后，单击“Launch”，启动服务器。

![](https://img-blog.csdn.net/20131221163422203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

访问http://localhost:8080/ProjectForge/，打开ProjectForge页面。注意不是http://localhost:8080/。

第一次运行的话，会出现配置页面。要求设置管理员的用户名密码等等操作。

![](https://img-blog.csdn.net/20131221163425328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

设置完成后，进入系统。可以看到菜单栏包含：Tasks，Calendar，Addresses，Books。下图所示是Task界面截图，该界面主要显示了包含的任务。

![](https://img-blog.csdn.net/20131221163428812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下图所示是Calendar界面截图，该界面主要用日历了方式显示了日程信息。

![](https://img-blog.csdn.net/20131221163432250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

单击日历上的一天，弹出以下界面。可以编辑任务信息。

![](https://img-blog.csdn.net/20131221163435812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下图所示是Address界面截图，该界面主要显示了联系人的信息。

![](https://img-blog.csdn.net/20131221163440609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

点击一个联系人，弹出以下界面，可以编辑联系人信息。

![](https://img-blog.csdn.net/20131221163443984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下图所示是Books界面截图，该界面主要显示了出版书籍的信息。这个的作用是什么？目前还没太弄懂。

![](https://img-blog.csdn.net/20131221163454453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

单击页面左上角的版本信息，可以查看ProjectForge的documenation（文档）。ProjectForge的文档很全，包含了面向使用者的，面向管理员的以及面向开发者的。

遇到不明白的问题，看文档基本上都能解决。

![](https://img-blog.csdn.net/20131221163458031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在菜单栏右边，有一个按钮“Main menu”（感觉有点像Windows里面的开始按钮）。单击该按钮的话，会弹出一个巨大的菜单，包含了ProjectForge方方面面的功能。由于功能太多，就不一一体验了。

![](https://img-blog.csdn.net/20131221163501531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

单击“Project Management”的“Gantt”（甘特图）后，出现如下界面，是和甘特图有关的信息。

![](https://img-blog.csdn.net/20131221163504781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







**Demo系统:**

[https://www.projectforge.org/demo/](https://www.projectforge.org/demo/)

Login ：demo

Password ：demo123



**ProjectForge包含下列版本：**


|**种类**|**描述**|
|----|----|
|Windows|Windows 下独立运行的版本（零配置）。包含了war包。|
|MacOS X|MacOS X下独立运行的版本（零配置）。包含了war包。|
|Generic|支持所有Java平台的独立运行的版本（零配置）。包含了war包。|
|WAR|完整的war包 (for HTTP and HTTPS).|
|WAR|需要SSL的完整的war包(HTTPS).|
|Sources|源代码|

**源代码位于：**

[https://github.com/micromata/projectforge-webapp](https://github.com/micromata/projectforge-webapp)



**文档：**
|**名称**|**语言**|**格式**|**描述**|
|----|----|----|----|
|Project Forge|English|[html](http://www.projectforge.org/doc/ProjectForge.html),[pdf](http://www.projectforge.org/doc/ProjectForge.pdf)|给新手提供一个快速指南.|
|User guide|English|[html](http://www.projectforge.org/doc/UserGuide.html),[pdf](http://www.projectforge.org/doc/UserGuide.pdf)|描述了 ProjectForge的功能和概念|
|Administration Guide|English|[html](http://www.projectforge.org/doc/AdministrationGuide.html),[pdf](http://www.projectforge.org/doc/AdministrationGuide.pdf)|管理员的文档：安装，维护，备份，修复|
|Release-News|English|[html](http://www.projectforge.org/doc/News.html)|版本说明|
|Admin Logbuch|English|[html](http://www.projectforge.org/doc/AdminLogbuch.html)|更改日志|
|Developer Guide|English|[html](http://www.projectforge.org/doc/DeveloperGuide.html),[pdf](http://www.projectforge.org/doc/DeveloperGuide.pdf)|开发的提示|
|Sure Fire|English|[html](http://www.projectforge.org/doc/site/project-reports.html)||



官网地址：[http://www.projectforge.org/](http://www.projectforge.org/)




