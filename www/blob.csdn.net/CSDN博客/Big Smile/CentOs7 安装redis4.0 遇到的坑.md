# CentOs7 安装redis4.0 遇到的坑 - Big Smile - CSDN博客
2018年07月28日 16:25:17[王啸tr1912](https://me.csdn.net/tr1912)阅读数：256
这次尝试了一下安装和使用redis4，结果遇到了不少的坑，和大家分享一下。
# 一、CentOs7连接网络
        由于是新安装的centos7，所以什么都没有配置，也没有连接过网络，使用ip addr查看，只有一个网卡开启了，就是127.0.0.1的网卡，然后另外一个网卡的网络地址为空的，这时我们需要去开启网络连接。
      首先去网络配置文件所在的目录：
`cd  /etc/sysconfig/network-scripts/`
![](https://img-blog.csdn.net/20180728160150801?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
图中的两个ifcfg的文件就是我们两个网卡的配置文件，其中lo的为127.0.0.1的网卡，我们需要修改另外一个
ps：每个机器的网卡名称对应都不同，请以实际为准
`vi ifcfg-ens33 `
![](https://img-blog.csdn.net/20180728160403538?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
把最后这项改成yes，然后reboot系统即可
# 二、make文件的时候出错
        首先我们需要先安装gcc的支持：
```
yum install gcc
yum install gcc-c++
yum install tcl
```
然后进行make的时候依旧会报错，报错为：  jemalloc/jemalloc.h：没有那个文件或目录
分配器allocator， 如果有MALLOC  这个环境变量，会有用这个环境变量的 去建立Redis。而且libc 并不是默认的分配器， 默认的是 jemalloc, 因为 jemalloc 被证明 有更少的 fragmentation problems 比libc。
但是如果新系统没有装jemalloc 而只有 libc 当然 make 出错。 所以加这么一个参数,运行如下命令：
`make MALLOC=libc`
这时make命令就不会出错了，然而他依然会建议你先进行make test
# 三、安装ruby的redis插件时候报错
        错误为：“**redis requires Ruby version >= 2.2.2**”
这个问题是因为centos7的yum源里面没有ruby2.2.2的安装资源，最高只到2.0.0，所以我们必须通过其他方法安装这个高版本的ruby，下面讲一种最快的方式，添加yum源：CentOS SCLo Software collections Repository
执行命令：
`yum install centos-release-scl-rh　　　　//会在/etc/yum.repos.d/目录下多出一个CentOS-SCLo-scl-rh.repo源`
这个时候我们就成功的安装了yum源了，然后我们需要接着找到新版本的ruby：
```
yum install rh-ruby23  -y　　　　//这样yum安装的是ruby2.3.0版本
scl  enable  rh-ruby23 bash　　　　//必要一步
```
这样之后，我们的ruby就安装成功了，我们可以执行ruby -v 查看版本：
![](https://img-blog.csdn.net/20180728162155755?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这样之后，我们就可以执行
`gem install redis`
来获取ruby的redis支持了，就可以执行redis-trib.rb这个二进制文件了。
# 最后
最后来强调一下执行集群命令的时候，中间的字符为1，不是L，已经碰到过两次这个坑了
`./redis-trib.rb create --replicas 1 127.0.0.1:7001 127.0.0.1:7002 127.0.0.1:7003 127.0.0.1:7004 127.0.0.1:7004 127.0.0.1:7005 127.0.0.1:7006`
![](https://img-blog.csdn.net/20180728162510852?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
