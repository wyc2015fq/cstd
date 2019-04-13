
# Linux下which、whereis、locate、find 区别 - 阳光岛主 - CSDN博客

2013年07月20日 15:30:29[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：5279


我们经常在linux要查找某个文件或命令，但不知道放在哪里了，可以使用下面的一些命令来搜索。
which      查看可执行文件的位置
whereis    查看文件的位置
locate     配合数据库查看文件位置
find       实际搜寻硬盘查询文件名称
**1、which**
语法：which 可执行文件名称
例如：
[root@redhat ~]\#which passwd
/usr/bin/passwd
which是通过 PATH 环境变量到该路径内查找可执行文件，所以基本的功能是寻找可执行文件
**2、whereis**
语法：whereis [-bmsu] 文件或者目录名称
参数说明：
-b ： 只找二进制文件
-m： 只找在说明文件manual路径下的文件
-s ： 只找source源文件
-u ： 没有说明文档的文件
例如：
[root@redhat ~]\#whereis passwd
passwd: /usr/bin/passwd /etc/passwd /usr/bin/X11/passwd /usr/share/man/man5/passwd.5.gz /usr/share/man/man1/passwd.1.gz /usr/share/man/man1/passwd.1ssl.gz
将和passwd文件相关的文件都查找出来
[root@redhat ~]\#whereis -b passwd
passwd: /usr/bin/passwd /etc/passwd /usr/bin/X11/passwd
只将二进制文件 查找出来
和find相比，whereis查找的速度非常快，这是因为linux系统会将系统内的所有文件都记录在一个数据库文件中，当使用whereis和下面即将介绍的locate时，会从数据库中查找数据，而不是像find命令那样，通过遍历硬盘来查找，效率自然会很高。
但是该数据库文件并不是实时更新，默认情况下时一星期更新一次，因此，我们在用whereis和locate 查找文件时，有时会找到已经被删除的数据，或者刚刚建立文件，却无法查找到，原因就是因为数据库文件没有被更新。
**3、 locate**
语法：locate 文件或者目录名称
例 如：
[root@redhat ~]\# locate passwd
/etc/passwd
/etc/passwd-
/etc/cron.daily/passwd
/etc/init/passwd.conf
/etc/init.d/passwd
/etc/pam.d/chpasswd
/etc/pam.d/passwd
/etc/security/opasswd
…………

**4、 find**
语法：find 路径 参数
参数说明：
时间查找参数：
-atime n :将n*24小时内存取过的的文件列出来
-ctime n :将n*24小时内改变、新增的文件或者目录列出来
-mtime n :将n*24小时内修改过的文件或者目录列出来
-newer file ：把比file还要新的文件列出来
名称查找参数：
-gid n       ：寻找群组ID为n的文件
-group name  ：寻找群组名称为name的文件
-uid n       ：寻找拥有者ID为n的文件
-user name   ：寻找用户者名称为name的文件
-name file   ：寻找文件名为file的文件（可以使用通配符）
例如：
[root@redhat ~]\#find / -name zgz
/home/zgz
/home/zgz/zgz
/home/weblogic/bea/user_projects/domains/zgz
/home/oracle/product/10g/cfgtoollogs/dbca/zgz
/home/oracle/product/10g/cfgtoollogs/emca/zgz
/home/oracle/oradata/zgz
[root@redhat ~]\#find / -name '*zgz*'
/home/zgz
/home/zgz/zgz1
/home/zgz/zgzdirzgz
/home/zgz/zgz
/home/zgz/zgzdir
/home/weblogic/bea/user_projects/domains/zgz
/home/weblogic/bea/user_projects/domains/zgz/zgz.log00006
/home/weblogic/bea/user_projects/domains/zgz/zgz.log00002
/home/weblogic/bea/user_projects/domains/zgz/zgz.log00004
/home/weblogic/bea/user_projects/domains/zgz/zgz.log
/home/weblogic/bea/user_projects/domains/zgz/zgz.log00008
/home/weblogic/bea/user_projects/domains/zgz/zgz.log00005
当我们用whereis和locate无法查找到我们需要的文件时，可以使用find，但是find是在硬盘上遍历查找，因此非常消耗硬盘的资源，而且效率也非常低，因此建议大家优先使用whereis和locate。

**总结：**
which     只能查可执行文件和别名(alias) ，并在PATH变量里面寻找
whereis   只能查二进制文件（含可执行文件）、说明文档，源文件等，从linux文件数据库（/var/lib/slocate/slocate.db 或 /var/lib/mlocate/mlocate.db）寻找，所以有可能找到刚刚删除，或者没有发现新建的文件
locate    在数据库里查找，数据库大至每天更新一次，文件名是部分匹配（见 3 locate passwd 的结果：opasswd）
find      最强大，什么都能查，根据条件查找文件，在硬盘上查找，效率很低


**参考推荐：**
[Linux下which、whereis、locate、find 命令的区别](http://blog.chinaunix.net/uid-20554039-id-3035417.html)
[Linux Shell 常用命令与目录分区的学习总结](http://blog.csdn.net/sunboy_2050/article/details/5726262)
[linux中的find 命令](http://blog.csdn.net/sunboy_2050/article/details/5937794)
[Linux xargs命令](http://blog.csdn.net/sunboy_2050/article/details/7303501)
[linux软链接和硬链接](http://blog.csdn.net/sunboy_2050/article/details/5666887)



