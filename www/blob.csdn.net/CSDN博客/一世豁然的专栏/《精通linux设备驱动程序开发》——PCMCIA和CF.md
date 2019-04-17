# 《精通linux设备驱动程序开发》——PCMCIA和CF - 一世豁然的专栏 - CSDN博客





2014年11月17日 20:27:01[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1424








一、PCMCIA和CF是什么

  1、PCMCIA是16位数据传输接口规范。CF卡比PCMCIA要小，但和PCMCIA相兼容。

    1）、CF卡有50个引脚，但使用无源的CF_PCMCIA转换的适配器，可插入笔记本的68针的PCMCIA槽中。

    2）、PCMCIA和CF被限制在笔记本和手持式设备中，没有进入台式机和高端的机器中。




  2、PCMCIA规范修订后，现在支持高速的32位的CardBus卡。术语PC卡也常用来纸袋PCMCIA或CardBus设备。CardBus近似于PCI总线，因此内核将对CardBus设备的支持从

        PCMCIA设备的支持从pcmcia层移到了PCI层。




  3、PC卡有三种形式，其厚度依次增加：Type I（3.3mm），Type II（5mm），Type III（10.5mm）。




  4、笔记本计算机上的PCMCIA（P180  图9-1）和嵌入式系统上的PCMCIA（P180  图9-2）。







二、Linux-PCMCIA子系统

  1、Linux对PCMCIA的支持包括基于Intel的笔记本计算机，以及ARM、MIPS和PowerPC这些体系架构。




  2、PCMCIA子系统组成包括：PCMCIA主机控制器驱动程序，各种卡的客户驱动程序，辅助热插拔的守护程序，用户模式，以及和以上部分所有模块交互的卡服务模块。




  3、Linux-PCMCIA子系统（P181  图9-3）。

    1）、主机控制器驱动程序，实现了和PCMCIA主机控制器通信的底层例程。

    2）、PCMCIA客户驱动程序，用于相应插座事件。

    3）、PCMCIA核心，为主机控制器驱动程序和客户驱动程序提供服务。

    4）、驱动程序服务模块（ds），给客户驱动程序提供注册接口和总线服务。

    5）、pcmciautils包，包括一些用于控制PCMCIA插座的状态并在不同的卡配置机制间加以选择的工具。




  4、PCMCIA驱动程序组件与PC硬件之间的关联。（P181  图9-4）







三、主机控制器驱动程序

  1、通用卡驱动程序负责处理由卡函数产生的中断，而主机控制器驱动程序负责处理由诸如卡插入和拔出等事件所触发的、总线特定的中断。







四、PCMCIA核心

  1、PCMCIA核心的主要作用就是提供PCMCIA卡服务。它即支持客户驱动程序，又支持主机控制驱动程序。包括用于轮询插座相关事件的内核线程pccardd。




  2、PCMCIA核心的另一组件是用于操作CIS的库。CIS是PCMCIA卡的一部分。PCMCIA/CF存储空间分为两部分：属性存储空间和普通存储空间。属性存储空间包括CIS和卡配

        置寄存器。普通存储空间用于存储磁盘数据。







五、驱动程序服务

  1、驱动程序服务提供了基础设施包括如下部分：

    1）、捕获由pccardd内核线程分发的事件警告的处理程序。

    2）、负责和内核的总线核心通信的层。

    3）、服务例程。







六、客户驱动程序

 一）、数据结构

  1、PCMCIA设备由pcmcia_device_id结构体所标识，定义于include/linux/mod  devicetable.h文件中：

        struct  pcmcia_device_id {

            /*  .  .  .  */

            __u16  manf_id;

            __u16  card_id;

            __u8  func_id;

            /*  .  .  .  */

        };

    1）、manf_id、card_id和func_id分别保存PCMCIA卡制造商的ID、卡ID以及功能ID。

    2）、PCMCIA核心提供了宏PCMCIA_DEVICE_MANF_CARD()，用于从提供制造商和卡ID创建pcmcia_device_id结构。

    3）、宏PCMCIA_DEVICE_TABLE()标记了在模块映像中支持的pcmcia_device_ids，以便在卡插入槽、pcmcia子系统从CIS空间收集匹配的制造商/卡/功能ID时，能够根据

              需要加载模块。

    4）、PCMCIA、PCI和USB的设备ID与热插拔方法（P184  表9-1）




  2、PCMCIA客户驱动程序需要和它们的pcmcia_device_id表相关联，pcmcia_device_id中有probe()和remove()方法，这种关联是通过pcmcia_driver结构来获得的：

         struct  pcmcia_driver {

             int  (*probe)(struct  pcmcia_device  *dev);

             void  (*remove)(struct  pcmcia_device  *dev);

             /*  .  .  .  */

             struct  pcmcia_device_id  *id_table;

             /*  .  .  .  */

          };




  3、pcmcia_device结构代表一个PCMCIA设备，定义于driver/pcmcia/ds.h文件中：

        struct  pcmcia_device {

           /*  .  .  .  */

           io_req_t  io;

           irq_req_t  irq;

           config_reg_t  conf;

           /*  .  .  .  */

           struct  device  dev;

           /*  .  .  .  */

        };




  4、CIS操作例程使用定义于include/pcmcia/cispl.h文件中的tuple_t结构体保存CIS信息单元。

         typedef  struct  tuple_t {

             /*  .  .  .  */

             cisdata_t  TupleCode;

             /*  .  .  .  */

             cisdata_t  DesiredTuple;

             /*  .  .  .  */

             cisdata_t  *TupleData;

          };




  5、CIS包含针对卡所支持的每种配置的配置表入口.cis_cftable_entry定义于include/pcmcia/cistpl.h文件中，保存了此入口：

         typedef  struct  cistpl_cftable_entry_t {

             /*  .  .  .  */

             cistpl_power_t  vcc,  vpp1,  vpp2;

             cistpl_io_t  irq;

             cistpl_irq_t  irq;

             cistpl_mem_t  mem;

             /*  .  .  .  */

         };




  6、cistparse_t也定义在include/pcmcia.cistpl.h文件中，保存PCMCIA核心部分分析过的元组：

         typedef  struct  cisparse_t {

              /*  .  .  .  */

              cistpl_manfid_t  manfid;

              /*  .  .  .  */

              cistpl_cftable_entry_t  cftable_entry;




二）、设备实例：PCMCIA卡


  1、代码清单9-1：注册客户驱动程序。

  2、代码清单9-2：配置通用设备驱动程序。







七、将零件组装在一起







八、PCMCIA存储

  1、现在的PCMCIA/CF存储支持吉字节的容量。存储卡多种多样：

    1）、迷你IDE硬盘驱动器或微盘。

    2）、模拟IDE的固定存储卡。

    3）、使用内存的存储卡，但不能模拟IDE。







九、串行PCMCIA

  1、使用串行传输的带PCMCIA/CF卡的网络。（P190  图9-5）




十、调试




