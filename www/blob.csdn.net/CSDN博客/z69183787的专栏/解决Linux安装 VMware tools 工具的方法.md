# 解决Linux安装 VMware tools 工具的方法 - z69183787的专栏 - CSDN博客
2018年02月07日 17:52:48[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2729
[http://blog.csdn.net/fanaticism1/article/details/8096494](http://blog.csdn.net/fanaticism1/article/details/8096494)
[http://blog.csdn.net/sz_bdqn/article/details/46778629](http://blog.csdn.net/sz_bdqn/article/details/46778629)
Linux安装 VMware tools 工具的方法     
VMware虚拟机中如何安装VMWare-Tools详解好处：可以支持图形界面，可以支持共享文件功能等
1 工具/原料
1）安装过虚拟机软件的计算机
2）linux操作系统
3）虚拟机配置VMware tools文件，
点击工具栏上的【虚拟机】，然后选择【虚拟机设置】，再选择CD/DVD(IDE)硬件，使用ISO映象文件，选择如下目录（我虚拟机是安装在C盘的）
C:\Program Files (x86)\VMware\VMware Workstation\linux.iso
2 方法/步骤
1)以root身份登陆计算机
2)开始安装Vmware
 选择VM-->install VMware Tools
3)输入如下命令
[root@localhost
 ~]# mkdir /mnt/cdrom
输入
 注意空格 
[root@localhost
 ~]#mount /dev/cdrom /mnt/cdrom/
4)
[root@localhost
 ~]# cd /mnt/cdrom/ 
[root@localhost
 cdrom]# ls 显示其下有哪些文件，类似 windows中的dir
5)将文件拷贝至根目录下的tmp这个临时目录下
//拷贝到/tmp下这里如果VMware的版本不同出现的数字也是不同的
 ，不过差不过，至于这串字符你要是怕输错了，可以复制粘贴 右击
[root@localhost
 cdrom]# cp VMwareTools-7.8.4-126130.tar.gz /tmp 
6)进入tmp文件夹
[root@localhost
 cdrom]# cd /tmp/
[root@localhost
 tmp]# tar zxvf VMwareTools-6.5.0-118166.tar.gz  //解压文件
7)安装开始
[root@localhost
 tmp]# cd vmware-tools-distrib
[root@localhost
 tmp]# ls
[root@localhost
 vmware-tools-distrib]# ./vmware-install.pl   //安装开始
8)
最后用“./install.pl”命令来运行该安装程序，然后根据屏幕提示一路回车。到此整个安装过程算是完成了。
或者参考如下：
1安装vmtools for linux: 
启动VM中的linux，
选择vmware workstation程序菜单中VM > install VMware tools... 
执行：
mkdir /mnt/cdrom 
mount -o ro /dev/cdrom /mnt/cdrom （vmtools的安装文件放在vmware虚拟的cdrom中，首先要mount上这个光驱才能找到安装文件） 
cd /mnt/cdrom 
tar -zxvf VMwareTools-5.0.0-12124.i386.tar.gz -C /tmp （把安装文件解压到/tmp） 
cd /tmp/vmware-tools-distrib 
./vmware-install.pl （执行vwware的安装脚本，这个脚本是用perl编写的） 
在这里，安装程序会询问安装文件存放位置和设置分辨率等一系列问题，在大多数情况下，安装默认配置vmware tools就可以正常工作，因此，这里对每一个问题按回车键选择默认配置 
安装完以后，vmware会添加一个vmhgfs的模块到内核中，可以使用lsmod查看
# bash: ./vmware-install.pl: /user/bin/perl: 坏的解释器:没有那个文件或目录
----------------安装VMwere Tools------------------------
bash: ./vmware-install.pl: /usr/bin/perl: bad interpreter: 没有那个文件或目录
解决方法
yum install perl gcc kernel-devel
yum upgrade kernel kernel-devel
如果出现
‍Searching for a valid kernel header path…
The path “” is not valid.
这是因为 kernel-devel版本和相应的kernel版本不一致，可以用uname-r看一下内核版本，再用rpm -q kernel-devel看一下kernel-devel的版本，有可能会出现kernel-devel未找到的错误，这里需要自己安装一下，可以执行
 sudo yum install kernel-devel，这个时候会安装最新的kernel-devel版本，重启一下，如果再出现问题，那么可以执行sudo yum upgrade kernel kernel-devel，把内核和kernel-devel更新到同一个版本，这样应该就不会有问题了。而GCC和PERL的问题提示比较简单。
建议在安装之前还是执行一下安装GCC和PERL，执行发下命令：yum install perl gcc kernel-devel。
