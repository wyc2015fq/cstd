# redhat Linux 虚拟机增加共享目录 - 战斗蜗牛的专栏 - CSDN博客





2014年07月26日 07:26:51[vbskj](https://me.csdn.net/vbskj)阅读数：747
个人分类：[Linux开发](https://blog.csdn.net/vbskj/article/category/643637)









**1.安装vmtools for linux**

选择linux虚拟机 -- 右键 -- install VMware tools... 

选择linux虚拟机 -- 右键 – settings  >removable Devices >CD-ROM >Edit >USE ISO Image (选择vmware安装目录中linux.iso) 

启动 guest 电脑中的linux，执行： 

mkdir /mnt/cdrom 

mount /dev/cdrom /mnt/cdrom （vmtools的安装文件放在vmware虚拟的cdrom中，首先要mount上这个光驱才能找到安装文件） 

cd /mnt/cdrom 

tar -zxvf VMwareTools-5.5.3-34685.tar.gz
 /tmp （先拷贝到/tmp目录，然后再解压； 把安装文件解压到/tmp） 

cd /tmp/vmware-tools-distrib 

./vmware-install.pl 

（执行vwware的安装脚本，这个脚本是用perl编写的） 

在这里，安装程序会询问安装文件存放位置和设置分辨率等一系列问题，在大多数情况下，安装默认配置vmware tools就可以正常工作，因此，这里对每一个问题按回车键选择默认配置。 

安装完以后，vmware会添加一个vmhgfs的模块到内核中，可以使用lsmod查看

**2.设置host computer共享的目录**

在host computer中切换到windows桌面，选择某个虚拟机 -- 右键 – settings  >Options>Shared Folders 

点击对话框右下的“add”按钮，点击“下一步” 

在文本框“name”中输入共享目录的名字（这里填写的目录名以后在ghost computer的linux系统中将显示出同样的目录名），假设为share_file；在“host folder”中填入host computer中windows系统想要共享出来的目录，假设为D:\share 然后点击完成 

下一个对话框是选择共享的方式:Enable this share是指这个共享长期有效，目录可读写；Read-only方式是指这个共享长期有效，目录只读；Disable after this session方式是指下次ghost computer被关闭或挂起后，共享将会失效。一般情况下选择Enable this share然后点击“完成” 

自此，shared folder设置完毕

**3.shared folder目录的使用**

切换到host computer的linux系统，执行： 

cd /mnt/hgfs 

ls 

可以看到share_file的目录，并且可以用cp等指令实现windows到linux，linux到windows的读写操作了


提示：如果你共享的文件夹建立在windows主机的桌面，可能在linux终端运行cd /mnt/hgfs    ls时，什么也没看到，是空白的，这时你把路径改在D盘或其他盘的根目录就行啦，注意文件夹最好不要有空格等特殊字符出现

**4.注意事项**

在vmware软件里添加一个shared folder或对shared folder的参数进行版本是立刻生效的，也就是说立刻可以在ghost computer的linux系统中看到新加的共享目录和体验到参数的改变 

结语：写这篇文章的目的是为了减少CU linux版上关于vmware host/ghost computer共享文件问题的新手重复发问和其他兄弟的重复回答，使版面更加简介些。另外，我看大家回答这个问题大多是使用ftp，samba甚至是nfs（在windows上装microsoft service for unix就可以实现nfs）来实现的，采用vmware自带的shared folder方式比使用网络协议实现文件传输要方便得多，并且对于新手来说，用cp命令拷贝文件比用ftp和smbmount命令要简单得多


虚拟linux共享设置正确后，运行cd /mnt/hgfs ls后是空白的什么也没有怎么回事？


我的主机是windows xp，在vmware6.0中装的linux，装好了vmware tools， 

并VM>Settings>Options>Shared Folders 中添加了在windows桌面上的share_[RedHat](http://www.linuxidc.com/topicnews.aspx?tid=10),总是启用， 

但是在linux终端运行cd /mnt/hgfs ls时什么也没出现，空白的，是怎么回事？


答：有可能是路径的关系，你把共享目录建立在D盘根目录，目录不要有汉字，空格等特殊字符，试试看








除此之外，使用玩vmware-configure-tools.pl 重新进行配置，之后shutdown，重启即可。



