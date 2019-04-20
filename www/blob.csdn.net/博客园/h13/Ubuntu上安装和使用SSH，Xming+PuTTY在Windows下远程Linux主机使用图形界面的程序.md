# Ubuntu上安装和使用SSH，Xming+PuTTY在Windows下远程Linux主机使用图形界面的程序 - h13 - 博客园
**自：http://blog.csdn.net/neofung/article/details/6574002**
**Ubuntu上安装和使用SSH **
网上有很多介绍在Ubuntu下开启SSH服务的文章，但大多数介绍的方法测试后都不太理想，均不能实现远程登录到Ubuntu上，最后分析原因是都没有真正开启ssh-server服务。最终成功的方法如下：
　　Ubuntu 下安装 OpenSSH Server 是无比轻松的一件事情，需要的命令只有一条：
　　sudo apt-get install openssh-server
(查看返回的结果，如果没有出错，则用putty、SecureCRT、SSH Secure Shell Client等SSH 客户端软件，输入您服务器的 IP 地址。如果一切正常的话，等一会儿就可以连接上了。并且使用现有的用户名和密码应该就可以登录了。)
　　然后确认sshserver是否启动了：（或用“netstat -tlp ”命令）
　　ps -e | grep ssh
　　如果只有ssh-agent那ssh-server还没有启动，需要/etc/init.d/ssh start，如果看到sshd那说明ssh-server已经启动了。
　　ssh-server配置文件位于/ etc/ssh/sshd_config，在这里可以定义SSH的服务端口，默认端口是22，你可以自己定义成其他端口号，如222。然后重启SSH服务：
　　sudo /etc/init.d/ssh resart
　　事实上如果没什么特别需求，到这里 OpenSSH Server 就算安装好了。但是进一步设置一下，可以让 OpenSSH 登录时间更短，并且更加安全。这一切都是通过修改 openssh 的配置文件 sshd_config 实现的。
　　首先，您刚才实验远程登录的时候可能会发现，在输入完用户名后需要等很长一段时间才会提示输入密码。其实这是由于 sshd 需要反查客户端的 dns 信息导致的。我们可以通过禁用这个特性来大幅提高登录的速度。首先，打开 sshd_config 文件：
　　sudo nano /etc/ssh/sshd_config
　　找到 GSSAPI options 这一节，将下面两行注释掉：
　　#GSSAPIAuthentication yes #GSSAPIDelegateCredentials no然后重新启动 ssh 服务即可：
　　sudo /etc/init.d/ssh restart
　　再登录试试，应该非常快了吧
　　利用 PuTTy 通过证书认证登录服务器
　　SSH 服务中，所有的内容都是加密传输的，安全性基本有保证。但是如果能使用证书认证的话，安全性将会更上一层楼，而且经过一定的设置，还能实现证书认证自动登录的效果。
　　首先修改 sshd_config 文件，开启证书认证选项：
　　RSAAuthentication yes PubkeyAuthentication yes AuthorizedKeysFile %h/.ssh/authorized_keys修改完成后重新启动 ssh 服务。
　　下一步我们需要为 SSH 用户建立私钥和公钥。首先要登录到需要建立密钥的账户下，这里注意退出 root 用户，需要的话用 su 命令切换到其它用户下。然后运行：
　　ssh-keygen
　　这里，我们将生成的 key 存放在默认目录下即可。建立的过程中会提示输入 passphrase，这相当于给证书加个密码，也是提高安全性的措施，这样即使证书不小心被人拷走也不怕了。当然如果这个留空的话，后面即可实现 PuTTy 通过证书认证的自动登录。
　　ssh-keygen 命令会生成两个密钥，首先我们需要将公钥改名留在服务器上：
　　cd ~/.ssh mv id_rsa.pub authorized_keys然后将私钥 id_rsa 从服务器上复制出来，并删除掉服务器上的 id_rsa 文件。
　　服务器上的设置就做完了，下面的步骤需要在客户端电脑上来做。首先，我们需要将 id_rsa 文件转化为 PuTTy 支持的格式。这里我们需要利用 PuTTyGEN 这个工具：
　　点击 PuTTyGen 界面中的 Load 按钮，选择 id_rsa 文件，输入 passphrase（如果有的话），然后再点击 Save PrivateKey 按钮，这样 PuTTy 接受的私钥就做好了。
　 　打开 PuTTy，在 Session 中输入服务器的 IP 地址，在 Connection->SSH->Auth 下点击 Browse 按钮，选择刚才生成好的私钥。然后回到 Connection 选项，在 Auto-login username 中输入证书所属的用户名。回到 Session 选项卡，输入个名字点 Save 保存下这个 Session。点击底部的 Open 应该就可以通过证书认证登录到服务器了。如果有 passphrase 的话，登录过程中会要求输入 passphrase，否则将会直接登录到服务器上，非常的方便。
**Xming+PuTTY在Windows下远程Linux主机使用图形界面的程序**
一、原理 
Linux/Unix的X Window具有网络透明性。X Window系统里有一个统一的Server来负责各个程序与显示器、键盘和鼠标等输入输出设备的交互，每个有GUI的应用程序都通过网络协议与 Server进行交互。所以对于任何一个应用程序，本地运行和远程运行的差别仅仅是X Server的地址不同，别的没有差别。所以在Windows运行一个X Server，就可以很方便的远程运行有GUI的Linux应用了。同时，OpenSSH具有X转发功能，可以将Linux主机的X程序通过SSH的管道 转发给客户端。于是，通过PuTTY到主机，再将接收到的由主机转发来的X程序的交互交给在本地Windows系统下运行的X Server来管理和显示，原则上说，这就可以实现远程使用Linux的GUI程序。 
主机上需要的配置： 
#vi /etc/ssh/sshd_config 
取消这一行的注释——如果没有这一行则手动添加之： 
X11Forwrding yes 
这样配置的作用就是允许SSH的X转发。其实本质上说，这样做是有一定安全风险的，SSH并不能绝对的保证你数据的安全性，不过，一般情况下，这也够了。 
二、安装Xming 
需要在本地Windows上运行一个X Server。选择有很多：Cygwin下的X Server恐怕是最著名的，但是为了这么一个小小的事情去安装一个“相对巨大”的Cygwin，似乎有些夸张；除了Cygwin，还有两个著名的商业软 件可以选用，Xwin32和Xmanager，上学期IC实验用的就是Xmanager，感觉很流畅很好用，不过，虽然Xmanager可以试用，但是终 究还是商业软件；还有一个选择，就是这里要讲的，开源软件Xming。 
Xming是一个配置简单而功能强大的开源X Server，可以运行在M$的XP/2003/Vista操作系统下。 
Xming的主页：http://www.straightrunning.com/XmingNotes/ 
Xming的SourceForge页面：http://sourceforge.net/projects/xming/ 
从SourceForge下载Xming-setup.exe（最新的版本是6.9.0.31），在本地安装。 
第一次运行时使用开始菜单里面的XLaunch来启动，产生一个初始的配置文件。对于简单的使用来说，不需要任何特殊的配置，一切使用默认即可。Xming的具体配置和使用可以参考Xming的Manual。 
需要记住的是下图中标示出的“Display number”中的数字，此处使用默认的0 
![](http://wly719.iteye.com/upload/attachment/130102/6eca95b7-bf19-3181-b510-8558e2090cfb.png)
启动完成后，在托盘区会出现一个“X”形状的图标，这表示Xming已经在运行了，将鼠标悬停上去，能看到当前使用的“Display number”。 
三、配置PuTTY 
Windows下有很多支持SSH/Telnet等远程登录协议的客户端，比如SecureCRT、XShell、PuTTY等，甚至大家用来登录BBS灌水用的FTerm都带有SSH的功能，只不过不如PuTTY这样的软件专业和强大罢了。 
我用的是著名的免费的PuTTY，相信很多人的选择也跟我一样。因此，就不再赘述PuTTY的配置和使用了。这里要说的，是添加PuTTY对X转 发的支持。运行PuTTY，在配置界面，选择“Connection-SSh-X11”，选中“Enable X11forwarding”，“X display location”中填上上一步中“Display number”中的数字（此处就是“localhost:0”）。 
![](http://wly719.iteye.com/upload/attachment/130104/72d7fd49-ae71-3d0f-816f-3e523b29c3f1.png)
然后连接登录远程主机，看上去一切正常的样子。 
四、问题及解决之道 
于是试着运行一下vbox 
![](http://wly719.iteye.com/upload/attachment/130109/7bfcaae3-7f5b-3ffd-8350-027be3775a6e.jpg)
