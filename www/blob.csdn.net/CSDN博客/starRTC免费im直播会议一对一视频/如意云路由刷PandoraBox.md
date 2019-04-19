# 如意云路由刷PandoraBox - starRTC免费im直播会议一对一视频 - CSDN博客
2018年05月08日 17:03:35[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：579
## 目录
- [1 准备固件](http://192.168.222.250/elesos_com/index.php?title=%E5%A6%82%E6%84%8F%E4%BA%91%E8%B7%AF%E7%94%B1%E5%88%B7PandoraBox#.E5.87.86.E5.A4.87.E5.9B.BA.E4.BB.B6)
- [2 使用uboot刷机](http://192.168.222.250/elesos_com/index.php?title=%E5%A6%82%E6%84%8F%E4%BA%91%E8%B7%AF%E7%94%B1%E5%88%B7PandoraBox#.E4.BD.BF.E7.94.A8uboot.E5.88.B7.E6.9C.BA)- [2.1 修改IP](http://192.168.222.250/elesos_com/index.php?title=%E5%A6%82%E6%84%8F%E4%BA%91%E8%B7%AF%E7%94%B1%E5%88%B7PandoraBox#.E4.BF.AE.E6.94.B9IP)
- [2.2 开始刷写](http://192.168.222.250/elesos_com/index.php?title=%E5%A6%82%E6%84%8F%E4%BA%91%E8%B7%AF%E7%94%B1%E5%88%B7PandoraBox#.E5.BC.80.E5.A7.8B.E5.88.B7.E5.86.99)
- [3 ssh登录](http://192.168.222.250/elesos_com/index.php?title=%E5%A6%82%E6%84%8F%E4%BA%91%E8%B7%AF%E7%94%B1%E5%88%B7PandoraBox#ssh.E7.99.BB.E5.BD.95)
- [4 刷回如意云](http://192.168.222.250/elesos_com/index.php?title=%E5%A6%82%E6%84%8F%E4%BA%91%E8%B7%AF%E7%94%B1%E5%88%B7PandoraBox#.E5.88.B7.E5.9B.9E.E5.A6.82.E6.84.8F.E4.BA.91)
## 准备固件
[http://downloads.openwrt.org.cn/PandoraBox/RY-1/](http://downloads.openwrt.org.cn/PandoraBox/RY-1/)
此处用的是PandoraBox-ralink-ry1-r273-20140415.bin [艺搜下载](http://dl.vmall.com/c0dphqq6oy)
不要下载3M的PandoraBox-ralink-ry1-r286-20140417.bin
## 使用uboot刷机
拔掉路由器WAN口网线，电脑网线连接LAN口。
路由器断电后，用笔捅住reset键不放，然后给路由器上电后倒数10秒钟后再松开笔。
### 修改IP
将ip改为192.168.1网段
### 开始刷写
打开浏览器(建议用firefox)输入192.168.1.1后上传固件“PandoraBox-ralink-ry1-r273-20140415.bin”进行升级。
完成后，可以将电脑IP更改为自动获取。
然后按Ctrl+F5强制刷新192.168.1.1页面，即可看见如下图所示登录页面：
其中用户名为root密码admin
## ssh登录
## 刷回如意云
可以直接在“备份/升级”里选中如意云固件“xRouter_RY-1A_Build20140422_V1.5.1.8.bin”[艺搜下载](http://dl.vmall.com/c07m5vs33i)进行刷写。
如何检测是否结束：可搜下无线网络，检测是否有“xRouter”类似的热点。
