# 科学网—计算机操作系统hosts文件快速攻略 - 王伟华的博文




# 计算机操作系统hosts文件快速攻略                           

已有 5265 次阅读2014-6-7 17:19|个人分类:[计算机&大数据](http://blog.sciencenet.cn/home.php?mod=space&uid=81613&do=blog&classid=11842&view=me)|系统分类:[科研笔记](http://blog.sciencenet.cn/home.php?mod=space&do=blog&view=all&uid=81613&catid=1)



 Hosts是计算机操作系统的一个没有扩展名的系统文件，可以用记事本等工具打开，其作用就是将一些常用的网址域名与其对应的IP地址建立一个关联“数据库”，当用户在浏览器中输入一个需要登录的网址时，系统会首先自动从Hosts文件中寻找对应的IP地址，一旦找到，系统会立即打开对应网页，如果没有找到，则系统再会将网址提交DNS域名解析服务器进行IP地址的解析. 如果发现是被屏蔽的IP或域名，就会禁止打开此网页！也就是说Hosts的请求级别比DNS高。


      知道了操作系统的Hosts文件的工作原理，我们可以通过修改Hosts文件为我们服务: 加快域名解析,屏蔽网站,强制将网站指定到正确的IP上(这个向当地重要哦)，手动查杀劫持HOSTS表的病毒等等！书归正传，还是先把Hosts文件找出来吧。

一.存储位置

hosts文件在不同操作系统的位置都不大一样：

Windows NT/2000/XP/Vista/7/8（即微软NT系列操作系统）：默认位置为%SystemRoot%system32driversetc，但也可以改变。动态目录由注册表键HKEY_LOCAL_MACHINESYSTEMCurrentControlSetServicesTcpipParametersDataBasePath决定。其中，%Systemroot%指系统安装路径。例如，Windows XP/Windows 7/Windows 8 安装在C:WINDOWS,那么Hosts文件就在C:WINDOWSsystem32driversetc中。

Linux及其他类Unix操作系统：/etc


Mac OS 9及更早的系统：System Folder: Preferences或System folder（文件格式可能与Windows和Linux所对应的文件不同）

Mac OS X：/private/etc（使用BSD风格的hosts文件）

OS/2及eComStation："bootdrive":mptnetc

Android：/system/etc/hosts

Symbian第1/2版手机：C:systemdatahosts

Symbian第3版手机：C:private10000882hosts，只能使用兼容AllFiles的文件浏览器访问，大部分都不行。

iPhone OS：/etc

iPad OS：/private/etc

webOS:/etc




二、具体作用


   来看一下Hosts文件的工作方式以及它在具体使用中起哪些作用。


1、加快域名解析

对于要经常访问的网站，我们可以通过在Hosts中配置域名和IP的映射关系，提高域名解析速度。由于有了映射关系，当我们输入域名计算机就能很快解析出IP，而不用请求网络上的DNS服务器。

2、方便局域网用户

在很多单位的局域网中，会有服务器提供给用户使用。但由于局域网中一般很少架设DNS服务器，访问这些服务器时，要输入难记的IP地址。这对不少人来说相当麻烦。可以分别给这些服务器取个容易记住的名字，然后在Hosts中建立IP映射，这样以后访问的时候，只要输入这个服务器的名字就行了。

3、屏蔽网站（域名重定向）

有很多网站不经过用户同意就将各种各样的插件安装到你的计算机中，其中有些说不定就是木马或病毒。对于这些网站我们可以利用Hosts把该网站的域名映射到错误的IP或本地计算机的IP，这样就不用访问了。在WINDOWS系统中，约定 127.0.0.1 为本地计算机的IP地址, 0.0.0.0是错误的IP地址。

如果，我们在Hosts中，写入以下内容：

127.0.0.1 # 要屏蔽的网站 A

0.0.0.0 # 要屏蔽的网站 B

这样，计算机解析域名A和 B时，就解析到本机IP或错误的IP，达到了屏蔽网站A 和B的目的。

4、顺利连接系统

对于Lotus的服务器和一些数据库服务器，在访问时如果直接输入IP地址那是不能访问的，只能输入服务器名才能访问。那么我们配置好Hosts文件，这样输入服务器名就能顺利连接了。

4屏蔽例子

    在hosts文件中加入如下内容就可以屏蔽文件中定义的对应的网址（以屏蔽百度IE搜索伴侣为例，#后面是注释）

127.0.0.1 localhost
127.0.0.1 bar.baidu.com #百度IE搜索伴侣
127.0.0.1 www.baidu.com #百度IE搜索伴侣
127.0.0.1 baidu.com #百度IE搜索伴侣


免疫一些病毒（实际上是屏蔽网站，禁止访问有毒网站从而达到免疫一些已知有毒网站上的病毒）。最后要指出的是，Hosts文件配置的映射是静态的，如果网络上的计算机更改了请及时更新IP地址，否则将不能访问。

5特殊作用

屏蔽网页插件

经常浏览网页或许会在某个网站上经常遇见弹出来的插件安装提示，虽然IE级别都高了一个档次，当总是提示又不想安装怎么办？，比如某免费在线电影站点，不过在打开每一部电影播放页面前，站点均会弹出一个要求安装百度工具条的网页并且不等用户同意便立即进入下载安装状态！尽管这个网页窗口可以一关了之，但要看的电影不是一部，每次都要连接下载肯定会影响正常网页的打开速度及正在播放视频的流畅。通过观察，在各个电影播放页面中弹出的这个百度工具条安装窗口都是同一网址，以www.XXX.com/aa.exe代替吧。打开Hosts文件，在文件中添加新行：0.0.0.0   www.XXX.com/aa.exe回车后保存文件退出，当再次打开该站点强出插件页面时，系统会自动将其解析到“0.0.0.0”这样一个不可能存在的IP地址上，这样也就屏蔽了该网页插件。一般的网页插件可以用类似的办法屏蔽。

一键登录局域网指定服务器

    单位的“高工”在公司的局域网中建了个CS对战服务器，于是我们这些一人吃饱全家皆饱的单身汉便又有了在下班时间消遣的好去处。不过有一点美中不足，局域网中没有再架设DNS服务器，所以我们每次只能输入IP地址进行登录，尽管只是数量不算多的一串数字，但毕竟数字枯燥啊！

在这种情况下，我们可以通过修改Hosts文件来达到一键登录局域网CS服务器的目的：打开Hosts文件，同样在新开启的空白行中输入“221.555.78.122 CSSer”（假定221.555.78.122是CS服务器在局域网中的IP地址），这样我们以后只要输入“CSSer”就可以直接登录局域网CS服务器了。

6病毒处理

首先必须要进入安全模式

下载360安全卫士或者金山卫士、瑞星卡卡、江民安全专家等软件，不要安装！ 进入安全模式后，关闭进程EXPLORER.EXE,然后再添加进程EXPLORER.EXE（具体操作 文件-新建任务“运行”-浏览-C:WINDOWSexplorer.exe）。然后安装运行，进行修复。

其次运行regedit删除如下项目，查找HKEY-LOCAL-MACHINESOFTWAREMicrosoftWindowsCurrentVersionRun

后面有多种可能性，发现有2种！ 名称为R或者A

1.R=C:WindowsSystem32rundl132.exectfmon.dll s

2.A=C:WINDOWSsystem32rundl132.exemsad.dll s

你如果发现了这2个其中一个，直接删除此项目。

然后找到 C:WindowsSystem32ctfmon.dll或者 C:WindowsSystem32msad.dll

删除该文件

修复HOSTS文件（其实安全模式下安装运行360安全卫士就已经可以修复了，这里说手动修复的方法）请按以下方式操作：

1。搜索系统中的hosts文件，也可以根据第一小节中介绍的各种系统中的Hosts文件位置直接查找。

2。首先将此文件备份；

3。右键点击hosts文件，选择{打开方式}，选择记事本；

4。删除记事本的内容，或干脆将hosts文件删除！（不影响系统的正常使用）

7文件修改

1。正确修改hosts文件：

一般打开hosts文件里面都会有个示例，按照其格式修改即可。值得注意的是以#开头的是注释行。

2。hosts文件的位置。详见第一节各种操作系统的Hosts文件的位置介绍。

3。hosts文件的系统属性：

hosts文件默认具有隐藏属性，系统默认设置情况下是看不到的，当然进到这个文件除了直接输入路经也是无法看到了。解决办法：打开我的电脑->点击工具->文件夹选项->查看->取消勾选隐藏受保护的系统文件->确定即可。

修改后无法保存：

在Hosts文件上面点右键->属性->把只读属性去掉->确定，即可保存。

在修改HOSTS文件时候，还常常遇到修改保存后无效的情况，这里要提醒大家注意的一点：很多人是写在最后行，写完最后一行后在没有回车的情况下，这一行是不生效的。一定要记得回车。

建议格式：“ip地址+Tab+域名+换行” 添加记录。

4。Hosts在UAC安全策略无法打开或编辑：

遇到windows VISTA 或者 windows 7 操作系统无法打开和编辑的情况，提示：“不能创建文件” 或者 “文件操作失败”等提示，是因为安全机制UAC造成的，你可以使用以下方法解决：

按着Shift键，然后Hosts文件上面右键， 以管理员方式运行即可。

5。Android中的Hosts

由于某些原因，可能需要修改 hosts 文件指定域名到对应的 IP 地址。Android 是基于 Linux 的系统，与 Linux 类似，通过 hosts 文件来设置。

在 Android 下，/etc 是 link 到 /system/etc 的，我们需要修改 /system/etc/hosts 来实现。但是这个文件是只读，不能通过 shell 直接修改。可以通过连接到 PC 上使用 adb 来修改。步骤如下：

1、获得root权限：adb root

2、设置/system为可读写：adb remount

3、将hosts文件复制到PC：adb pull /system/etc/hosts

4、修改PC机上文件

5、将PC机上文件复制到手机：adb push /system/etc/hosts

如果要查看是否修改成功，可以在PC上执行adb shell，运行cat /system/etc/hosts;或者在手机上运行cat /system/etc/hosts。

在Android系统中，hosts文件格式有一点与PC机Linux不同：不能在一行中一个IP对应多个域名。

6。WIN7或者VISTA

WIN7或者VISTA系统的需要提升用户对Hosts文件的操作权限，否则无效。

具体方法如下：

方法一：按着Shift键，然后Hosts文件上面右键，以管理员方式运行即可。

方法二：鼠标右键点击Hosts文件，选择属性，然后点击“安全”选项卡，选中登录使用的用户账户并点编辑”，在弹出的权限设置界面再次选中当前用户，然后勾选“完全控制”或“修改”都可以，最后点击应用生效即可。

8文件更新

1.访问googlecode网站获得Google Hosts的最新文件，这里提供了最新的映射IP地址，将获得的hosts文件附加到自己电脑的hosts中就可以了；

2.利用smarthosts.exe程序，下载smarthosts.exe程序，下载后双击这个程序会生成3个文件 hosts、lists和srvlist注意使用这个经常会弹出错误，这个时候不要着急，过几分钟后可以重试。


参考：[维基Hosts文件](http://zh.wikipedia.org/wiki/Hosts%E6%96%87%E4%BB%B6)


[百度百科：Hosts](http://baike.baidu.com/view/597330.htm?fromtitle=Hosts文件&fromid=8971674&type=syn)


[Host文件的含义和作用](http://blog.itful.com/articles/2010/2668_host-3.html)

[hosts文件位置及hosts快速修改方法！](http://www.piaoyi.org/computer/hosts-path.html)


说明：本文内容来自上述参考文献。我只是整理了一下。

另外给大家出两个思考题：

1、利用本文介绍的知识，不翻墙如何访问google网站使用google进行搜索（当然包括学术搜索哦）。![](/static/ueditor/dialogs/emotion/images/jx2/j_0071.gif)

2、能否利用Hosts文件的工作原理来绕过一些付费软件的使用期限的限制。![](/static/ueditor/dialogs/emotion/images/jx2/j_0059.gif)




仅作学术研究使用，请勿做它用，否则后果自负！



最近搜集了一个好的host，比较大，有近400条代理主机及ip地址，可以通畅无阻的上Google学术，Google网址，分享给大家。为了简便，  直接将内容做好host，各位博友直接下载解压，然后改名为hosts，把此hosts内容复制到 C:WindowsSystem32driversetc文件夹中即可。

[hosts](http://blog.sciencenet.cn/home.php?mod=attachment&id=59759)


[hosts.rar](http://blog.sciencenet.cn/home.php?mod=attachment&id=59793)


[hosts.rar](http://blog.sciencenet.cn/home.php?mod=attachment&id=64543)


[http://www.netsh.org/](http://www.netsh.org/)[http://serve.netsh.org/pub/gethosts.php](http://serve.netsh.org/pub/gethosts.php)


站长工具Ping来检查网站的所有CDN节点IP地址，选择最快的那个来用。

　　Ping工具使用方法1：打开[http://ping.chinaz.com/](http://ping.chinaz.com/)在框框里输入域名比如dynamic.12306.cn然后查询。

　　Ping工具使用方法2：打开[http://ping.aizhan.com/](http://ping.aizhan.com/)在框框里输入域名比如dynamic.12306.cn然后查询。





相关链接：



张林：[为了维稳把Google都关了是什么水平？](http://blog.sciencenet.cn/home.php?mod=space&uid=318012&do=blog&id=800631)

李珊珊：[对谷歌如此封杀为哪般？](http://blog.sciencenet.cn/blog-272495-800544.html)

张影：[哪些混蛋专家封掉了Google和Gmail？](http://blog.sciencenet.cn/blog-1244724-800726.html)

陈楷翰：[没Google学术后，哪里查第一手资料啊？](http://blog.sciencenet.cn/blog-561693-800774.html)

李子欣：[Google的学术搜索上不去了？](http://blog.sciencenet.cn/blog-662228-800386.html)

程丝：[Google VS. Baidu](http://blog.sciencenet.cn/home.php?mod=space&uid=266735&do=blog&id=799939)


朱朝东:[google学术 - This webpage is not available](http://blog.sciencenet.cn/blog-536560-799732.html)


朱树华：[又不能登陆Google学术搜索了，元方，你怎么看？](http://blog.sciencenet.cn/blog-89711-628239.html)


颜宁：[中国网络已死](http://blog.sciencenet.cn/home.php?mod=space&uid=65865&do=blog&id=490619)

张慧铭：[Google搜索学术图书打不开 以后怎么搜索学术资料？](http://blog.sciencenet.cn/home.php?mod=space&uid=752541&do=blog&id=799843)


曹聪:[【旧文重贴】关于谷歌（Google）与中国，弗里德曼如此说](http://blog.sciencenet.cn/blog-45671-801045.html)

[Google、Gmail 所有服务 遭全面屏蔽](http://ituibar.com/google-gmail-all-services-were-full-screen/)此次屏蔽的方法主要屏蔽Google部分IP地址的443端口，包括google.com.hk，accounts.google.com的部分IP的443端口被封，导致部分中国用户无法访问Google搜索和Gmail，由于Google的IP地址非常多，而被屏蔽的只是其中部分IP。解决的方法很简单，只要找一个未被屏蔽443端口的IP地址替代原有IP即可，设置的方法是，在hosts中设置www.google.com、www.google.com.hk、accounts.google.com这几个域名的IP为可用的美国Google官方IP即可。这个IP的寻找方法是，找一个PING工具，从世界各地PING目标域名，然后从里面的IP地址寻找可用IP地址即可。

我要上Google 1.1 是个好东西 有了它 妈妈再也不用担心 我上不了谷歌了

稳定上Google的神器国内Google非常不稳定，速度慢且经常上不去，通过“我要上Google"，可以安全稳定地使用Google、Gmail、Google+等平时需要特殊手段才能上去的网站。“我要上Google"1.0的hosts昨天被国家封锁，已更新到1.1，恢复正常。“我要上Google"并不会对用户造成安全性问题，只是利用Google在国外的一些稳定服务器来进行访问。

下载地址： [http://www.brieftools.info/software/googler/](http://www.brieftools.info/software/googler/)下载解压后点击“我要上Google_1.1.exe”完成安装。从此可以稳定上Google、Gmail和Google+了。建议通过 [https://www.google.com/ncr](https://www.google.com/ncr) 来防止国家重定向，这样会更佳稳定。

[http://www.mathsccnu.com/forum.php?mod=viewthread&tid=2076&page=1&extra=#pid11077](http://www.mathsccnu.com/forum.php?mod=viewthread&tid=2076&page=1&extra=#pid11077)

解决的办法

由于Google的产品全面遭到干扰，所以使用 Goole App Agine 作为跳板实现翻墙目的的 Goagent 也难幸免，迅速升级到最新版是一个解决办法，另外在不使用复杂翻墙工具的情况下，通过 [Smarthost](https://code.google.com/p/smarthosts/) 获取最新的 Google host IP地址然后更改 Host 文件，也是继续使用 Google 服务的一个较为简便的方法。Smarthosts是一个托管在谷歌代码上的项目，您可以轻松利用本项目使用到一份稳定的Hosts文件。这份Hosts文件可以帮助您顺利打开一些网站，提高某些国外服务的打开或下载速度。

new 注意： smarthosts即将迁移到[http://forum.projecth.us](http://forum.projecth.us/)中[ttp://forum.projecth.us](http://forum.projecth.us/)


转载本文请联系原作者获取授权，同时请注明本文来自王伟华科学网博客。
链接地址：[http://blog.sciencenet.cn/blog-81613-801323.html](http://blog.sciencenet.cn/blog-81613-801323.html)

上一篇：[蒙特卡罗(Monte Carlo Method)方法学习资料](blog-81613-793209.html)
下一篇：[U盘里存的文件（夹）突然都变成快捷方式怎么办？](blog-81613-809036.html)


