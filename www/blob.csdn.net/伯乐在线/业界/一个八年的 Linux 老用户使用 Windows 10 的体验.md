# 一个八年的 Linux 老用户使用 Windows 10 的体验 - 文章 - 伯乐在线
原文出处： [Avishek Kumar](http://www.tecmint.com/a-linux-user-using-Windows-10-after-more-than-8-years-see-comparison/)   译文出处：[Linux中国](https://linux.cn/article-7000-1.html)
Windows 10 是2015年7月29日上市的最新一代 Windows NT 系列系统，它是 Windows 8.1 的继任者。Windows 10 支持 Intel 32位平台，AMD64 以及 ARM v7 处理器。
![对比：Windows 10与Linux](http://ww4.sinaimg.cn/large/0064cTs2gw1f11g983oujj30k009lgmp.jpg)
*对比：Windows 10与Linux*
作为一个连续使用 linux 超过8年的用户，我想要去体验一下 Windows 10 ，因为有很多关于它的消息。这篇文章是我观察力的一个重大突破。我将从一个 linux 用户的角度去看待一切，所以这篇文章可能会有些偏向于 linux。尽管如此，本文也绝对不会有任何虚假信息。
1、用谷歌搜索“download Windows 10”并且点击第一个链接。
![搜索 Windows 10](http://ww2.sinaimg.cn/large/0064cTs2gw1f11g97nuzlj30lv071jrv.jpg)
*搜索 Windows 10*
你也可以直接打开： [[https://www.microsoft.com/en_us/software-download/Windows10[1]](https://www.microsoft.com/en_us/software-download/Windows10%5B1%5D)
2、微软要求我从 Windows 10， Windows 10 KN， Windows 10 N 和 Windows 10 单语言版中选择一个版本。
![选择版本](http://ww4.sinaimg.cn/mw690/0064cTs2gw1f11g98mf2tj30b304vt8s.jpg)
*选择版本*
以下是各个版本的简略信息：
- Windows 10 – 包含微软提供给我们的所有软件
- Windows 10N – 此版本不包含媒体播放器
- Windows 10KN – 此版本没有媒体播放能力
- Windows 10 单语言版 – 仅预装一种语言
3、我选择了第一个选项“Windows 10”并且单击“确认”。之后我要选择语言，我选择了“英语”。
微软给我提供了两个下载链接。一个是32位版，另一个是64位版。我单击了64位版–这与我的电脑架构相同。
![下载 Windows 10](http://ww2.sinaimg.cn/large/0064cTs2gw1f11g9951h9j30n20cm0t5.jpg)
*下载 Windows 10*
我的带宽是15M的，下载了整整3个小时。不幸的是微软没有提供系统的种子文件，否则整个过程会更加舒畅。镜像大小为 3.8 GB（LCTT译者注：就我的10M小水管，我使用迅雷下载用时50分钟）。
我找不到更小的镜像，微软并没有为 Windows 提供网络安装镜像。我也没有办法在下载完成后去校验哈希值。（LCTT 译注：你知道的，这对于 Linux 来说都是常识了）
我十分惊讶，Windows 在这样的问题上居然如此漫不经心。为了验证这个镜像是否正确下载，我需要把它刻到光盘上或者复制到我的U盘上然后启动它，一直静静的看着它安装直到安装完成。
首先，我用 dd 命令将 win10 的 iso 镜像刻录到U盘上。
- `# dd if=/home/avi/Downloads/Win10_English_x64.iso of=/dev/sdb1 bs=512M; sync`
这需要一点时间。在此之后我重启系统并在 UEFI（BIOS）设置中选择从我的U盘启动。
#### 系统要求
升级
- 仅支持从 Windows 7 SP1 或者 Windows 8.1 升级
全新安装
- 处理器: 1GHz 以上
- 内存: 1GB以上(32位)，2GB以上(64位)
- 硬盘: 16GB以上(32位)，20GB以上(64位)
- 显卡: 支持DirectX 9或更新 + WDDM 1.0 驱动
### Windows 10 安装过程
1、Windows 10启动成功了。他们又换了logo，但是仍然没有信息提示我它正在做什么。
![Windows 10 Logo](http://ww4.sinaimg.cn/mw690/0064cTs2gw1f11g9s5s9mj30sg0lcweo.jpg)
*Windows 10 Logo*
2、选择安装语言，时区，键盘，输入法，点击下一步。
![选择语言和时区](http://ww2.sinaimg.cn/large/0064cTs2gw1f11g9sk0a9j30sg0lcwfj.jpg)
*选择语言和时区*
3、点击“现在安装”。
![安装Ｗindows 10](http://ww3.sinaimg.cn/large/0064cTs2gw1f11g9ssqkcj30sg0lc0te.jpg)
*安装Ｗindows 10*
4、下一步是输入密钥，我点击了“跳过”。
![Windows 10 产品密钥](http://ww2.sinaimg.cn/large/0064cTs2gw1f11g9tcy8vj30sg0lct9u.jpg)
*Windows 10 产品密钥*
5、从列表中选择一个系统版本。我选择了 Windows 10专业版。
![选择系统版本](http://ww2.sinaimg.cn/large/0064cTs2gw1f11g9tzmpbj30sg0lcjsi.jpg)
*选择系统版本*
6、到了协议部分，选中”我接受”然后点击下一步。
![同意协议](http://ww2.sinaimg.cn/large/0064cTs2gw1f11g9uhxtwj30sg0lcdhr.jpg)
*同意协议*
7、下一步是选择（从 Windows 的老版本）升级到 Windows 10 或者安装 Ｗindows。我搞不懂为什么微软要让我自己选择：“安装Ｗindows”被微软建议为“高级”选项。但是我还是选择了“安装Windows”。
![选择安装类型](http://ww3.sinaimg.cn/large/0064cTs2gw1f11g9uxubaj30sg0lc75p.jpg)
*选择安装类型*
8、选择驱动器，点击“下一步”。
![选择安装盘](http://ww3.sinaimg.cn/large/0064cTs2gw1f11g9vax6pj30sg0lcwfq.jpg)
*选择安装盘*
9、安装程序开始复制文件，准备文件，安装更新，之后进行收尾。如果安装程序能在安装时输出一堆字符来表示它在做什么就更好了。
![安装 Windows](http://ww1.sinaimg.cn/large/0064cTs2gw1f11g9vrczqj30sg0lcmy0.jpg)
*安装 Windows*
10、在此之后 Windows 重启了。它们说要继续的话，我们需要重启。
![安装进程](http://ww1.sinaimg.cn/large/0064cTs2gw1f11g9w6h2qj30sg0lc3zd.jpg)
*安装进程*
11、我看到了一个写着“正在准备 Windows”的界面。它停了整整五分多钟！仍然没有说明它正在做什么。没有输出。
![正在准备 Windows](http://ww2.sinaimg.cn/large/0064cTs2gw1f11g8cru8zj30sg0lcmxe.jpg)
*正在准备 Windows*
12、又到了输入产品密钥的时间。我点击了“以后再说”，并使用快速设置。
![输入产品密钥](http://ww4.sinaimg.cn/large/0064cTs2gw1f11g8dceesj30sg0lct9x.jpg)
*输入产品密钥*
![使用快速设置](http://ww1.sinaimg.cn/large/0064cTs2gw1f11g8drbuyj30sg0lcac2.jpg)
*使用快速设置*
13、又出现了三个界面，作为 Linux 用户我认为此处应有信息来告诉我安装程序在做什么，但是我想多了。
![载入 Windows](http://ww3.sinaimg.cn/large/0064cTs2gw1f11g8e4uufj30sg0lcq34.jpg)
*载入 Windows*
![获取更新](http://ww2.sinaimg.cn/large/0064cTs2gw1f11g8ehtapj30sg0lcwex.jpg)
*获取更新*
![还是载入 Ｗindows](http://ww3.sinaimg.cn/large/0064cTs2gw1f11g8f3aahj30sg0lcaah.jpg)
*还是载入 Ｗindows*
14、安装程序想要知道谁拥有这台机器，“我的组织”或者我自己。选择我自己并继续。
![选择组织](http://ww3.sinaimg.cn/large/0064cTs2gw1f11g8fdfquj30sg0lcwf5.jpg)
*选择组织*
15、在单击继续之前，安装程序提示我加入“Aruze Ad”或者“加入域”。我选择了后者。
![连接网络](http://ww2.sinaimg.cn/large/0064cTs2gw1f11g8fulepj30sg0lcgm7.jpg)
*连接网络*
16、安装程序让我新建一个账户。所以我输入了“user_name”就点击了下一步，我觉得我会收到一个要求我必须输入密码的信息。
![新建账户](http://ww1.sinaimg.cn/large/0064cTs2gw1f11g8gf7xkj30sg0lc755.jpg)
*新建账户*
17、让我惊讶的是 Windows 甚至都没有显示一个警告或提示信息，告诉我必须创建密码。真粗心。不管怎样，现在我可以体验系统了。
![Windows 10的桌面环境](http://ww1.sinaimg.cn/large/0064cTs2gw1f11g8gw78zj30sg0lc0to.jpg)
*Windows 10的桌面环境*
#### Linux 用户(我)直到现在的体验
- 没有网络安装镜像
- 镜像文件太臃肿了
- 没有验证 iso 是否为正确的方法（官方没有提供哈希值）
- 启动与安装方式仍然与 XP，Win 7，Win 8 相同（可能吧…）
- 和以前一样，安装程序没有输出它正在干什么 – 正在复制什么和正在安装什么软件包
- 安装程序比 Linux 发行版的更加傻瓜和简单
#### 测试 Windows 10
18、默认桌面很干净，上面只有一个回收站图标。我们可以直接从桌面搜索网络。底部的快捷方式分别是任务预览、网络、微软应用商店。和以前的版本一样，消息栏在右下角。
![桌面图标](http://ww1.sinaimg.cn/large/0064cTs2gw1f11gabth39j30sg0lcwg6.jpg)
*桌面图标*
19、IE 浏览器被换成了 Edge 浏览器。微软把他们的老IE换成了 Edge （斯巴达计划）
![Edge 浏览器](http://ww2.sinaimg.cn/large/0064cTs2gw1f11gac6tltj30sg0lcmz9.jpg)
*Edge 浏览器*
测试起来，这个浏览器至少比 IE 要快。他们有相同的用户界面。它的主页包含新闻更新。它还有一个搜索标题栏是“下一步怎么走”。由于浏览器的全面性能提升，它的加载速度非常快。Edge 的内存占用看起来正常。
![性能](http://ww2.sinaimg.cn/large/0064cTs2gw1f11gacvim5j30sg0lcdi6.jpg)
*性能*
Edge 也有小娜加成（智能个人助理）、支持 Chrome 扩展、支持笔记（在浏览网页时记笔记）、分享（在选项卡上右击而不必打开其他选项卡）
#### Linux 用户(我)此时体验
20、微软确实提升了网页浏览体验。我绝对稳定性和质量还好。现在它并不落后。
21、对我来说，Edge 的内存占用不算太大。但是有很多用户抱怨它的内存占用太多。
22、很难说目前 Edge 已经准备好了与火狐或 Chrome竞争。让我们静观其变。
#### 更多的视觉体验
23、重新设计的开始菜单 — 看起来很简洁高效。Merto 磁贴大部分都会动。预先放置了最通用的应用。
![Windows](http://ww3.sinaimg.cn/large/0064cTs2gw1f11gap513lj30sg0lcmz2.jpg)
*Windows*
而在 Linux 的 Gnome 桌面环境下。我仅仅需要按下 Win 键并输入应用名就可以搜索应用。
![桌面内进行搜索](http://ww2.sinaimg.cn/large/0064cTs2gw1f11gapsaa5j311y0lcmyt.jpg)
*桌面内进行搜索*
24、文件浏览器 — 设计的很简洁。左边是进入文件夹的快捷方式。
![Windows 文件管理器](http://ww4.sinaimg.cn/large/0064cTs2gw1f11gaqilnsj30sg0lcjsx.jpg)
*Windows 文件管理器*
我们的 Gnome 下的文件管理也同样的简洁高效。从图标上移走了不需要的图形图像是个加分点。
![Gnome 的文件管理](http://ww3.sinaimg.cn/large/0064cTs2gw1f11gaq1ygjj30ph0fawff.jpg)
*Gnome 的文件管理*
25、设置 — 尽管 Windows 10的设置有点精炼，但是我们还是可以把它与 linux 的设置进行对比。
**Windows 的设置**
![Windows 10 设置](http://ww2.sinaimg.cn/large/0064cTs2gw1f11gar9wnkj30sg0lcgnb.jpg)
*Windows 10 设置*
**Linux Gnome 上的设置**
![Gnome 的设置](http://ww2.sinaimg.cn/large/0064cTs2gw1f11gari7iyj30m00i4wfg.jpg)
*Gnome 的设置*
26、应用列表 — 目前，Linux上的应用列表要好于之前的版本（据我所记，那时我还是一个普通的 Windows 用户），但是 Windows 10 的还比 Gnome 3 的差一点。
**Windows 的应用列表**
![Windows 10 的应用列表](http://ww4.sinaimg.cn/large/0064cTs2gw1f11gas4xejj30sg0lcwgk.jpg)
*Windows 10 的应用列表*
**Gnome3 的应用列表**
![Gnome3 的应用列表](http://ww3.sinaimg.cn/large/0064cTs2gw1f11gasqm9gj311y0lc76g.jpg)
*Gnome3 的应用列表*
27、虚拟桌面 — Windows 10 上的虚拟桌面是近来被提及最多的特性之一。
这是 Windows 10 上的虚拟桌面。
![Windows 的虚拟桌面](http://ww3.sinaimg.cn/large/0064cTs2gw1f11gat5z14j30sg0lcmz3.jpg)
*Windows 的虚拟桌面*
这是我们 Linux 用户使用了超过20年的虚拟桌面。
![Linux 的虚拟桌面](http://ww2.sinaimg.cn/large/0064cTs2gw1f11gatg6dsj311y0lc77o.jpg)
*Linux 的虚拟桌面*
#### Windows 10 的其他新特性
28、Windows 10 自带 wifi 感知。它会把你的 wifi 密码分享给他人。任何在你 wifi 范围内并且曾经通过 Skype， Outlook， Hotmail 或 Facebook与你联系的人都能够获得你的网络接入权。这个特性的本意是让用户可以省时省力的连接网络。
在微软对于 Tecmint 的问题的回答中，他们说道 — 用户需要在每次到一个新的网络环境时自己去同意打开 wifi 感知。如果我们考虑到网络安全这将是很不安全的一件事。微软的说法并没有说服我。
29、从 Windows 7 和 Windows 8.1 升级可以省下买新版的花费。（家庭版 $119 专业版$199）
30、微软发布了第一个累积更新，这个更新在一小部分设备上会让系统一直重启。Windows可能不知道这个问题或者不知道它发生的原因。
31、微软内建的“禁用/隐藏我不想要的更新”的功能在我这不起作用。这意味着一旦更新开始推送，你没有方法去禁用/隐藏他们。对不住啦，Windows 用户。
#### Windows 10 包含的来源于 Linux 的功能
Windows 10 有很多直接取自 Linux 的功能。如果 Linux 不以 GPL 发布的话，也许以下这些功能永远不会出现在 Windows上。
32、命令行的包管理器 — 是的，你没有听错！Windows 10内建了一个包管理器。它只在 Power Shell 下工作。OneGet 是Windows 的官方包管理器。
![Windows 10的包管理器](http://ww2.sinaimg.cn/large/0064cTs2gw1f11gau5cboj30sg0lcgna.jpg)
*Windows 10的包管理器*
- 无边窗口
- 扁平化图标
- 虚拟桌面
- 离线/在线搜索一体化
- 手机/桌面系统融合
### 总体印象
- 响应速度提升
- 动画很好看
- 资源占用少
- 电池续航提升
- Edge 浏览器很稳定
- 支持树莓派 2
- Windows 10 好的原因是 Windows 8/8.1 没有达到公众预期并且坏的可以
- 旧瓶装新酒：Windows 10基本上就是以前的那一套换上新的图标
测试后我对 Windows 10 的评价是：Windows 10 在视觉和感觉上做了一些更新（就如同 Windows 经常做的那样）。我要为斯巴达计划、虚拟桌面、命令行包管理器、整合在线/离线搜索的搜索栏点赞。这确实是一个更新后的产品 ，但是认为 Windows 10 将是 Linux 的最后一个棺材钉的人错了。
Linux 走在 Windows 前面。它们的做事方法并不相同。在以后的一段时间里 Windows 不会站到 Linux这一旁。也没有什么让 Linux 用户值得去使用 Windows 10。
这就是我要说的。希望你喜欢本文。如果你们喜欢本篇文章我会再写一些你们喜欢读的有趣的文章。在下方留下你的有价值的评论。
