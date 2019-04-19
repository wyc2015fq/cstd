# 克隆或拷贝的VMware虚拟机IP问题解决 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年09月01日 14:16:17[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：6370
克隆的虚拟机或是将虚拟机通过快照回到过去某个状态后，会上不了网。
如果不想看细节，可直接跳到小结部分.
问题描述：
运行service network restart时
Device eth0 does not seem to be present,delaying initialization.      [FAILED]
ifconfig没有发现eth0和eth1，但ifconfig -a时发现的是eth1（硬件地址是00:0C:29:45:41:1E）
查看/etc/udev/rules.d/70-persistent-net.rules发现硬件地址有2个，eth0的是00:0C:29:22:4A:32（原来的）， eth1的是00:0C:29:45:41:1E
查看/etc/sysconfig/network-scripts/ifcfg-eth0发现硬件地址是00:0C:29:22:4A:32（原来的）
而拷贝前的虚拟机这3处均为eth0，且硬件地址均为00:0C:29:22:4A:32
如果仍想用eth0标识，可以
**rm**-rf**/**etc**/**udev**/**rules.d**/**70-persistent-net.rules
然后将/etc/sysconfig/network-scripts/ifcfg-eth0的硬件地址改为00:0C:29:45:41:1E,如果是静态IP,可能还需要修改一下IP以免出现冲突
最后reboot重启机器。
## 小结
- ifconfig -a查看eth1的mac地址，
- rm -rf /etc/udev/rules.d/70-persistent-net.rules
- 修改/etc/sysconfig/network-scripts/ifcfg-eth0的硬件地址为上面查到的eth1的mac地址
- reboot
