# Win7下VMware的NAT网络模式不能正常工作 - 菜鸟路上的小白 - CSDN博客





2016年09月24日 10:31:05[somnus_小凯](https://me.csdn.net/u012486840)阅读数：2022









今天在Windows 7下实验VMware的NAT网络模式时遇到了障碍，Guest-OS不仅无法访问Internet，而且连Host-OS也ping不通。有时候仿佛能ping通Host-OS，但是却返回大量的(DUP!)包，如下图所示：![](https://img-blog.csdn.net/20160924102917601?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20160924102933507?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![ping-dup](https://img-blog.csdn.net/20160924102950363?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


根据这个提示，在Google上搜索“ping DUP!”得到大量的结果，有的说禁用VMnet8网卡然后重启即可解决，有的说可能是局域网上有IP冲突，有的说可能ping的是广播地址，还有的说局域网有回路等等，除了第一个禁用/启用VMnet8具有可操作性之外，其他都是些莫名其妙的不知道具体如何解决的答案。但是，遗憾的是禁用/启用VMnet8后问题依旧。 在Guest-OS上执行
```
iptables
 -L
```
并未发现有设置防火墙规则，并且在Host-OS上已经禁用防火墙了，所以应该可以排除防火墙的原因。然后在vmware社区上看到了[这篇帖子](https://communities.vmware.com/message/1168628)，有个人使用tcpdump来定位问题，实验后仍然没有发现什么异常，无果而终。

在ping DUP!这个问题上花费了大约整整一天的时间，最终意识到ping DUP!可能并不是虚拟机无法访问网络的症结所在，通过网络上给出的答案可以发现ping DUP!问题更多的原因可能是局域网内网络配置错误或者网卡异常导致的，而且出现这种现象的原因太多，很难入手。于是重新审视这个问题，把问题的所有现象一一列出来：
- Host-OS IP: 192.168.0.107
- Host-OS Gateway: 192.168.0.1
- Guest-OS IP: 192.168.220.128
- Guest-OS Gateway: 192.168.220.2
- Guest-OS ping自身192.168.220.128正常，ping网关192.168.220.2正常
- Guest-OS ping Host-OS 192.168.0.107返回大量DUP包
- Guest-OS ping Host-OS的网关 192.168.0.1返回大量DUP包
- **Host-OS ping Guest-OS 192.168.220.128正常**
- **打开网络和共享中心，VMnet1和VMnet8属于未识别的网络，而且公共网络不能修改**


最后两点现象很快就让人感到异常，特别是未识别网络这条，如下图所示：

![unidentified-networks](https://img-blog.csdn.net/20160924103007582?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


于是Google关键字“VMnet8 未识别的网络”，根据第一条记录的结果修改注册表中的`*NdisDeviceType=1`然后重启VMnet8网卡，问题竟然就解决了！修改注册表参见下图：

![ndis-device-type](https://img-blog.csdn.net/20160924103022415?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


原来是Windows 7网络访问的限制，也可以说是VMware的一个Bug，VMware在新建网卡时设备类型`*NdisDeviceType`使用了默认值0,0意味着网卡是标准的真实网卡，VMware应该将其设置为`NDIS_DEVICE_TYPE_ENDPOINT(1)`才对，这样Windows会知道这个网卡是个虚拟网卡并没有和真实的外部网络连接，在“网络和共享中心”中忽略这些网卡，不会受网络共享设置的限制。


通过解决这个问题，再次意识到真的“不能在一颗树上吊死”。解决问题往往有两种方式：
- 第一种方式是：首先找出问题的一个异常现象，然后通过各种手段解决这个异常，最终会有两种结果：一是异常解决并且问题解决，二是异常解决问题并未解决。如果是情况一，则问题解决，结束。如果是情况二，则排除了这个异常和问题的联系，这时需要寻找另一个异常现象，重复上述步骤继续解决
- 第二种方式是：首先列出所有发现的异常现象，然后针对每个异常现象快速确定和问题有联系的可能性（譬如利用搜索引擎），然后针对每个可能性再继续对问题进行分析


可以发现两种解决问题的方式正如图的两种遍历算法：深度优先算法和广度优先算法，方式一为深度优先，方式二为广度优先。深度优先的缺点是可能会导致无止境的往下延伸，越往下延伸解决方案离最初的问题可能就越远，直到排除所有可能性后才能解决最初的问题，在遇到奇葩的问题时，效率可能会非常低下。我们将每一次延伸称之为问题的解决路径，如果你的问题的解决路径很短，譬如我们这里的这个问题，根据现象“VMnet8 未识别网络”就可以直接解决问题，而根本不需要在ping DUP!问题上耗费无数的时间来研究。整个问题的解决示意图如下：

![solve](https://img-blog.csdn.net/20160924103037603?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


可见，如果根本不能确定问题原因时，通过广度遍历的方式，加上问题之间的联系，可能会更好更快的解决问题。所以，在面临问题时不要沉陷在自己的某一个想法中，可以从多个角度去看待问题。

## 参考
- [VMware Workstation环境中Linux
 ping返回出现大量“DUP！”的解决方法](http://www.linuxidc.com/Linux/2014-07/104415.htm)
- [ping DUP！
 错误](http://blog.csdn.net/huaxi1902/article/details/8761578)
- [Duplicate packets
 with ping on guest OS(Linux)](https://communities.vmware.com/message/1168628)
- [VMware
 Network Adapter VMnet1和VMnet8未识别的网络修复](http://blog.kilvn.com/vmware-network-adapter-vmnet1-8/)
- [Hide
 VMWare Virtual Network Interfaces from Windows Firewall and Network and Sharing Center](http://brianreiter.org/2011/08/29/hide-vmware-virtual-network-interfaces-from-windows-firewall-and-network-and-sharing-center/) [需翻墙]



