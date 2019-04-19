# VM虚拟机网络设置及其应用 - Big Smile - CSDN博客
2017年06月04日 23:42:52[王啸tr1912](https://me.csdn.net/tr1912)阅读数：594标签：[虚拟机																[网络																[应用](https://so.csdn.net/so/search/s.do?q=应用&t=blog)](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=虚拟机&t=blog)
个人分类：[总结																[项目经验](https://blog.csdn.net/tr1912/article/category/6211594)](https://blog.csdn.net/tr1912/article/category/6109422)
        提到VM的虚拟机，相信常做Linux系统和系统运维的同志们肯定不陌生了，因为对于我们做实验来，研究技术来说，虚拟机环境再合适不过了。我们平常使用虚拟机，肯定需要他们之间可以互联，或者他们可以连接上互联网，这就需要了解虚拟机里面的网络配置了，具体是怎么配置呢？我们下面就来讲解一下。
# 一、虚拟机连接网络的方式
        我们在建立虚拟机的时候可以很容易的就看到设置网络连接的时候有这么几个选项：
![](https://img-blog.csdn.net/20170604235123629?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
         我们可以清楚的看到，有桥接模式，NAT模式，仅主机模式，还有自定义模式。这里自定义模式其实也是逃不过上面三种模式的制约的，所以我们这里只是讨论上面的三种模式。
**桥接（Bridged）：**什么是桥接模式？桥接模式就是将主机网卡与虚拟机虚拟的网卡利用虚拟网桥进行通信。在桥接的作用下，类似于把物理主机虚拟为一个交换机，所有桥接设置的虚拟机连接到这个交换机的一个接口上，物理主机也同样插在这个交换机当中，所以所有桥接下的网卡与网卡都是交换模式的，相互可以访问而不干扰。在桥接模式下，虚拟机ip地址需要与主机在同一个网段，如果需要联网，则网关与DNS需要与主机网卡一致。其网络结构如下图所示：
![](https://img-blog.csdn.net/20170604235723631?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        至于虚拟交换机呢，我们可以把它理解为一个类似于网线的设备，他使各个设备之间不互相影响，所以他们都可以直接连接到主机外面的路由器上，从而可以直接被分配IP地址，我们也可以手动指定地址。
**NAT模式：**如果你的网络ip资源紧缺，但是你又希望你的虚拟机能够联网，这时候NAT模式是最好的选择。NAT模式借助虚拟NAT设备和虚拟DHCP服务器，使得虚拟机可以联网。其网络结构如下图所示：
![](https://img-blog.csdn.net/20170605000057847?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        这种连接网络的方式其实就是创建了一个虚拟的连接网络的整体环境，有虚拟的DHCP设备，有一个主的交换设备，完全把我们的实体主机当成了一个可以连接网络的源就行了。
**仅主机模式**：Host-Only模式其实就是NAT模式去除了虚拟NAT设备，然后使用VMware Network Adapter VMnet1虚拟网卡连接VMnet1虚拟交换机来与虚拟机通信的，Host-Only模式将虚拟机与外网隔开，使得虚拟机成为一个独立的系统，只与主机相互通讯。其网络结构如下图所示：
![](https://img-blog.csdn.net/20170605000714528?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        由于这个原因，所以我们在我们的网络设备只能看到VMnet1和VMnet8，根本没有VMnet0，但是这个配置项在VM虚拟机的网络编辑器中又是存在的，如图:
![](https://img-blog.csdn.net/20170605001119658?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在网络编辑器中，我们可以直接编辑各种网络模式的配置，只有NAT和仅主机模式可以配置子网的信息。
# 二、应用
        说道具体的应用呢，其实大家通过虚拟机的这几种网络模式可以大概的猜出来，我在这里简单说一下。
        如果需要连接网络呢，仅主机模式可以被抛弃了，因为他是不能连接外部网络的，但是其他两种模式是都可以的。所以仅主机模式适合与当做数据服务器的示例，比如在上面搭建了一个redis，或者安装了一个oracle数据库，这样的情况是可以的。
        如果你的虚拟机想被同网络下的其他机器远程，或者访问到，那么你就需要使用桥接模式了，这个模式下你可以给你的虚拟机分配一个和你的机器同网络的IP，然后就可以分享给其他人了，这样也可以用于搭建一个不同机器的集群使用。
        如果局域网的内容用不到，你只是想要你自己的主机去访问这个虚拟机，并且要求这个虚拟机连接外部网络的话，就需要用到NAT模式了，这个模式下可以方便服务器初始的配置需要下载支持包程序的时候使用，也可以用于自己私人使用的机器。
