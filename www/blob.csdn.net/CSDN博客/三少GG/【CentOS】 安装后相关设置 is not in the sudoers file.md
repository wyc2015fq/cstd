# 【CentOS】 安装后相关设置 is not in the sudoers file - 三少GG - CSDN博客
2013年04月22日 20:10:05[三少GG](https://me.csdn.net/scut1135)阅读数：1204
个人分类：[Ubuntu/OS X系统](https://blog.csdn.net/scut1135/article/category/680808)
安装参考快盘word文档~
或以下链接参考： [CentOS 6.3安装(详细图解教程)](http://www.startos.com/linux/guide/2012071635559.html)
**虚拟机标配：**
**4G内存， 4核处理器， 64M显存， 桥接网卡**
------------------------------------------------------------------------------------------------------------------------------------------------------------------------
【1】网络设置( For hadoop )
**选择“网络”，修改“连接方式”为“桥接网卡”。 (非Hadoop用NAT方便即可)**
 网络设置：
- 
vi /etc/sysconfig/network-scripts/ifcfg-eth0
- 
修改为
　　　　DEVICE=eth0
　　　　NM_CONTROLLED=yes
**　ONBOOT=yes**
- 
注：ONBOOT的值修改为yes；
- 
- 
service network restart
- 
试试可用否：yum install wget
- 
会看到美丽的安装条＝＝＝＝＝＝>
- 
至此，你可以在CentOS6.4上任意胡来，这里没有交警，没有城管，没有政府，只有program和你 !!!
## [VirtualBox虚拟机网络设置](http://www.cnblogs.com/coltiam/archive/2010/03/26/1696939.html)
VirtualBox是一款SUN出品的非常优秀的虚拟机程序，版本也非常齐全，你可以在官网上找到适合各种操作系统平台的版本。
这里我先给大家大致讲解下VBox的网络配置及应用。
VirtualBox的提供了四种网络接入模式，它们分别是：
1、NAT    网络地址转换模式(NAT,Network Address Translation)
2、Bridged Adapter    桥接模式
3、Internal    内部网络模式
4、Host-only Adapter  主机模式
下面我们分别对这四种网络模式进行分析解释：
第一种 NAT模式
解释：
NAT模式是最简单的实现虚拟机上网的方式，你可以这样理解：Vhost访问网络的所有数据都是由主机提供的，vhost并不真实存在于网络中，主机与网络中的任何机器都不能查看和访问到Vhost的存在。
虚拟机与主机关系：
只能单向访问，虚拟机可以通过网络访问到主机，主机无法通过网络访问到虚拟机。
虚拟机与网络中其他主机的关系：
只能单向访问，**虚拟机可以访问到网络中其他主机，其他主机不能通过网络访问到虚拟机。**
虚拟机与虚拟机之间的关系：
**相互不能访问，虚拟机与虚拟机各自完全独立，相互间无法通过网络访问彼此。IP:10.0.2.15**
网关：10.0.2.2
DNS：10.0.2.3
一台虚拟机的多个网卡可以被设定使用 NAT， 第一个网卡连接了到专用网 10.0.2.0，第二个网卡连接到专用网络 10.0.3.0，等等。默认得到的客户端ip（IP Address）是10.0.2.15，网关（Gateway）是10.0.2.2，域名服务器（DNS）是10.0.2.3，可以手动参考这个进行修改。
NAT方案优缺点：
笔记本已插网线时： 虚拟机可以访问主机，虚拟机可以访问互联网，在做了端口映射后（最后有说明），主机可以访问虚拟机上的服务（如数据库）。
笔记本没插网线时： 主机的“本地连接”有红叉的，虚拟机可以访问主机，虚拟机不可以访问互联网，在做了端口映射后，主机可以访问虚拟机上的服务（如数据库）。
第二种 Bridged Adapter模式
解释：
**网桥模式是我最喜欢的用的一种模式，**同时，模拟度也是相当完美。你可以这样理解，它是通过主机网卡，架设了一条桥，直接连入到网络中了。因此，它使得虚拟机能被分配到一个网络中独立的IP，所有网络功能完全和在网络中的真实机器一样。
虚拟机与主机关系：
可以相互访问，**因为虚拟机在真实网络段中有独立IP，主机与虚拟机处于同一网络段中，彼此可以通过各自IP相互访问。**
虚拟机于网络中其他主机关系：
可以相互访问，同样因为虚拟机在真实网络段中有独立IP，虚拟机与所有网络其他主机处于同一网络段中，彼此可以通过各自IP相互访问。
虚拟机于虚拟机关系：
可以相互访问，原因同上。
IP：一般是DHCP分配的，与主机的“本地连接”的IP 是同一网段的。虚拟机就能与主机互相通信。
笔记本已插网线时：（若网络中有DHCP服务器）主机与虚拟机会通过DHCP分别得到一个IP，这两个IP在同一网段。 主机与虚拟机可以ping通，虚拟机可以上互联网。
笔记本没插网线时：主机与虚拟机不能通信。主机的“本地连接”有红叉，就不能手工指定IP。虚拟机也不能通过DHCP得到IP地址，手工指定IP后，也无法与主机通信，因为主机无IP。
这时主机的VirtualBox Host-Only Network 网卡是有ip的，192.168.56.1。虚拟机就算手工指定了IP 192.168.56.*，也ping不能主机。
第三种 Internal模式
解释：
内网模式，顾名思义就是内部网络模式，**虚拟机与外网完全断开，只实现虚拟机于虚拟机之间的内部网络模式。**
虚拟机与主机关系：
不能相互访问，彼此不属于同一个网络，无法相互访问。
虚拟机与网络中其他主机关系：
不能相互访问，理由同上。
虚拟机与虚拟机关系：
可以相互访问，前提是在设置网络时，两台虚拟机设置同一网络名称。如上配置图中，名称为intnet。
IP: VirtualBox的DHCP服务器会为它分配IP ，一般得到的是192.168.56.101，因为是从101起分的，也可手工指定192.168.56.*。
笔记本已插网线时：虚拟机可以与主机的VirtualBox Host-Only Network 网卡通信
这种方案不受主机本地连接（网卡）是否有红叉的影响。
第四种 Host-only Adapter模式
解释：
主机模式，这是一种比较复杂的模式，**需要有比较扎实的网络基础知识才能玩转。可以说前面几种模式所实现的功能，在这种模式下，通过虚拟机及网卡的设置都可以被实现。**
我们可以理解为Vbox在主机中模拟出一张专供虚拟机使用的网卡，所有虚拟机都是连接到该网卡上的，我们可以通过设置这张网卡来实现上网及其他很多功能，比如（网卡共享、网卡桥接等）。
虚拟机与主机关系
默认不能相互访问，双方不属于同一IP段，host-only网卡默认IP段为192.168.56.X 子网掩码为255.255.255.0，后面的虚拟机被分配到的也都是这个网段。通过网卡共享、网卡桥接等，可以实现虚拟机于主机相互访问。
虚拟机与网络主机关系
默认不能相互访问，原因同上，通过设置，可以实现相互访问。
虚拟机与虚拟机关系
默认可以相互访问，都是同处于一个网段。
虚拟机访问主机 用的是主机的VirtualBox Host-Only Network网卡的IP：192.168.56.1  ，不管主机“本地连接”有无红叉，永远通。
主机访问虚拟机，用是的虚拟机的网卡3的IP： 192.168.56.101  ，不管主机“本地连接”有无红叉，永远通。
虚拟机访问互联网，用的是自己的网卡2， 这时主机要能通过“本地连接”有线上网，（无线网卡不行）
通过对以上几种网络模式的了解，我们就可以灵活运用，模拟组建出我们所想要的任何一种网络环境了。
比如我想模拟出来一个一台主机，监控一个局域网上网情况的网络环境。
首先我开启了两台虚拟机vhost1与vhost2，当然如果硬件允许，我同样可以再增加vhost3、vhost4…
所有的vhost我都设置成internat内网模式，网络名称为intnal，网关为192.168.56.100，意思就是通过 192.168.56.100网卡上网。其中有一台vhost1我设置为双网卡，一张为内网模式（192.168.56.100），一张为网桥模式 （192.168.1.101）。两张网卡设置双网卡共享上网
虚拟机之间为局域网，其中有一台虚拟机vhost1通过与外网相连，所有局域网中的虚拟机又通过vhost1来实现上外网。这样vhost1就可以监控整个虚拟机局域网上网情况了。
先科普一下网络技术：上网从技术上讲，无非是你的网卡(物理层)发出一个包(传输层)到某个服务器，有可能是遥远未知的服务器给你回应了一个包而已，这个包经过哪些地方，在Ubuntu下可以用tracepath诊断，在Windows下可以用traceroute来查看，贴个范例：
zhanggang@T410i:~$ tracepath -n www.rednet.cn
 1:  192.168.1.101                                         0.167ms pmtu 1500
 1:  192.168.1.1                                           5.352ms
 1:  192.168.1.1                                           3.210ms
 2:  192.168.1.1                                          12.254ms pmtu 1480
 2:  no reply
 3:  222.247.31.97                                        90.053ms
 4:  61.187.255.85                                       100.019ms
 5:  61.137.2.169                                        104.067ms
 6:  61.137.0.214                                        118.186ms
 7:  222.246.134.62                                       93.492ms
 8:  222.246.128.246                                      92.185ms
 9:  220.168.249.55                                      123.082ms reached
     Resume: pmtu 1480 hops 9 back 56
可以看见我的IP是192.168.1.101，经过9个中转站，到达了湖南红网这个目的地。
VirtualBox（或者VMware）有四种联网的方式：NAT(地址转换)，Bridged Adapter(桥接)，Internal(我不知道)，Host-only Adapter（只与宿主机通信），不管哪种方式，目的都是一样，让IP包可以互相通讯，虚拟机可以上网。
宿主机：你的真实主机，比如购买电脑的适合，按照的Windows系统或其他操作系统
虚拟机：使用VirtualBox（或者VMware)等虚拟软件虚拟出来的操作系统
## 第一种：NAT方式
网络设置：虚拟机自动获取IP地址，可能是10.0.2.0/24这个网段，有可能没有网关10.0.2.2，如果没有需要自行补充上(DNS可能是10.0.2.3)
优点：最傻瓜，IP通讯都是VirtualBox（或者VMware)的虚拟网卡来处理的，隐藏了技术细节，在尝试其他方式都不能上网的情况下，推荐使用这种方式。
缺点：宿主机不能访问到虚拟机（如通过10.0.2.2不能访问到10.0.2.0/24网络）
## 第二种：Bridge方式
网络设置：虚拟机IP地址和宿主机方式一样，如果宿主机需要手工配置（如192.168.1.102），那么虚拟机也一样需要手工配置，并且网段也要一样（如192.168.1.103），网关和DNS也需要一样
优点：配置IP地址稍显复杂，可以看作一个新的电脑加入网络，比如办公室来了一个新同事
缺点：基本没有，可能会浪费一个内网IP，但通常情况内网IP有的是，如192.168.1.x
【2】
Centos下一般用户切换到根用户root的方法：
直接输**su回车，输入root暗码回车就能够切换到root用户下. **普通用户登录胜利后，提醒符为“$”， root用户登录胜利，提醒符为“#”
(注意有- ，这和su是不同的，在用命令”su”的时候只是切换到root，但没有把root的环境变量传过去，还是当前用户的环境变量，用”su -”命令将环境变量也一起带过去，就象和root登录一样)
【3】问题描述： sudo 提示  is not in the sudoers file
解决方案：
在ubuntu中由于禁用了root用户，默认情况下会把安装系统时建立的用户添加到sudoers中。但在redhat和centos中并没有把任何root用户之外的用户默认的添加到sudoers之中。这样我们在执行sudo 命令时就会出现xxx is not in the sudoers file. This incident will be reported.这样的错误输出。
现在为了安全起见比较提倡使用普通用户做日常操作，而在需要超级用户的时候使用sudo 来做，这样，我们就有必要把一些用户添加到sudoers之中。
其实把用户添加到sudoers之中很简单。
首先利用whereis 命令查找sudoers配置文件的目录（默认会在/etc/sudoers)
[root@localhost xiaofei]#** whereis sudoers**
sudoers: /etc/sudoers /etc/sudoers.bak /usr/share/man/man5/sudoers.5.gz
然后需**要切换到root用户，更改/etc/sudoers的权限**
[root@localhost xiaofei]**# chmod u+w /etc/sudoers**
然后就可以利用vi编辑器来把用户添加到sudoers之中
[root@localhost xiaofei]#** vi /etc/sudoers**
然后找到root    ALL=(ALL)       ALL所在的位置，把所要添加的用户添加到文件之中，
下面是添加完的结果。
## Allow root to run any commands anywhere
root    ALL=(ALL)       ALL
**XXX     ALL=(ALL)       ALL              （这一行是添加的内容，XXX是我的用户名）**
**顺便提一下vi编辑器的用法。刚进入vi编辑器的时候牌命令行模式，这时可以通过方向键来移动光标，找到要编辑的位置之后按下“i”，然后就进入了插入模 式，这时候你可以输入或删除字符。编辑完成之后按“esc”键退出插入模式，进入命令行模式，这时候按“：”可以进入末行模式，输入“wq”保存并退出。**然后需要把sudoers 的写权限去掉（否则系统不允许执行suoders文件）：
[root@localhost xiaofei]# **chmod u-w /etc/sudoers**
至此，在退出root用户之后就可以利用sudo命令来执行超级用户的权限了。
【4】Oracle VM VirtualBox 虚拟机设置全屏与共享
在VirtualBox中安装好客户操作系统(GuestOS)之后，建议安装VirtualBox增强包(VirtualBox GuestAddition)，它的作用是对GuestOS的性能和可用性进行优化(也就是提高GuestOS的性能和提升使用者的使用体验)。
GuestAddition实质上是安装在GuestOS上的一组驱动程序和应用程序。
注意，很多人把下载的增强包双击之后，看到VirtualBox的“全局设定”--“扩展”中已经显示出了增强包的名字就认为已经安装好了，其实那样仅仅是 从增强包安装文件中解出了相关文件并以插件的形式安装到了VirtualBox中而已。真正的GuestAddition是该安装文件内的1个ISO文 件，必须在GuestOS中进行挂载并安装才能发挥作用，尤其是对于重装GuestOS的情况，“我分明装了GuestAddition，在扩展中也能看 到，怎么没用呢？”，我以前也是没搞明白这一点。
VirtualBox GuestAddition的功能：
**1.鼠标集成：** 不需要按下VirtualBox的HostKey(默认为右Ctrl)键，就可以从GuestOS中自动释放鼠标，从而无缝的在Host/Guest OS之间移动鼠标。GuestAddition中提供了一个GuestOS的鼠标驱动，可以与HostOS的鼠标驱动进行通讯，从而实现这一点。
**2.共享文件夹：**可以在Host/Guest OS之间交换文件。通过在Host OS上设置一个文件夹并指定为VirtualBox的共享文件夹，就可以在GuestOS中以网络共享文件夹的形式访问，即使GuestOS没有网络也可以。
**3.更好的视频支持：**VirtualBox的虚拟显卡只提供了最基本的视频特性，而GuestAddition中的显卡驱动提供了超高的分辨率和2D3D视频加速。你可以任意调节虚拟机窗口的大小且GuestOS也会自动适应，就如同在GuestOS中设置了任意的分辨率的一样。
4.无缝窗口：这个功能很强大。未安装GuestAddition时，虚拟机窗口只能是一个独立的窗口，安装GuestAddition并启用无缝窗口模式之后，GuestOS可以跳出虚拟机窗口并嵌入Host OS的桌面，就好像是Host OS的普通应用程序一样工作。
支持的GuestOS有：<Windows>和<运行X Window的Linux、Solaris>。
无缝模式的开关快捷键：HostKey+L
注：VirtualBox默认的HostKey是右ctrl。
5.通用的Host/Guest 通讯通道：这个功能主要跟程序员有关。GuestAddition中的"Guest Properties(客户机属性)"提供了一个通用的基于字符串的机制，使用此机制可以在Host/Guest之间交换数据，从而控制或者监控GuestOS。比如在Host OS上直接控制GuestOS的程序启动、运行、关闭等。
6.时间同步：因为某些原因，Host OS和Guest OS的系统时间可能不同步，虽然VirtualBox本身提供了同步功能，但GuestAddition的时间同步的效率和方式大大优于VirtualBox自身。
**7.共享剪贴板：**也就是可以在Host/Guest OS之间复制粘贴啦。
8.自动登录：可以使GuestOS自动登录，包括使用证书的情况下。
在CentOS 6.3下安装GuestAddition
在安装GuestAddition之间，需要让CentOS为编译生成外置核心模块做好准备。
(原文：Before installing the Guest Additions, you will have to prepare your guest system for building external kernel modules.)
不同的Linux distribution在这步的操作非常不同，这里只说CentOS 6.3的操作，其他操作请自行查阅VirtualBox_UserManual.pdf。
下列操作对CentOS, Red Hat Enterprise Linux and Oracle Enterprise Linux都适用：
建议以root身份运行下列命令。
1.如果你的CentOS 版本早于 6，那么需要在 /etc/grub.conf 中添加一行 divider=10，以将这个参数传递给核心，以减少 idle CPU load。
**2.#yum update **
把系统的所有软件升级到最新版本。
**3.#yum install gcc**
安装编译系统
**4.#yum install kernel-devel**
安装外置核心模块。
**5.#reboot**
 重启以使所有的升级或安装生效。
