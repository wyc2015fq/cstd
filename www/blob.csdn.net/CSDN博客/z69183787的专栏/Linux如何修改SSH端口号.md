# Linux如何修改SSH端口号 - z69183787的专栏 - CSDN博客
2017年07月26日 17:23:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：8331
[http://www.jb51.net/LINUXjishu/217951.html](http://www.jb51.net/LINUXjishu/217951.html)
**SSH是什么？**
SSH 为 Secure Shell
由 IETF 的网络工作小组（Network Working Group）所制定；
SSH 是建立在应用层和传输层基础上的一种安全协议。
SSH传输数据是**加密的**，可以有效防止传输过程被截取数据保障安全。
SSH的数据是经过**压缩的**，所以可以加快传输的速度
**查看SSH服务**
首先查看一下当前linux是否已经安装SSH软件包，使用 rpm -qa|grep ssh
![SSH是什么？Linux如何修改SSH端口号？](http://files.jb51.net/file_images/article/201409/201409150853039.jpg)
2.确认ssh服务已经开启，下面小编以centos 系统为例
![SSH是什么？Linux如何修改SSH端口号？](http://files.jb51.net/file_images/article/201409/2014091508530310.jpg)
3.找到SSh服务配置文件路径一般都是在 /etc/ssh这个目录下面 sshd_config 这个文件
![SSH是什么？Linux如何修改SSH端口号？](http://files.jb51.net/file_images/article/201409/2014091508530311.jpg)
END
**编辑修改SSH端口号**
1.使用VI \vim编辑器，打开sshd_config这个文件，搜索找到 port字段。如下图
![SSH是什么？Linux如何修改SSH端口号？](http://files.jb51.net/file_images/article/201409/2014091508530312.jpg)
2 将光标定位到port  22这行 yy  然后键盘 P复制一行， insert插入编辑22端口为2222
![SSH是什么？Linux如何修改SSH端口号？](http://files.jb51.net/file_images/article/201409/2014091508530313.jpg)
3设置好之后如下图，wq保存退出， 表示已经增加了一个2222端口号啦
![SSH是什么？Linux如何修改SSH端口号？](http://files.jb51.net/file_images/article/201409/2014091508530314.jpg)
 4.设置好之后，当然需要重启SSH服务了。 
![SSH是什么？Linux如何修改SSH端口号？](http://files.jb51.net/file_images/article/201409/2014091508530315.jpg)
5.如果您有设置防火墙，请修改增加防火墙规则，或者直接关闭防火墙也行
![SSH是什么？Linux如何修改SSH端口号？](http://files.jb51.net/file_images/article/201409/2014091508530316.jpg)
END
注意事项
SSH端口默认是22，如果要修改直接编辑22端口注意前面的“#”要去掉，然后保存重启
如果是增加端口号，直接按照小编的方法，增加一个port端口即可
为了系统的安全性，有时需要修改ssh的默认端口。修改方法如下：
     vim  /etc/ssh/sshd_config
     添加两个Port
        # $OpenBSD: sshd_config,v 1.69 2004/05/23 23:59:53 dtucker Exp $ 
       # This is the sshd server system-wide configuration file. See
       # sshd_config(5) for more information.
       # This sshd was compiled with PATH=/usr/local/bin:/bin:/usr/bin
       # The strategy used for options in the default sshd_config shipped with
       # OpenSSH is to specify options with their default value where
       # possible, but leave them commented. Uncommented options change a
       # default value.
**       Port 22    //以前这个前面是有 # 号的       Port 8284 **
       #Protocol 2,1
       #ListenAddress 0.0.0.0
       #ListenAddress ::
       # HostKey for protocol version 1
       #HostKey /etc/ssh/ssh_host_key
       # HostKeys for protocol version 2
       #HostKey /etc/ssh/ssh_host_rsa_key
       #HostKey /etc/ssh/ssh_host_dsa_key
       重启sshd服务
       /etc/init.d/sshd restart
      若防火墙开启，需要修改防火墙设置。
