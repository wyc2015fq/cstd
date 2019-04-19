# 2017 十大最佳用于隐私和安全保护的 Linux 发行版 - 文章 - 伯乐在线
原文出处： [开源中国 - 局长](https://my.oschina.net/editorial-story/blog/920550)
![](https://static.oschina.net/uploads/space/2017/0613/195235_iFxM_2918182.jpg)
## **介绍**
优秀的 Linux 操作系统是自由和开源的。因此，有数千种不同的“风味”可供选择——而某些类型的 Linux，例如 Ubuntu 是通用的，而且适用于许多不同的用途。
但安全意识高的用户会对专为隐私和安全设计的 Linux 发行版更有兴趣，它们可以帮助你通过加密操作来保护数据的安全，并支持在 Live 模式下运行，无需将数据写入在使用中的硬盘。
这篇文章将列出十个关注隐私和安全性的 Linux 发行版。
## **1. Qubes OS**
![](https://static.oschina.net/uploads/space/2017/0613/195433_DrFR_2918182.png)
虽然不是面向新手用户的发行版，但 Qubes 是顶级的关注隐私的发行版之一。该发行版必须使用图形化安装程序将操作系统安装到硬盘驱动器，这是被加密的。
Qubes OS 使用 Xen Hypervisor 来运行多个虚拟机，其主要理念就是基于隔离的安全，它会将系统隔离为“个人”、“工作”和“上网”。这样，即便你不小心在工作机器上下载了恶意软件，但个人文件不会受到影响。
主桌面会使用颜色编码的窗口，用于展示不同的虚拟机，以方便区分。
https://www.qubes-os.org/
## **2. Tails**
![](https://static.oschina.net/uploads/space/2017/0613/195623_NeGI_2918182.png)
Tails (The Amnesiac Incognito Live System) 可能是最有名的关注隐私的 Linux 发行版之一，它是基于 Debian 的自启动光盘和 USB 发行。Tails 可以在 Live 模式下从 DVD 运行，从而将其完全加载进系统 RAM，并且其活动不会产生任何痕迹。这款系统也可以像普遍的系统一样在“持久”模式下运行，关于系统的设置会存储在加密的 U 盘上。
Tails 为用户提供了完整的因特网匿名功能，所有的网络流量都会经过匿名网络 Tor，它可隐藏你的上网痕迹，使得网络流量难以被追踪。Tails 中配备的应用程序也是经过谨慎选择的，以安全为理念进行了预配置，以增强用户的隐私安全。例如，KeePassX 密码管理器、网页浏览器、IRC客户端、邮件mail客户端等。值得注意的是 Tails 会不断发现漏洞，因此请务必经常检查更新。（当然对待任何操作系统都应如此）
https://tails.boum.org/
## **3. BlackArch Linux**
![](https://static.oschina.net/uploads/space/2017/0613/195923_uICR_2918182.png)
这个基于 Arch Linux 的轻量级渗透测试发行，包含了 1,600 多种不同的黑客工具以用于渗透测试和计算机取证分析，这节省了每次下载需要的时间。BlackArch Linux 被设计为服务于系统渗透测试人员及安全研究人员，包含有多个轻量级窗口管理器如 Fluxbox、Openbox、Awesome、spectrwm。
BlackArch Linux的提供形式是一张自启动运行 DVD 镜像，可以从 U 盘或 CD 上直接运行，也可以安装到电脑或虚拟机，甚至可以安装在树莓派上以给你提供一个便携的渗透测试计算机。
特别值得一提的是它的 ‘anti-forensics’ 目录，因为它包含了为已加密的设备扫描内存的工具，这有助于保护机器免受“冷启动攻击”。
https://blackarch.org/
## **4. Kali**
![](https://static.oschina.net/uploads/space/2017/0426/080144_KZXe_2720166.png)
Kali Linux（以前叫做BackTrack），以印度教的一个女神命名，是最著名的渗透测试发行版之一，也是一份基于 Debian 的发行。它带有一套安全和计算机取证工具。其特色在于及时的安全更新（每周提供更新的 ISO 镜像），对 ARM 架构的支持（可在树莓派上运行），有四种流行的桌面环境供选择，以及能平滑升级到新版本。
Kali 有着令人敬畏的声望，它的创作者会通过 [Kali Linux Dojo](https://www.kali.org/kali-linux-dojo-workshop/) 提供培训。课程内容包括定制自己的 Kali Linux ISO 和学习渗透测试的基础。对于无法参加培训的人士，所有的课程教育资源都可通过 Kali 的网站免费获取。
https://www.kali.org/
## **5. IprediaOS**
![](https://static.oschina.net/uploads/space/2017/0613/200242_tDsT_2918182.jpg)
IprediaOS 是一个基于 Linux 的快速、强大和稳定的操作系统，提供了匿名的环境。所有的网络流量都会被自动和透明地加密和匿名化。这个面向隐私的操作系统基于 Fedora Linux，可在 Live 模式下运行也可安装到硬盘上。正如 Tails OS 会将所有网络流量通过 Tor 网络以避免被追踪，Ipredia 中所有的网络流量都会经过匿名的 I2P 网络。
它的功能包括匿名电子邮件和 BitTorrent 客户端，IRC 聊天，以及浏览 eepsites（特殊的 .i2p 扩展名）的功能。与 Tor 不同的是，I2P 不能作为访问正常的互联网的网关，所以 Ipredia 无法安全地访问常规网站。不过只能访问 eepsites 的优点是你的连接是真正无法被追溯的。
http://www.ipredia.org/os
## **6. Whonix**
![](https://static.oschina.net/uploads/space/2017/0613/200603_Z1TH_2918182.png)
引导一份 Live 操作系统是一个麻烦，因为必须重新启动计算机，但将其安装到硬盘意味着有被攻击的风险。Whonix 提供了一个优雅的方案，它被设计为运行在 Virtualbox 中作为一个虚拟机而工作。由于它在虚拟机中运行，所以 Whonix 与所有可运行 Virtualbox 的操作系统兼容。
Whonix 是一份聚焦于匿名性、隐私、安全的操作系统。它基于 Tor 匿名网络、Debian GNU/Linux、基于隔离的安全性。Whonix 包括两部分，一部分只运行在 Tor 上并扮演网关角色，这部分叫做 Whonix-Gateway；另一部分叫做 Whonix-Workstation，位于隔离网络中。只有经由 Tor 的连接被允许。有了 Whonix，你就可以匿名使用应用程序并在因特网上运行服务器。因匿名解析而造成的信息泄漏不可能存在，即便获得了根权限的恶意软件也无法发现用户的真实 IP 地址。
https://www.whonix.org/
## **7. Discreete Linux**
![](https://static.oschina.net/uploads/space/2017/0613/200511_1ovk_2918182.jpg)
这个故意拼写错的发行版是优秀的 Ubuntu Privacy Remix 的继承者。该操作系统不支持网络硬件或内部硬盘驱动器，因此所有的数据都离线存储在 RAM 或 USB 设备中。它可以在 Live 模式下运行，但当从卷启动时也允许将一些设置存储在加密的‘Cryptobox’中。
还有另外一个值得关注的功能是它的内核模块只能在 Discreete Linux 团队进行数字签名后才能安装。这可以防止黑客试图偷偷安装恶意软件。请注意，该操作系统目前尚处于 Beat 测试阶段。
https://www.discreete-linux.org/
## **8. Parrot Security OS**
![](http://static.oschina.net/uploads/space/2016/1017/071656_YYvN_2720166.png)
这款渗透测试系统由来自意大利的团队 Frozenbox 开发。和 Kali 和 BlackArch 一样，它也包含着许多易用的工具。Parrot Security OS 是面向安全的操作系统，它被设计为用于渗透测试、计算机取证、反向工程、攻击、云计算渗透测试、隐私/匿名、密码等场合。
Parrot 基于 Debian，其特色在于 MATE 桌面环境，拥有更丰富多彩的背景和菜单。因此，它对硬件的要求比其他渗透测试发行版（例如 Kali）更高。建议至少使用 2GB 的 RAM。
对于资源有限的用户，Parrot Cloud 是专门用于在服务器上运行的特殊发行版。它没有图形界面，但包含了一些网络和取证工具，可用于远程运行测试。
https://www.parrotsec.org/
## **9. Subgraph OS**
![](https://static.oschina.net/uploads/space/2017/0613/201019_7SbG_2918182.png)
Subgraph OS 是基于 Debian 的 Linux 发行，为超强的安全性而设计，它提供了多种安全、匿名上网、加固的特性。它的内核通过许多安全性的增强进行了加固，Subgraph 还在诸如浏览器之类的高风险应用中创建了虚拟的“沙盒”。因此，任何针对独立应用程序的攻击都不会危及整个系统。
Subgraph OS 使用加固过的 Linux 内核及应用防火墙来阻止特定的可执行程序去访问网络，并强制要求所有的因特网流量都经由 Tor 网络。每个应用程序需要连接到网络和访问其他应用程序的“沙盒”都需经过手动的允许。
该发行的文件管理器带有特色工具可以从数据文件中移除元数据，并且还集成了 OnionShare 文件共享软件。该发行使用 Icedove 邮件客户端以自动配合 Enigmail 对电子邮件进行加密。
在 Subgraph 中，对文件系统的加密是强制性的，这意味着没有写入未加密数据的危险。要注意的是，Subgraph 还处于测试阶段，因此不要依赖使用它来保护任何真正敏感的数据（并且一如既往地保持常规的备份）。
https://subgraph.com/
## **10. TENS**
![](https://static.oschina.net/uploads/space/2017/0613/194931_D4L5_2720166.png)
第十个发行版恰好是 TENS(Trusted End Node Security)。以前被称为 LPS(Lightweight Portable Security)，这份 Linux 发行版是美国国防部的产品。Trusted End Node Security (TENS)是基于 Linux 的自启动运行光盘，其目标是让用户能在计算机上工作而不会有向恶意软件、键盘记录程序及其他因特网时代的恶疾泄露信息凭证及私人数据的风险。
它包含了最必需的一套应用软件及实用工具，例如 Firefox 网页浏览器，以及一份加密向导以对个人文件进行加解密。但有一个‘Public Deluxe’（公开豪华版）也包含了 Adobe Reader 和 LibreOffice 这样的工具。所有的版本都包含一个自定义防火墙，值得注意的是该操作系统支持通过 Smart Card 登录。
https://spi.dod.mil/lipose.htm
