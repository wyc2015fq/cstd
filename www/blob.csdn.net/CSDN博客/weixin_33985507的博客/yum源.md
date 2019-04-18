# yum源 - weixin_33985507的博客 - CSDN博客
2016年10月03日 12:17:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
[http://www.cnblogs.com/mchina/archive/2013/01/04/2842275.html](http://www.cnblogs.com/mchina/archive/2013/01/04/2842275.html)
**四、配置本地yum源**
1、挂载系统安装光盘
# mount /dev/cdrom /mnt/cdrom/
2、配置本地yum源
# cd /etc/yum.repos.d/
# ls
会看到四个repo 文件
![](https://images0.cnblogs.com/blog/370046/201301/02161702-13be6e07506743d4a079d61f13a069af.jpg)
**CentOS-Base.repo 是yum 网络源的配置文件**
**CentOS-Media.repo 是yum 本地源的配置文件**
修改CentOS-Media.repo
在baseurl 中修改第2个路径为/mnt/cdrom（即为光盘挂载点）
将enabled=0改为1
3、禁用默认的yum 网络源
将yum 网络源配置文件改名为CentOS-Base.repo.bak，否则会先在网络源中寻找适合的包，改名之后直接从本地源读取。
