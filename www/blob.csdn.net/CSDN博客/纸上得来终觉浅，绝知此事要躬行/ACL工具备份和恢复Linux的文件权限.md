# ACL工具备份和恢复Linux的文件权限 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年06月24日 18:00:19[boonya](https://me.csdn.net/boonya)阅读数：864








原文地址：[**http://www.36nu.com/post/150.html**](http://www.36nu.com/post/150.html)

**1. 安装ACL工具**


在Debian， Ubuntu，Linux Mint上
$ sudo apt-get install acl
在CentOS，Fedora，RHEL上
$ sudo yum install acl
** 2. 备份当前目录下（包括子目录）所有文件的权限**
[root@linuxprobe tmp]#  ls -l
total 8
-rwxr--r--. 1 root root  0 Mar  3 04:40 install.txt
-rwxr-xr-x. 1 root root  0 Mar  3 04:41 linuxprobe.txt
[root@linuxprobe tmp]# getfacl -R . > permissions.txt
...
此命令将所有文件的ACL信息全部写入名为permissions.txt文件。

**3. 修改某一文件权限，如：修改linuxprobe.txt及install.txt权限**
[root@linuxprobe tmp]# chmod 733 linuxprobe.txt 
[root@linuxprobe tmp]# chmod 573 install.txt 
[root@linuxprobe tmp]# ls -l
total 8
-r-xrwx-wx. 1 root root    0 Mar  3 04:40 install.txt
-rwx-wx-wx. 1 root root    0 Mar  3 04:41 linuxprobe.txt
-rw-r--r--. 1 root root 4361 Mar  3 04:41 permissions.txt
......
**4. 恢复原有权限**

**1 cd到创建permissions.txt时所在的目录2 执行以下命令：**
setfacl --restore=permissions.txt
可以看到linuxprobe.txt及install.txt权限恢复了
[root@linuxprobe tmp]# setfacl --restore=permissions.txt
[root@linuxprobe tmp]# ls -l
total 8
-rwxr--r--. 1 root root    0 Mar  3 04:40 install.txt
-rwxr-xr-x. 1 root root    0 Mar  3 04:41 linuxprobe.txt
-rw-r--r--. 1 root root 4361 Mar  3 04:41 permissions.txt
......


