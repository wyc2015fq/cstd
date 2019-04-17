# SVN 版本控制 - ChihkAnchor的博客 - CSDN博客





2019年04月01日 08:49:09[Chihk-Anchor](https://me.csdn.net/weixin_40871455)阅读数：16标签：[版本控制																[svn](https://so.csdn.net/so/search/s.do?q=svn&t=blog)](https://so.csdn.net/so/search/s.do?q=版本控制&t=blog)
个人分类：[工具](https://blog.csdn.net/weixin_40871455/article/category/8805161)








******什么是版本控制******

版本控制（Revision Control）：是维护工程蓝图的标准做法，能追踪工程蓝图从诞生一直到定案的过程。是一种记录若干文件内容变化，以便将来查阅特定版本修订情况的系统。也是一种软体工程技巧，籍以在开发的过程中，确保由不同人所编辑的同一档案都得到更新。

******版本控制软件******
- CVS（Concurrent Versions System）代表协作版本系统或者并发版本系统，是一种版本控制系统，方便软件的开发和使用者协同工作。
- VSS （ Visual Source Safe ）只能在windows下,作为 Microsoft Visual Studio 的一名成员，它主要任务就是负责项目文件的管理
- Git是用于Linux内核开发的版本控制工具。它采用了分布式版本库的方式，不必服务器端软件支持，使源代码的发布和交流极其方便。Git的速度很快，这对于诸如Linux kernel这样的大项目来说自然很重要。Git最为出色的是它的合并跟踪（merge tracing）能力。
- SVN（Subversion ），是一个开放源代码的版本控制系统，采用了分支管理系统，它的设计目标就是取代CVS。

![](https://img-blog.csdnimg.cn/20190328165905652.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

安装

![](https://img-blog.csdnimg.cn/20190328170011609.png)

安装成功测试

![](https://img-blog.csdnimg.cn/20190328170117564.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

******创建仓库******

格式：cmd> ** svnadmin create 路径     创建仓库时目录必须是空的**

```bash
svnadmin create F:\repository\svn\bbs
```

![](https://img-blog.csdnimg.cn/20190328170458243.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

******启动******

格式：cmd>  **svnserve  -d  -r  仓库的路径**

-d后台执行

-r版本库的根目录

启动时，指定“仓库路径”不同，可以分为：多仓库和单仓库

**多仓库**

![](https://img-blog.csdnimg.cn/20190328170811202.png)

启动：svnserve -d -r 仓库父目录  ，表示启动时多仓库

例如：svnserve -d -r  F:\repository\svn

访问：svn://localhost:3690/bbs

**单仓库**

![](https://img-blog.csdnimg.cn/20190328171023635.png)

启动：svnserve -d -r 仓库的根  ，表示启动时单仓库

例如：svnserve -d -r   F:\repository\svn

访问：svn://localhost:3690

**操作**

![](https://img-blog.csdnimg.cn/20190328171328644.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

### **checkout**

格式：**svn  checkout  服务器地址  下载地址**

```bash
svn checkout svn://localhost:3690/bbs .
```

![](https://img-blog.csdnimg.cn/20190328172905315.png)

在下载地址目录下会有一个 .svn 的隐藏文件，如果Windows默认关闭查看隐藏文件，需要开启后才能看到

![](https://img-blog.csdnimg.cn/20190328173920307.png)

### commit

格式：**svn commit  资源**

![](https://img-blog.csdnimg.cn/20190328173108462.png)

使用add子命令 添加到本地版本库

![](https://img-blog.csdnimg.cn/20190328173221337.png)

再次commit

![](https://img-blog.csdnimg.cn/20190328173401639.png)

采用 -m  参数设置日志信息

![](https://img-blog.csdnimg.cn/20190328173604438.png)

没有权限

修改权限，设置匿名访问

进入到我们create的svn仓库中找到svnserve.conf文件，F:\repository\svn\bbs\conf\svnserve.conf

![](https://img-blog.csdnimg.cn/20190328174155276.png)

进行如下操作 

![](https://img-blog.csdnimg.cn/2019032817444893.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

再次进行commit

![](https://img-blog.csdnimg.cn/20190328174527885.png)

### update

格式： svn update

![](https://img-blog.csdnimg.cn/20190328174759114.png)




