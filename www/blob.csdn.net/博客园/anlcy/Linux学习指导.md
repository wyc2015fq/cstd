
# Linux学习指导 - anlcy - 博客园






# [Linux学习指导](https://www.cnblogs.com/camilla/p/7090272.html)
初次学习Linux，首先在虚拟机中尝试它。 虚拟机我推荐Virtual Box，我并不主张使用VM，原因是VM是闭源的，并且是收费的，而Virtual Box很小巧，Windows平台下安装包在80MB左右。Virtual Box下载地址：https://www.virtualbox.org/wiki/Downloads
Linux发行版并非Linux，Linux仅是指操作系统的内核：
我推荐的发行版如下：建议使用CentOS
UBUNTU 适合纯菜鸟，追求稳定的官方支持，对系统稳定性要求较弱，喜欢最新应用，相对来说不太喜欢折腾的开发者。
Debian，相对UBUNTU难很多的发行版，突出特点是稳定与容易使用的包管理系统，缺点是企业支持不足，为社区开发驱动。
Arch，追逐时尚的开发者的首选，优点是包更新相当快，无缝升级，一次安装基本可以一直运作下去，没有如UBUNTU那样的版本概念，说的专业点叫滚动升级，保持你的系统一定是最新的。缺点显然易见，不稳定。同时安装配置相对Debian再麻烦点。
Gentoo，相对Arch再难点，考验使用者的综合水平，从系统安装到微调，内核编译都亲历亲为，是高手及黑客显示自己技术手段，按需配置符合自己要求的系统的首选。
Slackware与Gentoo类似。
CentOS，社区维护的RedHat的复刻版本，完全使用RedHat的源码重新编译生成，与RedHat的兼容性在理论上来说是最好的。如果你专注于Linux服务器，如网络管理，架站，那么CentOS是你的选择。下载地址https://www.centos.org/download/，建议下载CentOS-7.0-x86_64-DVD-1503-01.iso : 标准安装版，一般下载这个就可以了。
LFS，终极黑客显摆工具，完全从源代码安装，编译系统。安装前你得到的只有一份文档，你要做的就是照文档你的说明，一步步，一条条命令，一个个软件包的去构建你的Linux，完全由你自己控制，想要什么就是什么。如果你做出了LFS，证明你的Linux功底已经相当不错，如果你能拿LFS文档活学活用，再将Linux从源代码开始移植到嵌入式系统，我敢说中国的企业你可以混的很好。





