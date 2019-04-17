# Putty 工具使用 - 在思索中前行！ - CSDN博客





2014年08月07日 21:31:55[_Tham](https://me.csdn.net/txl16211)阅读数：706标签：[putty																[linux远程](https://so.csdn.net/so/search/s.do?q=linux远程&t=blog)](https://so.csdn.net/so/search/s.do?q=putty&t=blog)
个人分类：[电脑杂料](https://blog.csdn.net/txl16211/article/category/2373823)








## [如何使用Putty远程(SSH)管理Linux VPS](http://www.vpser.net/other/putty-ssh-linux-vps.html)


Putty是一个免费的、Windows 32平台下的telnet、rlogin和ssh客户端，但是功能丝毫不逊色于商业的telnet类工具。用它来远程管理Linux VPS十分好用，其主要优点如下：

◆ 完全免费;

◆ 在Windows 下运行的都非常好;

◆ 全面支持ssh1和ssh2；

◆ 绿色软件，无需安装，下载后在桌面建个快捷方式即可使用；

◆ 体积很小；

◆ 操作简单，所有的操作都在一个控制面板中实现。

putty下载地址： http://soft.vpser.net/manage/putty/puttyfile_0.60cn.zip

下载完成，解压，即可使用。

以下是使用putty 登录Linux VPS进行远程（SSH）管理 的图片教程：

![putty-1](http://vpser.b0.upaiyun.com/uploads/2009/05/putty-1.png)

![putty-2](http://vpser.b0.upaiyun.com/uploads/2009/05/putty-2.png)

![putty-3](http://vpser.b0.upaiyun.com/uploads/2009/05/putty-3.png)

![putty-4](http://vpser.b0.upaiyun.com/uploads/2009/05/putty-4.png)

**提示：输入密码是不显示的！！**

![putty-5](http://vpser.b0.upaiyun.com/uploads/2009/05/putty-5.png)

![putty-6](http://vpser.b0.upaiyun.com/uploads/2009/05/putty-6.png)

呵呵，又好长时间不更新了，更新一些基础知识给VPSer新手们。

### **什么是SSH？**

SSH 为 Secure Shell 的缩写，由 IETF 的网络工作小组（Network Working Group）所制定；SSH 为建立在应用层和传输层基础上的安全协议。

传统的网络服务程序，如FTP、POP和Telnet其本质上都是不安全的；因为它们在网络上用明文传送数据、用户帐号和用户口令，很容易受到中间人（man-in-the-middle）攻击方式的攻击。就是存在另一个人或者一台机器冒充真正的服务器接收用户传给服务器的数据，然后再冒充用户把数据传给真正的服务器。

而 SSH 是目前较可靠，专为远程登录会话和其他网络服务提供安全性的协议。利用 SSH 协议可以有效防止远程管理过程中的信息泄露问题。透过 SSH 可以对所有传输的数据进行加密，也能够防止 DNS 欺骗和 IP 欺骗。

SSH 之另一项优点为其传输的数据是经过压缩的，所以可以加快传输的速度。SSH有很多功能，它既可以代替 Telnet，又可以为FTP、POP、甚至为 PPP 提供一个安全的“通道”。

2、 Putty的美化



1.       中文乱码问题。

这个问题由来已久，每当我查看 mount到linux下的windows 中文目录的时候，都是一堆乱码， putty 也拒绝我输入中文， 一句话，这玩意，对中文过敏。

解决方式：

1.1修改linux的环境变量。
|[root@localhost arm]#vi /etc/sysconfig/i18n|
|----|

修改LANG=zh_CN.GBK改为LANG=zh_CN.UTF-8
|[root@localhost arm]#source /etc/sysconfig/i18n|
|----|

PS：这里我们采用UTF-8的编码方式。

1.2修改putty   Window>Translation>Received data assumed to be in which character set

   选择 UTF-8

![](http://blogimg.chinaunix.net/blog/upfile2/101116123011.png)

1.3   不要忘记保存。

![](http://blogimg.chinaunix.net/blog/upfile2/101116123057.png)



保存到sessions里。 下次使用就不会变化了。

OK。 简单三步，解决中文问题：


|[root@localhost ztl]# 不正确的选项: ambiwidth=不正确的选项: ambiwidth=doublebash: 不正确的选项:: command not found[root@localhost ztl]# 不正确的选项: ambiwidth=doublebash: 不正确的选项:: command not found[root@localhost ztl]# vi /root/.vimrc处理 /root/.vimrc 时发生错误:行   88:不正确的选项: ambiwidth=double[root@localhost ztl]#[root@localhost ztl]# vi /root/.vimrc|
|----|

看，能正确显示中文。



![](http://blogimg.chinaunix.net/blog/upfile2/101116123336.png)

          能正确输入中文，并能正确显示中文目录。



2.       putty的字体设置。

2.1 在网上搜索了大半天，找到一个好用的字体： 微软雅黑字体：

[http://img.namipan.com/downfile/7c74a6406da9968cc126d6d33b4e4d11536ef7fbfd177a00/Consolas%e5%ad%97%e4%bd%93%e9%9b%85%e9%bb%91%e6%b7%b7%e5%90%88%e7%89%88YaHei.Consolas.1.11b.rar](http://img.namipan.com/downfile/7c74a6406da9968cc126d6d33b4e4d11536ef7fbfd177a00/Consolas%E5%AD%97%E4%BD%93%E9%9B%85%E9%BB%91%E6%B7%B7%E5%90%88%E7%89%88YaHei.Consolas.1.11b.rar)

不过上面连接是 纳米机器人的下载连接。你google一下“Consolas雅黑混合版“很多的。

下载之后拷贝到 c:\WINDOWS\Fonts下面。

2.2选择putty的字体：

 Window>Font settings>Change

选择Consolas即可

![](http://blogimg.chinaunix.net/blog/upfile2/101116123416.png)



3.       Vim 配置：

Vim配置，我直接从网上考了一段下来。

[http://www.renseng.com/learning/super-vim-configuration-file-be-updated.html](http://www.renseng.com/learning/super-vim-configuration-file-be-updated.html)

主要参考了这个， 我这里就不发了。

将上面的文件拷贝下来， 保存为/root/.vimrc即可。

4.       PS1设置
|export PS1="\[\e]0;\u \w\a\]\[\033[01;32m\][\u]\[\033[00m\]:\[\033[01;34m\]\W\[\033[00m\]\$"|
|----|





5.    最终效果：



6

![](http://blogimg.chinaunix.net/blog/upfile2/101116123933.png)

.       参考文献



PuTTY 中文教程

[http://chaifeng.com/blog/2007/06/putty_200611.html#_582484557233528](http://chaifeng.com/blog/2007/06/putty_200611.html#_582484557233528)

**微软官方雅黑字体Windows XP版与Consolas雅黑混合版适合编程字体下载**

http://www.iplaysoft.com/consolas.html







