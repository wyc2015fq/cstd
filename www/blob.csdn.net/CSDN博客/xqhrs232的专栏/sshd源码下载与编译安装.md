# sshd源码下载与编译安装 - xqhrs232的专栏 - CSDN博客
2017年11月29日 14:27:49[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：200
原文地址::[http://blog.csdn.net/vegebirdfly/article/details/47779921](http://blog.csdn.net/vegebirdfly/article/details/47779921)
相关文章
1、SSH源代码安装与免密码登录----[http://blog.csdn.net/todd911/article/details/38821451](http://blog.csdn.net/todd911/article/details/38821451)
2、[sshd服务---暴力破解应对策略](http://www.cnblogs.com/lcword/p/5917387.html)----[https://www.cnblogs.com/lcword/p/5917387.html](https://www.cnblogs.com/lcword/p/5917387.html)
3、ssh登陆过程分析和openssh的编译、使用方法----[https://wenku.baidu.com/view/42601f8d51e79b896802262f.html](https://wenku.baidu.com/view/42601f8d51e79b896802262f.html)
4、一款短小精致的SSH后门分析----[http://www.freebuf.com/articles/system/140880.html](http://www.freebuf.com/articles/system/140880.html)
1. 到官网http://www.openssh.com/找到对应系统的链接，选择一个镜像源，比如http://mirror.internode.on.net/pub/OpenBSD/OpenSSH/portable/
2.下载最新的openssh-7.0p1.tar.gz包
3.用U盘把tar包拷到目标机上
4. 惯例执行解包，配置，编译，安装
tar -zvxf openssh-7.0p1.tar.gz
./configure
make
##make 后出现一行警告提示"make: warning: Clock skew detected. Your build may be incomplete.", 使用“date -s "2015-08-19 14:00:00"”更新下目标机器的时间即可
##make clean;make
make install
##make install后报错"Privilege separation user sshd does not exist make: [check-config] Error 255 (ignored)", 在/etc/passwd 中加入： sshd:x:74:74:Privilege-separated SSH:/var/empty/sshd:/sbin/nologin 再次make install即可
/usr/local/sbin/sshd 启动sshd服务
netstat -tnlp|grep 22 检查服务状态
tcp 0 0 0.0.0.0:22 0.0.0.0:* LISTEN 16909/sshd

