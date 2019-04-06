#  			[QEMU,KVM及QEMU-KVM介绍](https://www.cnblogs.com/echo1937/p/7138294.html) 		



## What's QEMU

QEMU是一个主机上的VMM（virtual machine  monitor）,通过动态二进制转换来模拟CPU，并提供一系列的硬件模型，使guest  os认为自己和硬件直接打交道，其实是同QEMU模拟出来的硬件打交道，QEMU再将这些指令翻译给真正硬件进行操作。通过这种模式，guest  os可以和主机上的硬盘，网卡，CPU，CD-ROM，音频设备和USB设备进行交互。但由于所有指令都需要经过QEMU来翻译，因而性能会比较差：



## What's KVM?

KVM实际是linux内核提供的虚拟化架构，可将内核直接充当hypervisor来使用。KVM需要处理器硬件本身支持虚拟化扩展，如intel  VT 和AMD  AMD-V技术。KVM自2.6.20版本后已合入主干并发行，除此之外，还以模块形式被移植到FreeBSD和illumos中。除了支持x86的处理器，同时也支持S/390,PowerPC,IA-61以及ARM等平台。

### 工作原理

KVM包含一个内核模块kvm.ko用来实现核心虚拟化功能，以及一个和处理器强相关的模块如kvm-intel.ko或kvm-amd.ko。KVM本身不实现任何模拟，仅仅是暴露了一个/dev/kvm接口，这个接口可被宿主机用来主要负责vCPU的创建，虚拟内存的地址空间分配，vCPU寄存器的读写以及vCPU的运行。有了KVM以后，guest   os的CPU指令不用再经过QEMU来转译便可直接运行，大大提高了运行速度。但KVM的kvm.ko本身只提供了CPU和内存的虚拟化，所以它必须结合QEMU才能构成一个完整的虚拟化技术，也就是下面要介绍的技术。

## What's QEMU-KVM

从前面的介绍可知，KVM负责cpu虚拟化+内存虚拟化，实现了cpu和内存的虚拟化，但kvm并不能模拟其他设备，还必须有个运行在用户空间的工具才行。KVM的开发者选择了比较成熟的开源虚拟化软件QEMU来作为这个工具，QEMU模拟IO设备（网卡，磁盘等），对其进行了修改，最后形成了QEMU-KVM。

![img](https://upload-images.jianshu.io/upload_images/3143954-cdde1444c6a86c60.jpg?imageMogr2/auto-orient/strip%7CimageView2/2)

在QEMU-KVM中，KVM运行在内核空间，QEMU运行在用户空间，实际模拟创建、管理各种虚拟硬件，QEMU将KVM整合了进来，通过/ioctl  调用  /dev/kvm，从而将CPU指令的部分交给内核模块来做，KVM实现了CPU和内存的虚拟化，但KVM不能虚拟其他硬件设备，因此qemu还有模拟IO设备（磁盘，网卡，显卡等）的作用，KVM加上QEMU后就是完整意义上的服务器虚拟化。
综上所述，QEMU-KVM具有两大作用：

1. 提供对cpu，内存（KVM负责），IO设备（QEMU负责）的虚拟
2. 对各种虚拟设备的创建，调用进行管理（QEMU负责）

这个方案中，QEMU模拟其他的硬件，如Network, Disk，同样会影响这些设备的性能。于是又产生了pass through半虚拟化设备virtio_blk, virtio_net，提高设备性能。

![img](https://upload-images.jianshu.io/upload_images/3143954-ab897227bb8acc26.jpg?imageMogr2/auto-orient/strip%7CimageView2/2)

> QEMU-KVM，是QEMU的一个特定于KVM加速模块的分支，里面包含了很多关于KVM的特定代码，与KVM模块一起配合使用。
>
> 目前QEMU-KVM已经与QEMU合二为一，所有特定于KVM的代码也都合入了QEMU，当需要与KVM模块配合使用的时候，只需要在QEMU命令行加上 --enable-kvm就可以。



分类: [Virtualization](https://www.cnblogs.com/echo1937/category/1086868.html)