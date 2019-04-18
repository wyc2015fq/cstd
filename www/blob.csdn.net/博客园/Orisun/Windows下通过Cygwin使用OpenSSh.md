# Windows下通过Cygwin使用OpenSSh - Orisun - 博客园







# [Windows下通过Cygwin使用OpenSSh](https://www.cnblogs.com/zhangchaoyang/articles/1874647.html)





**Cygwin**

Cygwin功能强大的令我惊叹，它可将Linux下的自由软件移植到Windows系统上来应用！

一、下载

![](https://pic002.cnblogs.com/images/2010/103496/2010111110254344.png)


二、为你的Cygwin建一个目录，如D:\Cygwin

三、点击setup.exe安装

Install from Internet==>Root Directory选择D:\Cygwin,选择All Usres==>Local Package Directory是指你下载的安装临时文件放哪儿，选择D:\Donwloads==>Direct Coonection==>选择一个好的镜像

![](https://pic002.cnblogs.com/images/2010/103496/2010111110323288.png)


然后会列出Linux种种可用的开源软件，根据你的需要，选择性地安装。默认的都是skip即不安装，在search里面搜openssh，单击skip使之变为版本号，表示我们本次将要安装它。除skip外还有其他4种标志Default、Install、Reinstall和Uninstall，依次代表：默认状态、安装、重新安装和反安装。我们这次只安装openssh，如果以后还将其他工具，可以重新点击setup.exe到这一步来选择，那时你看到openssh前的默认标记为keep，意思是保持已安装的软件不变。

![](https://pic002.cnblogs.com/images/2010/103496/2010111110405459.png)


四、配置环境变量

新建系统变量:CYGWIN=ntsec tty

在path中追加D:\Cygwin\bin

五、配置SSH服务

点击桌面上的Cygwin，注意在win7中一定要以管理员身份运行，否则不能创建sshd服务。输入以下命令

cd /bin

ssh-host-config

配置中关键注意以下三项：

Should privilege separation be used? (yes/no) no
Do you want to install sshd as service? (yes/no) yes


Enter the value of cygwin for the daemon：ntsec


六、启动sshd服务

下次重启电脑的时候sshd服务就会自动启动，如果现在就想启动该服务，输入命令：net start sshd或cygrunsrv -S sshd


















