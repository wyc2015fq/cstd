# ubuntu突然无法连接网络解决 - miner_zhu的博客 - CSDN博客





2018年08月19日 20:41:47[miner_zhu](https://me.csdn.net/miner_zhu)阅读数：480








背景：ubuntu16.04   之前正常使用，突然某天提示连接中断，之后一直无法连接网络

试了无数教程无果，最后猜测可能还是网络服务的问题。

解决方法：

1.打开电脑的服务界面

在键盘上同时点击“win+R”键弹出运行界面,输入services.msc,按确定.

2.手动“启动”以VMware开头的几个服务

![](https://img-blog.csdn.net/20180819204032657?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21pbmVyX3podQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这样就解决了Ubuntu的网络离线问题,供大家参考 



