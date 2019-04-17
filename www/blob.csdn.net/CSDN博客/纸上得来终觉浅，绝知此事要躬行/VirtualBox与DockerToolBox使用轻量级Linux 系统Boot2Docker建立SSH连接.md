# VirtualBox与DockerToolBox使用轻量级Linux 系统Boot2Docker建立SSH连接 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年07月05日 09:40:29[boonya](https://me.csdn.net/boonya)阅读数：1055








VirtualBox采用轻量级Linux系统Boot2Docker通过SSH连接来访问需要做相应的设置（Boot2Docker本身是集成了SSH服务的），所以对于DockerToolBox而言使用Boot2Docker连接SSH的方式是一样的，因为使用的都是boot2docker.iso镜像系统。

# VirtualBox中连接SSH

## 设置SSH访问

方式一：默认端口22


`ssh docker@ip-address`注意：docker用户的密码是tcuser。



方式二：指定端口2022


`ssh -p 2022 docker@host-ip-address`


注意：root用户设置不一定生效，因为root没有密码，如果要重新设置密码可以使用命令:passwd root。


## 验证SSH访问

注：最开始网络采用的是桥接方式，可能因为没有使用docker用户进行SSH吧所以一直没法访问，后来改成Host-Only仅主机模式后成功。

在主机ifconfig获得的IP是192.168.99.100，通过方式一的设置后再Xshell中进行验证成功。

![](https://img-blog.csdn.net/20170705093752400)


至此，SSH访问成功。

# DockerToolBox中连接SSH

受上面的设置和验证启发，应该可以直接使用VirtualBox的SSH设置方法，试了下是可行的：

![](https://img-blog.csdn.net/20170705100350428)





# 参考资料

ssh Client into boot2docker：[https://stackoverflow.com/questions/25858086/ssh-client-into-boot2docker](https://stackoverflow.com/questions/25858086/ssh-client-into-boot2docker)

























