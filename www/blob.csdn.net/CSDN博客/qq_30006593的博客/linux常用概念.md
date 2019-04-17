# linux常用概念 - qq_30006593的博客 - CSDN博客





2017年09月29日 10:02:28[lshiwjx](https://me.csdn.net/qq_30006593)阅读数：93








[开机apport提示](https://askubuntu.com/questions/615478/constant-startup-error-issue-what-is-com-ubuntu-apport-support-gtk-root)：说明其他程序崩了 
[load average](http://heipark.iteye.com/blog/1340384)第一位1.30：表示最近1分钟平均负载 

第二位1.48：表示最近5分钟平均负载 

第三位1.69：表示最近15分钟平均负载
# 磁盘

索引节点用完 

 There is enough disk space, but you have reached the maximum number that a file system can hold, because of huge numbers of files on the disk. 

Run df -ih to see the used inode percentage, perhaps 100%?
# ssh

## 原理

整个过程是这样的：（1）远程主机收到用户的登录请求，把自己的公钥发给用户。（2）用户使用这个公钥，将登录密码加密后，发送回来。（3）远程主机用自己的私钥，解密登录密码，如果密码正确，就同意用户登录

## 风险

这个过程本身是安全的，但是实施的时候存在一个风险：如果有人截获了登录请求，然后冒充远程主机，将伪造的公钥发给用户，那么用户很难辨别真伪。因为不像https协议，SSH协议的公钥是没有证书中心（CA）公证的，也就是说，都是自己签发的。 

如果你是第一次登录对方主机，系统会出现下面的提示：无法确认host主机的真实性，只知道它的公钥指纹，问你还想继续连接吗？

## 无密码登录

使用密码登录，每次都必须输入密码，非常麻烦。好在SSH还提供了公钥登录，可以省去输入密码的步骤。 

所谓”公钥登录”，原理很简单，就是用户将自己的公钥储存在远程主机上。登录的时候，远程主机会向用户发送一段随机字符串，用户用自己的私钥加密后，再发回来。远程主机用事先储存的公钥进行解密，如果成功，就证明用户是可信的，直接允许登录shell，不再要求密码。 

这种方法要求用户必须提供自己的公钥。如果没有现成的，可以直接用ssh-keygen生成一个： 
$ ssh-keygen  运行结束以后，在$HOME/.ssh/目录下，会新生成两个文件：id_rsa.pub和id_rsa。前者是你的公钥，后者是你的私钥。 

这时再输入下面的命令，将公钥传送到远程主机host上面： 

　　$ ssh-copy-id user@host
# x-server

X Window System（常被简称为X11或X），是一套基于X display protocol的windowing system，X GUI环境的功能包括窗口的绘制、移动，以及与鼠标、键盘等输入设备的交互。

## 原理

X采用C/S模型（这是关键）：一个X server 和多个应用程序（client）通信。server接收client的请求绘制窗口，并将来自鼠标、键盘等设备的输入传递给client。 

因此 X server和client可以位于同一计算机上，例如在Linux主机上使用KDE等桌面环境就是这种模式。X server也可以通过同构网络、异构网络或Internet与client通信。 

X server与client之间的通信是不加密的，这个问题可以通过SSH解决。SSH是Secure Shell的简称，SSH可以看作是通信被加密压缩版的telnet。
## 配置文件

ssh_config和sshd_config都是ssh服务器的配置文件，二者区别在于，前者是针对客户端的配置文件，后者则是针对服务端的配置文件。 

每一行为“关键词&值”的形式，其中“关键词”是忽略大小写的

Host * 

“Host”只对匹配后面字串的计算机有效，“*”表示所有的计算机。

X11Forwarding no 

“X11Forwarding”设置是否允许X11转发。

ForwardX11 no 

“ForwardX11”设置X11连接是否被自动重定向到安全的通道和显示集（DISPLAY set）。

椭圆曲线数字签名算法 (ECDSA) 生成的密钥更小，安全性更高。OpenSSH 5.7 建议默认使用 ECDSA， 

[其他命令][http://blog.csdn.net/field_yang/article/details/51568861](http://blog.csdn.net/field_yang/article/details/51568861))

## ssh

ssh -x开启x11转发功能 

-p选择端口







