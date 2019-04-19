# Linux上安装使用SSH(ubuntu&&redhat) - xqhrs232的专栏 - CSDN博客
2016年03月23日 09:46:05[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：12383
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址：[http://blog.csdn.net/zxs9999/article/details/6930736](http://blog.csdn.net/zxs9999/article/details/6930736)
相关文章
1、[[Linux实用工具]Ubuntu环境下SSH的安装及使用](http://www.cnblogs.com/rond/p/3688529.html)----[http://www.cnblogs.com/rond/p/3688529.html](http://www.cnblogs.com/rond/p/3688529.html)
2、Linux之SSH服务详解----[http://jingyan.baidu.com/article/c910274be7bc6acd361d2da7.html](http://jingyan.baidu.com/article/c910274be7bc6acd361d2da7.html)
3、Linux平台下安装SSH----[http://www.linuxidc.com/Linux/2014-01/94793.htm](http://www.linuxidc.com/Linux/2014-01/94793.htm)
4、Linux系统下如何配置SSH？如何开启SSH？----[http://jingyan.baidu.com/article/08b6a591f0fafc14a9092275.html](http://jingyan.baidu.com/article/08b6a591f0fafc14a9092275.html)
### Ubuntu安装使用SSH
ubuntu默认并没有安装ssh服务，如果通过ssh链接ubuntu，需要自己手动安装ssh-server。判断是否安装ssh服务，可以通过如下命令进行：
- xjj@xjj-desktop:~$
 ssh localhost   
- ssh: connect to host localhost port 22:
 Connection refused   
如上所示，表示没有还没有安装，可以通过apt安装，命令如下：
- xjj@xjj-desktop:~$
 sudo apt-get install openssh-server  
系统将自动进行安装，安装完成以后，先启动服务：
- xjj@xjj-desktop:~$
 sudo /etc/init.d/ssh start  
启动后，可以通过如下命令查看服务是否正确启动
- xjj@xjj-desktop:~$
 ps -e|grep ssh   
- 6212 ?        00:00:00 sshd  
如上表示启动ok。注意，ssh默认的端口是22，可以更改端口，更改后先stop，
然后start就可以了。改配置在/etc/ssh/sshd_config下，如下所示。
- xjj@xjj-desktop:~$
 vi /etc/ssh/sshd_config   
- # Package generated configuration file   
- # See the sshd(8)
 manpage **for** details
   
- # What ports, IPs and protocols we listen **for**
- Port 22
最后，应该是连接的时候了。请看如下命令：
- xjj@xjj-desktop:~$
 ssh exceljava@192.168.158.129
========================================================
**Red Hat Linux上安装使用SSH**
**一、安装与启动SSH**
在Red Hat Linux上的发行版本中大多已经包含了与OpenSSH相关的软件包，如果没有，则可以OpenSSH的主页下载RPM包自行安装，OpenSSH的主页是地址是: www.openssh.com。主要安装如下几个包:
- 
openssh-3.5p1-6、openssh-server-3.5p1-6
- 
openssh-askpass-gnome-3.5p1-6
- 
openssh-clients-3.5p1-6
- 
openssh-askpass-3.5p1-6
使用如下命令进行安装: 
首先查询系统是否安装了上述软件包。
- # pm －qa |grep openssh   
如果没有安装则执行如下命令。
- # rpm －ivh openssh-3.5p1-6
- # rpm －ivh openssh-server-3.5p1-6
- # rpm －ivh openssh-askpass-gnome-3.5p1-6
- # rpm －ivh openssh-clients-3.5p1-6
- # rpm －ivh openssh-askpass-3.5p1-6
安装完成之后，可以使用下述两个命令中的任一个进行启动。
- # service sshd start   
- # /etc/rc.d/init.d/sshd start   
另外，如果想在系统启动时就自动运行该服务，那么需要使用setup命令，在system service的选项中，选中sshd守护进程即可。 
安装启动完OpenSSH之后，用下面命令测试一下。
- # ssh -l [username] [address of the remote host]  
说明:
- 
[username]　为远程主机的用户名
- 
[address of the remote host]　为远程主机的地址
如果OpenSSH工作正常，将会看到下面的提示信息: 
The authenticity of host [hostname] can't be established. 
Key fingerprint is 1024 5f:a0:0b:65:d3:82:df:ab:44:62:6d:98:9c:fe:e9:52. 
Are you sure you want to continue connecting (yes/no)?
在第一次登录的时候，OpenSSH将会提示用户它不知道这台登录的主机，只要键入“yes”，就会把这台登录主机的“识别标记”加到“~/.ssh/know_hosts”文件中。第二次访问这台主机的时候就不会再显示这条提示信息了。然后，SSH提示用户输入远程主机上用户账号的口令。这样，就建立了SSH连接，这之后就可以象使用telnet那样方便地使用SSH了。 
**二、SSH的密匙管理**
**1．生成用户自己的密匙对**
用下面的命令可以生成公钥/私钥对:
- # ssh-keygen t 类型  
。 
如果远程主机使用的是SSH 2.x就要用这个命令:
- # ssh-keygen d  
。 
在同一台主机上同时有SSH1和SSH2的密匙是没有问题的，因为密匙是存在不同文件中的。ssh-keygen命令运行之后会显示下面的信息:
- # ssh-keygen -t rsa   
Generating public/private rsa key pair. 
Enter file in which to save the key (/home/.username/ssh/id_rsa): 
Enter passphrase (empty for no passphrase): 
Enter same passphrase again: 
Your identification has been saved in /home/.username /.ssh/id_rsa. 
Your public key has been saved in /home/.username /.ssh/id_rsa.pub. 
The key fingerprint is: 
38:25:c1:4d:5d:d3:89:bb:46:67:bf:52:af:c3:17:0c username@localhost 
Generating RSA keys: 
Key generation complete.
“ssh-keygen －d”命令做的是同样的工作，但是它的一对密匙的保存路径默认情况下为: /home/[user]/.ssh/id_dsa（私人密匙）和/home/[user]/.ssh/id_dsa.pub（公用密匙）。现在用户有一对密匙了: 公用密匙要分发到所有用户想用SSH登录的远程主机上去; 私人密匙要好好地保管防止别人知道。用“
- # ls －l ~/.ssh/identity”或“# ls －l ~/.ssh/id_dsa”  
命令所显示的文件的访问权限必须是“-rw-------”。 
如果用户怀疑自己的密匙已经被别人知道了，应当马上生成一对新的密匙。当然，这样做之后还需要重新分发一次公用密匙，才能正常使用。 
**2．分发公用密匙 **
在每一个用户需要用SSH连接的远程服务器上，都要在自己的主目录下创建一个“.ssh”的子目录，把用户的公用密匙“identity.pub”拷贝到这个目录下并把它重命名为“authorized_keys”。然后执行命令:
- # chmod 644 .ssh/authorized_keys   
这一步是必不可少的。因为，如果除了用户之外别人对“authorized_keys”文件也有写的权限，那么如果遭到非法的破坏，SSH就不能正常工作。 
如果用户想从不同的计算机登录到远程主机，“authorized_ keys”文件也可以有多个公用密匙。在这种情况下，必须在新的计算机上重新生成一对密匙，然后把生成的“identify.pub”文件拷贝并粘贴到远程主机的“authorized_keys”文件里。当然，在新的计算机上用户必须有一个账号，而且密匙是用口令保护的。有一点很重要，就是当用户取消了这个账号之后，必须记住把这一对密匙删掉。 
**三、配置SSH的客户端 **
在Linux客户端下使用SSH，优点是操作更方便，无须其他软件。但缺点是不太直观。用户只需要使用系统提供的默认的配置文件“/etc/ssh/ssh_config”，并且使用如下简单的命令即可登录: 
// 以用户test登录远程服务器www.test.com
- # ssh -l test www.test.com  
下面主要介绍配置使用Windows环境下的putty工具来登录SSH服务器。该工具目前使用得相当普遍，可以从网上免费下载。目前网上的最新版本为: putty 0.58，对该版本进行安装后，进行如下步骤的配置:
- 
打开该软件，进入配置界面，软件初始自动打开Session窗口。
- 
在该界面的右半区域的【Host Name（or IP address）】编辑框中输入所要远程登录的服务器地址，这里设定为: 192.168.10.1，端口编辑框中输入默认的端口号22，然后单击【Save】按钮，保存输入配置，如图1所示。 ![](http://www2.ccw.com.cn/05/0535/d/pic/d04_3t1.jpg)
图1 配置IP地址及端口号
- 
单击【Open】按钮，该软件连接服务器，显示连接结果，用户就可以进行相应的远程管理操作了。
**四、配置SSH的自动登录 **
在上面介绍的SSH的使用过程中，用户每次登录服务器都需要输入密码，这对于用户来说未免有些麻烦。由于SSH充分使用了密钥机制，那么就可以通过一定的系统配置，而达到一次配置，以后都不用输入密码，方便登录的目的，下面以Windows的客户端为例，说明如何对SSH的自动登录进行配置。 
在Windows下，使用前面介绍的客户端软件putty同样可以方便地实现自动登录，主要是使用putty工具套件自带的puttygen工具，来产生公钥/私钥对来实现，原理与Linux下相同，下面介绍一下配置细节。 
1．打开puttygen工具，准备生成公钥/私钥对，如图2所示，选择生成SSH2 RSA的密钥类型。 
![](http://www2.ccw.com.cn/05/0535/d/pic/d04_3t2.jpg)
图2 PuttygenGenerator主界面 
2．单击【Generate】按钮，则进入公钥/私钥生成界面，用户需要在界面空白处不断地移动鼠标，以保证钥匙生成的随机性能。 
3．成功生成公钥/私钥后，系统提示用户保存公钥/私钥对。单击【Save public key】按钮以及【Save private key】按钮，分别指定路径保存公钥以及私钥。 
4．使用putty连接上服务器后，将公钥文件的内容拷贝到服务器的相应主目录下，用自己的账号登录远程系统，然后执行下面的命令。此时用记事本打开 id_rsa1.pub 文件，选中所有内容，按Ctrl+C复制到剪贴板中，然后在Putty窗口中按Shift+Ins粘贴,再按Ctrl+D键，完成文件的创建。这是完成公钥分发的过程。
- # cd ~   
- 
- # mkdir .ssh   
- 
- # cd .ssh   
- 
- # cat > authorized_keys   
5．再次使用putty连接服务器，则可以自动登录，而不需要用户输入密码。 
通过上述安装、配置及使用，用户可以放心、安全地使用SSH软件在Linux下进行数据传输。SSH软件的功能非常强大，除了本文讲述的内容之外，还有设置“秘密通道”，scp命令的使用等功能，有兴趣的读者可以参看相关资料，以获得更加安全、方便的使用效果。 
**五、通过WinSCP访问linux **
- 
下载一个WinSCP，下载后安装、运行
- 
其实和putty差不多Host name填入主机ip，User name填入用户名, Password填入密码，先点save按钮保存后再login按钮进入即可
**六、ssh配置**
ssh配置文件目录：/etc/ssh/sshd_config
- 
Port　22　　ssh预设的端口，可以重复使用Port定义多个端口
- 
Protocol　2　ssh的协定版本，可以是2或1
- 
ListenAddress　0.0.0.0　ssh监听地址默认为全部监听
- 
PermitRootLogin　on　　　是否允许以root用户登陆，默认是允许yes，
- 
UserLogin　on　　　　　在ssh下是否接受login登陆
**七、用sftp登录服务器**
用sftp登录服务器的命令为：
- # sftp -oPort=22 sshuser@61.145.34.33
- 
oPort　端口号
- 
sshuser@61.145.34.33　用户名@ip
回车后，输入服务器sshuser的密码登录，然后会显示一个sftp>的环境，这个时候，键入help你会看到所有命令的帮助，跟ftp命令行工具非常类似，比如get,put等等。
原帖 http://www.javaeye.com/topic/212750
[](http://blog.csdn.net/zxs9999/article/details/6930736#)[](http://blog.csdn.net/zxs9999/article/details/6930736#)[](http://blog.csdn.net/zxs9999/article/details/6930736#)[](http://blog.csdn.net/zxs9999/article/details/6930736#)[](http://blog.csdn.net/zxs9999/article/details/6930736#)[](http://blog.csdn.net/zxs9999/article/details/6930736#)
- 
顶

