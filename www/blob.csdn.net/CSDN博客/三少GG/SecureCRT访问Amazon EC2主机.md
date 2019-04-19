# SecureCRT访问Amazon EC2主机 - 三少GG - CSDN博客
2014年01月18日 20:57:58[三少GG](https://me.csdn.net/scut1135)阅读数：5360

AWS的EC2服务器是用密钥来认证的，在创建instance时，会提示，创建一个key pair,同时会提示下载一个xxx.pem的密钥文件到本地硬盘。下面是通过SecureCRT连接到EC2的操作步骤：
1.上传xxx.pem文件到一个linux系统下，修改其权限为600,。命令 chmod 600 xxx.pem
2.改写密钥格式为 OpenSSH，命令 ssh-keygen -p -f xxx.pem（如果询问passphrase可以直接回车）
3.生成公密钥 .pub 文件 命令 ssh-keygen -e -f xxx.pem > xxx.pem.pub
4.把生成的文件下载到本地。
5.在SecureCRT中新建连接，协议选择ssh2 ，主机名填写public dns（这个地址EC2的控制台中会提供），用户名填写ec2-user 。
6.在SecureCRT连接的列表中找到刚创建的连接，右键属性，左边树中选择SSH2,然后选择右边authentication中publicKey,点属性,在出来的对话框中选择使用身份或证书文件,通过浏览 文件指定到刚在Linux下生成的文件即可.注意xxx.pem.pub和xxx.pem要在同一文件夹下。
现在打开刚才新建的连接，就可以连上EC2服务器了
亚马逊推出了免费的云主机服务器 Amazon EC2，它是通过安全密钥来访问主机的。问题是下载的密钥在SecureCRT 上无法直接使用，需要转换。
下面的方法可以在自己的linux主机上生成securecrt需要的密钥。
首先在 AWS 管理面板中生成密钥对。
将密钥上传到一台自己的linux主机，下面举例文件名为 amazon-ec2-key.pem
修改亚马逊提供的密钥文件权限：
chmod og-r amazon-ec2-key.pem
改写密钥格式为 OpenSSH，如果询问passphrase可以留空（直接回车）
ssh-keygen -p -f amazon-ec2-key.pem
生成公密钥 .pub 文件。使用公密钥时，SecureCRT会询问私密钥或者.pem文件
ssh-keygen -e -f amazon-ec2-key.pem >> amazon-ec2-key.pem.pub
现在可以在 SecureCRT 中使用刚生成的公密钥了，连接到亚马逊的主机也无需再输入密码。
Amazon EC2的Linux主机采用证书登录形式，不过Amazon的证书不能直接用在SecureCRT中，需要做一个转换，下边说下过程。
1.创建Amazon EC2 Key Pair(这个在首次创建Instance的时候会自动创建)。
2.下载这个Key Pair到一台本地的Linux主机上，假设这个Key file的名字是amazon-ec2-key.pem。
3.修改Key file的权限。
chmod og-r amazon-ec2-key.pem
4.将Key file转换成OpenSSH格式。
ssh-keygen -p -f amazon-ec2-key.pem
5.将public key从Key file中解压出来。
ssh-keygen -e -f amazon-ec2-key.pem >> amazon-ec2-key.pem.pub
6.在SecureCRT中配置使用即可。
参考文档:
[http://www.question-defense.com/2009/10/01/convert-amazon-web-services-private-rsa-key-for-use-with-securecrt](http://www.question-defense.com/2009/10/01/convert-amazon-web-services-private-rsa-key-for-use-with-securecrt)
## Access Amazon EC2 with SSH/putty/SecureCRT
By [kamus](http://www.dbform.com/html/author/kamus-2) on March 23, 2011
这两天注册了Amazon [AWS](http://aws.amazon.com/about-aws/)，体验了一下[EC2](http://aws.amazon.com/ec2/)和[S3](http://aws.amazon.com/s3/)，总体来说很强大，很便宜，绝对是创业公司的首选云产品。
注册AWS需要一张国际信用卡，在注册完毕的时候，跟Apple iTunes一样会测试性地划款1美元，但是最终会退回到你的信用卡里，不必担心。对于AWS新用户，Amazon很大方地给予了一项免费体验套餐。
S3就是一份云存储，可以认为跟Dropbox或者Box.net是一样的东西，可以直接通过[AWS Management Console](http://aws.amazon.com/console/)操作。在第一年的每个月都将免费获得5G存储空间，20,000个下载请求, 2,000个上传请求, 15GB下载流量和15GB上传流量。
对于EC2而言，在第一年的每个月都将获得如下图中的免费资源。包括750小时的EC2运行时间，这基本上就是一个月31天每天24小时的免费运行。15G流量，10G存储。本月没有使用完的不会累计到下个月。
![AWS Free Tier](http://i0.wp.com/farm6.static.flickr.com/5023/5550624550_313b1777b0.jpg?zoom=1.5&resize=500%2C161)
本文主要讲述EC2，在某种意义上你可以认为EC2就是虚拟主机，支持Linux和Windows系统，每个虚拟主机就是一个EC2 Instance，一个EC2帐号支持创建多个Instance，每个虚拟主机都是你个人独享的，你可以选择启动/停止/删除。在AWS Management Console中可以通过copy模板（成为Amazon Machine Image，AMI）来很简单地创建自己的EC2 Instance。
免费的EC2套餐支持的Instance硬件情况如下图。Instance Type是Micro，最多1颗CPU，613M内存，8G存储空间。
![Free Tier EC2 Instance](http://i1.wp.com/farm6.static.flickr.com/5143/5550041857_be02200cce.jpg?zoom=1.5&resize=500%2C274)
创建完毕以后，在AWS Management Console的EC2页面下就可以选择启动该Instance。接下来就是本文的重点，该如何通过工具来连接该虚拟主机呢？
1. SSH
如果是Mac或者Linux用户，那么可以使用命令行方式的ssh，在EC2 Instance上点右键，选择Connect，会出现以下页面提示你该如何通过ssh连接此实例。
![Connect Amazon EC2 using SSH](http://i0.wp.com/farm6.static.flickr.com/5132/5550624322_75be8c5fdb.jpg?zoom=1.5&resize=475%2C448)
其中第2步提到的Private key file是在创建Instance的时候会要求你创建并下载的，该key file的名称可以随意。
第3步提到的chmod必须要执行，否则ssh时候会报key file权限错误。
在Example中提到的命令如下：
ssh -i kamuskey.pem root@ec2-50-17-88-48.compute-1.amazonaws.com
但是在实际使用中，发现使用root是无法登陆的，必须要使用ec2-user，也许这是免费用户的限制。
ssh -i kamuskey.pem ec2-user@ec2-50-17-88-48.compute-1.amazonaws.com
2. [putty](http://www.chiark.greenend.org.uk/~sgtatham/putty/)
在Windows下没有原生的ssh命令，所以可以选择免费的putty作为ssh工具。
观看[AWS EC2 Console Tutorial](http://media.amazonwebservices.com/console/AwsConsole.html)视频，其中详细描述了该如何使用puttygen来生成putty格式的密钥，并且在putty中该如何设置使用该密钥。
3. [SecureCRT](http://www.vandyke.com/products/securecrt/)
SecureCRT是Windows下（现在也有Mac版了）极受欢迎的ssh工具，它不是免费的。
我们需要通过ssh-keygen命令将AWS的密钥转换成SecureCRT支持的OpenSSH格式密钥，详细教程参看[此处](http://www.question-defense.com/2009/10/01/convert-amazon-web-services-private-rsa-key-for-use-with-securecrt)。
用ec2-user登录。
login as: ec2-user
Authenticating with public key "imported-openssh-key"
 
       __|  __|_  )  Amazon Linux AMI
       _|  (     /     Beta
      ___|\___|___|
 
See /usr/share/doc/system-release-2011.02 for latest release notes. :-)
[ec2-user@ip-10-117-45-99 ~]$ pwd
/home/ec2-user
主盘有8G存储空间，tmpfs只有300M，比较小。
[ec2-user@ip-10-117-45-99 ~]$ df -k
Filesystem           1K-blocks      Used Available Use% Mounted on
/dev/xvda1             8256952    901768   7271300  12% /
tmpfs                   305604        40    305564   1% /dev/shm
600M内存。
[ec2-user@ip-10-117-45-99 ~]$ free
             total       used       free     shared    buffers     cached
Mem:        611212      87612     523600          0       7032      40904
-/+ buffers/cache:      39676     571536
Swap:            0          0          0
虽然不可以用root登录，但是sudo命令可以正常使用。
[ec2-user@ip-10-117-45-99 ~]$ sudo ifconfig
eth0      Link encap:Ethernet  HWaddr 12:31:3D:03:22:99
          inet addr:10.117.45.99  Bcast:10.117.45.255  Mask:255.255.255.0
          inet6 addr: fe80::1031:3dff:fe03:2299/64 Scope:Link
          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
          RX packets:937 errors:0 dropped:0 overruns:0 frame:0
          TX packets:935 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000
          RX bytes:99655 (97.3 KiB)  TX bytes:139821 (136.5 KiB)
          Interrupt:9
 
lo        Link encap:Local Loopback
          inet addr:127.0.0.1  Mask:255.0.0.0
          inet6 addr: ::1/128 Scope:Host
          UP LOOPBACK RUNNING  MTU:16436  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:0
          RX bytes:0 (0.0 b)  TX bytes:0 (0.0 b)
Cheers.
