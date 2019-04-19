# ubuntu 没卸载 直接重装导致不能开机|| 电信用户用ubuntu系统ADSL（pppoe）拨号上网 - 三少GG - CSDN博客
2012年01月13日 00:48:35[三少GG](https://me.csdn.net/scut1135)阅读数：1357
难道是没有按之前的博文  所说干净的卸载ubuntu11.10，而是直接光盘覆盖安装导致的黑屏没有引导？。。。是不是破坏又没更新相关的mbr或者grub信息？哎。。。下次要干净的卸载后，再重装新的ubuntu版本。！！！！！！！ 2012.1.12 @home解决办法：**boot分区 ---> 分区需要选择Primary!(重装就好了。。。)**“你的boot分区是扩展分区中的逻辑分区，还是主分区？如果是逻辑分区，就会出现问题。如果是主分区则可以安装。一个硬盘可以分成4个主分区，其中一个可以作为扩展分区，扩展分区可以再划分为若干逻辑分区。你分成几个分区？从你的问题业看，你至少分成了3个分区，1个boot，一个UBUNTU，一个WIN7。假如你的分区是这位的：/dev/sda1-->boot,/dev/sda2-->ubuntu,/dev/sda3->WIN7，都是主分区，则可以安装。如果在扩展分区中分一个逻辑分区作为boot分区，就会出问题。一般是将第一个主分区作为boot分区（即挂载为/boot 目录），另外两个主分区可以分别安装ubuntu和WIN7。如果主分区不够，可以将ubuntu安装在逻辑分区上。这时MBR可以保留。如果你安装11.10时可以安装，则可以保留10.10的grub,在安装时可以选择不安装GRUB，安装完成后手修改grub.cfg。启动进入11.04后，可以运行grub-install重新安装grub。你也可以用11。04的LIVECD，先将GRUB安装到boot分区，再安装11.04。安装时选择不安装GRUB，第一次启动时，用手动启动，进入11.04，然后运行update-grub，建立启动选项。[http://zhidao.baidu.com/question/320403110.html](http://zhidao.baidu.com/question/320403110.html)+++++++++++++++++++++++++++++++++++++++++++++++++++++++3 选择新分区的类型：Primary（主分区）（单独安装Ubuntu到全新磁盘，/分区需要选择Primary，其它分区选择Logical逻辑分区就可 以；如果是安装双系统且已安装Windows的话，/分区的类型选择Primary或者Logical都可以，其它分区选择Logical逻辑分区。） ++++++++以下方法未能成功！+++++++++++[http://wowubuntu.com/fix-grub.html](http://wowubuntu.com/fix-grub.html)
4.输入
> 
sudo mount /dev/sda8 /mnt
因为小狼的单独把boot分区分出来了,并且它的位置在sda8.如果你没有单独把boot分区分出来.那就修改一下位置,改为
> 
sudo mount /dev/你的/分区所在位置 /mnt
5.输入
> 
sudo grub-install --root-directory=/mnt /dev/sda8
同样，如果你没有把boot分区单独分出来就改成
> 
sudo grub-install --root-directory=/mnt /dev/你的/分区的位置
这里应该不会有什么问题，因为如果位置不对，无法安装，你可以一直输这个命令直到位置对了为止.
6.如果出现了no error report。那你就差不多成功了.然后sudo init 6.重启。
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++**永久修改主机名**
**$ sudo vi /etc/hostname  和 hosts 两个文件把新的主机名写入即可。**
当系统重启后，会读出此文件中主机名。++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++**电信用户如何用ubuntu系统ADSL（pppoe）拨号上网****注意：以下方法均未使用：****直接： 网络连接-->DSL  输入宽带用户名和密码即可。****[http://wiki.ubuntu.org.cn/ADSL%EF%BC%88PPPOE%EF%BC%89%E6%8E%A5%E5%85%A5%E6%8C%87%E5%8D%97](http://wiki.ubuntu.org.cn/ADSL%EF%BC%88PPPOE%EF%BC%89%E6%8E%A5%E5%85%A5%E6%8C%87%E5%8D%97)**
### 配置 ADSL
**方法一：使用“网络连接”图形界面配置**
Ubuntu 里面已经自带了网络配置向导，在顶部菜单中进入系统->首选项->Network Configuration->DSL，进行设置，依次填写用户名，服务以及密码。
连接建立后，在桌面左键单击网络连接的图标，会列出所有的连接，其中在“有线网络”下就有刚才建立的ADSL连接，单击即可拨号上网。
**方法二：使用pppoeconf命令拨号**
启用有线连接：
sudo ifconfig eth0 up
在终端中输入:
sudo pppoeconf
一个基于文本菜单的程序会指导你进行下面的步骤：
1. 确认以太网卡已被检测到。
2. 输入你的用户名（由ISP所提供 注意：输入时请先清除输入框中的“username“，否则可能造成验证错误）。
3. 输入你的密码（由ISP所提供）。
4. 如果你已经配置了一个PPPoE的连接，会通知你这个连接将会被修改。
5. 弹出一个选项:你被询问是否需要'noauth'和'defaultroute'选项和去掉'nodetach',这里选择"Yes"。
6. Use peer DNS - 选择 "Yes".
7. Limited MSS problem - 选择 "Yes".
8. 当你被询问是否在需要在进入系统的时候自动连接，你可以选择"Yes"。
9. 最后，你会被询问是否马上建立连接。
在需要的时候启动ADSL连接，可以在终端中输入：
sudo pon dsl-provider
断开ADSL连接，可以在终端中输入：
sudo poff
如果你发现连接正常工作，尝试手动去调整你之前ADSL连接的配置（参考前一节）。
需要查看日志，可以在终端中输入：
plog
获得接口信息，可以在终端中输入：
ifconfig ppp0
使用pppoeconf拨号后，Network Manager显示设备未托管的解决办法：
在终端中输入以下命令，来配置网络连接管理文件：
sudo gedit /etc/NetworkManager/nm-system-settings.conf 打开后，找到 [ifupdown] managed=false 修改成： [ifupdown] managed=true
终端运行sudo gedit /etc/network/interfaces 只保留
auto lo iface lo inet loopback
删除dns设置 sudo mv /etc/resolv.conf /etc/resolv.conf_backup
之后重启 network-manager服务： sudo service network-manager restart
其余参考：1. [http://bbs.pcbeta.com/forum-viewthread-tid-307385-highlight.html](http://bbs.pcbeta.com/forum-viewthread-tid-307385-highlight.html)2. [http://apps.hi.baidu.com/share/detail/12556157](http://apps.hi.baidu.com/share/detail/12556157)
