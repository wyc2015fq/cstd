# 【WebRTC排错】ubuntu下git卸载 || 缺少alsa/asoundlib.h问题 || - 三少GG - CSDN博客
2011年10月20日 20:25:45[三少GG](https://me.csdn.net/scut1135)阅读数：4661
Error: Command git clone --progress http://git.chromium.org/webm/libvpx.git   .../libvpx/source/libvpx returned non-zero exit status 128 in .../WebRTC
排查得到：
webm$ git clone https://git.chromium.org/webm/libvpx.git
Cloning into libvpx...
fatal: Unable to find remote helper for 'https'
解决：
How did you install git, and which version of it do you have? Can you 
clone other git repos hosted via HTTP manually using "git clone"? http://lists-archives.org/git/708090-report-remote-helper-exec-failures.html suggests
**that this may be a problem with your copy of git not being able to run its remote helper  for HTTP.**
git 安装不正确！！！缺少对http的支持，有些包没装好
Git is reasonably self-sufficient, but does depend on a few external
   programs and libraries.  Git can be used without most of them by adding
   the approriate "NO_<LIBRARY>=YesPlease" to the make command line or
   config.mak file.
    - "zlib", the compression library. Git won't build without it.
    - "ssh" is used to push and pull over the net.
    - A POSIX-compliant shell is required to run many scripts needed
      for everyday use (e.g. "bisect", "pull").
    - "Perl" version 5.8 or later is needed to use some of the
      features (e.g. preparing a partial commit using "git add -i/-p",
      interacting with svn repositories with "git svn").  If you can
      live without these, use NO_PERL.
    - "openssl" library is used by git-imap-send to use IMAP over SSL.
      If you don't need it, use NO_OPENSSL.
      By default, git uses OpenSSL for SHA1 but it will use it's own
      library (inspired by Mozilla's) with either NO_OPENSSL or
      BLK_SHA1.  Also included is a version optimized for PowerPC
      (PPC_SHA1).
**    - "libcurl" library is used by git-http-fetch and git-fetch.  You      might also want the "curl" executable for debugging purposes.      If you do not use http:// or https:// repositories, you do not      have to have them (use NO_CURL).**
    - "expat" library; git-http-push uses it for remote lock
      management over DAV.  Similar to "curl" above, this is optional
      (with NO_EXPAT).
    - "wish", the Tcl/Tk windowing shell is used in gitk to show the
      history graphically, and in git-gui.  If you don't want gitk or
      git-gui, you can use NO_TCLTK.
  解决办法：
干脆不要最新版本，不要自己去官网下载make，直接  **2.**** sudo apt-get install git git-core 来安装GIT程序**，或者到新立得软件包里面 输入 git然后安装。首先要卸载掉之前make源码的版本才行，
从源码装的Git, 它的Makefile 里面没有make uninstall项，所以要删除历史版本，只能通过删除文件的方式。
先要看你的安装过程是怎么样的，我的如下：
$ tar -jxvf git-1.4.0.tar.bz2
$ cd git-1.4.0/
$ make
$ sudo make prefix=/usr/local install
所以最终安装目录在 /usr/local 下：
$ which git
/usr/local/bin/git
$ git --version
git version 1.4.0
$ ls /usr/local/bin/
git git-http-fetch git-repack
git-add git-http-push git-repo-config
git-am git-imap-send git-request-pull
git-annotate git-index-pack git-rerere
....
要把Git卸载掉，用下面的命令：**1.**
$ **sudo find /usr/local -depth -iname 'git*' -exec rm -rf {} \;**
当然，如果你使用的是configure 生成的Makefile,要卸载，如下：
安装过程：
$ ./configure --prefix=/opt/git
$ make all doc
$ sudo make install install-doc install-symlinks
卸载：
$ sudo rm -rf /opt/git
$ sudo find /usr/local -lname '/opt/git/*' -delete
GIT 是用于 Linux 内核开发的版本控制工具。与常用的版本控制工具 CVS, Subversion 等不同，它采用了分布式版本库的方式，不必服务器端软件支持，使源代码的发布和交流极其方便。 Git 的速度很快，这对于诸如 Linux kernel 这样的大项目来说自然很重要。 Git 最为出色的是它的合并跟踪（merge tracing）能力。
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
WebRTC 还是有错误
"Updating projects from gyp files...
Package gtk+-2.0 was not found in the pkg-config search path.
Perhaps you should add the directory containing `gtk+-2.0.pc'
to the PKG_CONFIG_PATH environment variable
No package 'gtk+-2.0' found
Exception: Call to 'pkg-config --cflags gtk+-2.0' returned exit status 1. while trying to load trunk/webrtc.gyp
Error: Command /usr/bin/python trunk/build/gyp_chromium --depth=trunk trunk/webrtc.gyp returned non-zero exit status 1 in ...WebRTC
"
解决办法：
[问题5:   No package 'gtk+-2.0' found 错误](http://bbs.51cto.com/thread-787904-1.html)
Package gtk+-2.0 was not found in the pkg-config search path.Perhaps you should add the directory containing `gtk+-2.0.pc'to the PKG_CONFIG_PATH environment variableNo package 'gtk+-2.0' foundERROR: pkg-config was not found, or unable to access the gtk+-2.0
 package.Install pkg-config and the gtk+ development package,or disable the gui debugger, or the wxWidgets display library (whichever is being used).
解决方法：
[thornbird@thornbird bochs-2.4]**$sudo apt-get install libgtk2.0-dev** 或者在新立德搜libgtk2.0-dev
终于成功啦！！！
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
peerconnection_server小问题，默认的端口号8888被占用了，故绑定失败。通过
netstat 查看已经连接的服务端口（ESTABLISHED） 
netstat -a 查看所有的服务端口（LISTEN，ESTABLISHED） 即可查看。改为8885就ok了。
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
peerconnection_client  涉及到 “audio_mixer_manager_alsa_linux.h:19: fatal error: alsa/asoundlib.h: 没有那个文件或目录“ 问题。
解决办法：
$ apt-file search alsa/asoundlib.h
libasound2-dev: /usr/include/alsa/asoundlib.h
So that’s it, **asoundlib.h is in the package libasound2-dev**.
apt-file需要使用apt-get install apt-file安装一下。 
原来是我没有装**libasound2-dev**.  dev即开发包，用新立德安装即可。
[ALSA声音编程介绍(译文)](http://apps.hi.baidu.com/share/detail/7910583)
ALSA声音编程介绍
ALSA表示高级Linux声音体系结构(Advanced Linux Sound Architecture)。它由一系列内核驱动，应用程序编译接口(API)以及支持Linux下声音的实用程序组成。这篇文章里，我将简单介绍ALSA项目的基本框架以及它的软件组成。主要集中介绍PCM接口编程，包括您可以自动实践的程序示例。
您使用ALSA的原因可能就是因为它很新，但它并不是唯一可用的声音API。如果您想完成低级的声音操作，以便能够最大化地控制声音并最大化地提高性能，或者如果您使用其它声音API没有的特性，那么ALSA是很好的选择。如果您已经写了一个音频程序，你可能想要为ALSA声卡驱动添加本地支持。如果您对音频不感兴趣，只是想播放音频文件，那么高级的API将是更好的选择，比如SDL,OpenAL以及那些桌面环境提供的工具集。另外，您只能在有ALSA支持的Linux环境中使用ALSA。
ALSA历史
ALSA项目发起的起因是Linux下的声卡驱动(OSS/Free drivers)没有得到积极的维护。并且落后于新的声卡技术。Jaroslav Kysela早先写了一个声卡驱动，并由此开始了ALSA项目，随便，更多的开发者加入到开发队伍中，更多的声卡得到支持，API的结构也得到了重组。
Linux内核2.5在开发过程中，ALSA被合并到了官方的源码树中。在发布内核2.6后，ALSA已经内建在稳定的内核版本中并将广泛地使用。
ALSA基础
ALSA由许多声卡的声卡驱动程序组成，同时它也提供一个称为libasound的API库。应用程序开发者应该使用libasound而不是内核中的ALSA接口。因为libasound提供最高级并且编程方便的编程接口。并且提供一个设备逻辑命名功能，这样开发者甚至不需要知道类似设备文件这样的低层接口。相反，OSS/Free驱动是在内核系统调用级上编程，它要求开发者提供设备文件名并且利用ioctrl来实现相应的功能。为了向后兼容，ALSA提供内核模块来模拟OSS，这样之前的许多在OSS基础上开发的应用程序不需要任何改动就可以在ALSA上运行。另外，libaoss库也可以模拟OSS，而它不需要内核模块。
ALSA包含插件功能，使用插件可以扩展新的声卡驱动，包括完全用软件实现的虚拟声卡。ALSA提供一系列基于命令行的工具集，比如混音器(mixer)，音频文件播放器(aplay)，以及控制特定声卡特定属性的工具。
ALSA体系结构
ALSA API可以分解成以下几个主要的接口：
1 控制接口：提供管理声卡注册和请求可用设备的通用功能
2 PCM接口：管理数字音频回放(playback)和录音(capture)的接口。本文后续总结重点放在这个接口上，因为它是开发数字音频程序最常用到的接口。
3 Raw MIDI接口:支持MIDI(Musical Instrument Digital Interface),标准的电子乐器。这些API提供对声卡上MIDI总线的访问。这个原始接口基于MIDI事件工作，由程序员负责管理协议以及时间处理。
4 定时器(Timer)接口：为同步音频事件提供对声卡上时间处理硬件的访问。
5 时序器(Sequencer)接口
6 混音器(Mixer)接口
。
。
。
总结
我希望这篇文章能够激励你尝试编写某些ALSA程序。伴随着2.6内核在Linux发布版本(distributions)中被广泛地使用，ALSA也将被广泛地采用。它的高级特征将帮助Linux音频程序更好地向前发展。
Jaroslav Kysela和Takashi lwai帮助查阅了本文的草稿并提出了宝贵的意见，在此表示感谢。
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
“pulse/pulseaudio.h: 没有那个文件或目录“
同理，解决办法为  安装 libpulse-dev 
