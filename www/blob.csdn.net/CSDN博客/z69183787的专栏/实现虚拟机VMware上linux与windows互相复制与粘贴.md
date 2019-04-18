# 实现虚拟机VMware上linux与windows互相复制与粘贴 - z69183787的专栏 - CSDN博客
2018年02月07日 11:22:03[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1388
from:http://blog.csdn.net/u012243115/article/details/40454063
系统环境： win7系统，虚拟机VMwareWorkstation上运行的CentOS系统。
解决方法：只需要在CentOS安装一个vmware-tools的工具。
1.打开虚拟机的菜单“虚拟机”，下拉框中会有一个“安装 VMwareTools”工具的安装选项。
点击之后，在CentOS的桌面下会出现VMwareTools...tar.gz 的文件。路径（/media/VMwareTools）
2.将此文件复制到/tmp文件下进行解压
cd /media/VMware Tools
cp  VMwareTools-9.6.0-1294478.tar.gz   /tmp 
cd  /tmp
tar -xzvf VMwareTools...gz
3.这是会出现解压后的目录。( vmware-tools-distrib目录）。然后执行编译操作
cd vmware-tools-distrib/
./vmware-install.pl
开始进行安装，一路回车就好了。。。
以上命令要在root用户下执行，普通用户不行。
4.安装成功
Enjoy,
--the VMware team
5.重新启动系统就行了。
reboot -f
