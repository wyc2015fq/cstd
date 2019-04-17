# 搭建基于Nagios的监控系统——之监控远程Linux服务器 - DoubleLi - 博客园






上一篇介绍了如何安装Nagios Core，这一篇跟大家分享一下如何将一台远程的Linux服务器加入纳入监控范围。



**第一部分：在远程Linux上安装Nagios Plugins和NRPE**



**第一步：下载Nagios Plugins和NREP扩展的安装程序**







**[plain]**[view plain](http://blog.csdn.net/topasstem8/article/details/20689987#)[copy](http://blog.csdn.net/topasstem8/article/details/20689987#)



- cd Downloads  
- wget http://nagios-plugins.org/download/nagios-plugins-2.0.tar.gz  


访问 http://www.nagios.org/download/addons/，点击 NRPE，在弹出的页面中点击 Download URL 就可以下载了。



**第二步：创建用户**







**[plain]**[view plain](http://blog.csdn.net/topasstem8/article/details/20689987#)[copy](http://blog.csdn.net/topasstem8/article/details/20689987#)



- /usr/sbin/useradd nagios  




**第三步：安装Nagios Plugins**





**[plain]**[view plain](http://blog.csdn.net/topasstem8/article/details/20689987#)[copy](http://blog.csdn.net/topasstem8/article/details/20689987#)



- tar xf nagios-plugins-2.0.tar.gz  
- cd nagios-plugins-2.0  
- export LDFLAGS=-ldl  
- sudo -s  



./configure --with-nagios-user=nagios --with-nagios-group=nagios --enable-redhat-pthread-workaroundmake && make install
chown nagios.nagios /usr/local/nagios<chown -R nagios.nagios /usr/local/nagios/libexec/





**第四步：安装NRPE**







**[plain]**[view plain](http://blog.csdn.net/topasstem8/article/details/20689987#)[copy](http://blog.csdn.net/topasstem8/article/details/20689987#)



- tar xf nrpe-2.15.tar.gz  
- cd nrpe-2.15  
- ./configure  
- make all  
- 
- make install-plugins  
- 
- make install-daemon  
- 
- make install-daemon-config  
- 
- make install-xinetd   




**第五步：将NRPE设置为守护进程（通过作为xinetd的一部分）**


- 打开 /etc/xinetd.d/nrpe 文件，将负责监控的那台服务器的IP地址，添加到 only_from 指令后面。多个IP地址之间要用空格隔开；
- 编辑 /usr/local/nagios/etc/nrpe.cfg ，在 allowd_hosts 指令后加上监控服务器的IP地址，多了地址用逗号隔开；
- 修改 /etc/services 文件，在文件末尾添加一行：



**[plain]**[view plain](http://blog.csdn.net/topasstem8/article/details/20689987#)[copy](http://blog.csdn.net/topasstem8/article/details/20689987#)



- nrpe 5666/tcp # NRPE  


- 启动服务



**[plain]**[view plain](http://blog.csdn.net/topasstem8/article/details/20689987#)[copy](http://blog.csdn.net/topasstem8/article/details/20689987#)



- service xinetd restart  


- 确认现在NRPE端口是否在监听



**[plain]**[view plain](http://blog.csdn.net/topasstem8/article/details/20689987#)[copy](http://blog.csdn.net/topasstem8/article/details/20689987#)



- netstat -at | grep nrpe  
- tcp 0      0 *:nrpe *:*                         LISTEN  


- 确认NRPE是否正常



**[plain]**[view plain](http://blog.csdn.net/topasstem8/article/details/20689987#)[copy](http://blog.csdn.net/topasstem8/article/details/20689987#)



- /usr/local/nagios/libexec/check_nrpe -H localhost  





**第六步：修改 /usr/local/nagios/etc/nrpe.cfg **



这个文件中定义的命令决定了该服务器能被监控的内容，也就是说，真正的监控是通过这个文件里的命令指定的程序来执行的。比如：
command[check_users]=/usr/local/nagios/libexec/check_users -w 5 -c 10
command[check_load]=/usr/local/nagios/libexec/check_load -w 15,10,5 -c 30,25,20
在添加到这个文件之前，最好在命令行中实际执行一下这个命令：
/usr/local/nagios/libexec/check_disk -w 20% -c 10% -p /dev/sda1




**第二部分：在监控服务器上增加远程服务器的配置**



**第一步：下载 NRPE 扩展，并且安装**







**[plain]**[view plain](http://blog.csdn.net/topasstem8/article/details/20689987#)[copy](http://blog.csdn.net/topasstem8/article/details/20689987#)



- tar xf nrpe-2.15.tar.gz  
- cd nrpe-2.15  
- ./configure  
- make all  
- make install-plugins  


确认一下NRPE是否能链接到远程服务器
/usr/local/nagios/libexec/check_nrpe -H [远程服务器IP]


**第二步：为远程监控服务器添加 HOST 和 Service 配置**





只要增加监控的电脑，就要增加这两部分！

这两种配置可以分成两个文件，也可以放到一个文件里面。我在这里将 /usr/local/nagios/etc/objects/localhost.cfg 复制了一下，并且命名为了 remotehost.cfg。

将 host 块儿中的 host_name 和 alias 改成相应的远程服务器的名字和别名，将 address 改成远程服务器的IP地址。

host_group 的定义就可以删除了，并且将这台远程服务器的名字添加到 localhost.cfg 中 host_group 块儿中，写在 localhost 后面既可，用逗号隔开。

下面所有的 services 定义中，就把 host 那么换一下就可以了。

在 /usr/local/nagios/etc/nagios.cfg 当中，增加一条 cfg_file 指令，将新创建的文件名加入到后面。



**第三步：重启Nagios**







**[plain]**[view plain](http://blog.csdn.net/topasstem8/article/details/20689987#)[copy](http://blog.csdn.net/topasstem8/article/details/20689987#)



- /etc/init.d/nagios start  
- 










