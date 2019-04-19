# 在Mac OS上安装IntelliJ IDEA - 零度的博客专栏 - CSDN博客
2017年05月04日 11:03:40[零度anngle](https://me.csdn.net/zmx729618)阅读数：33281
       这篇文章旨在介绍如何在Mac系统上安装IntelliJ IDEA，至于IntelliJ IDEA的介绍和使用方法，大家另行查阅，本篇的文章不再详细阐述。
　　简短解说，IntelliJ IDEA是可以用来进行java web和android 开发的，当然还有开发特点，但是平时用的多的还是这两个方面。所以，既然要进行java web和android的开发，那么肯定是需要在Mac上安装java环境的，必然也少不了java sdk 和 android sdk。
　　第一步，下载IntelliJ IDEA
　　官网下载地址为：[http://www.jetbrains.com/idea/](http://www.jetbrains.com/idea/)，如果是个人开发android的，可以选择community版，如果是开发java
 web的，那需要下载ultimate版，Mac上的应用都是dmg格式的，大家需看仔细。
　　注册码去这里找：[http://idea.lanyus.com](http://idea.lanyus.com/)，或者用授权服务器激活：http://www.iteblog.com/idea/key.php。
　　下载完成后，一步步安装，让其app在应用程序文件夹中显示。
       服务器激活：
         点击链接下载文件：[链接](https://pan.baidu.com/s/1bZ3upS)（该文件只在Mac上测试）
         添加文件执行权限：
`    chmod +x IntelliJIDEALicenseServer_darwin_amd64`
![](http://static.blog.csdn.net/images/save_snippets_01.png)
         可以用screen在后台执行，避免关闭terminal激活失败
`    screen -dmS IntelliJIDEALicenseServer_darwin_amd64 -d -m`
![](http://static.blog.csdn.net/images/save_snippets_01.png)
        运行该文件
```bash
sudo ./IntelliJIDEALicenseServer_darwin_amd64
```
![](http://static.blog.csdn.net/images/save_snippets_01.png)
现在就可以免费激活JetBrains啦，选License server，下面服务器地址输入“[http://127.0.0.1:1017](http://127.0.0.1:1017/)“
![激活页面](https://img-blog.csdn.net/20170304222131081?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hlbnhpYWJpbmZmZmY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
点击Activate即可。以后打开IDE就可以看到自己的用户名了 
　　第二步，下载java sdk
　　下载地址为： [http://www.oracle.com/index.html](http://www.oracle.com/index.html)，
　　我个人下载的jdk是1.8的，大家可自行选择。下载完成后，自行安装，
　　第三步，下载android sdk
　　由于天朝屏蔽了Google的很多服务，大家可以FQ去下载，这里提供一个不FQ下载的地址：[http://www.cr173.com/soft/78226.html](http://www.cr173.com/soft/78226.html)，这个是从网上搜到的。
　　下载完之后，解压到自己指定的目录下面。
　　第四步，导入java sdk 和 android sdk
　　在应用程序中，打开intellJ IDEA ，按照下图进行操作
![](http://images2015.cnblogs.com/blog/780021/201606/780021-20160625161931016-399641762.png)
　　就会进入到“工程设置”界面，在platform settings的sdks中，引入jdk 和 android sdk。
![](http://images2015.cnblogs.com/blog/780021/201606/780021-20160625162318547-2129855185.png)
　　引入的时候，需要选择解压出来的jdk 安装目录和android sdk 的安装目录。
　　注意：其中，在引入android sdk的目录时，会调用出android sdk manager的图形工具软件。
![](http://images2015.cnblogs.com/blog/780021/201606/780021-20160625163344735-286087134.png)
　　仔细查看每个包的作用是什么，选择所需要用到的包，进行安装，安装完即可。
