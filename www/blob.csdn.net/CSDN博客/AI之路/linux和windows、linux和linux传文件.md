# linux和windows、linux和linux传文件 - AI之路 - CSDN博客





2017年10月12日 08:03:28[AI之路](https://me.csdn.net/u014380165)阅读数：5417








下面的代码都在Ubuntu 16.04和Win7上实验过，可行。

**传输的前提是linux机器要先安装好ssh。**

## SSH

SSH分客户端`openssh-client`和服务端`openssh-server`。如果你只是想登陆别的机器，那么只需要安装`openssh-client`，命令如下：

```bash
sudo apt install openssh-client
```

如果要使本机开放SSH服务，也就是让别人可以连接你的机器，那么就需要安装`openssh-server`。命令如下：

```bash
sudo apt install openssh-server
```

一般安装完`openssh-server`后可以手动启动下server：
`/etc/init.d/ssh start`
如果要停止`openssh-server`，比如为了防止别人连接你的机器，可以用如下命令：
`/etc/init.d/ssh stop`
**如果你的机器没有启动openssh-server，那么别人就连接不了你的机器，也没法从你的机器拷贝文件，但是你可以连别的开启了openssh-server的机器，也可以拷贝文件，因为你本机的openssh-client安装后就默认打开。**

安装成功后，端口的设置文件在`/etc/ssh/sshd_config`，可以用
`vim /etc/ssh/sshd_config`
来查看和修改，最上面的Port 22 就是默认端口号是22，可以自行修改。修改完以后记得重启服务：
`/etc/init.d/ssh restart`
有了SSH，从本地linux机器登录另一台开启了openssh-server的linux机器（假设IP为192.168.0.1，端口为22）就可以这样登：
`ssh -p 22 user1@192.168.0.1`
这里参数p是小写，然后输入密码即可。

## linux和windows之间：

我采用的是pscp方式。 

首先下载pscp：[pscp下载路径](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html)

然后将其放入windows的system32 文件夹下，这样在dos命令窗口中就能直接调用使用了。
要传送文件的时候，假设你要将windows下的d:/test.txt传到IP为10.100.200.11的linux机器的/home/user1文件夹下，假设linux机器的端口是22，用户名是：user1，那么从Windows的开始中打开cmd。

**Windows—>Linux**

在Windows的cmd中输入：
`pscp -P 22 d:test.txt user1@10.100.200.11:/home/user1`
这里 -P 指定端口，@符号的前面是用户名，后面是Linux机器的地址。输完这个命令会提示输入密码，输入即可，下同。

**Linux—>Windows**

在Windows的cmd中输入
`pscp -P 22 user1@10.100.200.11:/home/user1/test.txt d:/`
这是将linux机器下的/home/user1/test.txt文件拷贝到windows机器的d盘下。

**如果要拷贝文件夹，则多加一个 -r 参数即可**
`pscp -r -P 22 user1@10.100.200.11:/home/user1/test d:/`
这是将linux机器下的/home/user1/test文件夹拷贝到windows机器的d盘下。

## linux和linux之间：

采用scp命令。 

假设从另外一台linux机器拷贝文件到本机，另外一台linux机器的地址是10.100.200.11，用户名是user1，端口是22，要拷的目录是/home/user1/test.txt，那么命令行输入：
`scp -P 22 user1@10.100.200.11：/home/user1/test.txt .`
最后一个.符号表示拷贝到本机的当前目录，也就是命令行当前所处的目录，当然也可以指定一个存放目录。

如果要拷贝文件夹，则多加一个 -r 参数即可

**可能遇到的问题：**

scp远程拷贝文件时提示错误： 

Warning: Permanently added ‘[10.100.200.11]:22’ (ECDSA) to the list of known hosts. 

Permission denied (publickey). 

解决： 

可能是你的机器重启但是没开ssh服务，因此登录10.100.200.11，重启ssh服务即可：`/etc/init.d/ssh restart`




