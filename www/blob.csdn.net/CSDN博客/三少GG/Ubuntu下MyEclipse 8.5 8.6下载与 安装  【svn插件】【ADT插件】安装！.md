# Ubuntu下MyEclipse 8.5  8.6下载与 安装 || 【svn插件】【ADT插件】安装！ - 三少GG - CSDN博客
2010年12月17日 14:01:00[三少GG](https://me.csdn.net/scut1135)阅读数：11620
**============///////////////////////=======**
**Attention！更改myeclipse-installer的权限：右键->属性->权限允许以程序执行文件 打勾(之前未改所以运行不了 2011.1.5）**
**============///////////////////////=================**
**svn安装：2011.12.14**
**MyEclipse 8.6安装svn插件，反反复复折腾了好几次。**
自从MyEclipse8版本以上，安装插件变得非常麻烦，需要通过MyEclipse Configuration Center进行安装，很多安装不成功，网上google发现是因为MyEclipse网站访问被屏蔽的原因。
后来上网找了多种安装SVN插件的方法，都不成功后，非常郁闷，经过多次的摸索后，终于找到安装的方式，赶快分享一下，步骤如下：
1.在MyEclipse安装目录的dropins 下新建目录 svn ， 在svn目录下新建eclipse目录。
2.解压SVN的插件site-1.6.13.zip， 将解压后的 features 和plugins 目录复制到 dropins\svn\eclipse 目录下， 注意：*.xml不需要复制过去。
3.重启MyEclipse后，MyEclipse提示Mylyn3.0的安装不成功， svn安装成功。
终于可以在MyEclipse8.6中使用svn插件了。
**注意必须要用svn插件 1.4.8版本（site-1.4.8.zip），1.6和1.8都有问题！！！**
**+++++++++++++++++//////////////////////+++++++++++++++++++++**
**MyEclipse   8.6离线下载adt，解压后放入dropin . 详情如下：**
**[http://archive.cnblogs.com/a/2251000/](http://archive.cnblogs.com/a/2251000/)**
**最近发现许多人都在问 MyEclipse 搭建 ADT 的问题，自己也找到了解决方案，希望公布给大家尝试，但不保证完全没问题。**
**我曾经说到 8.5 / 8.6 搭建 ADT，一方面因为中国大陆 GFW 的关系，另一方面也因为 MyEclipse 的软件中心的确有问题。详见：[http://www.littledai.com/20110204/232822/449](http://www.littledai.com/20110204/232822/449)当然，自己下载的 **ADT 压缩包内我们只需要 plugins 和 features 两个文件夹**即可。将**这两个文件夹装入 ADT 文件夹（自己建立）中。然后将 ADT 文件夹装入 MyEclipse 8.6 安装目录下的 dropins 文件夹。**先不要急着启动 MyEclipse，接着我们要进入和 dropins 同级的 configuration 文件夹，删除 org.eclipse.update 文件夹。这步很重要！然后我们就可以进入 MyEclipse 选择 ADT 了。**
**方法其实很简单，在早期版本中我们把 plugins 和 features 覆盖 MyEclipse 自身的文件夹即可，而 8.x 版本则不接受覆盖的方式了。但它提供了 dropins 文件夹来存放自定义插件（9.0 没有，所以具体方法待研究）。而关键的删除 org.eclipse.update 的操作是方式 MyEclipse 连接互联网来更新我们自定义的插件，一旦检查更新后我们的自定义插件就会失效了。**
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## [MyEclipse 8.6下载](http://xiexiejiao.cn/java/myeclipse-8-6-release-download.html)
[http://xiexiejiao.cn/java/myeclipse-8-6-release-download.html](http://xiexiejiao.cn/java/myeclipse-8-6-release-download.html)
**MyEclipse 8.6**在MyEclipse 8.5的基础上进行了诸多改进，想了解MyEclipse 8.6的主要改进可以参考笔者之前发布的“[**MyEclipse 8.6 M1下载**](http://xiexiejiao.cn/java/myeclipse-8-6-m1-release-download.html)”的介绍，不过让笔者处于意料的是MyEclipse 8.6并没有选择最近刚刚发布的Eclipse 3.6作为基础，而仍然选择了Eclipse
 3.5.2作为核心。想必在MyEclipse下一个版本中应该才会升级到Eclipse 3.6吧。另外值得一提的是MyEclipse已经意识到了其产品庞大的体积，因此官方已经很贴心的提供了自定义下载安装的功能，用户可以选择自己需要的功能插件组合下载，这样整体下载的体积会减少很多，也更方便一些。不过该功能需要访问MyEclipse官方网站进行定制，因此想尝试该功能的朋友需要翻越伟大的墙才可以。
**MyEclipse 8.6下载**地址：
**MyEclipse 8.6 for Windows下载**
[MyEclipse 8.6下载](http://downloads.myeclipseide.com/downloads/products/eworkbench/galileo/myeclipse-8.6.0-win32.exe)(window)
MyEclipse 8.6 文件大小:832.13 MB
MyEclipse 8.6 MD5 : 82801ab5ce7684a22651a6865f79ff3d
**MyEclipse 8.6 for [Linux](http://xiexiejiao.cn/)下载**
[MyEclipse 8.6下载](http://downloads.myeclipseide.com/downloads/products/eworkbench/galileo/myeclipse-8.6.0-linux-gtk-x86.tgz)(linux 32bit)
[MyEclipse 8.6下载](http://downloads.myeclipseide.com/downloads/products/eworkbench/galileo/myeclipse-8.6.0-linux-gtk-x86_64.tgz)(linux 64bit)
MyEclipse 8.6 文件大小: 865.09 MB
MyEclipse 8.6 MD5 : 32Bit:b4baeea6dccc713df78bf77f231a0b57 |64 BIt: 79699ff515cb1a7b1d15df46d8337495
**MyEclipse 8.6.1 注册码： windows-->Preferences-->MyEclipse-->Subscription**
**Subscriber: oyksoftSubscription Code: bLR8ZC-855444-6867865049821387**
=======================================================================++++++++++++++++++++++++++++++++++++++++++++
///////////////////////////////////////////////////////////////////////////////////////
1.  8.5 下载地址：打开网页
[http://www.hengxinsoft.com/2010/03/myeclipse-85-%E4%B8%8B%E8%BD%BD%E5%9C%B0%E5%9D%80/](http://www.hengxinsoft.com/2010/03/myeclipse-85-%E4%B8%8B%E8%BD%BD%E5%9C%B0%E5%9D%80/)
下载 linux（也有windows）版本
2.MyEclipse 8.5注册码 
name:myeclipse8.5 
code:zLR8ZC-855550-68567156703100078 
name:52accptech 
code:0LR8ZC-855550-68567157524981450 
name:bingchuan 
code:oLR8ZC-855550-68567157669572882 
把注册码贴到Window-->preferences-->myeclipse-->subscription里
3.在Ubuntu 10.04中安装MyEclipse 8.5
最近打算把开发环境换到Linux平台，刚脆就搬到新装的[Ubuntu](http://www.linuxidc.com/topicnews.aspx?tid=2) 10.04中。
新的[Ubuntu](http://www.linuxidc.com/topicnews.aspx?tid=2) 系统中去除了sun的jdk，取而代之的是OpenJDK，但这个并不适合用来开发。
首先在新立得中搜索openjdk全部删除。并添加软件源来安装sun-java6-jdk
root@copfee:~# add-apt-repository "deb http://archive.canonical.com/ubuntu maverick partner"
root@copfee:~# apt-get update
root@copfee:~# apt-get install sun-java6-jdk
安装完成后可以输入
root@copfee:~# java -version
确认安装成功。如果有OpenJDK字样说明openjdk没删除干净。
将下载好的myeclipse-8.5.0-linux-gtk-x86.tar解压，双击myeclipse-installer安装。
图形化的界面， 步骤和Windows下是一样的。
过几分钟就可以看到熟悉的界面拉。
启动速度感觉比Windows下的快很多。
![在Ubuntu 10.04中安装MyEclipse 8.5](http://www.linuxidc.com/upload/2010_05/10050605514427.png)
附：
1.请问下新立德软件包里标记以便删除与标记以便彻底删除的区别在哪里，如果选择删除是不是会不会留下一些文件？彻底删除比删除多删除了什么？
以便删除
会保留一些系统配置文件
彻底删除
全部干掉
如果只是普通删除，配置文件存在。甚至安装包还留着（这个有设置的）
彻底删除就会删得彻底。其实，有时候即使彻底删除，配置文件还是没有彻底消失，要自己手动去删。
配置分为全局配置和个人配置，全局配置在 /etc 下面，彻底删除会清除全局配置；个人配置在 ~/ 下面，无论用什么方法，个人配置始终是保留的，不能通过新立得删除。
2.如何删除程序？
删除下载的软件包应该是sudo apt-get autoclean
删除程序则是sudo apt-get remove 软件包名字。或者在新立德里搜索你要卸载的软件然后取消掉前面的那个钩就好了
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
[MyEclipse的优化](http://blog.163.com/jackswu@yeah/blog/static/140629123201091691947168/)
1、老是弹出Quick update error 、关闭MyEclipse的Quick Update自动更新功能
这个问题的解决办法是关闭自动更新：
Windows > Preferences > MyEclipse Enterprise Workbench > Community Essentials,然后把选项 ”Search for new features on startup”的前勾去掉即可。
2 、关闭updating indexes
Window > Preferences > Myeclipse Enterprise Workbench > Maven4Myeclipse > Maven>
![MyEclipse 8.6 优化 与 SVN安装 - Jack - Jack Web Develop](http://www.liurain.com/upload/2010/09/2010.9.8-1.jpg)
禁用Download repository index updates on startup 。
注意：强调一个问题，很多人发现无法找到这个，原因其实是它已经关闭了。
3 、关闭MyEclipse的自动validation
validation有很多，包括xml、jsp、jsf、js等等，我们没有必要全部都去自动校验一下，只是需要的时候才会手工校验一下，速度将很快提升好几个档次。
![MyEclipse 8.6 优化 与 SVN安装 - Jack - Jack Web Develop](http://www.liurain.com/upload/2010/09/2010.9.8-2.jpg)
windows > perferences > myeclipse > validation
将Build下全部勾取消，如果你需要验证某个文件的时候，我们可以单独去验证它。方法是：在需要验证的文件上( 右键 -> MyEclipse -> Run Validation 。
4、 启动优化，关闭不需要使用的模块
一个系统20%的功能往往能够满足80%的需求，MyEclipse也不例外，我们在大多数时候只需要20%的系统功能，所以可以将一些不使用的模块禁止加载启动。
![MyEclipse 8.6 优化 与 SVN安装 - Jack - Jack Web Develop](http://www.liurain.com/upload/2010/09/2010.9.8-3.jpg)
Window > Preferences > General > Startup andy Shutdown 在这里列出的是MyEclipse启动时加载的模块。
我这里只让它加载tomcat5，勾选MyEclipse EASIE Tomcat 5。接下来，关闭工作空间启动刷新：
![MyEclipse 8.6 优化 与 SVN安装 - Jack - Jack Web Develop](http://www.liurain.com/upload/2010/09/2010.9.8-4.jpg)
接下来关闭拼写检查：
![MyEclipse 8.6 优化 与 SVN安装 - Jack - Jack Web Develop](http://www.liurain.com/upload/2010/09/2010.9.8-5.jpg)
只需要取消Enable spell checking：
![MyEclipse 8.6 优化 与 SVN安装 - Jack - Jack Web Develop](http://www.liurain.com/upload/2010/09/2010.9.8-6.jpg)
看到auto activation triggers for java之后，在里面输入a-z这些字幕吧，然后选择应用，接着去编辑器里查看提示，上面那个选项是延迟多少时间提示，建议不修改。
![MyEclipse 8.6 优化 与 SVN安装 - Jack - Jack Web Develop](http://www.liurain.com/upload/2010/09/2010.9.8-7.jpg)
如图那样选择就完成了，然后我们到myeclipse安装目录>myeclipse.ini打开修改这个文件，建议在做之前备份，不然错了没法启动。
- -startup  
- ../Common/plugins/org.eclipse.equinox.launcher_1.0.201.R35x_v20090715.jar  
- –launcher.library  
- ../Common/plugins/org.eclipse.equinox.launcher.win32.win32.x86_1.0.200.v20090519  
- -install  
- E:/develop/MYEC85/MyEclipse 8.5 M2  
- -vm  
- E:/develop/MYEC85/Common/binary/com.sun.java.jdk.win32.x86_1.6.0.013/jre/bin/client/jvm.dll  
- -vmargs  
- -Xmx386m  
- -XX:MaxPermSize=386m
- -XX:ReservedCodeCacheSize=64m
主要是修改：
- -Xmx386m  
- -XX:MaxPermSize=386m
- -XX:ReservedCodeCacheSize=256m
【From 网上共享，非原创】
