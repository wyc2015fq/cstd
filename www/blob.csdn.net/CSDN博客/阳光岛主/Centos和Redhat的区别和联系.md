
# Centos和Redhat的区别和联系 - 阳光岛主 - CSDN博客

2013年08月11日 01:24:39[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：15500


RedHat在发行的时候，有两种方式：二进制和源代码的发行方式。无论是哪一种发行方式，你都可以免费获得（例如从网上下载），并再次发布。但如果你使用了他们的在线升级（包括补丁）或咨询服务，就必须要付费。

**CentOS与Redhat关系**
RedHat一直都提供源代码的发行方式，CentOS就是将RedHat发行的源代码重新编译一次，形成一个可使用的二进制版本。由于LINUX的源代码是GNU，所以从获得RedHat的源代码到编译成新的二进制，都是合法。只是RedHat是商标，所以必须在新的发行版里将RedHat的商标去掉。
RedHat对这种发行版的态度是：“我们其实并不反对这种发行版，真正向我们付费的用户，他们重视的并不是系统本身，而是我们所提供的商业服务。”所以，CentOS可以得到RedHat的所有功能，甚至是更好的软件。但CentOS并不向用户提供商业支持，当然也不负上任何商业责任。
如果你要将你的RedHat转到CentOS上，因为你不希望为RedHat升级而付费。当然，你必须有丰富UNIX使用经验，因此，RedHat的商业技术支持对你来说并不重要。但如果你是单纯的业务型企业，那么还是建议你选购RedHat软件并购买相应服务。这样可以节省你的IT管理费用，并可得到专业服务。
一句话，选用CentOS还是RedHat，取决于你所在公司是否拥有相应的技术力量：
1、完全可以把CentOS理解为RedHat AS系列！它完全就是对RedHat AS进行改进后发布的，各种操作、使用和RedHat没有区别
2、CentOS完全免费，不存在像RedHat AS系列那样需要序列号的问题
3、CentOS独有的yum命令支持在线升级，可以即时更新系统，不像RedHat那样需要花钱购买支持服务
4、CentOS修正了许多RedHat AS的BUG
5、CentOS版本说明：
CentOS4.0等同于RedHat AS4
CentOS4.1等同于RedHat AS4 U1
CentOS4.2等同于RedHat AS4 U2
CentOS4.3等同于RedHat AS4 U3
CentOS4.4等同于RedHat AS4 U4
**Red Hat**
Redhat有两大Linux产品系列：免费和收费
**1）免费的Fedora Core系列**
主要用于桌面版本，提供了较多新特性的支持。
Red Hat公司的产品中，有RedHat Linux（如Redhat8、9）和针对企业发行的版本Red Hat Enterprise Linux，都能够通过网络FTP免费的获得并使用，但是在2003年的时候，Red Hat Linux停止了开发，这也是为什么没有Redhat10的原因，它的项目有Fedora Project这个项目所取代，并以Fedora Core这个名字发行并提供普通用户免费使用。Fedora Core这个Linux发行版更新很快，大约半年左右就有新的版本出来，目前的版本是Fedora Core 6，这个Fedora Core试验的韵味比较浓厚，每次发行都有新的功能被加入到其中，得到的成功结果将被采用道RHEL的发布中。虽说这样，频繁的被改进更新的不安定产品对于企业来说并不是最好的选择，大多数企业还是会选择有偿的RHEL产品。
**2）收费的Enterprise系列**
这个系列分成：AS/ES/WS等分支。Red Hat Enterprise Linux ：redhat 的企业版本，简写RHEL。包括如下版本：
1、Red Hat Enterprise Linux AS
Red Hat企业Linux AS (Advanced Server) 是企业Linux解决方案中最高端的产品，它专为企业的关键应用和数据中心而设计。Red Hat企业Linux AS是唯一支持IBM i 系列，p 系列和z 系列/S-390系统的产品。在Intel X86平台上，Red Hat企业Linux AS可以支持2个以上CPU和大于8GB的内存。
典型的Red Hat企业Linux AS 应用环境如下：
数据库和数据库应用软件；WEB和中间件；CRM，ERP，SCM
2、Red Hat Enterprise Linux ES Red Hat企业Linux ES (Entry Server)
为Intel X86 市场提供了一个从企业门户到企业中层应用的服务器操作系统。它支持1-2CPU和小于8GB内存的系统。它提供了与Red Hat企业Linux AS同样的性能，区别仅在于它支持更小的系统和更低的成本。
典型的Red Hat企业Linux ES 应用环境如下：
公司WEB架构；网络边缘应用 (DHCP, DNS, 防火墙等)；邮件和文件/打印服务；中小规模数据库和部门应用软件
3、Red Hat Enterprise Linux WS
Red Hat企业Linux WS (Workstation) 是Red Hat企业Linux AS和ES的桌面/客户端合作伙伴。Red Hat企业Linux WS支持1-2CPU 的intel和AMD系统，是桌面应用的最佳环境。它包含各种常用的桌面应用软件(Office工具，邮件，即时信息，浏览器等)，可以运行各种客户-服务器配置工具，软件开发工具和各种应用软件(例如EDA和Oil/Gas应用软件)。Red Hat企业Linux Ws和服务器产品由同样的源代码编译而成，但它不提供网络服务功能(如DNS和DHCP)，它只适合作客户端应用。

**AS、ES、WS关系**
Advanced Server，缩写即AS。AS在标准Linux内核的基础上，做了性能上的增强，并提高了可靠性，集成了众多常见服务器的驱动程序。可轻松识别IBM/DELL/HP等常见机架式服务器的磁盘阵列卡等设备。AS主要版本2.x/3.x/4.x，也就是我们所说的AS3/AS4，每一个版本还有若干个升级，例如最早推出的AS4后，遇到了一些更新。此时就会发布AS4 Update1，以后还会陆续有AS4 Update2/Update3等出现简称AS4u1/AS4u2/AS4u3等。这和微软的发布形式也是非常类似的，微软的Windows NT4 从SP1出到SP6，Windows2000从SP1出到SP4等。AS这些Update版本所包含的主要程序包版本都有一定差别，最好不要混用，否则很容易出现问题。Prima和Plesk的安装包，对于各种发行版都有了针对性的设计在下载页面上，通常都会标识出来，支持哪些版本和哪些update的系统。
ES，是AS的精简版本。他与常见的AS系列的区别是，AS支持到4路以上CPU，而ES只能支持两路CPU。AS和ES在大多数程序包上并无区别，只在内核等少数软件包上有差异。AS和ES的售价差别比较大，通常ES用在随服务器一同购买的OEM版本中。例如购买DELL服务器，搭配的Linux会是ES系列。如果要搭配AS系列，则需要多花数千元。
WS，是ES的进一步简化版，主要针对企业内部的桌面办公市场，国内较少采用。
Redhat的Fedora Core Linux和Enterprise Linux，都需要遵循GNU协议，即需要发布自己的源代码。所以，对于免费的Fedora Core Linux，从Redhat网站上可以直接下载ISO刻盘，还能下载到SRPM的ISO，即程序包源码光盘。对于收费的Enterprise Linux系列，是一款商业产品，所以网站上不能下载到ISO文件，需要购买正式授权方可。由于Enterprise Linux也需要遵循GNU协议，故必须发布源代码。所以在Redhat的网站上，可以获得AS/ES/WS系列的SRPM源码ISO文件。这些文件可以被自由的下载，修改代码，重新编译使用。

**CentOS**
Community ENTerprise Operating System的项目诞生，它的缩写既是CentOS。CentOS社区将Redhat的网站上的所有源代码下载下来，进行重新编译。重新编译后，由于AS/ES/WS是商业产品，必须将所有Redhat的Logo和标识改成自己的CentOS标识。比如将AS4原版的SRPM源码编译后，就成为了CentOS 4.0。AS4Update1的源码编译后，就成为了CentOS4.1。AS4Update2的源码编译后，就成为了CentOS4.2。同理，CentOS的3.x/4.x都对应着相应的版本。
所以，CentOS就是Redhat的AS/ES/WS的免费版本。使用CentOS，可以获得和AS/ES相同的性能和感受。CentOS除了提供标准的编号1~4或者1~5的若干张ISO以外，还提供了最小化1CD的Server光盘。用Server光盘安装好的系统，就是一个最小化的Linux内核加上常用的httpd/mysql等包，不包含Xwindows桌面等对于服务器无用的软件。

**CentOS与REDHAT区别：**
1. GPL就是Linux内核所采用的软件许可证，GPL的特点是：你拿人家的代码修改用了，必须把修改后的代码公布。
2. 所有的Linux都是采用的GPL许可，GPL许可允许GPL软件卖钱，但必须公布源码，所以每个Linux发行版的代码都是全公开的，只是，使用这些代码的人必须也公开修改过的代码。
3. Redhat的代码是公开的，但是他的二进制RPM包更新却不免费，这并不违反GPL许可。于是开始有人想到了建立CentOS项目。
4. 由于Redhat的源代码是公开的，所以CentOS项目的人拿来自己再编译，同样的代码，同样的编译器，编译出来的自然是同样的东西。只不过里面删除了Redhat的Logo以及相应信息，而核心的管理工具还是rpm，只是用一个免费的软件包管理器yum（yellow dog update manager）替代了Redhat中的up2date，up2date更新是连接到Redhat的收费服务站点的，通过钱买来的服务代码通过认证。
5. CentOS的使用者众多，因为世界范围内很多人想使用Redhat却有苦恼于Redhat的收费服务。现在CentOS在Linux发行版中排行前10，这个网页的第10个发行版本就是CentOS：[Top Ten Distributions](http://distrowatch.com/dwres.php?resource=major)
6. 从品质上来说，CentOS从理论上应该跟Redhat一样的，毕竟是同样的源码。
上面的6点足够说明CentOS了。
**什么是FreeBSD：**
FreeBSD是个完整的系统，所有ls、cd这样的小命令都是自己的项目组开发的，而Linux发行版本其实是下载一个Linux内核，下载GNU的Coreutils（包括了常用的核心小命令），自己定制的系统。你应该记得，前些天，我还自己手动编译了一套系统。  FreeBSD是从4.4BSD演变过来的Unix-Like操作系统，并不是也使用Linux内核。
FreeBSD使用的软件许可证是BSDL，BSDL的特点是：你可以免费使用我的代码，基于我的代码修改后的代码你不必公开，使用我的代码出了问题你不要告我（原：don’t pay me, don’t sue me）。很多喜欢BSDL的人主要是因为他们认为只要代码能得到最大幅度的利用就是最好的，不象GPL，用了代码还要还。所以很多网络设备以及一些商业公司都赞助FreeBSD的开发，这是个完全免费的软件项目：[freebsdfoundation.org](http://www.freebsdfoundation.org/donate/sponsors.shtml)


**参考推荐：**
[Centos和Redhat的区别和联系](http://www.linuxdiyf.com/viewarticle.php?id=218621)
[Linux发行版：CentOS、Ubuntu、RedHat、Android、Tizen、MeeGo](Linux%E5%8F%91%E8%A1%8C%E7%89%88%EF%BC%9ACentOS%E3%80%81Ubuntu%E3%80%81RedHat%E3%80%81Android%E3%80%81Tizen%E3%80%81MeeGo)



