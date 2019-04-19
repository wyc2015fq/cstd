# 关于Mercurial(Hg) - 逍遥剑客 - CSDN博客
2010年02月20日 00:06:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：6097
转自
决定采用Mercurial(Hg)了
[http://www.keakon.cn/bbs/thread-1865-1-1.html](http://www.keakon.cn/bbs/thread-1865-1-1.html)
===================================================================
最近想用版本控制软件来保存汉化文件，但又觉得SVN太麻烦，于是想到了最近较为流行的分布式版本控制工具。
而Git和Mercurial（意思为水银的，于是经常缩写为Hg）自然是其中最为流行的工具。大名鼎鼎的Linux就用Git作源码管理，而Python和Firefox则采用Hg（你可以在这找到一堆[使用Hg的项目](http://mercurial.selenic.com/wiki/ProjectsUsingMercurial)）。
比较了一番后，最终我选择了后者。因为Git的优势主要在于分支，而汉化并不需要太多分支；而Git对Windows的支持似乎不如Mercurial，ssh也比http麻烦，比较难教汉化组成员们使用；此外还有个特别的原因：Hg主要是用Python实现的（小部分使用C实现）。
当然，Hg也有个很严重的缺点：不支持针对单个文件夹的分支。如果剧本翻译和改图要建立分支都必须复制整个仓库，而对翻译来说，图像文件并不是他需要的部分。
此外，Google还发了篇[《Analysis of Git and Mercurial》](http://code.google.com/p/support/wiki/DVCSAnalysis)，说明为什么Google Code决定支持Hg，而不支持Git。
Git的优势： 
- 
- 客户端存储控制。可以删除任意的版本，而无需考虑该分支的后续版本，而Hg做不到。
- 无限的父结点数。Hg一次只能分成2个分支，合并N个分支要进行很多次操作。而Git没有限制。
- 支持rebase。它可以将当前版本移到最新的分支去。例如假设有个本地库在开发1.0的新特性，而基线已经升级到1.1了；一般情况下是需要合并到1.1去的，而rebase可以避免这种操作。
Hg的优势： 
- 
- 学习曲线。由于很多因素，Git比Hg的学习曲线更陡峭。
- 支持Windows。
- 维护。Git需要定期维护。
- 历史是不可侵犯的。Git很强大，可以干几乎任何事，因此也会丢失历史信息。
看起来Git在技术上是要强于Hg的，不过由于文件操作的实现依赖于操作系统，移植到Bigtable会存在麻烦；而且Hg有很好的基于HTTP的无状态pushing和pulling，容易与Google的构架整合（Google几乎所有的服务都基于HTTP）。
对于这点，Google还特意提供了测试数据，指出在使用HTTP时，Git比Hg慢1个数量级。（提到了2个数字，分别慢22倍和12倍。）
在继续介绍Hg之前，我先说下[分布式配置管理](http://wiki.woodpecker.org.cn/moin/DistributedScm)（DSCM, Distributed Software Configuration Management）。
之前曾用过一段时间的SVN，这也是集中式SCM中非常流行的工具。给我的印象是速度慢（因为每个文件都是单独下载，速度根本上不去），每个文件夹下面都有个影响心情的.svn文件夹。
而今天用了几个小时的Hg，明显感觉到了不同。
首先就是速度快，上传和下载都是打包并压缩的。我的一个20.7M的项目，上传上去只用了几分钟，占用4.3M的空间，不得不说压缩率很高。
其次就是仓库是分布的，可以在本地建立多个镜像，然后分别进行更改；提交也只提交到本地的镜像，并不影响主仓库；各个仓库还可以互相进行合并，最终达到多个版本的一致，再上传到主仓库。
这对汉化来说是非常方便的，因为翻译们可以建立自己的镜像，翻译完告知校译；校译们获取各个翻译的版本，进行合并，校译完再通知润色；润色则可以获取校译的版本，然后推送到主仓库。由于主仓库不需要多次更改，也就避免了在主仓库上建立多个分支，导致管理的混乱。
顺便列出我找到的几个可以用于免费托管代码的网站：
- 
- [GitHub](http://github.com/plans)：这个网站非常出名，是用RoR建的，300MB的免费空间，支持Git，但不支持Hg（虽然有插件，但教别人用太麻烦），而且没有私有库。
- [bitbucket](http://bitbucket.org/plans/)：这个是用Django建的，可以建1个私有库，150MB的免费空间，支持Hg。
- [ProjectLocker](https://www.projectlocker.com/signup/startup)：只有（无限个）私有库，500MB的免费空间，支持SVN和Git，但只能有5个用户。闭源项目推荐，但显然不适合庞大的tomo汉化组。
- [Unfuddle](http://unfuddle.com/about/tour/plans)：200MB的免费空间，支持SVN和Git，只能有1个库和2个用户。
最终我选择了最小的bitbucket，因为也差不多够用了。
（注：Google Code现已支持Hg，但只能托管源码，不能用于其他目的，所以我只能放弃。但如果你是开源软件开发，那么Google Code是最大方的，还可以发信申请扩大配额，只是偶尔会被GFW。）
接着就开始装软件了。习惯了SVN的乌龟，所以仍然选择了乌龟汞（[TortoiseHg](http://tortoisehg.bitbucket.org/)）。
目前这个版本支持Windows XP、Vista和Windows 7，所以不担心兼容性。
安装很简单，装好后要重启（其实不重启也可以用）。
喜欢命令行的还可以在CMD里输入hg试试。
不过装好后界面是英文的，不清楚是不是我的操作系统是英文系统的原因。
翻了一下文档（C:/Program Files/TortoiseHg/docs/TortoiseHG.pdf），发现实际上是支持中文的，不过需要建一个“LANGUAGE”环境变量（在系统属性里），值为“zh_CN”。
![决定采用Mercurial(Hg)了 - keakon的blog -](https://dl.dropbox.com/u/1065182/Picture/TortoiseHg/1.png)
接着还得去这个文件夹：C:/Program Files/TortoiseHg/cmenu_i18n，导入thg-cmenu-zh_CN.reg文件。
修改之后，软件界面和命令行（hg和hgtk）就已经是中文了，但右键菜单还是英文，重启或注销即可变成中文。
在使用前，建议看看[《Mercurial 使用教程》](http://mercurial.selenic.com/wiki/ChineseTutorial)，了解下几个术语。
Init是指初始化，也就是创建一个空的仓库。
Clone是指创建仓库的复本，创建的来源可以是一个http或ssh链接表示的仓库，也可以是本地仓库。
Commit是指提交，即接受所有的改动，将当前版本作为最新版本（也叫tip）。注意这个提交和集中式SCM不同，它只提交的本地仓库，而且本地仓库会详细记录所有的变动（称为变更集或改动集）。因此不用客气，多提交几次也没关系，因为不影响别人的仓库。而如果怕影响自己的话，可以clone一个复本，再对复本进行改写和提交。
Revert是指恢复，即撤销所有的改动，恢复到未更改的状态。右键菜单里你是找不到它的，原因后面再提。
Pull的意思是拉，是将其他仓库有，而当前仓库没有的变更集“下载”到当前仓库。不过这只是添加了变更集，文件并不会改动。（变更集有一个唯一的hash标识，所以很容易区分。）
Update的意思就是更新，当获取了其他变更集，且需要更改以保持和其他仓库一致时，就可以执行它了。
Push的意思是推，就是将当前仓库的变更集“上传”到其他仓库去。其他仓库必须支持该操作，并可能需要经过验证。如果是从本地仓库A push到本地仓库B，就相当于用B pull A。此外，push也不会更新仓库的版本，因为它是共享的，需要的人直接选择想要的版本即可。
Merge就是指合并。当一个文件同时被多次修改，并存放在多个仓库时，要获得一个统一的版本，就需要合并。对于翻译而言，一个文本可能同时会分给多个人，于是合并时只需要接受每个人的改动即可。
如果你还想了解更多，可以读读[《Mercurial: The Definitive Guide》](http://hgbook.red-bean.com/read/)。
这些术语介绍完后，就可以看看别人写的使用方法了：
[Git Vs Mercurial hg? 异地协同开发，分布式SCM方案选择！](http://bwlee.javaeye.com/blog/376503)
[CodeBeamer+Mercurial实务操作手册](http://hi.baidu.com/limp_t/blog/item/46a396ce665cd433b600c887.html)
当然，那个PDF文档也是很有用的资料。
不过为了汉化组的同志们，我还是截图说明如何进行完整的操作吧。
首先去[我的bitbucket主页](http://bitbucket.org/keakon/)，找到[FamilyProject](http://bitbucket.org/keakon/familyproject/)这个项目。
下面会有一些提示： 
##### 引用:
> Clone this repository (size: 4.3 MB): HTTPS / SSH
$ hg clone [https://keakon@bitbucket.org/keakon/familyproject/](https://bitbucket.org/keakon/familyproject/)
接着在你的硬盘任何位置新建一个文件夹，起名为FamilyProject。
进入这个文件夹，右键点击空白处，在TortoiseHg菜单里选“克隆”。
![决定采用Mercurial(Hg)了 - keakon的blog -](https://dl.dropbox.com/u/1065182/Picture/TortoiseHg/2.png)
在源路径处填入刚才看到的地址：[https://keakon@bitbucket.org/keakon/familyproject/](https://bitbucket.org/keakon/familyproject/)，再点“克隆”按钮，等待下载完成即可。
![决定采用Mercurial(Hg)了 - keakon的blog -](https://dl.dropbox.com/u/1065182/Picture/TortoiseHg/3.png)
下载完成后，你便拥有完整的项目文件了，可以对其进行更改等操作。
接下来进行一个克隆操作，将它复制到另一个本地仓库。方法很简单，和之前的克隆一样，不过源路径填刚才生成的文件夹，目标路径填新文件夹：
![决定采用Mercurial(Hg)了 - keakon的blog -](https://dl.dropbox.com/u/1065182/Picture/TortoiseHg/4.png)
这次操作应该1、2秒就完成了，因为是本地复制。
接着用文本编辑器打开FamilyProject里的一个文件，例如SNR02.ISF。
会发现第1句是：●0001●　ロードに失敗しました。
把它改成：读取失败。
然后右键点FamilyProject文件夹，选择“HG 提交”。
![决定采用Mercurial(Hg)了 - keakon的blog -](https://dl.dropbox.com/u/1065182/Picture/TortoiseHg/5.png)
这时会列出改动的文件和具体的差异。填写一下描述，再点提交按钮。
![决定采用Mercurial(Hg)了 - keakon的blog -](https://dl.dropbox.com/u/1065182/Picture/TortoiseHg/6.png)
再打开FamilyProject-new里的同名文件，找到第2句：●0002●　終了します。
把它改成：读取失败。
然后也进行同样的提交操作。
现在你在2个仓库里就有2个不同的分支，于是看看如何合并它。
先右键点FamilyProject文件夹，选择“HG 仓库浏览器”。
![决定采用Mercurial(Hg)了 - keakon的blog -](https://dl.dropbox.com/u/1065182/Picture/TortoiseHg/7.png)
然后在地址栏里填上新仓库的地址，点“拉取改动集”按钮：
![决定采用Mercurial(Hg)了 - keakon的blog -](https://dl.dropbox.com/u/1065182/Picture/TortoiseHg/8.png)
这时你会发现，最新的tip版变成了FamilyProject-new里的版本了，而且多了一个分支。
但是打开文件来看，仍然没变。此时我们可以右键点最上面的改动集，选择“更新”操作（上一步也可以选择“拉取后更新”来自动完成），来切换版本。
![决定采用Mercurial(Hg)了 - keakon的blog -](https://dl.dropbox.com/u/1065182/Picture/TortoiseHg/9.png)
注意版本已经列出来了，有改动集编号、改动集hash标识、描述、是否为default和tip版本等几种提示，所以应该不会弄错。
![决定采用Mercurial(Hg)了 - keakon的blog -](https://dl.dropbox.com/u/1065182/Picture/TortoiseHg/10.png)
确认无误后就可以点“更新”按钮了，这时你会发现你的文件变成FamilyProject-new里的版本了，但自己的改动却丢失了（别紧张，文件没丢失）。
原因很简单，更新只是切换改动集版本而已，并不会进行合并。如果要换回自己的版本，右键点下面那个改动集，选“更新”就行了。
但是我的目的是合并，而非更新，所以下面继续说明合并。
操作很简单，右键点第2个分支，选择“合并”操作。
![决定采用Mercurial(Hg)了 - keakon的blog -](https://dl.dropbox.com/u/1065182/Picture/TortoiseHg/11.png)
点击“合并”按钮。
![决定采用Mercurial(Hg)了 - keakon的blog -](https://dl.dropbox.com/u/1065182/Picture/TortoiseHg/12.png)
由于改动的地方都不一样，所以并不冲突，于是顺利合并成功了。接着点“提交”按钮。
![决定采用Mercurial(Hg)了 - keakon的blog -](https://dl.dropbox.com/u/1065182/Picture/TortoiseHg/13.png)
这时就列出改动的地方了，-号表示删除，+表示新增。可以看到2处改动都正确判断了，因此不需要我们做其他的事，直接点“提交”按钮即可。
![决定采用Mercurial(Hg)了 - keakon的blog -](https://dl.dropbox.com/u/1065182/Picture/TortoiseHg/14.png)
再看看改动集，分支已经合并了，而你的文件也成功更新了。
![决定采用Mercurial(Hg)了 - keakon的blog -](https://dl.dropbox.com/u/1065182/Picture/TortoiseHg/15.png)
不过FamilyProject-new里的文件还是旧版本。
如果你不需要了，直接删除这个文件夹就行了；如果还想继续使用的话，也打开它的“HG 仓库浏览器”，选择“拉取后更新”，再点“拉取改动集”按钮：
![决定采用Mercurial(Hg)了 - keakon的blog -](https://dl.dropbox.com/u/1065182/Picture/TortoiseHg/16.png)
操作完后，2个仓库就完全一样了，你就可以继续其他操作了。
如果你改完了，需要传给别人，则需要push给别人，或叫别人来pull，这个操作实际上和本地仓库的操作是差不多的。
最简单的方法就是发布到本地的Web服务器。
右键点文件夹，选择“TortoiseHg - Web服务器”：
![决定采用Mercurial(Hg)了 - keakon的blog -](https://dl.dropbox.com/u/1065182/Picture/TortoiseHg/17.png)
出现了一个窗口（顺带一提，这个玩意很容易就能看出是Python写的），不过先别着急，还得点“配置”按钮设置一下：
![决定采用Mercurial(Hg)了 - keakon的blog -](https://dl.dropbox.com/u/1065182/Picture/TortoiseHg/18.png)
把“SSL加密推送”设为“False”，因为你没有SSL证书。在“允许推送”处填允许的用户名，为了省事，填个“*”就允许所有人推送了。下面的编码也得改成和文件编码一致，免得在线查看文件时乱码。
![决定采用Mercurial(Hg)了 - keakon的blog -](https://dl.dropbox.com/u/1065182/Picture/TortoiseHg/19.png)
确定后返回，在端口处填一个小于65535的数，默认为8000，然后点“启动”按钮。
![决定采用Mercurial(Hg)了 - keakon的blog -](https://dl.dropbox.com/u/1065182/Picture/TortoiseHg/20.png)
启动好后，就可以通过浏览器来访问了，地址为：http://你的IP:端口号/。
![决定采用Mercurial(Hg)了 - keakon的blog -](https://dl.dropbox.com/u/1065182/Picture/TortoiseHg/21.png)
接下来就试试推送吧。
打开FamilyProject-new的文件夹，更改一下文件，然后提交。
这次我就不用仓库浏览器了，因为它太庞大了，而改用小巧的“同步”操作：
![决定采用Mercurial(Hg)了 - keakon的blog -](https://dl.dropbox.com/u/1065182/Picture/TortoiseHg/22.png)
填入刚才你用浏览器访问的地址，然后点“推送”按钮，推送就完成了。
![决定采用Mercurial(Hg)了 - keakon的blog -](https://dl.dropbox.com/u/1065182/Picture/TortoiseHg/23.png)
接着你就可以去FamilyProject文件夹查看改动集了，需要的话可以按前面所说的进行更新和合并操作。
不过这个IP地址有可能是外部不能访问的，也许是网关或防火墙的设置导致的，这样你的Web服务器就不能被其他人访问了。
一个临时的解决办法就是邮寄。不过在这之前，你还得改下邮件设置。
于是右键点文件夹，选择“TortoiseHg - 全局设置”：
![决定采用Mercurial(Hg)了 - keakon的blog -](https://dl.dropbox.com/u/1065182/Picture/TortoiseHg/24.png)
在“邮件”里填上你的邮箱信息：
![决定采用Mercurial(Hg)了 - keakon的blog -](https://dl.dropbox.com/u/1065182/Picture/TortoiseHg/25.png)
这里有[Gmail的配置说明](http://mail.google.com/support/bin/answer.py?answer=78799)，其他的照着填就行了。
注意密码是邮箱的登录密码。如果你填了，会以明文的形式保存起来，所以如果你的电脑还有别人用，就不要填，不过发送邮件时就需要手动输入密码了。
配置完后回到同步窗口，点“邮寄”按钮，然后填写邮件内容。注意把“附件”勾上，然后点击发送。
![决定采用Mercurial(Hg)了 - keakon的blog -](https://dl.dropbox.com/u/1065182/Picture/TortoiseHg/26.png)
不过网络不好时很可能失败，成功后对方会收到一封邮件，包含一个.patch类型的附件。
但是TortoiseHg并不支持导入这个patch文件，只能通过命令行来执行。
于是接受者可以运行CMD，切换到需要导入的仓库，然后执行这行命令： 
##### 引用:
> hg import patch文件名
当然，命令行对某些人来说是很恐怖的，于是再换个方法：去bitbucket建立自己的仓库。
注册我就不说了，可以新注册一个账号，也可以使用OpenID。
登录以后然后到FamilyProject项目去，点击那个[fork](http://bitbucket.org/keakon/familyproject/fork/)链接。
![决定采用Mercurial(Hg)了 - keakon的blog -](https://dl.dropbox.com/u/1065182/Picture/TortoiseHg/27.png)
Fork的意思是以我的仓库为蓝本，创建一个复本。和clone差不多，只不过是在远程进行的。
在新页面填写项目的名字，然后点击“Advanced settings”，进行更多的设定，最后点“Create fork”按钮来创建。
![决定采用Mercurial(Hg)了 - keakon的blog -](https://dl.dropbox.com/u/1065182/Picture/TortoiseHg/28.png)
和你在本地clone一样，远程的fork也是几秒钟就完成了，并自动重定向到了这个仓库。（结果发现这个仓库居然小了一半，看来空间占用是很诡异的。）
![决定采用Mercurial(Hg)了 - keakon的blog -](https://dl.dropbox.com/u/1065182/Picture/TortoiseHg/29.png)
接下来你就可以对这个仓库进行操作了，方法和自建的Web服务器一样。
当改动完时，需要通知别人时，再次来到FamilyProject项目，点击那个“pull request”按钮。
在新的页面里填写邮件内容，附上你的仓库地址；再把需要通知的人勾选上；最后点“Send request”按钮发送即可。
收到邮件的人就可以去你的仓库进行pull操作来获取更新了。
之后的合并等操作前面也说过了，也就不重提了。
如果空间不够用了，再创建一个账号是最方便的方法。
当然也可以去“Admin”标签，点右下角的“Delete repository”按钮，这样就能删除不要的仓库的。
此外，这里还有个“Repository management”链接，进去后可以执行strip操作，删除不需要的版本。
不过删除后好像没增加空间，郁闷了…于是发信给客服，整了半天还没搞定，我们通信的频率大概是2小时一次…
貌似没什么其他的要说了，于是本教程结束
