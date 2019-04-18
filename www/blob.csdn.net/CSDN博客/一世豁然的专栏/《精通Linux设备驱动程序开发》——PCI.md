# 《精通Linux设备驱动程序开发》——PCI - 一世豁然的专栏 - CSDN博客





2014年11月20日 15:45:11[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1192








一、PCI系列

  1、PCI的同类和子类（P194  表10-1）




  2、PCI应用领域




  3、对于驱动程序开发人员来讲，PCI系列的优势是：设备自动配置系统。




  4、PC机南桥里的PCI（P195  图10-1）










二、寻址和识别

  1、PCI设备的地址由总线号、设备号和功能号组成，分别称为厂家ID，设备ID和设备类代码。




  2、lspci工具解析（书中实例）

    1）、XX代表PCI的总线号。一个PCI域能容纳256个总线。

    2）、YY是PCI设备编号。每个总线支持32个PCI设备。

    3）、Z表示PCI的总线号，每个PCI设备可容纳8个PCI功能。

    4）、运行lspci  -t等到PCI设备和PCI总线组成的树状结构图。




  3、PCI设备拥有256B的空间，用于存放配置寄存器。该空间是辨别PCI卡型号和性能的关键。

    1）、lspci  -x查看配置区具体情况。

    2）、PCI配置空间的含义（P197  表10-2）










三、访问PCI

 一）、配置区

  1、内核为驱动程序提供了6个可调用的函数来访问PCI配置区：

        pci_read_config_[byte | word | dword](struct  pci_dev  *pdev,  int  offset,  int  *value);

        pci_write_config_[byte | word | dword](struct  pci_dev  *pdev,  int  offset,  int  *value);

    1）、struct  pci_dev是PCI设备结构体。

    2）、offset是想访问的配置空间中的字节位置。

    3）、对读函数来讲，value是数据缓冲区的指针；对写函数来讲，value放的是准备写的数据。




  2、书中相关实例（P198）







 二）、I/O和内存

  1、PCI卡有6个I/O内存区域，I/O内存区域包含寄存器，内存区域存放数据。




  2、内恶化提供了一系列的辅助函数，可以用来操作PCI设备的I/O和内存区域

        unsigned  long  pci_resouce_[start | len | end | flags] (struct  pci_dev  *pdev,  int  bar);










四、DMA

  1、DMA是外围设备直接传送数据到内存，不经过CPU的干涉。

    1）、DMA由DMA控制器初始化。

    2）、DMA也存在高速缓存一致性的问题。

    3）、DMA可以同步传送数据也可以异步传送数据。

    4）、DMA缓冲区是DMA传送时用作源端地址或目的地址的内存区。

       I、可用下面的函数告诉内核系统中可用作DMA缓冲区的地址的特殊要求：

        dma_set_mask(struct  device  8dev,  u64  mask);




  2、I/O设备从总线控制器或IOMMU的角度看待DMA缓冲区。所以，I/O设备需要的是DMA缓冲区的总线地址，而不是物理地址或内核虚拟地址。总线地址的类型是

         dma_addr_t，在文件include/asm-your-arch/types.h。




  3、DMA两个概念

    1）、一个是回弹缓冲区。回弹缓冲区驻留在可作为DMA缓冲区的内存区域里，在DMA请求的源或目的地址没有DMA功能的内存区域时，它可以作为临时内存区存放数据。

    2）、第二个概念是：分散/聚集。当要传输的数据分布在不连续的内存上时，分散/聚集能对这些不连续缓冲区的数据进行一次性发送，反过来，DMA也能把数据从卡正确地

              传送到地址不连续的缓冲区中。




  4、内核为PCI驱动程序提供两类DMA服务

    1）、一致性DMA访问方法。这些程序保证DMA传输数据的一致性。要得到能保证数据一致性的DMA缓冲区，用下面的API：

      void  *pci_alloc_consistent(struct  pci_dev  *pdev,  size_t  size,  dma-addr_t  *dma_handle);

    2）、流式DMA访问函数。这些API不保证数据的一致性，所以速度更快。

             此函数有两类：pci_[map | unmap | dma_sync]_single()和pci_[map | unmap | dma_sync]_single()。

       I、第一组是可以映射、去映射以及同步一个预先分配的单一DMA缓冲区。

            dma_addr_t  pci_map_single(struct  pci_dev  *pdev,  void  *ptr,  size_t  size,  int  direction);

       II、第二组是可以映射、去映射以及同步一个分散/聚集的单一DMA缓冲区链。

            int  pci_map_sg(struct  pci_dev  *pdev,  struct  scatterlist  *sgl,  int  num_entries,  int  direction);




  5、一致性DMA和流式DMA的特点。










五、设备实例：以太网-调制解调器卡

 一）、初始化和探测

  1、PCI驱动程序用设备结构体pci_device_id数组描述它支持的一系列卡，该结构定义在文件/include/linux/mod_device.h中。

        struct  pci_device_id {

              __u32  vendor,  device;

              __u32  subvendor,subdevice;

              __u32  class,  classmask;

        Kernel_ulong_t  driver_data;

        };




  2、注册网络功能：代码清单10-1。




  3、注册调制解调器功能：代码清单10-2。  




  4、探测以太网功能：代码清单10-3。




  5、探测调制解调器功能：代码清单10-4。







 二）、数据传输

  1、为实现卡与CPU传输数据，CardBus卡的网络功能采用以下策略传输数据。驱动程序安排了两个DMA接收器和两个DMA发送数据描述符，没和DMA描述符包括DMA缓冲区

        的地址、传送数据的长度和就控制字。

    1）、CardBus卡的DMA描述符和数据缓冲区示意图（图10-3）.

    2）、CardBus卡网络功能的寄存器布局（表10-3）。




  2、DMA描述符和数据缓冲区的建立过程：代码清单10-5。




  3、接受和发送数据：代码清单10-6。







六、调试




