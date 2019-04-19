# 后台运行 screen命令 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年01月06日 14:33:42[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：137
nohub不能用的，用这个
后台运行
yum install screen
只要Screen本身没有终止，在其内部运行的会话都可以恢复
登录到主机上执行**screen -r**就可以恢复会话的运行。
同样在暂时离开的时候，也可以执行分离命令**detach到后台**
screen -S yourname -> 新建一个叫yourname的session
screen -ls -> 列出当前所有的session
screen -r yourname -> 回到yourname这个session
screen -d yourname -> 远程detach某个session
[https://www.cnblogs.com/mchina/archive/2013/01/30/2880680.html](https://www.cnblogs.com/mchina/archive/2013/01/30/2880680.html)
