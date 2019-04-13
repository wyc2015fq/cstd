
# CentOS 6.4安装在物理机上，Atheros AR8161-AR8162网卡驱动问题 - 萌萌的It人 www.itmmd.com - CSDN博客


2014年12月06日 13:31:54[Jlins](https://me.csdn.net/dyllove98)阅读数：1353


被这个问题虐惨了，只为后来者不要再被虐，故写此文。
CentOS 6.4装在在物理机上和Win7形成双系统，但是CentOS却一直没法联网，仔细一看才发现网线接口根本不亮。因为网卡驱动不存在！你执行ifconfig命令也显示不出eth0！
在Windows下个驱动精灵搞定驱动问题极其方便，但是在Linux就要你自己找了。
现在我已经找好了，对于CentOS 6.X 系列的来说，如果你的网卡是Atheros AR8161/AR8162系列，那就下载本博文下方附件（仔细找找，字比较小，找不到给我留言），放到CentOS里，将之解压，进入目录，直接make，完事儿后，再进入src子目录，然后执行insmod alx.ko命令。看看吧，你的网线接口已经闪光了。执行ifconfig命令可以看到eth0了！
再去下个pppoe拨号软件，拨号吧少年。

