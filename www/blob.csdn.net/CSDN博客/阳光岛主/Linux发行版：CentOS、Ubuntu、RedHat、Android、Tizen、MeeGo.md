
# Linux发行版：CentOS、Ubuntu、RedHat、Android、Tizen、MeeGo - 阳光岛主 - CSDN博客

2013年08月10日 06:16:14[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：20352


[Linux](http://en.wikipedia.org/wiki/Linux)，最早由Linus Benedict[Torvalds](http://en.wikipedia.org/wiki/Linus_Torvalds)在1991年开始编写。在这之前，Richard Stallman创建了Free Software Foundation（[FSF](http://en.wikipedia.org/wiki/Free_Software_Foundation)）组织以及GNU项目，并不断的编写创建[GNU](http://en.wikipedia.org/wiki/GNU)程序（此类程序的许可方式均为[GPL](http://en.wikipedia.org/wiki/GPL): General Public License）。在不断的有杰出的程序员和开发者加入到GNU组织中后，便造就了今天我们所看到的Linux，或称GNU/Linux。
Linux的发行版本可以大体分为两类，一类是商业公司维护的发行版本，一类是社区组织维护的发行版本
前者以著名的Redhat（RHEL）为代表，后者以Debian为代表。

**Linux 发行版：**
**Redhat**，应该称为Redhat系列，包括RHEL(Redhat Enterprise Linux，也就是所谓的Redhat Advance Server，收费版本)、Fedora Core(由原来的Redhat桌面版本发展而来，免费版本)、CentOS(RHEL的社区克隆版本，免费)。Redhat应该说是在国内使用人群最多的Linux版本，甚至有人将Redhat等同于Linux，而有些老鸟更是只用这一个版本的Linux。所以这个版本的特点就是使用人群数量大，资料非常多，言下之意就是如果你有什么不明白的地方，很容易找到人来问，而且网上的一般Linux教程都是以Redhat为例来讲解的。Redhat系列的包管理方式采用的是基于RPM包的YUM包管理方式，包分发方式是编译好的二进制文件。稳定性方面RHEL和CentOS的稳定性非常好，适合于服务器使用，但是Fedora Core的稳定性较差，最好只用于桌面应用。
**Debian**，或者称Debian系列，包括Debian和Ubuntu等。Debian是社区类Linux的典范，是迄今为止最遵循GNU规范的Linux系统。Debian最早由Ian Murdock于1993年创建，分为三个版本分支（branch）： stable, testing 和 unstable。其中，unstable为最新的测试版本，其中包括最新的软件包，但是也有相对较多的bug，适合桌面用户。testing的版本都经 过unstable中的测试，相对较为稳定，也支持了不少新技术（比如SMP等）。而stable一般只用于服务器，上面的软件包大部分都比较过时，但是 稳定和安全性都非常的高。Debian最具特色的是apt-get / dpkg包管理方式，其实Redhat的YUM也是在模仿Debian的APT方式，但在二进制文件发行方式中，APT应该是最好的了。Debian的资 料也很丰富，有很多支持的社区，有问题求教也有地方可去:)
**Ubuntu****，**严格来说不能算一个独立的发行版本，Ubuntu是基于Debian的unstable版本加强而来，可以这么说，Ubuntu就是 一个拥有Debian所有的优点，以及自己所加强的优点的近乎完美的 Linux桌面系统。根据选择的桌面系统不同，有三个版本可供选择，基于Gnome的Ubuntu，基于KDE的Kubuntu以及基于Xfc的 Xubuntu。特点是界面非常友好，容易上手，对硬件的支持非常全面，是最适合做桌面系统的Linux发行版本。
**Gentoo**，伟大的Gentoo是Linux世界最年轻的发行版本，正因为年轻，所以能吸取在她之前的所有发行版本的优点，这也是Gentoo 被称为最完美的Linux发行版本的原因之一。Gentoo最初由Daniel Robbins（FreeBSD的开发者之一）创建，首个稳定版本发布于2002年。由于开发者对FreeBSD的熟识，所以Gentoo拥有媲美 FreeBSD的广受美誉的ports系统 ——Portage包管理系统。不同于APT和YUM等二进制文件分发的包管理系统，Portage是基于源代码分发的，必须编译后才能运行，对于大型软 件而言比较慢，不过正因为所有软件都是在本地机器编译的，在经过各种定制的编译参数优化后，能将机器的硬件性能发挥到极致。Gentoo是所有Linux 发行版本里安装最复杂的，但是又是安装完成后最便于管理的版本，也是在相同硬件环境下运行最快的版本。
**FreeBSD**，是一个自由的、免费的类UNIX操作系统(Unix-like)，经由BSD UNIX由AT&T UNIX衍生而来，FreeBSD由于法律原因不能称为UNIX，但由于直接衍生于BSD UNIX，并且一些原来BSD UNIX的开发者后来转到FreeBSD的开发，使得FreeBSD在内部结构和系统API上和UNIX有很大的兼容性。由于FreeBSD宽松的法律条款，其代码被好多其他系统借鉴包括苹果公司的MacOS X，正因此由于MacOS X的UNIX兼容性，使得MacOS X获得了UNIX商标认证。
它并不是一个Linux系统！但FreeBSD与Linux的用户群有相当一部分是重合的，二者支持的硬件环境也比较一致，所采用的软件也比较类似，所以可以将FreeBSD视为一个Linux版本来比较。FreeBSD拥有两个分支： stable和current。顾名思义，stable是稳定版，而 current则是添加了新技术的测试版。FreeBSD采用Ports包管理系统，与Gentoo类似，基于源代码分发，必须在本地机器编后后才能运行，但是Ports系统没有Portage系统使用简便，使用起来稍微复杂一些。FreeBSD的最大特点就是稳定和高效，是作为服务器操作系统的最佳选 择，但对硬件的支持没有Linux完备，所以并不适合作为桌面系统。

**SUSE Linux**，简称"SuSE"，发音为/zuz/，意思为"Software- und System-Entwicklung"，这是一句德文，英文为"Software and system development"。现在这家公司的名字再度更改成SUSE Linux。它原是德国的 SuSE Linux AG公司发行维护的Linux发行版，是属于此公司的注册商标。2004年这家公司被Novell公司收购。广义上SUSELinux是一系列Linux发行版，第一个版本出现在1994年初，SUSE现存的最古老的商业发行版之一，起源于德国，而SUSE Linux 针对个人用户。SUSELinux原是以Slackware Linux为基础，并提供完整德文使用界面的产品。1992年 Peter McDonald成立了Softlanding Linux System(SLS)这个发行版。这套发行版包含的软件非常多，更首次收录了X Window及TCP/IP等套件。Slackware就是一个基于SLS的发行版。

**Mandriva**，是目前全球最优秀的Linux发行版之一，稳居于linux排行榜第一梯队。2005年之前稳居linux排行榜NO.1。它是目前最易用的linux发行版，也是众多国际级linux发行版中唯一一个默认即支持中文环境的linux。它是法国的Mandriva公司（前身为Mandrake公司）开发的Linux发行版。Mandriva公司现在仍然是欧洲最大的Linux厂商，Mandriva Linux的前身为著名的Mandrake Linux。Mandriva（Mandrake）项目是世界上第一个为非技术类用户设计的易于使用、安装和管理的linux版本。Mandriva（Mandrake Linux）早期方便的字体安装工具和默认的中文支持，为Linux普及做出了很大的贡献。现在的Mandriva是由mandrake和Conectiva结合发展而来的。

**Slackware**，是由Patrick Volkerding开发的GNU/Linux发行版。与很多其他的发行版不同，它坚持KISS（Keep It Simple Stupid）的原则，就是说没有任何配置系统的图形界面工具。一开始，配置系统会有一些困难，但是更有经验的用户会喜欢这种方式的透明性和灵活性。 Slackware Linux的另一个突出的特性也符合KISS原则：Slackware没有如RPM之类的成熟的软件包管理器。Slackware的软件包都是通常的tgz(tar/gzip)格式文件再加上安装脚本。Tgz对于有经验的用户来说，比RPM更为强大，并避免了RPM之类管理器的依赖性问题。Slackware与其他的发行版本（Red Hat、Debian、Gentoo、SuSE、 Mandriva、Ubuntu等）不同的道路，它力图成为“UNIX风格”的Linux发行版本。只吸收稳定版本的应用程序，并且缺少其他linux版本中那些为发行版本定制的配置工具。


**Linux 发行版（移动）：**
**Android**，是一种基于Linux的自由及开放源代码的操作系统，主要使用于移动设备，如智能手机和平板电脑，由Google公司和开放手机联盟领导及开发。尚未有统一中文名称，中国大陆地区较多人使用“安卓”或“安致”。Android操作系统最初由Andy Rubin开发，主要支持手机。2005年8月由Google收购注资。2007年11月，Google与84家硬件制造商、软件开发商及电信营运商组建开放手机联盟共同研发改良Android系统。随后Google以Apache开源许可证的授权方式，发布了Android的源代码。第一部Android智能手机发布于2008年10月。Android逐渐扩展到平板电脑及其他领域上，如电视、数码相机、游戏机等。2011年第一季度，Android在全球的市场份额首次超过塞班系统，跃居全球第一。 2012年11月数据显示，Android占据全球智能手机操作系统市场76%的份额，中国市场占有率为90%。

**MeeGo**，是一种基于Linux的自由、开放源代码的移动便携设备操作系统。它在2010年2月的全球移动通信大会中发布，主要推动者为诺基亚与英特尔。MeeGo融合了诺基亚的Maemo及英特尔的Moblin平台，并由Linux基金会主导。MeeGo主要定位在移动设备、家电数码等消费类电子产品市场，可用于智能手机、平板电脑、上网本、智能电视和车载系统等平台。2011年9月28日，继诺基亚宣布放弃开发meego之后，英特尔也正式宣布将MeeGo与LiMo合并成为新的系统：Tizen。2012年7月，在诺基亚的支持下，Jolla Mobile公司成立。并基于Meego研发sailfish os，将在华发布新一代Jolla手机。

**Chrome OS**，是一款基于Linux的开源PC操作系统，定位于上网本、紧凑型以及低成本电脑。这款开源软件将被命名为Chrome OS，谷歌公司于2010年12月7日在美国举行Chrome相关产品发布会，发布会上正式发布Chrome Web store和Chrome OS。Chromium OS是Google Chrome OS计算机操作系统的开源项目，主要是提供给开发者用的，每个人可以检查、修改代码甚至是编译出自己的版本。

**webOS**，是一个嵌入式操作系统，以Linux内核为主体并加上部份Palm公司开发的专有软件，它主要是为Palm 智能手机而开发。该平台于2009年1月8日的拉斯维加斯国际消费电子展宣布给公众，并于2009年6月6日发布。该平台是事实上的PalmOS继任者，webOS将在线社交网络和Web 2.0一体化作为重点。第一款搭载webOS系统的智能手机是Palm Pre，于2009年6月6日发售。由于Palm被HP收购，webOS被收归HP旗下。2011年8月19日凌晨，在惠普第三季度财报会议上，惠普宣布正式放弃围绕TouchPad平板电脑和webOS手机的所有运营。

**Tizen**，（中文名：泰泽）是两大Linux联盟 LiMo Foundation和 Linux Foundation整合资源优势，携手英特尔和三星电子，共同开发针对手机和其它设备的操作系统。该系统整合了LiMo和MeeGo两个操作系统，并且预计于2012年2月正式公布。Tizen系统是英特尔MeeGo系统与三星LiMo系统的混合体。 　随着惠普放弃开发webOS智能手机和平板电脑，RIM的Playbook艰难求生，诺基亚剧变，运营商们希望能出现一个可替代的新操作系统，没人希望看到Android和iOS双寡头垄断的局面。 　同时由于英特尔、三星各自在MeeGo及Bada发展遭遇困难，双方便携手全新平台Tizen系统，这样可以降低开发资源与人力，并强化技术及应用程序商店功能，包括宏碁、华硕均将加入Tizen平台开发计划，宏达电也正评估中，Tizen平台是否有机会形成 OS 平台第4势力，与Android、iOS、Windows相较劲，备受业界关注。

**BADA**，是韩国三星电子自行开发的智能手机平台，底层为Linux核心。支援丰富功能和用户体验的软件应用，于2009年11月10日发布。bada在韩语裏是“海洋”的意思。bada的设计目标是开创人人能用的智能手机的时代。它的特点是配置灵活、用户交互性佳、面向服务优，非常重视SNS整合和基于位置服务应用。bada系统由操作系统内核层、设备层、服务层和框架层组成，支持设备应用、服务应用和Web与Flash应用。


**Linux 发行版（****国内****）：**
**RedFlag**，红旗Linux是由北京中科红旗软件技术有限公司开发的一系列Linux发行版，包括桌面版、工作站版、数据中心服务器版、HA集群版和红旗嵌入式Linux等产品。目前在中国各软件专卖店可以购买到光盘版，同时官方网站也提供光盘镜像免费下载。红旗Linux是中国较大、较成熟的Linux发行版之一。

**Kylin**，中标麒麟（原名中标普华Linux），2010年12月底，中标普华与国防科技大学强强联手进行操作系统方面的技术合作，合作之后的中标普华Linux操作系统更名为“中标麒麟”，而且也进一步更换了新操作系统的Logo。此次合作，是民用“中标普华”操作系统的易操作性与军用的“银河麒麟”的高度安全性在技术上的深层次合作，双方分别将各自的特色性能融合到新操作系统中。中标麒麟操作系统采用强化的Linux内核，分成桌面版、通用版、高级版和安全版等，满足不同客户的要求，已经广泛的使用在能源、金融、交通、政府、央企等行业领域。中标麒麟增强安全操作系统采用银河麒麟KACF强制访问控制框架和RBA角色权限管理机制，支持以模块化方式实现安全策略，提供多种访问控制策略的统一平台，是一款真正超越“多权分立”的B2级结构化保护操作系统产品。

**华镭Linux**，通用操作系统RAYS 是由新华科技系统软件有限公司自主研发的中文Linux操作系统。它在Linux稳定内核的基础上融合了多项先进技术，能全面满足政府、公司、个人、家庭在办公、娱乐、学习和教育等多方面的应用需求。此外，华镭通用操作系统全面支持包括龙芯、众志在内的多款国产CPU，实现了“国产CPU+国产操作系统+国产应用软件”的完整产业链。

**Magic Linux**，是一个新的Linux发行，简称 MGC，是针对中国人的版本，由中国Linux公社的社区会员和Linux爱好者制作的中文 Linux 发行版，现基于Fedora修改而来。Magic Linux的目的很简单：让大家真正地用Linux，而不是去一个接一个的汉化Linux。Magic Linux的口号是：没有实现不了的梦。2002年，cjacker 在中国Linux公社的论坛发布了自己为了学习而做的 Linux中文版，在 Red Hat 8.0的基础上修改而成，版本1.0。之后 cjacker 投身于北京中科红旗软件技术有限公司，Magic Linux 后续开发遇到了前所未有的困难。KanKer、KDE、sejishikong 等中国 Linux 公社会员加入了开发，重写了 Magic Linux，发布了 Magic Linux 2.0，最终形成了现在独立的稳定发行版本 2.5.X，2012年开发组正积极地研发以 Live 安装为主的 3.0。

**共创Linux**，一款基于Linux的桌面操作系统，功能丰富，可以部分地替代现有常用的Windows桌面操作系统。它采用类似于Windows XP风格的图形用户界面，符合Windows XP的操作习惯，让用户使用起来感觉更熟悉，更易用，降低了培训成本。提供了优秀的中文支持能力，字体显示效果十分美观。高度智能化的中文拼音输入法，使用方便、输入效率高，达到Windows下拼音输入法同等的水平。


**Linux 桌面版：**
如果你只是需要一个桌面系统，而且既不想使用盗版，又不想花大量的钱购买商业软件，那么你就需要一款适合桌面使用的Linux发行版本了，如：Ubuntu
如果你不想自己定制任何东西，不想在系统上浪费太多时间，那么很简单，你就根据自己的爱好在ubuntu、kubuntu以及xubuntu中选一款吧，三者的区别仅仅是桌面程序的不一样。
如果你需要一个桌面系统，而且还想非常灵活的定制自己的Linux系统，想让自己的机器跑得更欢，不介意在Linux系统安装方面浪费一点时间，那么你的唯一选择就是Gentoo，尽情享受Gentoo带来的自由快感吧！
如果你需要的是一个服务器系统，而且你已经非常厌烦各种Linux的配置，只是想要一个比较稳定的服务器系统而已，那么你最好的选择就是CentOS了，安装完成后，经过简单的配置就能提供非常稳定的服务了。
如果你需要的是一个坚如磐石的非常稳定的服务器系统，那么你的唯一选择就是FreeBSD。
如果你需要一个稳定的服务器系统，而且想深入摸索一下Linux的各个方面的知识，想自己定制许多内容，那么我推荐你使用Gentoo。

**Linux****服务器：**
**CentOS**，其实是Redhat的内核，感觉是会成熟稳定一点，相比旧Redhat，他多了一个yum的命令，学Debian一样自动安装软件；但我感觉CentOS的可以Yum的东西相比Debian或Ubuntu实在太少了；CentOS介于年轻与成熟之间，它比Ubuntu更注重安全性，默认一装好就自带并打开防火墙就知道；而且有一个叫SELinux的东东在里面，让不同的程序之间权限有所限制，这也是我装CentOS一些软件经常莫名奇妙地告诉我“没有权限”的一个原因。后来因为一些软件搞了一夜，都没有搞成功，比如VSftp+PAM_MYSQL的认证，一气之下把他格掉了。
**[Ubuntu](http://www.ubuntu.com/)**，就是Debian的延续版，apt-get自动安装、更新软件的方式，是Debian首创。相比起来apt-get软件库及源强大很多，也多很多，我们的服务器除了一个magickWand(imageMagick的PHP API)，其它东西全部都是apt-get下来的，安装不需要一分钟；感觉Ubuntu更加自由，小巧，灵活，有点年轻人的感觉。
**[Gentoo](http://www.gentoo.org/)**，估计是性能最好的，因为他安装超麻烦；据说他的理念就是，让你明白Linux的每一样东西是如何出来的!全部由你定制来决定。
**[FreeBSD](https://www.freebsd.org/)**，是另外一种类UNIX内核，不属于Linux，适做服务器就是采用FreeBSD，据说是性能最强劲的。但内核不一样，我们这次不敢尝试。



### Linux发行版 (列表)
|基于Debian|▪|[Debian](http://baike.baidu.com/view/40687.htm)|▪|[Ubuntu](http://baike.baidu.com/view/4236.htm)|▪|[Linux Mint](http://baike.baidu.com/view/872449.htm)|
|▪|[Knoppix](http://baike.baidu.com/view/638124.htm)|▪|[MEPIS](http://baike.baidu.com/view/745998.htm)|▪|[sidux](http://baike.baidu.com/searchword/?word=sidux&pic=1&sug=1&enc=utf8)|
|▪|[CrunchBang Linux](http://baike.baidu.com/view/9300153.htm)|▪|[Chromium OS](http://baike.baidu.com/view/4657115.htm)|▪|[Google Chrome OS](http://baike.baidu.com/view/2627636.htm)|
|
|
|基于Red Hat|▪|[Red Hat Enterprise Linux](http://baike.baidu.com/view/1389601.htm)|▪|[Fedora](http://baike.baidu.com/view/182182.htm)|▪|[CentOS](http://baike.baidu.com/view/26404.htm)|
|▪|[Scientific Linux](http://baike.baidu.com/view/4076421.htm)|▪|[Oracle Linux](http://baike.baidu.com/view/4615585.htm)|
|
|
|基于Mandriva|▪|[Mandriva Linux](http://baike.baidu.com/view/1525837.htm)|▪|[PCLinuxOS](http://baike.baidu.com/view/1215788.htm)|▪|[Unity Linux](http://baike.baidu.com/view/9300626.htm)|
|▪|[Mageia](http://baike.baidu.com/view/4720774.htm)|
|
|
|基于Gentoo|▪|[Gentoo Linux](http://baike.baidu.com/view/66593.htm)|▪|[Sabayon Linux](http://baike.baidu.com/view/1525544.htm)|▪|[Calculate Linux](http://baike.baidu.com/view/9301132.htm)|
|▪|[Funtoo Linux](http://baike.baidu.com/searchword/?word=Funtoo%20Linux&pic=1&sug=1&enc=utf8)|
|
|
|基于Slackware|▪|[Slackware](http://baike.baidu.com/view/10899.htm)|▪|[Zenwalk](http://baike.baidu.com/view/4139948.htm)|▪|[VectorLinux](http://baike.baidu.com/view/9301252.htm)|
|
|
|其它|▪|SUSE|▪|[Arch Linux](http://baike.baidu.com/view/2090428.htm)|▪|[Puppy Linux](http://baike.baidu.com/view/1545351.htm)|
|▪|[Damn Small Linux](http://baike.baidu.com/view/923420.htm)|▪|[MeeGo](http://baike.baidu.com/view/3263982.htm)|▪|[Slitaz](http://baike.baidu.com/view/1746302.htm)|
|▪|[Tizen](http://baike.baidu.com/view/6565213.htm)|▪|[StartOS](http://baike.baidu.com/view/9148046.htm)|
|
|


![](https://img-blog.csdn.net/20130810112430625)



**参考推荐：**
[Linux扫盲篇：CentOS、Ubuntu、Gentoo](http://www.williamlong.info/info/archives/197.html)
[国内十大Linux版本一览](http://bbs.pcbeta.com/viewthread-455179-1-1.html)
[国内Linux发行版介绍](http://hi.baidu.com/senya/item/d193b512e51f238c88a95698)



