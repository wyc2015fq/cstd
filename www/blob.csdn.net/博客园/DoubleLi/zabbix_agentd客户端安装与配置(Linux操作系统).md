# zabbix_agentd客户端安装与配置(Linux操作系统) - DoubleLi - 博客园






**标注：**官网下载zabbix安装包(zabbix安装包里包含了zabbix_agentd客户端安装包，我们只选择zabbix_agentd客户端安装)



zbbix官网下载地址： **[http://www.zabbix.com/download](http://www.zabbix.com/download)**



![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170215184814972-657777692.png)











**客户端操作**





**提示：**Linux软件编译安装都需要依赖两个安装包 **gcc gcc-c++ make   ****可使用网络 yum -y install  gcc   gcc-c++   make  在线安装。**







1、 添加用户群组



[root@cloucentos6 ~]# **groupadd zabbix**                                         #创建zaggix用户组

[root@cloucentos6 ~]# **useradd zabbix -g zabbix -s /sbin/nologin**           #创建zabbix用户，并限制zabbix用户登录系统，/sbin/nologin禁用用户不能登录系统







2、解压安装包，编译安装



[root@cloucentos6 ~]# **mkdir /usr/local/src/zabbix  **                           #创建存放zabbix安装的目录

[root@cloucentos6 ~]#** cd /usr/local/src/zabbix/  **

[root@cloucentos6 zabbix]#** ls**

zabbix-3.2.3.tar.gz

[root@cloucentos6 zabbix]#** tar -zxvf zabbix-3.2.3.tar.gz  **                #解压zabbix安装包

[root@cloucentos6 zabbix]#** ls**

zabbix-3.2.3  zabbix-3.2.3.tar.gz

[root@cloucentos6 zabbix]# **cd zabbix-3.2.3**

[root@cloucentos6 zabbix-3.2.3]#** ./configure --prefix=/usr/local/zabbix_agentd --enable-agent**                   #编译zabbix安装包，配置参数只安装zabbix_agentd客户端

[root@cloucentos6 zabbix-3.2.3]# **make install    **                           #安装zabbix







3、修改zabbix_agent.conf配置文件



[root@cloucentos6 etc]#** cd /usr/local/zabbix_agentd/etc/**               #进入zabbix_agentd安装路径

[root@cloucentos6 etc]# **vim zabbix_agentd.conf  **                         #修改zabbix_agentd.conf配置文件



**EnableRemoteCommands=1**                                                        #允许zabbix服务器远程zabbix_agentd执行命令

**LogRemoteCommands=1**                                                            #开启远程执行命令日志

**Server=10.8.9.155  **                                                                   #填写zabbix服务器IP地址

**ServerActive=10.8.9.155  **                                                          #填写zabbix服务器IP地址

**Hostname=cloucentos6.0**                                                           #填写zabbix_agentd客户端计算机名

**:wq          **                                                                                #退出并保存







4、添加zabbix_agentd客户端开机自启动



[root@cloucentos6 ~]#** cd /usr/local/src/zabbix/zabbix-3.2.3  **                                                                     #进入zabbix安装包文件夹

[root@cloucentos6 zabbix-3.2.3]# **cp misc/init.d/fedora/core/zabbix_**agentd** /etc/init.d/zabbix_agentd    **          #复制zabbix_agentd客户端启动文件到系统启动目录

[root@cloucentos6 zabbix-3.2.3]# **chmod 755 /etc/init.d/zabbix_agentd    **                                                   #赋予zabbix_agentd文件执行权限

[root@cloucentos6 zabbix-3.2.3]#** chkconfig zabbix_agentd on**                                                                    #zabbix_agentd添加开机自启动

[root@cloucentos6 zabbix-3.2.3]# **vim /etc/init.d/zabbix_agentd  **                                                                #修改zabbix_agentd启动文件



**BASEDIR=/usr/local/zabbix_agentd/      **                                                                                                    #填写zabbix_agentd安装路径

**:wq**                                                                                                                                                         #退出并保存







5、启动zabbix_agentd客户端



[root@cloucentos6 zabbix-3.2.3]# **/etc/init.d/zabbix_agentd start**

















**服务端操作**







1、创建zabbix_agentd客户端主机。



![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170215185717925-268231657.png)









2、填写zabbix_agentd客户端主机信息。



![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170215185740191-1934447911.png)









3、添加zabbix_agentd客户端主机模板。



![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170215185806019-1027902064.png)









4、查看zabbix_agentd客户端主机是否连接zabbix服务器。



![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170215185834488-1147174665.png)







5、拓扑图添加zabbix_agentd客户端主机。



![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170215185857863-586452912.png)

![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170215185910004-694577100.png)

![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170215185927925-1525403572.png)

![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170215185941316-1536178913.png)

![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170215185954832-679161635.png)

















**特殊操作**





1、 zabbix是普通用户，所以，无权限执行linux系统的shutdown、reboot、poweroff等命令,有两种方法可以解决：





**方法一**：设置linux命令让zabbix普通用户也能执行root所有者相当的权限(shutdown命令比较特殊，普通用户是执行不了，需要使用方法2)



[root@cloucentos6 ~]# **which reboot**                                      #查看reboot命令路径

/sbin/reboot

[root@cloucentos6 ~]# **ls -l /sbin/reboot  **                               #查看reboot所有者

-rwxr-xr-x. 1 root root 16808 11月 12 2010 /sbin/reboot    

[root@cloucentos6 ~]#** chmod 4755 /sbin/reboot**                     #把reboot命令让其它用户执行文件时，具有与所有者相当的权限。

[root@cloucentos6 ~]# **ls -l /sbin/reboot**

-rwsr-xr-x. 1 root root 16808 11月 12 2010 /sbin/reboot











**方法二：**把zabbix用户拉进root用户组里即可。



[root@cloucentos6 ~]#** groups zabbix  **                             #查看zabbix用户所属用户组

zabbix : zabbix

[root@cloucentos6 ~]# **usermod -a -G root zabbix**             #把zabbix用户拉进root用户组，-a不必离开原来的用户组

[root@cloucentos6 ~]# **groups zabbix  **                             #查看zabbix用户所属用户组

zabbix : zabbix root









