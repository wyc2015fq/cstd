# 对比 Ubuntu 18.04 和 Fedora 28 - 文章 - 伯乐在线
原文出处： [Linuxandubuntu](http://www.linuxandubuntu.com/home/ubuntu-1804-vs-fedora-28)   译文出处：[Linux中国/Andy Song](https://linux.cn/article-9952-1.html)
![](http://jbcdn2.b0.upaiyun.com/2018/08/ddb1f665d74f913109feb4dfead998f1.jpg)
大家好，我准备在今天突出说明一下两大主流 Linux 发行版，即 **Ubuntu 18.04** 和 **Fedora 28**，包括一些特性和差异。两者都有各自的包管理系统，其中 Ubuntu 使用 DEB，Fedora 使用 RPM；但二者使用同样的[桌面环境Desktop Environment](http://www.linuxandubuntu.com/home/5-best-linux-desktop-environments-with-pros-cons) （DE）[GNOME](http://www.linuxandubuntu.com/home/walkthrough-on-how-to-use-gnome-boxes)，并致力于为 Linux 用户提供高品质的桌面体验desktop experience。
**Ubuntu 18.04** 是 Ubuntu 目前最新的 [长期支持版本Long Term Support](http://www.linuxandubuntu.com/home/ubuntu-1804-codename-announced-bionic-beaver)（LTS），为用户提供 GNOME 桌面系统。**Fedora 28** 也提供 GNOME 桌面系统，但落实到具体的软件包管理方面，二者的桌面体验存在差异；在用户界面User Interfaces方面也显然存在差异。
### 基本概念
不知你是否了解，虽然 Ubuntu 基于 Debian，但 Ubuntu 比 Debian 更早提供最新版本的软件。举个例子，当 Ubuntu 提供流行网页浏览器 Firefox Quantum 时，Debian 仍在提供 Firefox 的延期支持版Extended Support Release（ESR）。
（LCTT 译注：从 2012 年 1 月开始，Firefox 进入快速版本期，每 6 周发布新的主线版本，每隔 7 个主线版本发布新的 ESR 版本。Firefox 57 的桌面版发布时被命名为 Firefox Quantum，同期的 ESR 版本与 Firefox 52 一同发布并基于 Firefox 48。参考 [Wiki: History_of_Firefox](https://en.wikipedia.org/wiki/History_of_Firefox)）
同样的情况也适用于 Fedora，它为终端用户提供前沿的软件，也被用作下一个稳定版本的 RHEL (Red Hat Enterprise Linux) 的测试平台。
### 桌面预览
Fedora 提供原汁原味的vanilla GNOME 桌面体验；相比之下，Ubuntu 18.04 对 GNOME 做了若干方面的微调，以便长期以来的 Unity 用户可以平滑的过渡到 GNOME 桌面环境。
*为节省开发时间，Canonical （从 Ubuntu [17.10](http://www.linuxandubuntu.com/home/what-new-is-going-to-be-in-ubuntu-1704-zesty-zapus) 开始）已经决定放弃 Unity 并转向 GNOME 桌面，以便可以将更多精力投入到 IoT 领域。*
因此，在 Fedora 的桌面预览中，我们可以看到一个简洁的无图标桌面和一个自动隐藏的侧边栏，整体外观采用 GNOME 默认的 Adwaita 主题。
![fedora 28 gnome](http://jbcdn2.b0.upaiyun.com/2018/08/34ff23eea5a5293be2e1e3357157a3d9.jpg)
相比之下，Ubuntu 采用其经典的有图标桌面样式，左侧边栏用于模拟其传统的“程序坞dock”，使用 Ubuntu Ambiance 主题定制化窗口，与其传统的（Unity 桌面）外观和体验基本一致。
![Ubuntu gnome 18.04](http://jbcdn2.b0.upaiyun.com/2018/08/5e63dbae97afc66e955d1185c6f26ce7.jpg)
虽然存在一定差异，但习惯使用其中一种桌面环境后切换到另外一种并不困难。毕竟二者设计时都充分考虑了简洁性和用户友好性，即使是新用户也不会对这两种 Linux 发行版感到不适应。
但外观或 UI 并不是决定用户选择哪一种 Linux 发行版的唯一因素，还有其它因素也会影响用户的选择。下面主要介绍两种 Linux 发行版在软件包管理相关方面的内容。
### 软件中心
Ubuntu 使用 dpkg（即 Debian Package Management）将软件分发给终端用户；Fedora 则使用 rpm（全称为 Red Hat Package Management）。它们都是 Linux 社区中非常流行的包管理系统，对应的命令行工具也都简单易用。
![ubuntu software center](http://jbcdn2.b0.upaiyun.com/2018/08/3390655eab10c9026788ce80d68f0e02.jpg)
但在具体分发的软件方面，各个 Linux 发行版会有明显差异。Canonical 每 6 个月发布新版本的 Ubuntu，一般是在每年的 4 月和 10 月。对每个版本，开发者会维护一个开发计划；Ubuntu 新版本发布后，该版本就会进入冻结freeze状态，即停止新软件的开发和测试。
相比之下，Fedora 也采用相似的 6 个月发布周期，看起来很像一种滚动更新rolling release的 Linux 发行版（其实并不是这样）。与 Ubuntu 不同之处在于，（Fedora 中的）几乎所有软件包更新都很频繁，让用户有机会尝试最新版本的软件。但这样也导致软件 Bug 更频繁出现，给用户带来“不稳定性”，虽然还不至于导致系统不可用。
### 软件更新
我上面已经提到了 Ubuntu 版本的冻结状态。好吧，由于它对 Ubuntu 软件更新方式有着重要的影响，我再次提到这个状态：当 Ubuntu 新版本发布后，该版本的开发（这里是指测试新软件）就停止了。
*即将发布的下个版本的开发也随之开始，先后历经 “每日构建daily build” 和 “测试版beta release” 阶段，最后作为新版本发布给终端用户。*
在冻结状态下，Ubuntu 维护者不会在软件源package repository中增加最新版软件，除非用于解决严重的安全问题。因此，Ubuntu 用户可用的软件更新更多涉及 Bug 修复而不是新特性，这样的好处在于系统可以保持稳定，不会扰乱用户的使用。
Fedora 试图为终端用户提供最新版本的软件，故用户的可用软件更新相比 Ubuntu 而言会更多涉及新特性。当然，开发者为了维持系统的稳定性，也采取了一系列措施。例如，在操作系统启动时，用户可以从最多三个可用内核working kernel（最新内核处于最上方）中进行选择；当新内核无法启动时，用户可以回滚使用之前两个可用内核。
### Snaps 和 flatpak
它们都是新出现的酷炫工具，可以将软件发布到多个 Linux 发行版上。Ubuntu 提供 **snaps**，而 Fedora 则提供 **flatpak** 。二者之中 snaps 更加流行，更多流行软件或版权软件都在考虑上架 snap 商店。Flatpak 也在吸引关注，越来越多的软件上线该平台。
不幸的是，由于二者出现的时间都不久，很多人遇到“窗口主题不一致window theme-breaking”问题并在网上表达不满。但由于二者都很易于使用，在二者之间切换并不是难事。
（LCTT 译注：按译者理解，由于二者都增加了一层安全隔离，读取系统主题方面会遇到问题；另外，似乎也有反馈 snap 专用主题无法及时应用于 snap 的问题）
### 应用对比
下面列出一些在 Ubuntu 和 Fedora 上共有的常见应用，然后在两个平台之间进行对比：
#### 计算器
Fedora 上的计算器程序启动速度更快。这是因为 Fedora 上的计算器程序是软件包形式安装的，而 Ubuntu 上的计算器程序则是 snap 版本。
#### 系统监视器
可能听上去比较书呆子气，但我认为观察计算机性能并杀掉令人讨厌的进程是必要且直观的。程序启动速度对比与计算器的结果一致，即 （软件包方式安装的）Fedora 版本快于（snap 形式提供的）Ubuntu 版本。
#### 帮助程序
我已经提到，（为便于长期以来的 Untiy 用户平滑切换到 GNOME），Ubuntu 提供的 GNOME 桌面环境是经过微调的版本。不幸的是，Ubuntu 开发者似乎忘记或忽略了对帮助程序的更新，用户阅读文档（入门视频）后会发现演示视频与真实环境有略微差异，这可能让人感到迷惑。
![ubuntu 18.04 help manual](http://jbcdn2.b0.upaiyun.com/2018/08/cf301305235cc47e24ebcc91d2e12952.jpg)
### 结论
Ubuntu 和 Fedora 是两个主流的 Linux 发行版。两者都各自有一些华而不实的特性，因而新接触 Linux 的人很难抉择。我的建议是同时尝试二者，这样你在试用后可以发现哪个发行版提供的工具更适合你。
希望你阅读愉快，你可以在下方的评论区给出我漏掉的内容或你的建议。