至此，所有的准备工作就完成了，下面开始安装GuestAddition。
6.在虚拟机窗口的菜单条下选择 "Device"--“Mount CD/DVD-ROM” ，**选择 "CD/DVD-ROM image"**，会弹出 Virtual Media Manager 窗口，然后在资源浏览器中找到 VBoxGuestAdditions.iso 文件(这里假设 Host OS 是 Windows)，VBoxGuestAdditions.iso 通常位于 VirtualBox 的安装目录下。
注意：VirtualBox虚拟机窗口的菜单很多时候是隐藏的，显示的快捷键是HostKey+C。我找这个急出一身汗。
至此，就相当于在 CentOS 虚拟机中插入了 GuestAddition 的光盘。接下来需要挂载该光盘。
7.#mount -t auto /dev/cdrom1 /mnt
挂载光盘。
注意：VirtualBox默认有2个光驱，分别是 /dev/cdrom 和 /dev/cdrom1 。
如果你挂载命令执行后卡了很久，最后出现：you must specify the filesystem type，说明你需要换个设备名。
我就是在这一步卡了很久，尿都憋出来了，后来发现 VirtualBox 有2个光驱，nnd。
8.#cd /mnt
挂载成功后进入挂载目录。
9.#sh ./VBoxLinuxAdditions.run
安装 GuestAddition。安装过程比较久，慢慢等。
安装成功的画面如下：
![](http://www.linuxcast.net/uploads/AskImage/images/264/90540ff0.png)
好了，整个安装完成。最后需要重启以使 GuestAddition 生效。
#reboot
**优质资源：**[虚拟机安装CentOS以及SecureCRT设置【完美无错版】](http://blog.csdn.net/21aspnet/article/details/7629109)
