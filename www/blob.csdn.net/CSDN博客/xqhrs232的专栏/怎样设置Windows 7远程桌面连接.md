# 怎样设置Windows 7远程桌面连接 - xqhrs232的专栏 - CSDN博客
2016年04月11日 09:35:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：432
原文地址::[http://jingyan.baidu.com/article/656db918b3fe44e381249c1c.html](http://jingyan.baidu.com/article/656db918b3fe44e381249c1c.html)
相关文章
1、打开远程桌面命令----[http://jingyan.baidu.com/article/a65957f4b1538724e67f9ba2.html](http://jingyan.baidu.com/article/a65957f4b1538724e67f9ba2.html)

- 
![怎样设置Windows 7远程桌面连接](http://b.hiphotos.baidu.com/exp/whcrop=92,69/sign=3528666c5bb5c9ea62a655a1ba498b39/faf2b2119313b07ecb33006f0cd7912396dd8cce.jpg)1
- 
![怎样设置Windows 7远程桌面连接](http://c.hiphotos.baidu.com/exp/whcrop=92,69/sign=4ba716a782025aafd3672889949d965d/eac4b74543a9822655487b408a82b9014b90ebc8.jpg)2
- 
![怎样设置Windows 7远程桌面连接](http://b.hiphotos.baidu.com/exp/whcrop=92,69/sign=ea3efb060fb30f2435cfba41a7e5ec7a/faedab64034f78f06681ca9479310a55b2191cca.jpg)3
- 
![怎样设置Windows 7远程桌面连接](http://g.hiphotos.baidu.com/exp/whcrop=92,69/sign=306eb06eb6003af34def8a225a5afb65/64380cd7912397dda7f9c8635982b2b7d1a287d0.jpg)4
- 
![怎样设置Windows 7远程桌面连接](http://h.hiphotos.baidu.com/exp/whcrop=92,69/sign=f76d98484134970a4726466dfabaecf1/9e3df8dcd100baa192af17dc4710b912c9fc2ed1.jpg)5
- 
![怎样设置Windows 7远程桌面连接](http://f.hiphotos.baidu.com/exp/whcrop=92,69/sign=792353862f2eb938ec382cb0ba12b80c/fcfaaf51f3deb48fa190f1b9f01f3a292cf578df.jpg)6
- 
![怎样设置Windows 7远程桌面连接](http://a.hiphotos.baidu.com/exp/whcrop=92,69/sign=bec65fa43a87e9504242a52e7f486e7a/902397dda144ad345144d7e0d0a20cf430ad85d9.jpg)7
[分步阅读](http://jingyan.baidu.com/album/656db918b3fe44e381249c1c.html)
本文介绍Windows 7远程桌面连接设置方法，仍然是入门基础教程，虽然Windows 7在远程桌面上改进了很多，但远程桌面终究是个简单的模块，更多的改进只能归于鸡肋功能，九成以上的远程用户都不会用到，比如远程桌面中的体验功能，也许只有在非常特殊的场合才有用处，大部分时间纯粹是在消耗系统资源。
远程桌面设置分为两种，一种是Windows 7连向非Windows 7的远程桌面，第二种是Windows 7连向Windows Vista/7/2008这样的远程桌面，所以本文分别以Windows 7连向Windows 2003/XP和连向Windows 7两种进行介绍，由于设置是双向的，也就是说，一方要设置连接，另一方肯定要设置接受。连向其它系统的远程桌面设置，以后再做补充。
## 一、首先启用目标计算机的远程桌面
- 
1
(提醒：如果目标计算机已经设置，或不在自己的受控范围，请略过本段，可以直接连接测试。)
- 
2
Windows XP上启用远程桌面
在桌面上鼠标右键点击“我的电脑”图标，选择“属性”，在属性窗口中，点击“远程”选项卡，如下图：点击“允许用户远程连接到此计算机”，默认管理员具有此权限，远程桌面登录需要输入对应的账户和密码，如果你只打算让某个用户远程访问，可以在“远程用户”里添加。设置完成后点击确定即可。默认的端口是3389，设置后，Windows XP或Windows 2003都会默认为防火墙添加此规则，但是如果你手动修改了远程桌面端口，切忌一定要把防火墙中的端口号一定记得修改或单独添加许可规则，否则远程计算机连接可能都会被Windows
 防火墙阻止。
![怎样设置Windows 7远程桌面连接](http://b.hiphotos.baidu.com/exp/w=500/sign=47906ca090529822053339c3e7cb7b3b/faf2b2119313b07ecb33006f0cd7912396dd8cce.jpg)
- 
3
Windows 7远程桌面功能开启
本设置方法同样适用用Vista和Windows Server 2008，依次点击“开始”菜单-控制面板-系统和安全-系统，再点击左侧的远程设置，如下图：
要开启Windows 7远程桌面，有三个选项，第一个如果是不允许连接到本机，则选择“不允许连接到这台计算机”，这样可以阻止任何人使用远程桌面或 RemoteApp连接到您的计算机。后面两个选项，“允许任意版本远程桌面计算机”和“仅运行使用网络级别身份验证的远程桌面的计算机”，两者差别如下：
■“允许运行任意版本远程桌面的计算机连接”——如果不确定将要连接过来的计算机操作系统，可以选择这一项。安全性较第三个选项低。
■“只允许运行带网络级身份验证的远程桌面的计算机连接”——允许使用运行带网络级身份验证的远程桌面或 RemoteApp 版本计算机的人连接到您的计算机。目前Vista、Windows 2008、Windows 7均支持网络级身份验证，Windows XP等可以通过修改注册表等方法支持，如需要请自行GOOGLE搜索。
**网络级别身份验证查看方法：**鼠标点击远程桌面连接左上角的小图标，选中关于，如下窗口，有是否支持提示。
![怎样设置Windows 7远程桌面连接](http://c.hiphotos.baidu.com/exp/w=500/sign=94407e82ad6eddc426e7b4fb09dab6a2/eac4b74543a9822655487b408a82b9014b90ebc8.jpg)
![怎样设置Windows 7远程桌面连接](http://b.hiphotos.baidu.com/exp/w=500/sign=524a99549245d688a302b2a494c37dab/faedab64034f78f06681ca9479310a55b2191cca.jpg)
END
## 二、Windows 7远程桌面连接设置
- 
运行远程桌面程序
点击开始菜单，输入mstsc，运行mstsc.exe，位置:C:\windows\system32\远程桌面设置界面，如下图，在远程管理时，如果需要与远程计算机进行交互传输文件，或互访粘贴板数据，需要点开“选项”进行设置一下。
![怎样设置Windows 7远程桌面连接](http://g.hiphotos.baidu.com/exp/w=500/sign=0a4eb29d7bcb0a4685228b395b62f63e/64380cd7912397dda7f9c8635982b2b7d1a287d0.jpg)
![怎样设置Windows 7远程桌面连接](http://h.hiphotos.baidu.com/exp/w=500/sign=c69bd9665c6034a829e2b881fb1249d9/9e3df8dcd100baa192af17dc4710b912c9fc2ed1.jpg)
- 
设置远程桌面“本地资源”
点开“本地资源”选项卡，如下图：如果需要访问本地粘贴板，则选中剪贴板，如果需要拷贝文件，则点击详细信息，需要从哪个驱动器复制或拷贝文件就选择哪个驱动器即可。
![怎样设置Windows 7远程桌面连接](http://f.hiphotos.baidu.com/exp/w=500/sign=e744006f0cd79123e0e094749d355917/fcfaaf51f3deb48fa190f1b9f01f3a292cf578df.jpg)
![怎样设置Windows 7远程桌面连接](http://a.hiphotos.baidu.com/exp/w=500/sign=730d6f11f21fbe091c5ec3145b610c30/902397dda144ad345144d7e0d0a20cf430ad85d9.jpg)
- 
3
连接远程桌面
设置完成后，点击“连接”，可能会弹出防火墙警告，只需要确定即可。示例连接Windows XP的远程桌面登录界面：输入登录的用户和密码（用户和密码来源，参加上文远程桌面设置）即可登录，使用完成后，点击右上角的关闭即可。 
![怎样设置Windows 7远程桌面连接](http://d.hiphotos.baidu.com/exp/w=500/sign=73dc6c73ab18972ba33a00cad6cc7b9d/f703738da977391210d1d506f8198618377ae2e6.jpg)
![怎样设置Windows 7远程桌面连接](http://d.hiphotos.baidu.com/exp/w=500/sign=ae37d7ac43166d223877159476220945/3b87e950352ac65cebf43614fbf2b21192138ae0.jpg)
- 

