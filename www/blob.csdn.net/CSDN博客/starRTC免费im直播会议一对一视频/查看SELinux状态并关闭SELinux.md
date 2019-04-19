# 查看SELinux状态并关闭SELinux - starRTC免费im直播会议一对一视频 - CSDN博客
2017年03月05日 14:39:03[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：517
SELinux(Security-Enhanced Linux)是Linux上最杰出的新安全子系统。在linux内核级别上提供了一个灵活的强制访问控制系统(MAC)，这个强制访问控制系统是建立在自由访问控制系统(DAC)之上的。
## 查看SELinux状态
/usr/sbin/sestatus      ##如果SELinux status参数为enabled即为开启状态
SELinux status:         enabled
getenforce              ##也可以用这个命令查看
## 关闭SELinux
- 临时关闭（不用重启机器）：
setenforce 0          ##设置SELinux 成为permissive模式
                      ##setenforce 1 设置SELinux 成为enforcing模式
- 修改配置文件需要重启：
修改/etc/selinux/config 文件
将SELINUX=enforcing改为SELINUX=disabled
(注/etc/sysconfig/selinux是/etc/selinux/config的一个软连接)
然后重启机器即可。
