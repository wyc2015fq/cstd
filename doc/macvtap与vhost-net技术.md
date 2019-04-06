#  			[macvtap与vhost-net技术](https://www.cnblogs.com/echo1937/p/7249812.html) 		



MacVlan的功能是给同一个物理网卡配置多个MAC地址，可以在软件上配置多个以太网口，属于物理层的功能。MacVTap是用来替代TUN/TAP和Bridge内核模块的。MacTap是基于MacVlan这个模块，提供TUN/TAP中TAP设备使用的接口，使用MACVTap以太网口的虚拟机能够通过TAP设备接口，直接将数据传递到内核中对应的MacVTap以太网中。

### MacVTap工作模式

1.VEPA

同一物理网卡下的MacVTap设备之间的流量也要发送到外部交换机再由外部交换机转发回服务器，前提是交换机必须支持hairpin模式。

2.Bridge

同一物理网卡下的MacVTap设备可以直接进行以太网帧交换，不需要外部交换机介入。

3.private

同一物理网卡下的MacVTap设备互相无法联通，无论外部交换机支不支持hairpin模式。

### 创建MacVTap

```
[root@Linux78 ~]# ip link add link eth1 name macvtap0 type macvtap
[root@ Linux78 ~]# ip link set macvtap0 address 1a:2b:3c:4d:5e:6a up 
[root@ Linux78 ~]# ip link show macvtap0
9: macvtap0@eth1: <BROADCAST,MULTICAST,UP,M-DOWN> mtu 1500 qdisc pfifo_fast state UNKNOWN qlen 500
    link/ether 1a:2b:3c:4d:5e:6a brd ff:ff:ff:ff:ff:ff
```

KVM虚拟机使用MacVTap接入网络的方法，在Libvirt中使用MacVTap的置配。

```
<interface type='direct'>
   <mac address='1a:2b:3c:4d:5e:6a'/>
   <source dev=‘eth0’ mode=‘bridge' />
   <model type=‘e1000’ />
   <address type='pci' domain='0x0000' bus='0x00' slot='0x03' function='0x0'/>
</interface>
```

虚拟机创建后，宿主机会自动创建出一台MacVTap设备给虚拟机使用，这台MacVTap设备附属于母设备eth0，工作模式为bridge。

> ### 虚拟机之间及物理机与虚拟机的通信
>
> macvlan有bridge、VEPA、private、passthru 4种工作模式，其中private和passthru我没用过， 这里主要讲另外两种。
>
> VEPA(Virtual Edge Port Aggregator)是默认的工作模式，它的初衷是希望由物理的交换机来进行所有包交换， 这样可以在交换机上统一配置DHCP filtering之类的策略。
>
> 因此这个模式下的macvlan会把所有的包都扔到外部端口上去，期待交换机进行包交换，  把目的地址为本机机器的包再传回来。很多交换机考虑安全原因（防止包打环）是不支持这样的行为的，  但是一些较新的交换机上有这样一个特性，叫hairpin、VEPA或者802.1Qbg。
>
> bridge模式则考虑到某些情况下需要更高效的macvlan之间的通信，因此会在内存中进行包交换，提高速度。
>
> 但是无论哪种模式，在没有外部交换机的支持的情况下，都是不可能支持物理端口到macvlan端口的包交换的。  上面的原理部分已经提到了，macvlan的port是在物理端口注册了一个rx_handler，  它只会对物理端口收到的包进行处理，而物理端口发出去的包macvlan是不会看到的。
>
> private模式我没有细看，但应该是drop掉了目的端口为其他macvlan端口的包。
>
> 综上，结论如下：
>
> - 对于有交换机支持的网络中，使用VEPA模式和bridge模式都可以实现物理机与虚拟机之间的所有通信。
> - 在无交换机支持的网络中， 
>   - 使用VEPA模式，虚拟机之间及物理机与虚拟机之间不能进行任何形式的通信；
>   - 使用bridge模式，虚拟机之间可以正常通信，虚拟机与物理机不能正常通信。

 

vhost-net技术使虚拟机的网络通信绕过用户空间的虚拟化层，可直接和内核通信，从而提高虚拟机的网络性能，MacVTap则是跳过内核的网桥。使用vhost-net必须使用Virtio半虚拟化网卡。

vhost-net是对于Virtio的优化。Virtio本来是设计用于进行客户系统的前端与VMM的后端通信，减少硬件虚拟化方式根模式和非模式的切换。vhost-net是对于Virtio的优化，Virtio是虚拟化层的前端优化方案，减少硬件虚拟化方式下根模式与非根模式的切换，而vhost-net是虚拟化层后端优化方案。不使用vhost-net，进入CPU的根模式后，需要进入用户态将数据发送到tap设备后，再次切入内核态，使用vhost-net方式后，进入内核态后不需要进行内核用户态的切换，进一步减少物权切换的开销。

### vhost_net技术

运行虚拟机是由用户空间的QEMU和内核KVM共同完成，QEMU负责模拟各种设备提供给虚拟机，KVM负责完成CPU和内存的虚拟化。Virtio的后端处理程序一般是由用户空间的QEMU提供。为减少延迟，提高性能，新的内核中增加了一个vhost_net驱动模块，在内核中实现Virtio的后端处理程序。

### vhost_net配置

```
<interface type='bridge'>
      <mac address='1a:2b:3c:4d:5e:6a'/>
      <model type=‘virtio’ />
      <driver name=‘vhost’ />
      <address type='pci' domain='0x0000' bus='0x00' slot='0x03' function='0x0'/>
 </interface>
```

 