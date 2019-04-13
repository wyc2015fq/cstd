
# SharePoint 2013 本地开发解决方案以及远程调试 - 致林 - 博客园






# [SharePoint 2013 本地开发解决方案以及远程调试](https://www.cnblogs.com/bincoding/p/5711544.html)
转自：http://www.cnblogs.com/jianyus/p/3523387.html
在SharePoint开发中，我们需要在部署有SharePoint环境的服务器中开发，这是一件让人很苦恼的事情，毕竟不能一个项目多人开发配备多台服务器，这就需要本地开发。
本来自己以为SharePoint 2013在不再支持客户端系统的条件下，只能服务器上开发，QQ群里的朋友提醒说也可以本地开发，远程调试，以为自己是知道SharePoint 2010可以通过注册表修改，本地安装SharePoint的，顺着这个思路，经过一番谷歌/bing，然后实现了，在此总结一下，分享给大家。
思  路
本地安装Visual Studio 2013，导入SharePoint注册表，复制必须的dll，使用远程调试；
缺  陷
不能本地创建SharePoint解决方案，只能编辑；
场  景
可以使用源代码管理，将文件下载到本地进行开发，然后在服务器上获取，打包/部署，然后本地调试。
1、在本地尝试使用VS开发SharePoint的朋友，都知道新建/打开SharePoint解决方案，会报如下错误：
![](https://images0.cnblogs.com/blog/256729/201401/170024347059.jpg)
2、在SharePoint 服务器中，打开注册表，找到如下位置：
我查资料没有查到2013版本的，但是2010版本说要把服务器上Web Server Extensions下面，14这个节点导出，然后导入到本地即可；
由于2013版本，Web Server Extensions下面，有14和15两个文件夹，自己不知道导入哪个，索性就把Shared Tools整个节点导出，然后导入到本机（Win8）；
![wps_clip_image-30078[5]](https://images0.cnblogs.com/blog/256729/201401/170011394869.png)
3、导入注册表，新建SharePoint解决方案依然报错，因为默认需要连接调试的站点，而本地没有，必然报错（或许修改不让默认连接，也能解决，待解决）；
但是，在服务器上创建一个解决方案，本地编辑已经可以了，但是会报如下错误，很正常，因为本地没有环境必然没有这两个引用，去服务器拷过来，放到bin/debug下：
![wps_clip_image-11724[4]](https://images0.cnblogs.com/blog/256729/201401/170011404086.png)
4、这两个文件所在位置，服务器物理路径15\ISAPI下，如下图：
![wps_clip_image-19334[4]](https://images0.cnblogs.com/blog/256729/201401/170011414239.png)
5、本地重新生成，VS显示全部重新生成已成功，如下图：
这样，我们本地编写SharePoint 2013的解决方案，已经可以了，下面我们进行远程调试的配置：
![wps_clip_image-17886[4]](https://images0.cnblogs.com/blog/256729/201401/170011427204.png)
6、使用远程调试，有几个步骤需要配置：
a.本地/服务器必须有相同的管理员帐号；
b.将本地VS中的Remote Debugger拷贝到服务器，并执行msvsmon.exe；
c.设置msvsmon.exe为windows认证，将调试使用账号授权允许调试；
d.暂时关闭两台机器防火墙，或者允许调试通过；
7、服务器和本地，共同创建一个账号mydebug（用户名/密码相同）；
我在完成这步以后，测试了可以不必使用这个账号调试，但是这个账号两边必须有，而且必须授权管理员；
![wps_clip_image-9712[4]](https://images0.cnblogs.com/blog/256729/201401/170011437050.png)
8、找到本地的Remote Debugger，拷贝到服务器，路径如下：
C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\IDE\Remote Debugger，调试的时候，注意使用里面64位进行调试；
![wps_clip_image-31575[4]](https://images0.cnblogs.com/blog/256729/201401/170011445484.png)
9、在拷贝到服务器的Remote Debugger中，右键msvsmon.exe，以管理员运行，然后会看到Visual Studio远程调试监视器打开；
![wps_clip_image-12670[4]](https://images0.cnblogs.com/blog/256729/201401/170011455489.png)
10、如下图，可以看到在等待新的连接：
![wps_clip_image-26212[4]](https://images0.cnblogs.com/blog/256729/201401/170011463304.png)
11、端口号随意设置，要使用Windows身份认证，权限要勾上调试账号；
![wps_clip_image-4685[4]](https://images0.cnblogs.com/blog/256729/201401/170011477206.png)
12、在本地VS中，附加进程，如下图：
可以看到，我们附加的已经是远程服务器的进程了，选择w3wp.exe
![wps_clip_image-31923[4]](https://images0.cnblogs.com/blog/256729/201401/170011485957.png)
13、可以看到Visual Studio远程调试监视器，已经接到我们的连接了；
![wps_clip_image-18769[4]](https://images0.cnblogs.com/blog/256729/201401/170011494084.png)
14、远程调试注意要部署最新的程序，否则断点不会命中；
![wps_clip_image-32117[4]](https://images0.cnblogs.com/blog/256729/201401/170011501270.png)
15、部署以后，还要单独部署pdb文件，使用命令行的方式；
Copy SharePointProject.pdb  C:\Windows\Microsoft.NET\assembly\GAC_MSIL\ SharePointProject\v4.0_1.0.0.0__3475aec342981a3a
在SharePoint 2013中，由于.Net 4.0和Windows系统兼容问题，4.0以上版本的dll，都不熟到C:\Windows\Microsoft.NET\assembly下了，不再是我们熟悉的GAC，这一点要特别注意；
pdb文件部署路径C:\Windows\Microsoft.NET\assembly\GAC_MSIL\SharePointProject\ v4.0_1.0.0.0__3475aec342981a3a，“3475aec342981a3a”是这个程序集的强名称；
![wps_clip_image-6848[4]](https://images0.cnblogs.com/blog/256729/201401/170011513929.png)
16、所有工作完成，看看远程调试的效果吧，还是不错的。
![wps_clip_image-16047[4]](https://images0.cnblogs.com/blog/256729/201401/170011522834.png)
总  结
SharePoint 2013本地开发，就是把SharePoint注册表从服务起导出然后从本地导入；远程调试有几个要注意的地方，本地/远程Remote Debugger版本要一致，本地/远程要有相同的管理员账号，远程运行Remote Debugger本地连接，Remote Debugger配置Windows认证并授权调试账号，代码要是最新pdb文件要部署到GAC下，GAC已经不再原来的位置。
以上就是SharePoint2013本地开发解决方案以及配置远程调试的全部内容，在此，谢谢大家的阅读并欢迎点赞，希望霖雨的博客能够为大家带来帮助。





