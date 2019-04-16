# Darwin Streaming Server 安装流程 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年11月25日 23:48:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：32











**Darwin StreamingServer **安装流程

Darwin StreamingServer 支持开放源代码和基于标准的实时传输协议/实时流协议（RTP/ RTSP）、MPEG-4 和MP3 流协议。

**一、安装前的准备**

**Darwin StreamingServer **的下载

Quicktime StreamingServer 是由Apple 公司开发的，是为Mac OS X 服务器系统设计的。同时为了使更多的用户接收它，推出了基于其它服务器系统设计的Darwin Streaming Server，大小为8.38MB

可以苹果公司的网站下载：http://developer.apple.com/darwin/projects/streaming/得到（注意下载之前必须先注册一个帐号，而且下载时一定要注意下载版本所适用的操作系统）。

**Perl **的下载与安装

Darwin StreamingServer 需要Perl 的支持，因此安装它之前，必须先安装Perl。 Perl 可以从地址： http://www.activestate.com/Products/ActivePerl/下载（注意版本的区别，请按照自己机器的操作系统进行选择），此次选择的版本为ActivePerl 5.8.0 build 806。

双击下载得到的文件 ActivePerl-5.8.0.806-MSWin32-x86.msi

运行安装程序，一路“Next”，完成安装。



**Quicktime **媒体播放器的下载与安装（这个不是必须的，可以使用其他客户端软件）

可以从 Apple 公司的网站http://www.apple.com/quicktime/download/上免费下载得到，最新版本为6.3，文件大小约10MB。

将下载得到的.zip 文件解压到某文件夹中，运行解压后的文件QuickTimeInstaller.exe

运行安装程序，一路“Next”，完成安装。



**二、Darwin Streaming Server 的安装**

解压下载得到的文件 DarwinStreamingSrvr4.1.3-Windows.exe，然后执行解压得到的文件

install.bat。然后要求输入管理员名称和密码。

![](https://img-blog.csdn.net/20131124141514062)

1、 输入管理员帐号（自己任意给），并回车

![](https://img-blog.csdn.net/20131124141518015)



2、 然后输入管理员口令（自己任意给），并回车再次输入同样的口令，并回车。



**三、Darwin Streaming Server 的配置**

在配置 Darwin Streaming Server 之前，首先要启动Streaming AdminServer 服务。

![](https://img-blog.csdn.net/20131124141521156)



1、双击安装目录（一般为 C:\ProgramFiles\Darwin Streaming Server）下的 streamingadminserver.pl 文件，运行它。

注意：运行该文件后出现一个空的DOS 窗口，不要关闭它。初次安装完Darwin Streaming Server 软件，在没有关闭命令提示符窗口时，说明Streaming Admin Server 已经被启动，可以直接执行下面的步骤。

启动浏览器 IE。

![](https://img-blog.csdn.net/20131124141524734)



1、输入 URL ：http://127.0.0.1:1220 后回车。

然后输入用户名、口令 pk1/pkit ,单击“Log In”按钮

![](https://img-blog.csdn.net/20131124141527843)



1、输入 MP3 广播新口令和确认口令

2、指定媒体内容存放位置

3、最后点击“Finish”按钮

如果出现如下界面, 表明服务器启动成功。

![](https://img-blog.csdn.net/20131124141530890)



**四、Darwin Streaming Server 的测试**

整个安装配置完成以后，让我们放松一下，用QuickTime 测试一下，看能不能访问到流式媒体服务器。不过在测试前，一定要将媒体文件存放到上面指定媒体存放目录中。测试步骤如下:

1、打开QuickTime软件，选择“File”菜单。单击“Open URL in New Player…”命令

2、输入 URL, rtsp://192.168.1.201/sample_100kbit.mov

单击“OK”按钮

其中 192.168.1.201 是本服务器的静态IP 地址，Sample_100kbit.mov 是放在媒体内容存放位置的媒体文件名。

测试结果：在弹出的播放器窗口下面出现“Buffering……”等字样，并且接下去就进行播放，说明工作站访问QuickTime 流服务器成功。到此为止整个系统构建就完成了。






