# 利用scp传输文件小结 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年09月18日 15:46:18[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：210
个人分类：[未分类](https://blog.csdn.net/elesos/article/category/6361263)
## 从本地复制到远程
**scp** mysql-5.5.29-linux2.6-x86_64.tar.gz 192.168.1.11:**/**opt
指定端口：
**scp **-P 60022 **/**opt**/**ray**/**nginx-1.5.3.tar.gz 192.168.160.44:**/**opt**/**ray**/**
如果报错“bash: scp: **command** not found”的解决方法：
**yum**-y **install** openssh-clients
## 从远程复制到本地
**scp** root**@**112.126.72.250:**/**root**/**db.tar.bz2 **/**root**/**
如果不想每次输入密码，请参见[SSH无密码登录配置小结](http://192.168.1.100/elesos_com/index.php?title=SSH%E6%97%A0%E5%AF%86%E7%A0%81%E7%99%BB%E5%BD%95%E9%85%8D%E7%BD%AE%E5%B0%8F%E7%BB%93)
拷贝文件夹多加上一个-r 参数即可
Permission denied, please try again.  （未解决）
~~要修改root的ssh权限，即修改 /etc/ssh/sshd_config文件中~~
~~PermitRootLogin no 改为 PermitRootLogin yes~~
~~vim /etc/ssh/sshd_config  添加一行。~~
~~AllowUsers  chenzhonghua~~
~~重启ssh服务~~
~~/etc/init.d/sshd   restart ~~
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E5%88%A9%E7%94%A8scp%E4%BC%A0%E8%BE%93%E6%96%87%E4%BB%B6%E5%B0%8F%E7%BB%93&action=edit&section=3)]艺搜参考
[http://os.51cto.com/art/201003/187301.htm](http://os.51cto.com/art/201003/187301.htm)
