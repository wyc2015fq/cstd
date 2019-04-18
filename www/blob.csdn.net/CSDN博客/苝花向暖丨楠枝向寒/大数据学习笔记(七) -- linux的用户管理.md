# 大数据学习笔记(七) -- linux的用户管理 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年09月24日 17:33:18[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：44标签：[创建用户																[创建用户组																[多用户共享资源](https://so.csdn.net/so/search/s.do?q=多用户共享资源&t=blog)](https://so.csdn.net/so/search/s.do?q=创建用户组&t=blog)](https://so.csdn.net/so/search/s.do?q=创建用户&t=blog)
个人分类：[大数据](https://blog.csdn.net/weixin_40247263/article/category/8073779)


**Table of Contents**

[权限管理](#%E6%9D%83%E9%99%90%E7%AE%A1%E7%90%86)

[创建用户](#%E5%88%9B%E5%BB%BA%E7%94%A8%E6%88%B7)

[创建用户组](#%E5%88%9B%E5%BB%BA%E7%94%A8%E6%88%B7%E7%BB%84)

# 权限管理

r 可读，w 可写，x 对于程序来说可执行，文本一般没有x，对于目录来说可进入，所以一般目录都有x权限，否则没什么意义。

**更改权限语法一**

```bash
chmod augo +/- rwx 文件名
```

a 代表所有人，u代表所有者，g代表组，o代表组外其他人。  如果是增加权限就用 + ,去掉权限用 -

**更改权限语法二**

r 4    w 2     x 1

```bash
chmod 755
```

755 就代表，文件所属者权限为7，即 有 r 4 ，有 w 2 ，有 x  1 ，4+2+1 = 7 ， 文件所属组为5，即 有r 4 ,有 x 1，组外其他人为 5 ，同理。

目录一般都是 755。

# 创建用户

通过以下两个命令创建用户以及为用户设置密码 

```bash
[root@node001 home]# useradd zzh02
[root@node001 home]# passwd zzh02
```

 删除用户用如下命令

```bash
[root@node001 /]# userdel groupshare
```

创建完的用户会出现在 home 目录下。

我们可以通过

```bash
[c:\~]$ ssh 用户名@ip地址
```

进行用户的登录。

也可以通过

```bash
su 用户名
```

进行用户切换，通过exit 退回到原用户

```bash
[root@node001 home]# su god
[god@node001 home]$ exit
exit
[root@node001 home]#
```

每个用户在   /home/自己目录    创建文件别的用户是看不到的，但是root用户拥有一切的权限。

# 创建用户组

如果想实现两个用户部分数据的共享，则需要创建一个组，然后将两个用户追加到组中，再将组与共享的资源绑定，最终再更改权限。

创建一个组，组名为groupshare

```bash
[root@node001 /]# groupadd groupshare
```

 删除组用如下命令

```bash
[root@node001 /]# groupdel groupshare
```

查看 用户所属组信息

```bash
[root@node001 /]# id zzh01
uid=503(zzh01) gid=503(zzh01) groups=503(zzh01)
```

可以看出 用户zzh01 目前所属 zzh01这个组。

我们将用户追加到groupshare组中

```bash
[root@node001 /]# usermod -a -G groupshare zzh01
[root@node001 /]# id zzh01
uid=503(zzh01) gid=503(zzh01) groups=503(zzh01),505(groupshare)
```

zzh02同理

```bash
[root@node001 /]# usermod -a -G groupshare zzh02
[root@node001 /]# id zzh02
uid=504(zzh02) gid=504(zzh02) groups=504(zzh02),505(groupshare)
```

将组与共享资源绑定

```bash
[root@node001 /]# ll
total 94
dr-xr-xr-x.  2 root root  4096 Sep 21 03:27 bin
dr-xr-xr-x.  5 root root  1024 Sep 21 03:27 boot
drwxr-xr-x. 18 root root  3700 Sep 21 06:01 dev
drwxr-xr-x. 61 root root  4096 Sep 22 04:51 etc
drwxr-xr-x.  7 root root  4096 Sep 22 04:22 home
dr-xr-xr-x.  8 root root  4096 Sep 21 03:27 lib
dr-xr-xr-x.  9 root root 12288 Sep 21 03:27 lib64
drwx------.  2 root root 16384 Sep 21 03:25 lost+found
drwxr-xr-x.  2 root root  4096 Sep 23  2011 media
dr-xr-xr-x.  7 root root  4096 Nov 29  2013 mnt
drwxr-xr-x.  2 root root  4096 Sep 22 04:53 ooxx
drwxr-xr-x.  2 root root  4096 Sep 21 17:05 opt
dr-xr-xr-x. 88 root root     0 Sep 21 06:00 proc
dr-xr-x---.  2 root root  4096 Sep 22 04:05 root
dr-xr-xr-x.  2 root root 12288 Sep 21 03:27 sbin
drwxr-xr-x.  7 root root     0 Sep 21 06:00 selinux
drwxr-xr-x.  2 root root  4096 Sep 23  2011 srv
drwxr-xr-x. 13 root root     0 Sep 21 06:00 sys
drwxrwxrwt.  3 root root  4096 Sep 22 04:27 tmp
drwxr-xr-x. 13 root root  4096 Sep 21 03:26 usr
drwxr-xr-x. 17 root root  4096 Sep 21 03:26 var
[root@node001 /]# chown :groupshare ooxx
```

```bash
[root@node001 /]# ll
total 94
dr-xr-xr-x.  2 root root        4096 Sep 21 03:27 bin
dr-xr-xr-x.  5 root root        1024 Sep 21 03:27 boot
drwxr-xr-x. 18 root root        3700 Sep 21 06:01 dev
drwxr-xr-x. 61 root root        4096 Sep 22 04:51 etc
drwxr-xr-x.  7 root root        4096 Sep 22 04:22 home
dr-xr-xr-x.  8 root root        4096 Sep 21 03:27 lib
dr-xr-xr-x.  9 root root       12288 Sep 21 03:27 lib64
drwx------.  2 root root       16384 Sep 21 03:25 lost+found
drwxr-xr-x.  2 root root        4096 Sep 23  2011 media
dr-xr-xr-x.  7 root root        4096 Nov 29  2013 mnt
drwxr-xr-x.  2 root groupshare  4096 Sep 22 04:53 ooxx
drwxr-xr-x.  2 root root        4096 Sep 21 17:05 opt
dr-xr-xr-x. 88 root root           0 Sep 21 06:00 proc
dr-xr-x---.  2 root root        4096 Sep 22 04:05 root
dr-xr-xr-x.  2 root root       12288 Sep 21 03:27 sbin
drwxr-xr-x.  7 root root           0 Sep 21 06:00 selinux
drwxr-xr-x.  2 root root        4096 Sep 23  2011 srv
drwxr-xr-x. 13 root root           0 Sep 21 06:00 sys
drwxrwxrwt.  3 root root        4096 Sep 22 04:27 tmp
drwxr-xr-x. 13 root root        4096 Sep 21 03:26 usr
drwxr-xr-x. 17 root root        4096 Sep 21 03:26 var
```

我们可以发现 ooxx 目录的所属组改变了。 如果想改变目录的所属者，只需在上面 命令中的冒号前面 加上所属者的用户名即可

如

```bash
[root@node001 /]# chown zzh01:groupshare ooxx
```

然后就是更改权限了

上面的权限 是   rwx  r-x  r-x   ，我们应给组内所有用户加上写的权限，给其他组用户去掉 读和执行的权限

```bash
[root@node001 /]# chmod g+w  ooxx
[root@node001 /]# chmod o-rx ooxx
```

重新登录 

```bash
[c:\~]$ ssh 用户名@ip地址
```

然后我们就可以进入到 ooxx 目录中了，使用zzh01用户在该目录下创建一个文本 zzh01 ,创建完毕以后会发现 该文件的所属组是zzh01

```bash
[zzh02@node001 ooxx]$ ll
total 4
-rw-rw-r--. 1 zzh01 zzh01 11 Sep 22 05:07 zzh01.txt
```

如果想让zzh02用户能够修改里面的内容，则必须再对该文件所属组做修改

```bash
[zzh01@node001 ooxx]$ chown :groupshare zzh01.txt 
[zzh01@node001 ooxx]$ ll
total 4
-rw-rw-r--. 1 zzh01 groupshare 11 Sep 22 05:07 zzh01.txt
```

测试 ： 在zzh02用户下向 zzh01.txt 写入一行数据，再返回zzh01用户下查看

```bash
[zzh02@node001 ooxx]$ vi zzh01.txt 
[zzh01@node001 ooxx]$ cat zzh01.txt 
from zzh01
from zzh02
```


