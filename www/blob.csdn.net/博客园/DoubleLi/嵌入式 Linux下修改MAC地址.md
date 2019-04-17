# 嵌入式 Linux下修改MAC地址 - DoubleLi - 博客园






Linux下修改MAC地址


方法一：

1.关闭网卡设备
ifconfig eth0 down
2.修改MAC地址
ifconfig eth0 hw ether MAC地址
3.重启网卡
ifconfig eth0 up

方法二：
以上方法一修改后linux重启后MAC又恢复为原来的，为了下次启动时修改后的MAC仍有效，我们可以修改文件file:/etc/rc.d /rc.sysinit(RedFlag Linux为这个文件，其他版本的linux应该不同)的内容，在该文件末尾加以下内容：
ifconfig eth0 down
ifconfig eth0 hw ether MAC地址
ifconfig eth0 up

方法三：
很简单的，只是在./etc/sysconfig/network-scripts/ifcfg-eth0中加入下面一句话：
MACADDR=00:AA:BB:CC:DD:EE
方法四：
Linux 下如何更改网卡MAC地址 
--------------------------------------------------------------------------------

简单的办法是在/etc/rc.d/rc.sysinit文件中加入那些命令:
ifconfig eth0 down
ifconfig eth0 hw ether xx:xx:xx:xx:xx:xx
ifconfig eth0 up
因为这个脚本运行在network之前,所以,MAC跟IP就是对应的
方法五：
Linux下的MAC地址更改
首先用命令关闭网卡设备。
/sbin/ifconfig eth0 down
然后就可以修改MAC地址了。
/sbin/ifconfig eth0 hw ether xxxxxxxxxxx
（其中xx是您要修改的地址）
最后重新启用网卡
/sbin/ifconfig eth0 up
网卡的MAC地址更改就完成了。


----------------------------------------------------------------------------------
修改/etc/sysconfig/network-scripts/ifcfg-eth0，将HWADDR：xxxxxxxxxxxx,增加MACADDR:XXXXXXXXXXX(伪装的mac地址)










