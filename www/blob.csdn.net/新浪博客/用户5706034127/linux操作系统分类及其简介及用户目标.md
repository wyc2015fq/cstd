# linux操作系统分类及其简介及用户目标_用户5706034127_新浪博客
|||
原文链接：http://os.51cto.com/art/201307/404309.htm
【2013年7月23日 51CTO外电头条】你可曾知道Linux的魅力或威力来自哪里？那就是，由于众多发行版百花齐放，Linux的阵营日益壮大，每一款发行版都拥有一大批用户，开发者自愿为相关项目投入精力。Linux发行版可谓是形形色色，它们旨在满足每一种能想得到的需求。本文就是为了简述某一款发行版为何存在、该发行版的目标用户是哪些，以及它与其他发行版相比有什么样的特殊功能。
**1. Debian**
Debian运行起来极其稳定，这使得它非常适合用于服务器。[Debian](http://os.51cto.com/art/201305/393876.htm)平时维护三套正式的软件库和一套非免费软件库，这给另外几款发行版（比如Ubuntu和Kali等）带来了灵感。Debian这款操作系统派生出了多个Linux发行版。它有37500多个软件包，这方面唯一胜过Debian的其他发行版只有Gentoo。Debian使用apt或aptitude来安装和更新软件。
Debian这款操作系统无疑并不适合新手用户，而是适合系统管理员和高级用户。Debian支持如今的大多数架构（处理器）。
下载Debian ISO映像文件：[http://www.debian.org/distrib/](http://www.debian.org/distrib/)
附有屏幕截图的Debian安装：《[Debian
7.0"Wheezy"安装指南](http://www.tecmint.com/debian-gnulinux-7-0-code-name-wheezy-server-installation-guide/)》
![](http://s4.51cto.com/wyfs01/M01/11/17/wKioJlHt5vjC-TKOAABHyPGNhFU792.jpg-wh_651x-s_3489747668.jpg)
Debian Linux
**2. Gentoo**
与Debian一样，Gentoo这款操作系统也包含数量众多的软件包。Gentoo并非以预编译的形式出现，而是每次需要针对每个系统进行编译。连Gentoo社区都觉得Gentoo安装和使用起来很困难；不过它被认为是最佳学习对象，可以进而了解Linux操作系统的内部运作原理。提到Gentoo总有人这么说："如果你要学用Linux发行版，那就学用该发行版吧；如果你学会了Gentoo，也就学会了Linux。"Gentoo使用portage来安装和更新软件。
Gentoo这款操作系统适合对Linux已经完全驾轻就熟的那些用户。
下载和安装Gentoo：[http://www.gentoo.org/main/en/where.xml](http://www.gentoo.org/main/en/where.xml)
![](http://s4.51cto.com/wyfs01/M01/11/17/wKioJlHt5vjBalpMAABL4oqPtu0218.jpg)
Gentoo Linux
**3. Ubuntu**
Ubuntu是Debian的一款衍生版，也是当今最受欢迎的免费操作系统。Ubuntu侧重于它在这个市场的应用，在服务器、云计算、甚至一些运行Ubuntu
Linux的移动设备上很常见。作为Debian Gnu
Linux的一款衍生版，Ubuntu的进程、外观和感觉大多数仍然与Debian一样。它使用apt软件管理工具来安装和更新软件。它也是如今市面上用起来最容易的发行版之一。Ubuntu使用基于apt的程序包管理器。
Ubuntu是新手用户肯定爱不释手的一款操作系统。
下载Ubuntu ISO映像文件：[http://www.ubuntu.com/download](http://www.ubuntu.com/download)
附有屏幕截图的Ubuntu安装：《[Ubuntu
13.04"Raring Ringtail"安装指南](http://www.tecmint.com/ubuntu-13-04-installation-guide/)》
![](http://s9.51cto.com/wyfs01/M01/11/19/wKioOVHt5viAPsHGAABCRuNHQPI181.jpg)
Ubuntu Linux
**4. Damn Vulnerable Linux**
当然，大多数人可能对这款发行版前所未闻，不过该发行版在本文中还是占有一席之地。那么，它有何过人之处呢？ Damn Vulnerable
Linux恰如其名：其字面意思就是"该死的易受攻击的Linux"。Vulnerable
Linux（DVL）根本不是一般意义上的优秀的Linux发行版。它有意捆绑了坏的、配置不当的、过时的、很容易被不法分子攻击的软件。
它的目的在于借机训练Linux管理员。还有什么比给Linux管理员一款坏的发行版去排解问题来得更管用的吗？面对Apache、MySQL、PHP、FTP和SSH等比较旧或破的版本，接受训练的管理员够有得忙了。
Damn Vulnerable Linux堪称旨在训练管理员的实验室。
下载Damn Vulnerable Linux（DVL）ISO映像文件：[DVL_1.5_Infectious_Disease.iso](http://sourceforge.jp/projects/sfnet_virtualhacking/downloads/os/dvl/DVL_1.5_Infectious_Disease.iso/)
![](http://s1.51cto.com/wyfs01/M01/12/82/wKioOVH6LC6Dm6xcAACRX1ikz3I028.jpg)
Damn Vulnerable Linux
**5. 红帽企业级Linux**
这是第一款面向商业市场的[Linux发行版](http://os.51cto.com/art/201305/393328.htm)。它有服务器版本，支持众多处理器架构，包括x86和x86_64。红帽公司通过课程红帽认证系统管理员/红帽认证工程师（RHCSA/RHCE），对系统管理员进行培训和认证。就全球市场而言，总利润中80%来自支持，另外20%来自培训和认证，不过在印度不是这样。
在印度，红帽的利润中80%来自认证和培训，只有20%来自支持。而Fedora是个平台，而不是开发新产品或新应用程序的测试环境；一旦成为稳定版，就与红帽企业级Linux捆绑在一起，包括支持。红帽提供了非常多的稳定版应用程序，但是众所周知的缺点是，把太多旧程序包打包起来，支持成本确实相当高。不过，如果安全是关注的首要问题，那么红帽企业级Linux的确是款完美的发行版，它使用YUM程序包管理器。
红帽企业级Linux是系统管理员的第一选择，它有众多的程序包，还有非常到位的支持。
由于该发行版是商业化产品，所以不是免费的。不过，你可以下载用于教学用途的测试版。
下载红帽企业级Linux测试版DVD ISO映像文件：RedHat Enterprise Linux（[测试版](https://www.redhat.com/wapps/sso/login.html?redirect=https://rhn.redhat.com/rhn/software/channels/Beta.do)）
附有屏幕截图的RHEL 6安装：《[RHEL
6安装指南](http://www.tecmint.com/red-hat-enterprise-linux-rhel-6-installation-guide-with-screenshots/)》
补充说明：通常认为，开发了该发行版的Marc
Ewin将该产品命名为红帽，因为他丢失了似乎很心爱的那顶红色帽子，帽子是他爷爷在他过生日时送的礼物。
![](http://s8.51cto.com/wyfs01/M00/11/17/wKioJlHt5viz4COpAABDkrn1UvU673.jpg)
红帽企业级Linux
**6. CentOS**
[CentOS](http://os.51cto.com/art/201005/200790.htm)是一款企业级Linux发行版，它使用红帽企业级Linux中的免费源代码重新构建而成。这款重构版完全去掉了注册商标以及Binary程序包方面一个非常细微的变化。有些人不想支付一大笔钱，又能领略红帽企业级Linux；对他们来说，CentOS值得一试。此外，CentOS的外观和行为似乎与母发行版红帽企业级Linux如出一辙。
CentOS使用YUM来管理软件包。
非常稳定的程序包；谁要是想在桌面端测试一下服务器的运作原理，都应该试试这款操作系统。
下载[CentOS
6.4](http://os.51cto.com/art/201305/393020.htm)DVD ISO映像文件：[http://wiki.centos.org/Download](http://wiki.centos.org/Download)
附有屏幕截图的CentOS 6.4安装：《[CentOS
6.4安装指南](http://www.tecmint.com/centos-6-4-installation-guide-with-screenshots/)》
![](http://s3.51cto.com/wyfs01/M00/11/17/wKioJlHt5vjCkPgtAABopSU_0nk138.jpg)
CentOS Linux
**7. Fedora**
小巧的Fedora适合那些人：想尝试最先进的技术，等不及程序的稳定版出来。其实，Fedora就是红帽公司的一个测试平台；产品在成为企业级发行版之前，在该平台上进行开发和测试。Fedora是一款非常好的发行版，有庞大的用户论坛，软件库中还有为数不少的软件包。Fedora同样使用YUM来管理软件包。
下载[Fedora
18](http://os.51cto.com/art/201301/379088.htm)（Spherical Cow）DVD ISO映像文件：[http://fedoraproject.org/en/get-fedora](http://fedoraproject.org/en/get-fedora)
附有屏幕截图的Fedora 18（Spherical Cow）安装：《[Fedora
18（Spherical Cow）安装指南](http://www.tecmint.com/fedora-18-installation-guide/)》
![](http://s5.51cto.com/wyfs01/M00/11/19/wKioOVHt5vixn0MSAACAwNNOuyA607.jpg)
Fedora Linux
**8. Kali Linux**
Kali Linux是[Debian](http://os.51cto.com/art/201102/244746.htm)的一款衍生版。Kali旨在用于渗透测试。它大概在三个月前才发行。Kali的前身是Backtrack。用于Debian的所有Binary软件包都可以安装到Kali
Linux上，而Kali的魅力或威力就来自于此。此外，支持Debian的用户论坛为Kali加分不少。Kali随带许多的渗透测试工具，无论是Wifi、数据库还是其他任何工具，都设计成立马可以使用。Kali使用APT来管理软件包。
毫无疑问，Kali Linux是一款渗透测试工具，或者是文明黑客（我不想谈论恶意黑客）青睐的操作系统。
下载Kali Linux DVD ISO映像文件：[Kali
Linux 6](http://www.tecmint.com/download-kali-linux-dvd-iso-images/)
![](http://s6.51cto.com/wyfs01/M01/11/19/wKioOVHt5vjDOkXtAAB66Ud3GOA707.jpg)
Kali Linux
**9. Arch Linux**
[Arch](http://os.51cto.com/art/201006/206869.htm)是一款采用滚动发行方式的操作系统：只要安装一次就够了；每当发行了某个新版本，就可以升级发行版，不需要重新安装。Pacman是Arch
Linux的软件包管理器。Arch
Linux既支持X86处理器架构，又支持X86_64架构，安装程序可以从光盘或U盘来运行。Arch旨在从开发者的角度而不是从用户的角度做到力求简单。Arch配置和安装起来超容易。它真是一款面向高手的发行版，让你可以了解Linux系统的每一个细枝末节。
下载Arch Linux ISO映像文件：[https://www.archlinux.org/download/](https://www.archlinux.org/download/)
![](http://s4.51cto.com/wyfs01/M01/11/17/wKioJlHt5vnw8y5zAAAp3Eh4lJ0354.jpg)
Arch Linux
**10. OpenSuse**
[OpenSuse](http://os.51cto.com/art/201005/202196.htm)这款Linux发行版是免费的，并不供商业用途使用，仍然供个人使用。OpenSuse的真正竞争对手是红帽企业级Linux。它使用Yast来管理软件包。有了Yast，使用和管理服务器应用程序就非常容易。此外，Yast安装向导程序可以配置电子邮件服务器、LDAP服务器、文件服务器或Web服务器，没有任何不必要的麻烦。它随带snapper快照管理工具，因而可以恢复或使用旧版的文件、更新和配置。由于让滚动发行版本成为可能的Tumbleweed，可将已安装的操作系统更新到最新版本，不需要任何的新发行版。
SUSE在管理员当中的名气更大，因为它有Yast以及让[系统管理员](http://os.51cto.com/art/201302/382170.htm)能够自动管理任务的其他此类应用程序，同样水准的其他发行版没有这项功能。
下载OpenSuse 12.3 DVD ISO映像文件：[http://software.opensuse.org/123/en](http://software.opensuse.org/123/en)
附有屏幕截图的[OpenSuse
12.3](http://os.51cto.com/art/201303/384379.htm)安装：《[OpenSuse
12.3安装指南](http://www.tecmint.com/opensuse-12-3-installation-guide-with-screenshots/)》
![](http://s8.51cto.com/wyfs01/M01/11/17/wKioJlHt5vvBvu0KAABxz60NgFQ048.jpg)
OpenSuse Linux
本文不是什么大盘点。市面上有好几百款发行版，每款发行版在某个方面都与众不同。不可能在此全部罗列，本文只罗列了十款最常见的Linux发行版（世界上只有两种人，一种是懂二进制的，另一种是不懂二进制的）。请宣传Linux的魅力或威力。
