# Docker Centos安装Openssh - 零度的博客专栏 - CSDN博客
2017年03月27日 17:37:48[零度anngle](https://me.csdn.net/zmx729618)阅读数：454
环境介绍：
    Docker版本：1.5.0
    镜像：docker.io:centos latest
操作步骤：
    1、启动镜像
```
docker run -ti centos /bin/bash
```
    2、安装passwd
```
yum install passwd -y
```
   3、修改Centos root密码
```
# passwd
Changing password for user root.
New password: 123456
BAD PASSWORD: The password is shorter than 8 characters
Retype new password: 123456
passwd: all authentication tokens updated successfully.
```
   4、安装openssh
```
yum install openssh-server -y
```
   5、生成公钥、私钥
```
[]()
[root@378ab88a06c8 /]# ssh-keygen -t rsa -f /etc/ssh/ssh_host_rsa_key
Generating public/private rsa key pair.
Enter passphrase (empty for no passphrase): (直接回车)
Enter same passphrase again: (直接回车)
Your identification has been saved in /etc/ssh/ssh_host_rsa_key.
Your public key has been saved in /etc/ssh/ssh_host_rsa_key.pub.
The key fingerprint is:
33:3c:34:49:e4:76:7d:45:cc:69:ac:46:85:ab:27:9e root@378ab88a06c8
The key's randomart image is:
+--[ RSA 2048]----+
|       .o     +=+|
|       o . . o =o|
|        * . o =  |
|       + o   =   |
|        S   o    |
|         + o .   |
|          . +    |
|           E     |
|                 |
+-----------------+
[root@378ab88a06c8 /]# ssh-keygen -t rsa -f /etc/ssh/ssh_host_ecdsa_key
Generating public/private rsa key pair.
Enter passphrase (empty for no passphrase): (直接回车)
Enter same passphrase again: (直接回车)
Your identification has been saved in /etc/ssh/ssh_host_ecdsa_key.
Your public key has been saved in /etc/ssh/ssh_host_ecdsa_key.pub.
The key fingerprint is:
09:ac:b0:61:55:de:e8:4f:5e:20:d9:fc:1e:b6:d7:79 root@378ab88a06c8
The key's randomart image is:
+--[ RSA 2048]----+
|    ...          |
|   . o *         |
|  +   B =        |
| . + o o +       |
|  . . . S =      |
|       + + o . . |
|        o o . o E|
|           .   . |
|                 |
+-----------------+
[root@378ab88a06c8 /]# ssh-keygen -t rsa -f /etc/ssh/ssh_host_ed25519_key
Generating public/private rsa key pair.
Enter passphrase (empty for no passphrase): (直接回车)
Enter same passphrase again: (直接回车)
Your identification has been saved in /etc/ssh/ssh_host_ed25519_key.
Your public key has been saved in /etc/ssh/ssh_host_ed25519_key.pub.
The key fingerprint is:
63:0d:b5:fb:55:a4:56:47:43:6d:68:c0:47:2e:84:24 root@378ab88a06c8
The key's randomart image is:
+--[ RSA 2048]----+
|        E.ooooo=*|
|         o.o..++=|
|        . . .o+..|
|         o . o . |
|        S o   .  |
|       . . . .   |
|            .    |
|                 |
|                 |
+-----------------+
[]()
```
   6、编写启动脚本
```
# vi /run.sh
#!/bin/bash
/usr/sbin/sshd -D
# chmod +x /run.sh
```
   7、修改SSH默认端口
```
# vi /etc/ssh/sshd_config 
Port 222
```
   8、退出容器，保存镜像
```
[]()
[root@localhost ~]# docker ps -a
CONTAINER ID        IMAGE               COMMAND             CREATED             STATUS                      PORTS               NAMES
378ab88a06c8        centos:latest       "/bin/bash"         52 minutes ago      Exited (0) 29 seconds ago                       ecstatic_hawking    
[root@localhost ~]# docker commit 378ab88a06c8 sshd:centos
9f400c5422109a3403779f4b055c673fae57d1a8809a84bad7a20fcc3574b3dc
[]()
```
   8、启动容器
```
[root@localhost ~]# docker images
REPOSITORY                 TAG                 IMAGE ID            CREATED             VIRTUAL SIZE
sshd                       centos              9f400c542210        2 minutes ago       271.3 MB
192.168.31.202:5000/test   latest              f6c0d93ac266        3 days ago          315.6 MB
docker.io/centos           latest              fd44297e2ddb        3 weeks ago         215.7 MB
docker.io/busybox          latest              8c2e06607696        4 weeks ago         2.43 MB
```
```
[root@localhost ~]# docker run --net=host -d sshd:centos /run.sh
bbd39bbd7e03ffbd22b88725041793c74c50dbd4316d25bd20c8053265c3f8f1
```
   9、远程连接测试
```
[]()
[root@localhost ~]# ssh 192.168.31.203 -p 222
The authenticity of host '[192.168.31.203]:222 ([192.168.31.203]:222)' can't be established.
RSA key fingerprint is 79:2f:94:52:bb:22:8c:32:f2:2c:0d:82:cb:b2:26:75.
Are you sure you want to continue connecting (yes/no)? yes
Warning: Permanently added '[192.168.31.203]:222' (RSA) to the list of known hosts.
root@192.168.31.203's password: 
Last login: Tue Jun 23 07:23:32 2015 from 192.168.7.46
[root@localhost ~]# ll
total 0
[root@localhost ~]# cd /
[root@localhost /]# ll
total 44
lrwxrwxrwx   1 root root    7 Apr 15 14:28 bin -> usr/bin
drwxr-xr-x   5 root root  360 Jun 23 07:22 dev
drwxr-xr-x  48 root root 4096 Jun 23 07:22 etc
drwxr-xr-x   2 root root 4096 Jun 10  2014 home
lrwxrwxrwx   1 root root    7 Apr 15 14:28 lib -> usr/lib
lrwxrwxrwx   1 root root    9 Apr 15 14:28 lib64 -> usr/lib64
drwx------   2 root root 4096 Apr 15 14:26 lost+found
drwxr-xr-x   2 root root 4096 Jun 10  2014 media
drwxr-xr-x   2 root root 4096 Jun 10  2014 mnt
drwxr-xr-x   2 root root 4096 Jun 10  2014 opt
dr-xr-xr-x 348 root root    0 Jun 23 07:22 proc
dr-xr-x---   2 root root 4096 Jun  4 02:20 root
drwxr-xr-x  11 root root  240 Jun 23 07:22 run
-rwxr-xr-x   1 root root   30 Jun  4 02:19 run.sh
lrwxrwxrwx   1 root root    8 Apr 15 14:28 sbin -> usr/sbin
drwxr-xr-x   2 root root 4096 Jun 10  2014 srv
dr-xr-xr-x  13 root root    0 May 15 07:27 sys
drwxrwxrwt   7 root root  140 Jun 23 07:30 tmp
drwxr-xr-x  13 root root 4096 Apr 15 14:28 usr
drwxr-xr-x  19 root root 4096 Apr 15 14:29 var
[root@localhost /]# 
[]()
```
