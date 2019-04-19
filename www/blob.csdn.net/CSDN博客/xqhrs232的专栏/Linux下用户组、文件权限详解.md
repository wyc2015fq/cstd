# Linux下用户组、文件权限详解 - xqhrs232的专栏 - CSDN博客
2017年03月08日 17:38:55[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：299
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://www.cnblogs.com/123-/p/4189072.html](http://www.cnblogs.com/123-/p/4189072.html)
相关文章
1、[linux查看及修改文件权限以及相关](http://www.cnblogs.com/wenqiangwu/p/3317030.html)----[http://www.cnblogs.com/wenqiangwu/archive/2013/09/12/3317030.html](http://www.cnblogs.com/wenqiangwu/archive/2013/09/12/3317030.html)
2、[修改linux文件权限命令：chmod](http://www.cnblogs.com/avril/archive/2010/03/23/1692809.html)----[http://www.cnblogs.com/avril/archive/2010/03/23/1692809.html](http://www.cnblogs.com/avril/archive/2010/03/23/1692809.html)
## 用户组
在linux中的每个用户必须属于一个组，不能独立于组外。在linux中每个文件有所有者、所在组、其它组的概念
- 所有者
- 所在组
- 其它组
- 改变用户所在的组
所有者
一般为文件的创建者，谁创建了该文件，就天然的成为该文件的所有者
用ls ‐ahl命令可以看到文件的所有者
也可以使用chown 用户名 文件名来修改文件的所有者
文件所在组
当某个用户创建了一个文件后，这个文件的所在组就是该用户所在的组
用ls ‐ahl命令可以看到文件的所有组
也可以使用chgrp 组名 文件名来修改文件所在的组
其它组
除开文件的所有者和所在组的用户外，系统的其它用户都是文件的其它组
## 文件权限
ls
 -l中显示的内容如下：
-rwxrw-r‐-1 root root 1213 Feb 2 09:39 abc
- 10个字符确定不同用户能对文件干什么
- 第一个字符代表文件（-）、目录（d），链接（l）
- 其余字符每3个一组（rwx），读（r）、写（w）、执行（x）
- 第一组rwx：文件所有者的权限是读、写和执行
- 第二组rw-：与文件所有者同一组的用户的权限是读、写但不能执行
- 第三组r--：不与文件所有者同组的其他用户的权限是读不能写和执行
也可用数字表示为：r=4，w=2，x=1  因此rwx=4+2+1=7
- 1 表示连接的文件数
- root 表示用户
- root表示用户所在的组
- 1213 表示文件大小（字节）
- Feb 2 09:39 表示最后修改日期
- abc 表示文件名
改变权限的命令
chmod 改变文件或目录的权限
chmod 755 abc：赋予abc权限rwxr-xr-x
chmod u=rwx，g=rx，o=rx abc：同上u=用户权限，g=组权限，o=不同组其他用户权限
chmod u-x，g+w abc：给abc去除用户执行的权限，增加组写的权限
chmod a+r abc：给所有用户添加读的权限
![](http://images.cnitblog.com/blog/412924/201412/271932597804763.png)
![](http://images.cnitblog.com/blog/412924/201412/271933217021233.png)
改变所有者（chown）和用户组（chgrp）命令
chown xiaoming abc：改变abc的所有者为xiaoming
chgrp root abc：改变abc所属的组为root
chown root ./abc：改变abc这个目录的所有者是root
chown ‐R root ./abc：改变abc这个目录及其下面所有的文件和目录的所有者是root
改变用户所在组
在添加用户时，可以指定将该用户添加到哪个组中，同样用root的管理权限可以改变某个用户所在的组
- usermod ‐g 组名 用户名
你可以用
- usermod ‐d 目录名 用户名，改变该用户登录的初始目录
【综合案例】
【题1.1】建立两个用户组group1和group2，以及三个用户dennis、daniel、abigale，并且将前2个用户分配在group1用户组下，后一个分配在group2用户组下
![](http://images.cnitblog.com/blog/412924/201412/271935472029181.png)
【题1.2】以dennis用户登录，创建一个Hello.java文件
【题1.3】以daniel用户登录，观察是否可以访问/home/dennis目录以及读或写其创建的Hello.java文件
【题1.4】以dennis用户登录，修改目录/home/dennis及Hello.java文件的读写权限（更正：修改目录权限的时候，应该使用770，而不是760，否则权限不足）
![](http://images.cnitblog.com/blog/412924/201412/271936207334170.png)
【题1.5】重复【题1.3】
【题1.6】改变abigale的用户组由group2变为group1
![](http://images.cnitblog.com/blog/412924/201412/271936542336402.png)
然后，可以使用cat /etc/passwd查看并确定
![](http://images.cnitblog.com/blog/412924/201412/271937123586032.png)
【参考】
- groupadd 组名，在linux中添加组
- vi /etc/group，查看linux中所有组信息，可以看可以编辑
- cat /etc/group，查看linux中所有组信息，只可以看不可以编辑
- useradd ‐g 组名 用户名，创建用户的同时指定将该用户分配到哪个组下
- vi /etc/passwd，查看linux中所有用户信息，可以看可以编辑
- cat /etc/passwd，查看linux中所有用户信息，只可以看不可以编辑
既然绝非庸才，为何甘于平凡？
分类: [Linux](http://www.cnblogs.com/123-/category/644206.html)
