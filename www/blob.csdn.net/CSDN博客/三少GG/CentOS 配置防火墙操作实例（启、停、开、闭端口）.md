# CentOS 配置防火墙操作实例（启、停、开、闭端口） - 三少GG - CSDN博客
2013年08月03日 19:59:10[三少GG](https://me.csdn.net/scut1135)阅读数：852
个人分类：[Web网站设计](https://blog.csdn.net/scut1135/article/category/762263)
CentOS 配置防火墙操作实例（启、停、开、闭端口）：
**注：防火墙的基本操作命令：**
**查询防火墙状态:**
**[root@localhost ~]# service   iptables status<回车>**
**停止防火墙:**
**[root@localhost ~]# service   iptables stop <回车>**
**启动防火墙:**
**[root@localhost ~]# service   iptables start <回车>**
**重启防火墙:**
**[root@localhost ~]# service   iptables restart <回车>**
**永久关闭防火墙:**
**[root@localhost ~]# chkconfig   iptables off<回车>**
**永久关闭后启用:**
**[root@localhost ~]# chkconfig   iptables on<回车>**
1、查看防火墙状态
[root@localhost ~]# service iptables status<回车> 
![](http://hi.csdn.net/attachment/201112/5/0_1323075641bEbn.gif)
2、编辑/etc/sysconfig/iptables文件。我们实例中要打开8080端口和9990端口
用编辑器打开/etc/sysconfig/iptables 
![](http://hi.csdn.net/attachment/201112/5/0_1323075641bEbn.gif)
3、依葫芦画瓢，我们添加8080端口和9990端口
![](http://hi.csdn.net/attachment/201112/5/0_1323075667rVBe.gif)
4、保存/etc/sysconfig/iptables文件，并在终端执行
[root@localhost ~]# service iptables restart <回车>
![](http://hi.csdn.net/attachment/201112/5/0_1323075710katw.gif)
5、从新查看防火墙状态
[root@localhost ~]# service iptables status<回车>
![](http://hi.csdn.net/attachment/201112/5/0_1323075734NnNg.gif)
6、这时候，服务器的8080和9990端口就可以对外提供服务了。
7、其他端口的开放模式就是类似如此开放模式。
