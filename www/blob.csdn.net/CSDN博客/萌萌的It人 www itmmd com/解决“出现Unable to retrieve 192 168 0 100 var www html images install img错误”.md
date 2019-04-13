
# 解决“出现Unable to retrieve 192.168.0.100-var-www-html-images-install.img错误” - 萌萌的It人 www.itmmd.com - CSDN博客


2014年12月06日 13:33:57[Jlins](https://me.csdn.net/dyllove98)阅读数：6737


解决“出现Unable to retrieve 192.168.0.100/var/www/html/images/install.img错误”
分析：我们必须了解这个错误出现在哪个阶段才能正确找到错误原因，出现这个错误时已经是在开机PXE引导顺利从dhcp server中获得了ip，（或者 光盘引导）均正常，而且正常的读取了ks文件，正确的找到了安装源，否则不会提示无法获取install.img文件。一般无法获取某个文件，不是该文件损坏就是网络不通的问题。
解决办法：
1）我们先证明安装源服务器是正常的，我们用其他机器下载install.img文件，使用下面的命令：
wget![spacer.gif](http://linuxzkq.blog.51cto.com/e/u261/themes/default/images/spacer.gif)http://192.168.0.100/var/www/html/images/install.img
若可以正常的下载这个文件，说明安装源服务器（即kickstart服务器）正常。
2）查找install.img文件的前一阶段是配置网卡、网关阶段，检查你的ks文件的配置的网卡部分，是否与安装源的ip不是一个网段，若不是一个网段就会造成网络不通，产生这个错误。这个问题也是困扰了我好几个小时的问题，迷惑性很大。
总结：让我们仔细分析错误产生的过程，因为开机能够正常获取到dhcp的ip并正常找到ks文件，大多数人都会忽略网络问题，而恰恰在获取install.img文件之前，会读取ks文件配置网卡的部分，重新配置了新的ip，若这个ip与安装源服务器不是一个网段，接下来自然就无法retrieve install.img文件了。

