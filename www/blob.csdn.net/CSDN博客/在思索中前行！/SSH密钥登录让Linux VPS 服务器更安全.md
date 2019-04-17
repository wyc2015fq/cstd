# SSH密钥登录让Linux VPS/服务器更安全 - 在思索中前行！ - CSDN博客





2015年09月23日 16:41:50[_Tham](https://me.csdn.net/txl16211)阅读数：788标签：[自动登录																[putty密钥自动登录](https://so.csdn.net/so/search/s.do?q=putty密钥自动登录&t=blog)](https://so.csdn.net/so/search/s.do?q=自动登录&t=blog)
个人分类：[linux学习](https://blog.csdn.net/txl16211/article/category/2231071)








查看 /var/log 登录日志文件，突然发现服务器被人扫描端口在进行暴力攻击

平常登录服务器，都是使用用户密码登录，看来这样的做法并不安全，不得不关闭密码登录了。

总结了一个putty使用密钥自动登录 ssh 的实践报告：



安装 openssh
sudo apt-get install openssh-server


安装好后如果没有防火墙的阻挡，那么就可以使用  putty (用户-密码)来远程登录ubuntu 了。



编辑 ssh 配置文档
sudo vi /etc/ssh/sshd_config

Protocol 2 ← 这行表示仅使用SSH2
ServerKeyBits 768 ← 如果需要可以改成 1024 ，而 puttygen.exe 默认是1024 bit
PermitRootLogin no ← 这行表示不允许用root进行登录
PermitEmptyPasswords no ← 这行表示禁止空密码进行登录

PasswordAuthentication no ← 添加这行表示不允许密码方式的登录



增加安全性，按需设置
sudo vi /etc/hosts.deny

sshd: ALL ← 添加这一行，屏蔽来自所有的SSH连接请求



sudo vi /etc/hosts.allow
sshd: 192.168.0. ← 添加这一行，只允许来自内网的SSH连接请求



下面生成公钥和私钥,在普通用户下：
ssh-keygen -t rsa ← 建立公钥与私钥

Enter file in which to save the key (/home/comet/.ssh/id_rsa): ← 钥匙的文件名，这里保持默认直接回车

Enter passphrase (empty for no passphrase): ← 输入私钥密码

Enter same passphrase again: ← 再次私钥输入密码

Your identification has been saved in /home/comet/.ssh/id_rsa ← 私钥的路径

Your public key has been saved in /home/comet/.ssh/id_rsa.pub ← 公钥的路径


cd ~/.ssh ← 进入 /home/comet/.ssh
cp id_rsa.pub authorized_keys  ← 复制公钥为验证文件
chmod 400 authorized_keys  ← 设置验证文件为 root 只读



复制 id_rsa 到 windows 的机器上。

使用 PUTTYGEN.EXE 转换成 putty 可以使用的。

打开 PUTTYGEN.EXE ，点击 Load，选取服务器端生成的私钥，如果上面设置了私钥密码，这里就会提示输入。修改一下 key comment,如果需要密码，可以在 key passphrase 输入并确认。

点击“Save private key”，保存为 .ppk 文件,这里是 uServer.ppk



在 putty 中使用转换后的私钥登录。

打开 putty.exe ，设置好IP，然后在 Connection - SSH - Auth 的 Private key file for authentication 中选择 uServer.ppk 。然后 open  就可以使用密钥登录而不用密码了。

如果在生成 uServer.ppk 的时候没有输入密码，那么可以在 putty.exe 的 Connection - Data 的 Auto-login username 中输入私钥的用户，那么打开 session 之后就可以自动。






## [SSH密钥登录让Linux VPS/服务器更安全](http://www.vpser.net/security/linux-ssh-authorized-keys-login.html)



2011年01月10日 上午 | 作者：[VPS侦探](http://www.vpser.net/author/admin)


随着PHP越来越流行，Linux VPS/服务器的使用也越来越多，Linux的安全问题也需要日渐加强，如果你安装过[DenyHosts](http://www.vpser.net/security/denyhosts.html)并设置过邮件提醒，你每天可能会受到数封[DenyHosts](http://www.vpser.net/security/denyhosts.html) Report将前来破解SSH密码的IP加入/etc/hosts.deny。

Linux SSH登录有两种：

**1、使用密码验证登录**

通常VPS或服务器开通后都是直接提供IP和root密码，使用这种方式就是通过密码方式登录。如果密码不够强壮，而且没有安装[DenyHosts](http://www.vpser.net/security/denyhosts.html)之类的[防止SSH密码破解的软件](http://www.vpser.net/security/denyhosts.html)，那么系统安全将存在很大的隐患。

**2、使用密钥验证登录**

基于密钥的安全验证必须为用户自己创建一对密钥，并把共有的密钥放在需要访问的服务器上。当需要连接到SSH服务器上时，客户端软件就会向服务器发出请求，请求使用客户端的密钥进行安全验证。服务器收到请求之后，先在该用户的根目录下寻找共有密钥，然后把它和发送过来的公有密钥进行比较。如果两个密钥一致，服务器就用公有的密钥加密“质询”，并把它发送给客户端软件（[putty](http://www.vpser.net/other/putty-ssh-linux-vps.html),xshell等）。客户端收到质询之后，就可以用本地的私人密钥解密再把它发送给服务器，这种方式是相当安全的。

## 一、生成密钥

因为puttygen生成的密钥有问题可能会出现：“Server refused our key”，最好使用XShell生成密钥或者在远程Linux VPS/服务器生成密钥。

### 1、在Linux远程服务器生成密钥：

登录远程Linux  VPS/服务器，执行：

root@vpser:~# **ssh-keygen -t rsa** //先运行这个命令

Generating public/private rsa key pair.

Enter file in which to save the key (/root/.ssh/id_rsa):                   //直接回车

Created directory '/root/.ssh'.

Enter passphrase (empty for no passphrase):                                    //输入密钥密码

Enter same passphrase again:                                                                 //重复密钥密码

Your identification has been saved in /root/.ssh/id_rsa.         //提示公钥和私钥已经存放在/root/.ssh/目录下

Your public key has been saved in /root/.ssh/id_rsa.pub.

The key fingerprint is:

15:23:a1:41:90:10:05:29:4c:d6:c0:11:61:13:23:dd root@vpser.net

The key's randomart image is:

+--[ RSA 2048]----+

|=&@Bo+o o.o      |

|=o=.E  o . o     |

| .    .   .      |

|         .       |

|        S        |

|                 |

|                 |

|                 |

|                 |

+-----------------+

root@vpser:~#

将/root/.ssh/下面的id_rsa和id_rsd.pub妥善保存。

### 2、使用XShell生成密钥

Xshell是一款Windows下面功能强大的SSH客户端，能够按分类保存N多会话、支持Tab、支持多密钥管理等等，管理比较多的VPS/服务器使用XShell算是比较方便的，推荐使用。

[下载XShell](http://soft.vpser.net/manage/xshell/xsh040074b.exe)，安装，运行XShell，点击菜单：Tool ->User Key Generation Wizard，出现如下提示：

![](http://vpser.b0.upaiyun.com/uploads/2011/01/xshell-key-generation-wizard-1.jpg)

![](http://vpser.b0.upaiyun.com/uploads/2011/01/xshell-key-generation-wizard-2.jpg)

![](http://vpser.b0.upaiyun.com/uploads/2011/01/xshell-key-generation-wizard-3.jpg)

![](http://vpser.b0.upaiyun.com/uploads/2011/01/xshell-key-generation-wizard-4.jpg)

点击Save as file将密钥保存为id_rsa.pub。

## 二、将密钥添加到远程Linux服务器

1、用[winscp](http://www.vpser.net/manage/winscp.html)，将id_rsa.pub文件上传到/root/.ssh/下面（如果没有则创建此目录），并重命名为：authorized_keys（如果是在Linux服务器上生成的密钥直接执行：**mv /root/.ssh/id_rsa.pub****/root/.ssh/authorized_keys**），再执行：**chmod 600 /root/.ssh/authorized_keys** 修改权限。



2、修改/etc/ssh/sshd_config 文件，将RSAAuthentication 和 PubkeyAuthentication
 后面的值都改成yes ，保存。

3、重启sshd服务，Debian/Ubuntu执行**/etc/init.d/ssh restart** ；CentOS执行：**/etc/init.d/sshd restart**。

## 三、客户端测试使用密钥登录

### 1、使用putty登录

putty使用的私钥文件和Linux服务器或XShell的私钥格式不同，如果使用putty的话，需要将Linux主机上生成的id_rsa文件下载的本地。运行[putty压缩包](http://soft.vpser.net/manage/putty/puttyfile_0.60cn.zip)里面的puttygen.exe，选择Conversions->Import
 key选择私钥文件id_rsa，输入密钥文件的密码，会出现如下界面：

![](http://vpser.b0.upaiyun.com/uploads/2011/01/putty-export-ppk.jpg)

点击“Save Private Key”，将私钥保存为id_rsa.ppk

运行putty，在Host Name填写：root@主机名或ip

![](http://vpser.b0.upaiyun.com/uploads/2011/01/putty-1.jpg)

![](http://vpser.b0.upaiyun.com/uploads/2011/01/putty-2.jpg)

![](http://vpser.b0.upaiyun.com/uploads/2011/01/putty-3.jpg)

如果设置了密钥密码，出现：Passphrase for key "imported-openssh-key"时输入密钥密码。

如果设置没问题就会登录成功，出现用户提示符。

### 2、XShell登录

运行XShell，选择菜单File->New，按如下提示填写：

![](http://vpser.b0.upaiyun.com/uploads/2011/01/xshell-new-session-1.jpg)

![](http://vpser.b0.upaiyun.com/uploads/2011/01/xshell-new-session-2.jpg)

打开创建好的Session

![](http://vpser.b0.upaiyun.com/uploads/2011/01/xshell-select-session.jpg)

![](http://vpser.b0.upaiyun.com/uploads/2011/01/xshell-key-login.jpg)

如果设置没问题就会登录成功，出现用户提示符。

### 3、Linux客户端登录测试

在Linux客户端执行：**chmod 600 /root/id_rsa** 再执行：**ssh root@www.vpser.net -i /root/id_rsa** /root/id_rsa为私钥文件，第一次链接可能会提示确认，输入yes即可，再按提示输入密钥密码，没有问题就会出现用户提示符。

## 四、修改远程Linux服务器sshd服务配置

### 1、修改/etc/ssh/sshd_config 文件

将PasswordAuthentication yes 修改成 PasswordAuthentication no

### 2、重启sshd服务

Debian/Ubuntu执行/etc/init.d/ssh restart ；CentOS执行：/etc/init.d/sshd restart。

ok，设置完成。

再提醒一下一定要保存好Putty私钥文件id_rsa.ppk或Linux服务器下载下来的id_rsa私钥文件。







