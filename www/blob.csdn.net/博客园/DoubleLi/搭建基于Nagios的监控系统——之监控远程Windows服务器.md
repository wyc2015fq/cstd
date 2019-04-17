# 搭建基于Nagios的监控系统——之监控远程Windows服务器 - DoubleLi - 博客园






分享了如何监控Linux服务器，我们来看看使用Nagios如何监控Windows服务器。



**第一部分：配置被监控的Windows服务器**



首先，访问 http://sourceforge.net/projects/nscplus/，下载符合自己服务器的 NSClinent++。我下载下来以后的文件名为：NSCP-0.4.1.73-Win32.msi

双击安装程序，选择 Complete Install，并且选择生成实例配置文件，其他的就一路下一步就OK了。

其次，在 NSClient++ 的安装目录打开 nsclient.ini，将监控服务器的IP地址加到 allowed hosts 指令后面。并且注意 Password 指令后面的密码，默认是 12489，监控服务器连接这台Windows服务器的时候要用到。

OK，最后从开始菜单中启动 NSClient++ 既可。



**第二部分：配置监控服务器**



第一步：跟增加Linux服务器一样，也创建相应的配置文件，将 HOST 和 SERVICE 配置加到其中。我们可以复制 Nagios 自带的 windows.cfg，然后把IP地址，Host Name换成自己相应的就可以了。

第二步：在 /usr/local/nagios/etc/nagios.cfg 中增加一条 cfg_file 指令，将新创建的配置文件添加到指令后面；

第三步：修改 /usr/local/nagios/etc/objects/command.cfg，在 check_nt 指令后面增加 -s 'Password' ，将刚才那个密码填到这里。

第四步：重启 Nagios 即可。









