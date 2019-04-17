# MFC应用程序打包发布教程 - 勿在浮砂筑高台 - CSDN博客





2015年11月12日 17:50:27[勿在浮砂筑高台](https://me.csdn.net/luoshixian099)阅读数：13148标签：[MFC																[打包发布](https://so.csdn.net/so/search/s.do?q=打包发布&t=blog)](https://so.csdn.net/so/search/s.do?q=MFC&t=blog)
个人分类：[MFC](https://blog.csdn.net/luoshixian099/article/category/5941469)








MFC应用程序打包发布

转载请注明出处：http://blog.csdn.net/luoshixian099/article/details/49766051


本篇文章介绍如何把做好的MFC软件打包，变成一个安装文件，方便在其他电脑上运行。使用上篇文章制作好的"视频播放器"工程作为例子。

**1.在同一个解决方案下，新建一个Setup工程，工程名为VPSetup;**

![](https://img-blog.csdn.net/20151110222259338)





**2.右击VPSetup工程->view->文件系统**

如下图所示选择项目输出，选择VideoPlayer工程作为作为主输出




![](https://img-blog.csdn.net/20151110222820535)




**3.它会自动检测工程中调用的链接库，安装完成后会自动释放到指定文件夹下。如果缺少部分链接库，也可以手动添加进去。**

![](https://img-blog.csdn.net/20151110224327320)




**4.为用户桌面上和开始菜单栏设置启动快捷方式；**

右击应用主输出文件（上图"应用程序"标示），创建两份shortcut，重命名为 "视频播放器”分别剪切放在“用户桌面”文件夹和"开始菜单栏"文件夹。可以提前在"开始菜单栏"新建一个“视频播放器”文件夹；




**5.为应用程序添加卸载功能；**

在C:\Windows\System32 文件下找到msiexec.exe文件，添加进主文件夹，同时创建一个shortcut，重命名为”卸载视频播放器“，剪切放在"开始菜单栏"文件夹下。

![](https://img-blog.csdn.net/20151112164240178)


右击查看VPSetup工程属性，找到产品代码(ProductCode),复制到“卸载播放器”的参数里，前面加上“/x”；

![](https://img-blog.csdn.net/20151112165315966)![](https://img-blog.csdn.net/20151112173118944)





**6.为应用程序添加图标；**

先把图标加入到主文件夹，然后选择在“用户桌面”和“开始菜单”文件夹下，更改“视频播放器”的属性即可；




**7.还有其他很多属性可以更改，包括安装文件夹路径，以及安装界面等。**

![](https://img-blog.csdn.net/20151112171228087)





**8.选择Release版本，编译后即可产生安装文件,发布Release文件下的两个文件即可；**

![](https://img-blog.csdn.net/20151112173224083)











