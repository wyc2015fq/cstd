# 查看并修改Linux主机名命令hostname - starRTC免费im直播会议一对一视频 - CSDN博客
2017年01月10日 12:09:26[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：18762
## 查看主机名
hostname可以查看主机名
export也可以查看
## 修改主机名
echo new-hostname > /proc/sys/kernel/hostname (系统启动时，从此文件中读取主机名字)
hostname new-hostname （即时生效，但系统重启后将失效）
修改配置文件，使得下次重启的时候，使用新的主机名
编辑文件：vim /etc/sysconfig/network
修改HOSTNAME=localhost.localdomain为 HOSTNAME=elesos
当然要重启才能生效；接下来可对/etc/hosts文件进行相应修改(一般不用修改)，
通过配置本机的域名解析文件/etc/hosts，使得本机的应用程序能够解析新的主机名，
类似： 127.0.0.1 new-hostname.domainname new-hostname
ubuntu hostname change:
修改/etc/hosts和/etc/hostname
## 艺搜参考
[http://bbs.csdn.net/topics/90136729](http://bbs.csdn.net/topics/90136729)
[http://jimson.blog.51cto.com/119769/323435](http://jimson.blog.51cto.com/119769/323435)
