# 阿里云Linux系统挂载数据盘 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月31日 13:26:47[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：333
个人分类：[未分类](https://blog.csdn.net/elesos/article/category/6361263)

Linux云服务器数据盘未做分区和格式化，我们可以根据以下步骤进行分区以及格式化操作。
## 目录
 [[隐藏](http://192.168.1.100/elesos_com/index.php?title=%E9%98%BF%E9%87%8C%E4%BA%91Linux%E7%B3%BB%E7%BB%9F%E6%8C%82%E8%BD%BD%E6%95%B0%E6%8D%AE%E7%9B%98#)] 
- [1 查看数据盘](http://192.168.1.100/elesos_com/index.php?title=%E9%98%BF%E9%87%8C%E4%BA%91Linux%E7%B3%BB%E7%BB%9F%E6%8C%82%E8%BD%BD%E6%95%B0%E6%8D%AE%E7%9B%98#.E6.9F.A5.E7.9C.8B.E6.95.B0.E6.8D.AE.E7.9B.98)
- [2 对数据盘进行分区](http://192.168.1.100/elesos_com/index.php?title=%E9%98%BF%E9%87%8C%E4%BA%91Linux%E7%B3%BB%E7%BB%9F%E6%8C%82%E8%BD%BD%E6%95%B0%E6%8D%AE%E7%9B%98#.E5.AF.B9.E6.95.B0.E6.8D.AE.E7.9B.98.E8.BF.9B.E8.A1.8C.E5.88.86.E5.8C.BA)
- [3 查看新的分区](http://192.168.1.100/elesos_com/index.php?title=%E9%98%BF%E9%87%8C%E4%BA%91Linux%E7%B3%BB%E7%BB%9F%E6%8C%82%E8%BD%BD%E6%95%B0%E6%8D%AE%E7%9B%98#.E6.9F.A5.E7.9C.8B.E6.96.B0.E7.9A.84.E5.88.86.E5.8C.BA)
- [4 格式化新分区](http://192.168.1.100/elesos_com/index.php?title=%E9%98%BF%E9%87%8C%E4%BA%91Linux%E7%B3%BB%E7%BB%9F%E6%8C%82%E8%BD%BD%E6%95%B0%E6%8D%AE%E7%9B%98#.E6.A0.BC.E5.BC.8F.E5.8C.96.E6.96.B0.E5.88.86.E5.8C.BA)
- [5 添加分区信息](http://192.168.1.100/elesos_com/index.php?title=%E9%98%BF%E9%87%8C%E4%BA%91Linux%E7%B3%BB%E7%BB%9F%E6%8C%82%E8%BD%BD%E6%95%B0%E6%8D%AE%E7%9B%98#.E6.B7.BB.E5.8A.A0.E5.88.86.E5.8C.BA.E4.BF.A1.E6.81.AF)
- [6 挂载新分区](http://192.168.1.100/elesos_com/index.php?title=%E9%98%BF%E9%87%8C%E4%BA%91Linux%E7%B3%BB%E7%BB%9F%E6%8C%82%E8%BD%BD%E6%95%B0%E6%8D%AE%E7%9B%98#.E6.8C.82.E8.BD.BD.E6.96.B0.E5.88.86.E5.8C.BA)
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E9%98%BF%E9%87%8C%E4%BA%91Linux%E7%B3%BB%E7%BB%9F%E6%8C%82%E8%BD%BD%E6%95%B0%E6%8D%AE%E7%9B%98&action=edit&section=1)]查看数据盘
在没有分区和格式化数据盘之前，使用 “df –h”命令是无法看到数据盘的，可以使用“fdisk -l”命令查看。如下图：
注：若您执行fdisk -l命令，发现没有 /dev/xvdb 表示您的云服务无数据盘，那么您无需进行挂载，此时本教程对您不适用。
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E9%98%BF%E9%87%8C%E4%BA%91Linux%E7%B3%BB%E7%BB%9F%E6%8C%82%E8%BD%BD%E6%95%B0%E6%8D%AE%E7%9B%98&action=edit&section=2)]对数据盘进行分区
执行 fdisk /dev/xvdb 命令，对数据盘进行分区； 根据提示，依次输入“n”，“p”“1”，两次回车，“wq”，分区就开始了，很快就会完成。
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E9%98%BF%E9%87%8C%E4%BA%91Linux%E7%B3%BB%E7%BB%9F%E6%8C%82%E8%BD%BD%E6%95%B0%E6%8D%AE%E7%9B%98&action=edit&section=3)]查看新的分区
使用“fdisk -l”命令可以看到新的分区xvdb1已经建立完成了。
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E9%98%BF%E9%87%8C%E4%BA%91Linux%E7%B3%BB%E7%BB%9F%E6%8C%82%E8%BD%BD%E6%95%B0%E6%8D%AE%E7%9B%98&action=edit&section=4)]格式化新分区
使用“mkfs.ext3 /dev/xvdb1”命令对新分区进行格式化，格式化的时间根据硬盘大小有所不同。 (也可自主决定选用 ext4 格式)
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E9%98%BF%E9%87%8C%E4%BA%91Linux%E7%B3%BB%E7%BB%9F%E6%8C%82%E8%BD%BD%E6%95%B0%E6%8D%AE%E7%9B%98&action=edit&section=5)]添加分区信息
使用
echo '/dev/xvdb1  /mnt ext3    defaults    0  0' >> /etc/fstab
命令写入新分区信息。
然后使用“cat /etc/fstab”命令查看，出现以下信息就表示写入成功。
注： 如果需要把数据盘单独挂载到某个文件夹，可以修改以上命令中的/mnt部分
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E9%98%BF%E9%87%8C%E4%BA%91Linux%E7%B3%BB%E7%BB%9F%E6%8C%82%E8%BD%BD%E6%95%B0%E6%8D%AE%E7%9B%98&action=edit&section=6)]挂载新分区
使用“mount -a”命令挂载新分区，然后用“df -h”命令查看，出现以下信息就说明挂载成功，可以开始使用新的分区了。
