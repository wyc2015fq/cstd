# 《Linux设备驱动程序》——PCI驱动程序 - 一世豁然的专栏 - CSDN博客





2014年11月24日 16:32:03[一世豁然](https://me.csdn.net/Explorer_day)阅读数：752








一、PCI接口 


 一）、相关简介 


  1、PCI架构被设计为ISA标准的替代品，它有三个主要目标： 


    1）、获得计算机和设备之间传输数据时更好的性能。 


    2）、尽可能的平台无关。 


    3）、简化往系统中添加和删除外设的工作 





  2、通过使用比ISA更高的时钟频率，PCI总线获得更好的性能。此外，PCI配置了32位的数据总线，而且规范已经包括了64位扩展。平台无关性通常也是计算机总线的一个设计目


        标，对PCI来说平台无关性尤其重要。 





  3、和驱动程序编写者息息相关的问题是接口板的自动检测。OCI设备是无跳线设备，可以在引导阶段自动配置。







 二）、PCI

  1、每个PCI外设由一个总线编号、一个设备编号及一个功能编号来标识。

    1）、PCI规范允许单个系统拥有高达156个总线。目前，linux目前支持PCI域，每个PCI域最多拥有256个总线。

    2）、每个总线上可支持32个设备，而每个设备可以是多功能板，最多有八种功能。

    3）、每种功能都可以在硬件级由一个16位的地址（或键）来标识。




  2、PCI系统的整体布局为树形，其中每个总线到上一级总线，知道树根的0号总线。典型PCI系统的布局。（P301  图12-1）




  3、尽管和PCI外设关联的16位硬件地址通常隐藏在struct  pci_dev对象中，但有时仍然可见，尤其是这些设备正在被使用时。lspci的输出以及/proc/pci和/proc/bus/pci中信息

        的布局就是这种情况。




  4、在显示硬件地址时，有时显示为两个值（一个8位的总线编号和一个8位的设备及功能编号）、有时显示为三个值（总线、设备、功能），有时显示四个值（域、总线、设

         备和功能）；所有的值通常都以16进制显示。




  5、每个外设板的硬件电路对如下三种地址空间的查询进行应答：

    1）、内存位置。

    2）、I/O端口。

    3）、配置寄存器。




  6、对驱动程序而言，内存和I/O区域是以惯常的方式，即通过inb和readb等等进行访问的。另一方面，配置事务是通过调用特定的内核函数访问寄存器来执行的。关于中断，

        每个PCI槽有四个中断引脚，每个设备功能可使用其中一个，而不用考虑这些引脚如何连接到CPU。




  7、PCI总线中的I/O空间使用32位地址空间，而内存可通过32位或64位地址来访问。PCI配置空间职工每个设备功能由256个字节组成，配置寄存器的布局是标准化的。




  8、PCI接口在ISA之上的主要创新在于配置地址空间。因此，除了通常的驱动程序代码之外，PCI驱动程序还要访问配置空间的能力，以便免去冒险探测的工作。










 三）、引导阶段


  1、当PCI设备上电时，硬件保持未激活状态。换句话说，该硬件只会对配置事务做出响应。




  2、每个PCI主板均配置有能够处理PCI的固件，称为BIOS、NVRAM或PROM。这取决于具体平台。固件通过读写PCI控制器中的寄存器，提供了对设备地址空间的访问。







 四）、配置寄存器和初始化

  1、标准化的PCI配置器

    1）、所有的PCI设备都有至少256字节的地址空间。前64字节是标准化的，而其余的是设备相关的。

    2）、某些PCI配置寄存器是必须的，而某些是可选的。

    3）、PCI寄存器是小头的。驱动程序编写者在访问多字节的配置寄存器时，要十分注意字节序。




  2、配置寄存器的详细介绍

    1）、vendorID：16位寄存器，用于标识硬件制造商。

    2）、deviceID：16位寄存器，由制造商选择；无需对设备OD进行官方注册。

    3）、class：每个外部设备属于某个类，16位寄存器，其中高8位标识了“基类”，或者组。

    4）、subsystem  vendorID和subsystem  deviceID：这两个字段进一步标识设备。




  3、struct  pci_device_id结构体用于定义该驱动程序支持的不同类型的PCI设备列表。

    1）、_ _u32  vendor和_ _u32 device：指定了设备的PCI厂商和设备ID。

    2）、_ _u32 subvendor和_ _u32  subdevice：指定了设备的PCI子系统厂商和子系统ID。

    3）、_ _u32  class和_ _u32  class_mask：这两个值使驱动程序可以指定它支持一种PCI类设备。

    4）、kernel_ulong_t  driner_data：用来保存PCI驱动程序用于区分不同设备的信息。




  4、struct  pci_device_id结构体的初始化

    1）、PCI_DEVICE(vendor,  device)。

    2）、PCI_DEVICE_CLASS(device,  device_class_mask)。







 五）、MODULE_DEVICE_TABLE

  1、pci_device_id结构体需要导出到用户空间，是热插拔和模块装载系统知道什么模块针对什么硬件设备。宏MODULE_DEVICE_TABLE完成此工作







 六）、注册PCI驱动程序

  1、为了正确地注册到内核，所有的PCI驱动程序都必须创建的主要结构体是struct  pci_driver结构体。该结构体由许多回调函数和变量组成，向PCI核心描述了PCI驱动程序。




  2、pci_driver结构体

    1）、const  char  *name：驱动程序的名字。

    2）、const  struct  pci_device_id  *id_table：指向struct  pci_device_id表的指针。

    3）、int  (*probe)  (struct  pci_dev  *dev,  const  struct  pci_device_id  *id)：指向PCI驱动程序中的探测函数的指针。

    4）、void  (*remove)  (struct  pci_dev  *dev)：指向一个移除函数的指针。

    5）、int  (*suspend)  (struct  pci_dev  *dev,  u32  state)：指向一个挂起函数的指针。

    6）、int  (*resume)  (struct  pci_dev  *dev)：指向一个恢复函数的指针。




  3、为了把struct  pci_driver注册到PCI核心中，需要调用以struct  pci_driver指针为参数的pci_register_driver函数。




  4、当PCI驱动程序将要被卸载的时候，需要把struct  pci_driver从内核注销。这是通过调用pci_unregister_driver函数来完成的。







 七）、老式PCI探测

  1、查找特定的PCI设备

    1）、struct  pci_dev  *pci_get_device(unsigned  int  vendor,  unsigned  int  device,  struct  pci_dev  *from);

    2）、struct  pci_dev  *pci_get_subsys(unsigned  int  vendor,  unsigned  int  device,unsigned  int  ss_vendir,  unsigned  int  ss_device,  struct  pci_dev  *from);

    3）、struct  pci_dev  *pci_get_bus(struct  pci_bus  *bus,  unsigned  int  devfn);







 八）、激活PCI设备

  1、在PCI驱动程序的探测函数中，在驱动程序可以访问PCI设备的任何设备资源之前，驱动程序必须调用函数：

        int  pci_enable_device(struct  pci_dev  &dev);







 九）、访问配置空间

  1、在驱动程序检测到设备之后，它通常需要读取或写入三个地址空间：内存、端口和配置。




  2、对于驱动程序而言，可通过8位、16位或32位的数据传输访问配置空间。

    1）、从由dev标识的设备空间读入一个、连个或四个字节

          int  pci_read_config_byte(struct  pci_dev  *dev,  int  where,  u8  *vall);

          int  pci_read_config_word(struct  pci_dev  *dev,  int  where,  u16  *vall);

          int  pci_read_config_dword(struct  pci_dev  *dev,  int  where,  u32  *vall);

    2）、向配置空间写入一个、两个或四个字节

          int  pci_write_config_byte(struct  pci_dev  *dev,  int  where,  u8  *vall);

          int  pci_write_config_word(struct  pci_dev  *dev,  int  where,  u16  *vall);

          int  pci_write_config_dword(struct  pci_dev  *dev,  int  where,  u32  *vall);




  3、在驱动程序不能访问struct  pci_dev的任何时刻，都可以使用这些函数来代替上述函数

    1）、从由dev标识的设备空间读入一个、连个或四个字节

         int  pci_bus_read_config_byte(struct  pci_bus  *bus,  unsigned  int  devfn,  int  wgere,  u8  *val);

         int  pci_bus_read_config_word(struct  pci_bus  *bus,  unsigned  int  devfn,  int  wgere,  u16  *val);

         int  pci_bus_read_config_dword(struct  pci_bus  *bus,  unsigned  int  devfn,  int  wgere,  u32  *val);

    2）、向配置空间写入一个、两个或四个字节

         int  pci_bus_write_config_byte(struct  pci_bus  *bus,  unsigned  int  devfn,  int  wgere,  u8  *val);

         int  pci_bus_write_config_word(struct  pci_bus  *bus,  unsigned  int  devfn,  int  wgere,  u16  *val);

         int  pci_bus_write_config_dword(struct  pci_bus  *bus,  unsigned  int  devfn,  int  wgere,  u32  *val);



 十）、访问I/O和内存空间  

  1、一个PCI设备可实现多达6个I/O地址区域。

    1）、I/O地址区域可以是内存也可以是I/O地址。

    2）、I/O寄存器不应该由CPU缓存。




  2、获得区域信息的首选接口由如下函数组成：

    1）、unsigned  long  pci_resource_start(struct  pci_dev  *dev,  int  bar);

    2）、unsigned  long  pci_resource_end(struct  pci_dev  *dev,  int  bar);

    3）、unsigned  long  pci_resource_flags(struct  pci_dev  *dev,  int  bar);




  3、资源标志用来定义单个资源的某些特性。所有资源定义在<linux/ioport.h>中；下面列出其中最重要的几个：

    1）、IORESOURCE_IO和IORESOURCE_MEM：如果相关的I/O区域存在，将设置这些标志之一。

    2）、IORESOURCE_PREFETCH和IORESOURCE_READOKLY：这些标志表明内存区域是否为可预取的和/或是写保护的。







 十一）、PCI中断

  1、在Linux处理阶段，计算机固件已经为设备分配了一个唯一的中断号，驱动程序只需要使用该中断号。中断号保存在配置寄存器60（PCI_INTERRUPT_LINE）中。如果设

        备不支持中断，寄存器61（PCI_INTERRUPT_PIN）是0，否则为非零。




  2、位于PCI_INTERRUPT_PIN的只读寄存器告诉计算机实际使用的是哪个引脚。另一方面，PCI_INTERRUPT_LINE是可读可写的。







 十二）、硬件抽象

  1、用于实现硬件抽象的机制，就是包含方法的普通结构



  2、用于配置寄存器访问的相关接口仅包含两个字段：

        struct  pci_ops {

            int  (*read) (struct  pci_bus  *bus,  unsigned  int  devfn,  int  where,  int  size,  u32  *val);

            int  (*write) (struct  pci_bus  *bus,  unsigned  int  devfn,  int  where,  int  size,  u32  *val);

        }；










二、ISA回顾

 一）、相关简介

  1、ISA的有点与缺点

    1）、速度慢。

    2）、仅仅与PC架构关联。

    3）、去烧地址寻址。

    4）、设计简单。







 二）、硬件资源

  1、一个ISA设备可配置有I/O端口、内存区域以及中断线。







 三）、ISA编程

  1、对编程而言，内核或BIOS都没有提供任何特懂得帮助来使访问设备更加容易。我们能利用的唯一设施是I/O端口以及IRQ线。







 四）、即插即用规范

  1、某些新的ISA设备板遵循特殊的设计规则，需要一个特殊的初始化序列，以便简化附加接口板的安装和配置。这些接口板的设计规范称为PnP。




  2、PnP的目标就是获得类似PCI设备那样的灵活性，而无须修改还底层的电气接口。










三、PC/104和PC/104+







四、其他的PC总线

  1、MCA

  2、EISA

  3、VLB

  4、SBus

  5、NuBus

  6、外部总线




