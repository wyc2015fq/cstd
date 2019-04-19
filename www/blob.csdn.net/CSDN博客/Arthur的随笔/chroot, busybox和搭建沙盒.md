# chroot, busybox和搭建沙盒 - Arthur的随笔 - CSDN博客
2013年06月11日 13:00:40[largetalk](https://me.csdn.net/largetalk)阅读数：5504
有时为了安全，我们需要将用户程序的执行环境和外部隔离开来，  在linux下 chroot可完成如此工作。
chroot，即 change root directory (更改 root 目录)。在 linux 系统中，系统默认的目录结构都是以 `/`，即是以根 (root) 开始的。而在使用 chroot 之后，系统的目录结构将以指定的位置作为 `/` 位置。 ----- from：[http://www.ibm.com/developerworks/cn/linux/l-cn-chroot/](http://www.ibm.com/developerworks/cn/linux/l-cn-chroot/)
按wiki上说法是， 一个chroot环境可用于创建并运行一个隔离的虚拟软件系统拷贝。
so， how to use chroot？
还是根据上面IBM那篇文章，先准备一个busybox的东东，BusyBox 是一个集成了一百多个最常用[linux](http://baike.baidu.cn/view/1634.htm)命令和工具的软件。主页：[http://www.busybox.net/](http://www.busybox.net/)
下载busybox可执行文件，并建立如下目录：
```
$ tree .
.
├── bin
│   ├── ash -> busybox
│   └── busybox
├── etc
└── newhome
```
然后 chroot . bin/ash 这样就切换root了， 现在这个环境中没什么程序，不过有个busybox， 可以这样busybox  ls 执行些小程序。
这个环境还太简单了，有个叫jailkit的东西能快速建立沙盒环境， [http://olivier.sessink.nl/jailkit/](http://olivier.sessink.nl/jailkit/)
这里有偏介绍用jailkit建立受限ssh帐户的文章 [http://dongbeta.com/2012/02/setup-an-limited-account-on-linux-by-jailkit/](http://dongbeta.com/2012/02/setup-an-limited-account-on-linux-by-jailkit/)
jailkit 给出的一个例子：
```
# Initialise the jail
mkdir /home/sftproot
chown root:root /home/sftproot
chmod 0755 /home/sftproot
jk_init -j /home/sftproot jk_lsh
jk_init -j /home/sftproot sftp
jk_init -j /home/sftproot scp
# Create the account
jk_addjailuser -j /home/sftproot test
# Edit the jk_lsh configfile in the jail; see man jk_lsh..
# You can use every editor you want; I choose 'joe'
joe /home/sftproot/etc/jailkit/jk_lsh.ini
# Restart jk_socketd so that log messages are transferred 
killall jk_socketd
jk_socketd
# Test the account
sftp test@localhost
# Check the logs to see if everything is correct
tail /var/log/daemon.log /var/log/auth.log
```
