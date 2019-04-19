# Linux基础(四)网络设置 - _天枢 - 博客园
## [Linux基础(四)网络设置](https://www.cnblogs.com/yhleng/p/8397941.html)
2018-02-01 10:59 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=8397941)
**环境:**
linux系统版本为:CentOS 7
虚拟机:vm12
共享本地计算机网络,达到我们linux系统上网的目的
**虚拟机,添加网卡-设置**
设置成,自定义vmnet8 NAT模式即可,不需要添加什么其它的网卡,设置桥接之类的.
主要,在linux中启用网络即可.直接共享本地网络.
网上,查了很多资料,五花八门,怎么设置的都有,很麻烦,而且不一定管用.
**![](https://images2017.cnblogs.com/blog/1149221/201802/1149221-20180201103925390-697788300.png)**
以上设置好后,启动虚拟机,进入linux系统
命令:su root切换到root权限,要不然没有权限修改ifcfg-ens33文件
ifcft-你的网卡名称
```
vim /etc/sysconfig/network-scripts/ifcfg-ens33
```
![](https://images2017.cnblogs.com/blog/1149221/201802/1149221-20180201104433562-1011303736.png)
 ONBOOT=no改成yes  ,启动网络即可.
![](https://images2017.cnblogs.com/blog/1149221/201802/1149221-20180201104712578-1927916621.png)
之后输入命令,重启网络服务:
```
service network restart
```
查看一下网络信息:
```
ifconfig
```
我们可以看到,已经有网络ip自动分配给系统.
![](https://images2017.cnblogs.com/blog/1149221/201802/1149221-20180201104957234-469597860.png)
ping一个百度,测试一下,能否上网.
```
ping www.baidu.com
```
![](https://images2017.cnblogs.com/blog/1149221/201802/1149221-20180201105041968-1876087042.png)
从图上看,网络已;连通,已经可以共享本地的网络上外网了.
**Linux and python学习3群:563227894**
**1,2群已满,3群新开等你加入**
**分享经验,等你来共勉**
**不前进,不倒退,停止的状态是不存在的.**
