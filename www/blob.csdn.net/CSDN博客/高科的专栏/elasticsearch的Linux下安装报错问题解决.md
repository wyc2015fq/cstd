
# elasticsearch的Linux下安装报错问题解决 - 高科的专栏 - CSDN博客

2018年11月13日 17:37:30[高科](https://me.csdn.net/pbymw8iwm)阅读数：93


# 1.启动报错如下:
![](https://img-blog.csdnimg.cn/20181113173048996.png)
vim /etc/security/limits.conf
然后修改如下
`* soft nofile 65536
* hard nofile 65536`sudo vi /etc/pam.d/common-session
添加 session required pam_limits.so
sudo vi /etc/pam.d/common-session-noninteractive
添加 session required pam_limits.so
vi /etc/security/limits.d/90-nproc.conf
然后修改如下
`*          soft    nproc     4096`切换到root用户
执行命令：
sysctl -w vm.max_map_count=262144
查看结果：
sysctl -a|grep vm.max_map_count
以上部分操作需要退出账号重新登录的

