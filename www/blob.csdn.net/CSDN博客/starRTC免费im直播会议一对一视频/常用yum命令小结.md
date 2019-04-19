# 常用yum命令小结 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年03月10日 12:33:18[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：288
基于rpm的软件包管理器
yum，是Yellow dog Updater, Modified的简称，是一种软件包管理器。它能够从指定的服务器自动下载RPM包并安装，可以自动处理依赖性关系，并且一次安装所有依赖的软件包，无须繁琐地一次次下载、安装。yum提供了查找、安装、删除某一
个、一组甚至全部软件包的命令。
## 目录
 [[隐藏](http://192.168.1.100/elesos_com/index.php?title=%E5%B8%B8%E7%94%A8yum%E5%91%BD%E4%BB%A4%E5%B0%8F%E7%BB%93(%E6%9B%B4%E6%96%B0%E4%B8%AD...)#)] 
- [1 安装](http://192.168.1.100/elesos_com/index.php?title=%E5%B8%B8%E7%94%A8yum%E5%91%BD%E4%BB%A4%E5%B0%8F%E7%BB%93(%E6%9B%B4%E6%96%B0%E4%B8%AD...)#.E5.AE.89.E8.A3.85)
- [2 更新和升级](http://192.168.1.100/elesos_com/index.php?title=%E5%B8%B8%E7%94%A8yum%E5%91%BD%E4%BB%A4%E5%B0%8F%E7%BB%93(%E6%9B%B4%E6%96%B0%E4%B8%AD...)#.E6.9B.B4.E6.96.B0.E5.92.8C.E5.8D.87.E7.BA.A7)- [2.1 update与upgrade
 的区别](http://192.168.1.100/elesos_com/index.php?title=%E5%B8%B8%E7%94%A8yum%E5%91%BD%E4%BB%A4%E5%B0%8F%E7%BB%93(%E6%9B%B4%E6%96%B0%E4%B8%AD...)#update.E4.B8.8Eupgrade_.E7.9A.84.E5.8C.BA.E5.88.AB)
- [3 查找](http://192.168.1.100/elesos_com/index.php?title=%E5%B8%B8%E7%94%A8yum%E5%91%BD%E4%BB%A4%E5%B0%8F%E7%BB%93(%E6%9B%B4%E6%96%B0%E4%B8%AD...)#.E6.9F.A5.E6.89.BE)
- [4 删除](http://192.168.1.100/elesos_com/index.php?title=%E5%B8%B8%E7%94%A8yum%E5%91%BD%E4%BB%A4%E5%B0%8F%E7%BB%93(%E6%9B%B4%E6%96%B0%E4%B8%AD...)#.E5.88.A0.E9.99.A4)
- [5 清除缓存](http://192.168.1.100/elesos_com/index.php?title=%E5%B8%B8%E7%94%A8yum%E5%91%BD%E4%BB%A4%E5%B0%8F%E7%BB%93(%E6%9B%B4%E6%96%B0%E4%B8%AD...)#.E6.B8.85.E9.99.A4.E7.BC.93.E5.AD.98)
- [6 艺搜参考](http://192.168.1.100/elesos_com/index.php?title=%E5%B8%B8%E7%94%A8yum%E5%91%BD%E4%BB%A4%E5%B0%8F%E7%BB%93(%E6%9B%B4%E6%96%B0%E4%B8%AD...)#.E8.89.BA.E6.90.9C.E5.8F.82.E8.80.83)
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E5%B8%B8%E7%94%A8yum%E5%91%BD%E4%BB%A4%E5%B0%8F%E7%BB%93(%E6%9B%B4%E6%96%B0%E4%B8%AD...)&action=edit&section=1)]安装
**yum install ****perl****//**安装**perl** 包
**yum install ****perl*********//**安装**perl** 开头的包
**yum groupinstall ****<**groupnames**>****//**安装指定软件组中的软件包
查看**yum groupinstall **"Development Tools"时安装了哪些软件：
**yum groupinfo **"Development Tools"
localinstall：安装本地的rpm软件包； 
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E5%B8%B8%E7%94%A8yum%E5%91%BD%E4%BB%A4%E5%B0%8F%E7%BB%93(%E6%9B%B4%E6%96%B0%E4%B8%AD...)&action=edit&section=2)]更新和升级
**yum update****//**更新系统已安装的所有软件包
**yum update** package1 **//**更新指定程序包package1
**yum upgrade****//**大规模的版本升级,与**yum update**不同的是,连旧的,淘汰的包也升级
**yum upgrade** package1 **//**升级指定程序包package1
**yum groupupdate** group1 **//**升级程序组group1
### [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E5%B8%B8%E7%94%A8yum%E5%91%BD%E4%BB%A4%E5%B0%8F%E7%BB%93(%E6%9B%B4%E6%96%B0%E4%B8%AD...)&action=edit&section=3)]update与upgrade 的区别
*#cat /etc/issue*
CentOS release 6.3*#uname -r*
2.6.32-279.el6.x86_64
**yum update** -y并重启后
CentOS release 6.5
2.6.32-431.29.2.e16.x86_64  **//**已更新
 
**yum**  upgrade -y并重启后
CentOS release 6.5
2.6.32-431.29.2.e16.x86_64  **//**已更新
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E5%B8%B8%E7%94%A8yum%E5%91%BD%E4%BB%A4%E5%B0%8F%E7%BB%93(%E6%9B%B4%E6%96%B0%E4%B8%AD...)&action=edit&section=4)]查找
**yum info** package1 **//**显示安装包信息package1
**yum search****//**查找软件包
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E5%B8%B8%E7%94%A8yum%E5%91%BD%E4%BB%A4%E5%B0%8F%E7%BB%93(%E6%9B%B4%E6%96%B0%E4%B8%AD...)&action=edit&section=5)]删除
**yum remove****<**package_name**>****yum groupremove** group1 **//**删除程序组group1
**yum deplist** package1 **//**查看程序package1依赖情况
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E5%B8%B8%E7%94%A8yum%E5%91%BD%E4%BB%A4%E5%B0%8F%E7%BB%93(%E6%9B%B4%E6%96%B0%E4%B8%AD...)&action=edit&section=6)]清除缓存
**yum clean** all **//**清除缓存目录下的软件包及旧的headers
若有不对之处，欢迎指出，以便更新，防止误人，谢谢。
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E5%B8%B8%E7%94%A8yum%E5%91%BD%E4%BB%A4%E5%B0%8F%E7%BB%93(%E6%9B%B4%E6%96%B0%E4%B8%AD...)&action=edit&section=7)]艺搜参考
[http://centos5.blog.51cto.com/1921280/844119](http://centos5.blog.51cto.com/1921280/844119)
[https://www.centos.bz/2011/07/yum-all-command-explanation/](https://www.centos.bz/2011/07/yum-all-command-explanation/)
[http://www.w3cschool.cc/linux/linux-yum.html](http://www.w3cschool.cc/linux/linux-yum.html)
