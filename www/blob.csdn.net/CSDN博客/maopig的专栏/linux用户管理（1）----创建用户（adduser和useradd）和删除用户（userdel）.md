# linux用户管理（1）----创建用户（adduser和useradd）和删除用户（userdel） - maopig的专栏 - CSDN博客
2017年07月15日 16:21:50[maopig](https://me.csdn.net/maopig)阅读数：1509
 arm linux的系统用户管理：
1、删除root用户：deluser root
2、删除tt用户:deluser tt
3、建立root用户：adduser root
4、修改用户密码：登录相应的用户后，用passwd来修改密码
4、linux用户和密码的管理（ftp，系统用户等等）主要在:/etc/目录下面的四个目录：
        group，   group-，passwd ，    passwd-
5、建立好用户名后和密码，可以将上面的四个目录拷贝在其他单板上的etc目录，替换掉原来的四个
  文件，被拷贝的单板就有了这个里面包含的用户名和目录
