# Samba服务实现树莓派与Windows之间的文件共享 - 小灰笔记 - CSDN博客





2017年11月12日 23:02:14[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：694










       很多时候树莓派更适合做一个后台性质或者嵌入式性质的服务端，在许多方面我们常用的PC要比树莓派好用很多，不管是Mac还是Windows。但是，出色的低功耗可以让树莓派作为全天24h不间断运行的家用服务器。如果专门给做这样服务的树莓派配一套键盘、鼠标和显示器就有点不划算了。

       其实，两种方式可以解决这个困境。一个是远程登录，第二个是文件共享。远程登录的实现很多种，我自己使用的是SSH，后面的操作都是我在putty中完成的。而文件共享使用Samba还是比较合适的，安装配置也很简单。

       接下来做一下Samba服务配置的简单示范。

1， 首先，需要安装samba服务

sudo apt-get install samba samba-common-bin

2， 接着开启samba用户认证服务以及让用户可以读写自己的home目录，这两项都需要修改下面的文件：

/etc/samba/smb.conf

一个是开启用户认证，再一个就是把只读给去掉。两个修改点分别如下：

用户认证：

####### Authentication #######



# "security = user" is always agood idea. This will require a Unix account

# in this server for every user accessingthe server. See

#/usr/share/doc/samba-doc/htmldocs/Samba3-HOWTO/ServerType.html

# in the samba-doc package for details.

security = user

读写权限：

#======================= Share Definitions=======================



[homes]

   comment = Home Directories

  browseable = no



# By default, the home directories areexported read-only. Change the

# next parameter to 'no' if you want to beable to write to them.

  read only = no

       还有一些其他更详细的配置可以在此文件中实现。

3， Windows上访问树莓派的文件

![](https://img-blog.csdn.net/20171112230212940?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       这个跟Windows上的共享文件夹的访问方式一致。



