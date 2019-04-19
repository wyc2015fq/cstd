# IIS应用程序池总是关闭问题 - 左直拳的马桶_日用桶 - CSDN博客
2018年02月25日 18:04:38[左直拳](https://me.csdn.net/leftfist)阅读数：1311
打开本地网站，有的总是提示服务不可用，打开IIS，发现对应的应用程序池已经停止了。开启再运行，马上又停止。
但又不是所有的应用程序池都不行，有几个是好的，将报错的网站对应过去，一切正常。这排除了网站本身的毛病，问题出在应用程序池身上。新建了几个应用程序池，都是不行的。真奇怪。难道系统对应用程序池的数量有限制？删掉几个，还是不行。
查看系统日志，提示：
```
Windows 不能让您登录，因为不能加载您的配置文件。请检查您是否连接到网络，而且网络是否工作正常。
 详细信息 - 系统找不到指定的路径。
```
![这里写图片描述](https://img-blog.csdn.net/20180225174552372?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
我想起来了，年前因为硬盘空间不够，我将c:\users\下的一些文件夹删掉了，里面有许多名字跟应用程序池名称一致。我当时也没细想，就将一些久不使用的干掉了。所以提示找不到配置文件。
那为啥新建的也不行？估计是系统加载的时候发现有故障，所以这个功能就崩溃了。
我将应用程序池的默认设置 - 标识 - 内置账户 改为 localSystem，就可以了。原本默认是ApplicationPoolIdentityID 
![这里写图片描述](https://img-blog.csdn.net/20180225175312477?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这个ApplicationPoolIdentityID是IIS7提升的安全性举措。就是每个应用程序池都对应一个隐形的账号。这样做的好处是，可以针对这个网站的权限作特别的设置。因为这些账号是系统自动生成的，省去了不少麻烦；并且隐形，如果用不着，也省心。但不好的地方就是耗空间，在C:/user下创建了许多文件夹，每个都140多兆。
localSystem，顾名思义，就是当前登录windows的账号了吧。应用程序池用这个账号跑，权限就很大了，就不够安全。不过，我是本机用的，应该没啥问题吧。
有关这个IIS 账号问题，现在已找到[解决办法](http://blog.csdn.net/leftfist/article/details/79426710)。
参考文章： 
[IIS 7.5 下应用程序池的预定义账户](http://www.cnblogs.com/beginor/archive/2012/03/26/2418403.html)
[用户”IIS APPPOOL\xxxxxxxx”登录失败解决方案](http://www.cnblogs.com/VAllen/articles/IIS_Application.html)
