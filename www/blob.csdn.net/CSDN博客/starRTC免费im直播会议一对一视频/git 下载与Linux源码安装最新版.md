# git 下载与Linux源码安装最新版 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年05月11日 19:35:59[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：720
win：
[https://git-for-windows.github.io/](https://git-for-windows.github.io/)
或
[https://git-scm.com/downloads](https://git-scm.com/downloads)   官网！

源码安装git
Git 的工作需要调用 curl，zlib，openssl，expat，libiconv 等库的代码，所以需要先安装这些依赖工具。在有 yum 的系统上（比如 Fedora）可以用下面的命令安装：
$ yum install curl-devel expat-devel gettext-devel   openssl-devel zlib-devel
yum install  perl cpio  
git clone [https://github.com/git/git](https://github.com/git/git)
tar -zxf git-1.7.2.2.tar.gz
$ cd git-1.7.2.2
$ make prefix=/usr/local all
报错：
Can't locate ExtUtils/MakeMaker.pm
yum install perl-devel  perl-CPAN   perl-ExtUtils-CBuilder perl-ExtUtils-MakeMaker
$ sudo make prefix=/usr/local install
现在已经可以用 git 命令了，用 git 把 Git 项目仓库克隆到本地，以便日后随时更新：
$ git clone git://git.kernel.org/pub/scm/git/git.git
