# windows下端口映射（端口转发） - xqhrs232的专栏 - CSDN博客
2018年07月24日 11:36:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：147
原文地址::[https://blog.csdn.net/i1j2k3/article/details/70228043](https://blog.csdn.net/i1j2k3/article/details/70228043)
相关文章
1、netsh--windows下的端口转发----[https://blog.csdn.net/caiqiiqi/article/details/52855690](https://blog.csdn.net/caiqiiqi/article/details/52855690)
`本文是对网文的归纳整理，算不上原创，摸索过程亲手测试过`` `
# 一.搞清楚概念
`所谓的映射、转发是针对接收数据的端口而言的，一般用作服务端，要侦听的`` ``应用场景如：原本有个服务程序在PC2上运行，侦听着PC2上的B端口，现在希望在不动PC2及服务程序的前提下，外界能通过PC1的A端口与PC2上服务程序通信，这时就需要用端口映射（端口转发）来解决。`` ``将PC1的A端口映射到PC2的B端口，是指将PC1的A端口收到的数据转发到PC2的B端口。外界都来连PC1的A端口，PC1的A端口只做受理窗口，具体业务处理放在PC2的B端口。其实，PC2的B端仍然能用，就是多了一个受理窗口（PC1的A端口）。`` ``所以端口映射体现的是横向扩展，不是移花接木`` `
# 二.实现
## 1.用windows的netsh实现
`优点：``（1）部署简单。Windows自带，支持IPv4和IPv6`
```
（2）不用重启机器，还长记性。命令即时生效,重启系统后配置仍然存在.
缺点:
```
```
（1）不支持UDP
（2）XP/2003需要先安裝IPV6，Win7以上系统自带.
```
` ``用法：``下面都是命令行`` `
```
（1）安装环境支持
netsh interface ipv6 install
（2）查看已配置的“端口映射”清单
netsh interface portproxy show v4tov4
（3）添加“端口映射”
netsh interface portproxy add v4tov4 listenaddress=192.168.222.145 listenport=15001 connectaddress=192.168.222.63 connectport=81
```
```
将本机（192.168.222.145）的15001端口映射到192.168.222.63的81端口
（4）删除“端口映射”
netsh interface portproxy delete v4tov4 listenaddress=192.168.222.145 listenport=15001
```
## 2.用SSH端口转发实现
```
好贴：https://www.ibm.com/developerworks/cn/linux/l-cn-sshforward/
```
`好例：http://koumm.blog.51cto.com/703525/1318975`
