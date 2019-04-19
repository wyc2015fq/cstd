# 从外网 SSH 进局域网，反向代理+正向代理解决方案----本人备注 - xqhrs232的专栏 - CSDN博客
2017年04月27日 14:36:01[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：334
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://blog.csdn.net/zhangmiaoping23/article/details/54881181](http://blog.csdn.net/zhangmiaoping23/article/details/54881181)
相关文章
1、ssh反向代理方法----[http://jingyan.baidu.com/article/b7001fe1b68bd70e7282dda5.html](http://jingyan.baidu.com/article/b7001fe1b68bd70e7282dda5.html)
2、SSH正向与反向代理----[http://www.tuicool.com/articles/RjQFBby](http://www.tuicool.com/articles/RjQFBby)
3、[SSH做反向代理](http://www.cnblogs.com/Gbeniot/p/5600099.html)----[http://www.cnblogs.com/Gbeniot/p/5600099.html](http://www.cnblogs.com/Gbeniot/p/5600099.html)
ssh -f -N -D 127.0.0.1:1080 root@localhost -p 22
ssh -CNfg -L 1081:localhost:1080 root@localhost
转：https://segmentfault.com/a/1190000002718360
相信很多同学都会碰到这样一个问题。在实验室有一台机器用于日常工作，当我回家了或者回宿舍之后因为没法进入内网，所以访问不到了。如果这个时候我需要 SSH 进去做一下工作，那么怎么解决这个问题呢？本文将给出一种使用 SSH 的代理功能的解决方案。
## 问题描述：
### 机器状况
|机器号|IP|用户名|备注|
|----|----|----|----|
|A|192.168.0.A|usr_a|目标服务器，在局域网中，可以访问 A|
|B|B.B.B.B|usr_b|代理服务器，在外网中，无法访问 A|
|C|-|-|可以直接访问 B，无法直接访问 A|
### 目标
从 C 机器使用 SSH 访问 A
## 解决方案
在 A 机器上做到 B 机器的反向代理；在 B 机器上做正向代理本地端口转发
### 环境需求
- 每台机器上都需要 SSH 客户端
- 
A、B 两台机器上需要 SSH 服务器端。通常是 openssh-server。
在 Ubuntu 上安装过程为
```bash
sudo apt-get install openssl-server
```
### 实施步骤
- 
建立 A 机器到 B 机器的反向代理【A 机器上操作】
```bash
ssh -fCNR <port_b1>:localhost:22 usr_b@B.B.B.B
```
`<port_b1>` 为 B 机器上端口，用来与
 A 机器上的22端口绑定。
- 
建立 B 机器上的正向代理，用作本地转发。做这一步是因为绑定后的 端口只支持本地访问【B 机器上操作】
```bash
ssh -fCNL "*:<port_b2>:localhost:<port_b1>' localhost
```
`<port_b2>` 为本地转发端口，用以和外网通信，并将数据转发到 `<port_b1>`，实现可以从其他机器访问。
其中的`*`表示接受来自任意机器的访问。
- 
现在在 C 机器上可以通过 B 机器 ssh 到 A 机器
```bash
ssh -p <portb2> usra@B.B.B.B
```
至此方案完成。
附：
### SSH 参数解释
```
-f 后台运行
-C 允许压缩数据
-N 不执行任何命令
-R 将端口绑定到远程服务器，反向代理
-L 将端口绑定到本地客户端，正向代理
```
//=================================================================================================================================
//备注::
1>参考下面的文章就基本可以走通反向ssh了
1、ssh反向代理方法----[http://jingyan.baidu.com/article/b7001fe1b68bd70e7282dda5.html](http://jingyan.baidu.com/article/b7001fe1b68bd70e7282dda5.html)

