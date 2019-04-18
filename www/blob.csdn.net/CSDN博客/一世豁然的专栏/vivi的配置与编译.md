# vivi的配置与编译 - 一世豁然的专栏 - CSDN博客





2015年06月15日 20:31:08[一世豁然](https://me.csdn.net/Explorer_day)阅读数：764








本博客转载于：[http://www.cnblogs.com/hnrainll/archive/2011/05/17/2049295.html](http://www.cnblogs.com/hnrainll/archive/2011/05/17/2049295.html)







今天主要学习了vivi，当然，这是个入门级别的bootloader，但通过分析vivi来学习uboot是一个非常好的选择，vivi的配置仿 照kernel的配置，所以也可以作为配置kernel的入门材料，至于vivi的一些启动过程这里就不做详细的解释了，这里只对vivi的配置中的一些 选项做一下解析。



make menuconfig 

出现的菜单主要有以下一些选项：

System Type --->  【系统配置：主要设置芯片、平台类型 底层启动选项等】                

General setup---> 【通用设置：vivi基地址、系统缓存、电源管理等 】                   

Private Data --->  【私有数据设置：vivi的私有数据 启动的一些用户私有数据】           

Serial Port  --->   【串口：是否支持串口（vivi只支持串口，怎么敢不选），串口传输协议】 

Memory Technology Devices(MTD)-->  【存储设备相关选项】                          

Add Built-in Commands  ---->        【vivi支持命令】                            

System hacking  ---->        【系统调试相关选项】                                

Debugging messages   --->    【调试信息相关】                                     

----                                                                           

Load and Alternate Configuration File  【载入配置文件，vivi自带一个sdmk的配置文件】 

Save Configuration to an Alternate File  【保存自己的配置信息】



下边一一详解。


system type:


这里有连个选项，非常的简单，就是选择我们的arm芯片的类型，我这里使用的是s3c2410,

所以在arm system type 中选择 s3c2410-based.

这里还有一个implementation选项，这个选项主要选择一些平台的其他信息，这里需要知道一个简单smdk


Symbol Mobility Developer Kit (SMDK) ，通用移动设备开发套件，现在很多的s3c2410的板子都属于这个，所以要选择这个，如果不是的话，可能vivi就不能满足你的bootloader的 要求了，可以选择uboot，这里有一些关于启动的选项，我这里是只有一块nand flash,所以选择nand boot.

(S3C2440-based)   ARM System  type  

  (   )   SA1100   –   based  

  (   )   PXA250/210   –   based          //英特尔Xscale arm芯片

  (   )   S3C2400   –   based            //三星公司arm芯片

  (  )   S3C2410   –   based             //三星公司arm芯片

  ( )   S3C2440   –   based              //三星公司arm芯片

  Implementations       //启动位置  

  (SMDK)    Platform  

  (x)SMDK  

  (   )MPORT3  

  (   )MPORT1  

  [ ]Support   NAND              //Boot 支持NAND启动  

  [   ]Support   AMD              //Boot 支持AMD启动  

  –   Low   Level   Hardware Debugging         //底层调试（硬件级）  

  [   ]Enable   simple   memory  test             //简单的存储器测试   



关于通用配置，不知道是什么问题，在我的机器上如果加入support reset handler,会编译出错，只能取消这个选项了，关于缓存，这里不太明白，需要请下昊昱，



General   Setup //通用设置  

  [   ]   Define   TEXT   Address        //定义text段地址  

  (0)   vivi   base   address            //vivi基地址  

[   ]   support   reset   handler    //支持复位处理，可以不选，一般平台都有硬件复位。 

  –   CACHE   Enable          //缓存  

  [   ]   I   –   Cache   on      //指令缓存  

  [   ]   D   –   Cach   on      //数据缓存 

  –   Power   Management //电源管理  

  [   ]   Support   power   management    //支持电源管理  

–          Others  

–            [   ]   Display   progress   bar            //显示进度  

–            [   ]   Support   MD5                    //支持MD5  

私有数据选项中，具体的现象含义还不知道，等研究完vivi的源码在补充吧，但这里需要支持的是，这里的选项是和用户有关的，那么这些数据是非必须的，推荐全选。

Private   Data         //专用数据  

  [   ]   Support   VIVI   private   data          //支持vivi专用数据  

  [   ]   Parse vivi private data from  MTD //从MTD上解析vivi专用数据  

  [   ]   Use user–define  parameter block      //使用用户设定参数块  

  [   ]   Enable   debugging   messages         //打开调试信息




现在到了vivi最为中要的地方了，串口相关的设计，我们知道原版的vivi只能通过串口进行数据的传输，当然不包括现在网上流传的一些改进版 的vivi了，由于vivi只支持串口进行数据的传输，所以速度非常的慢，你可以想象下，如果你的跟文件系统有10M，而串口的速度一般不会超过 10K/s，算算要多长时间才能传完你的根文件系统，但让，vivi也自动了一个很方便的工具那就是imagewrite，但前提是你有一个已经可以运作 的linux才能使用imagewrite,至少你第一次少文件系统的时候需要半个多小时的等待吧。

Serial   Port    //串口  

  [   ]   Serial   Port   (UART)            //support 串口支持  

  [   ]   Support   serial   terminal      //支持串口终端  

  (   EXTENDED)User   interface         //用户界面  

  (   )STANDARD        //标准  

  (   )EXTENDED         //扩展  

  Default   prompt   “vivi”   //默认的提示信息:"vivi"，

–          Ports             //端口  

–            [   ]   Support   UART   0       //支持UART0  

–            [   ]   Support   UART   1       //支持UART1  

–            [   ]   Support   UART   2       //支持UART2  

–            [   ]   Support   UART   3       //支持UART3  

–            –   Transfer   Protocol             //传输协议  

–            [   ]   Support   X   –   Modem //支持   X   Modem   传输  

–            [   ]   Support   Y   –   Modem //支持   Y   Modem   传输  

–            [   ]   Support   Z   –   Modem //支持   Z   Modem   传输  

下边，我们开始存储器管理部分，这部分是比较麻烦一些的。
Memory   Technology   Devices   (MTD) //存储器设备种类  

  [   ]   Memory   Technology   Devices   (MTD)  // support 支持存储器  

  [   ]   Debugging           //调试 

  NOR   Flash   chip   drivers             //Nor Flash 芯片驱动  

  [   ]   NOR   Device   Support         支持 Nor Flash 设备  

 [ ] Detect flash chips by Common Flash Interface (CFI) //probe 检测 CF 接口  

  [  ] Flash chip   driver   advanced   configuration   options   (NEW)    

//高级的闪存设置选项  

  (   ) Flash cmd/query data swapping //设置查看FLASH上的数据的字节序  

  (   )   NO  

  (   )   BIG_ENDIAN_BYTE  

  (   )   LITTLE_   ENDIAN_BYTE  

  [   ] Specific CFI Flash geometry  selection  (NEW)   //特殊的CF选择 

  [   ] Support   8-bit   buswidth   (NEW)    //支持8位总线宽度  

  [   ] Support   16-bit   buswidth   (NEW) //支持16线宽度  

  [   ] Support   32-bit   buswidth   (NEW) //支持32线宽度  

  [   ] Support   64-bit   buswidth   (NEW) //支持64线宽度  

  [   ] Support   1-chip   flash   interleave   (NEW) //支持1片闪存  

  [   ] Support   2-chip   flash   interleave   (NEW) //支持2片闪存  

  [   ] Support   4-chip   flash   interleave   (NEW) //支持4片闪存  

  [   ] Support   8-chip   flash   interleave   (NEW) //支持8片闪存  

  [   ] Support   for   Intel/Sharp   flash   chips //支持Intel或harp   的闪存  

  [   ] Older   (theoretically   obsoleted   now)   deivers   for   non-CFI   chips  //不支持CFI芯片的旧设备。  

  [   ] AMD   compatible   flash   chip   support   (non-CFI) //支持AMD兼容闪存（无cfi）  

  Mapping   drivers   for   chip   access  

  [   ]   Flash   device   mapped   on   S3C2410   //S3C2410上的   

NAND   Flash   Device   Drivers NAND闪存设备驱动  

  [   ]   Enable   ECC   correction   algorithm      //ECC修正算法  

  [   ]   Verify   NAND   page   writes                 //NAND页写入校验  

  [   ]   SMC   Device   Support                          //支持SMC设备  

  [   ]   bonfs   support                            //支持bonfs

这里是选择vivi支持的命令，part是flash分区相关的命令，可以show save del 等

param 是设置一些启动参数。amd这条命令在我的实验板上编译不通过，只能取消它。


Add   Built   –   in   Command     //内部命令  

  [   ]   men   command                    //存储器指令  

  [   ] mem   test   command            //存储器测试指令  

  [   ]   param   command                 //参数指令  

  [   ]   part   command                    //退出指令  

  [   ]   bon   command                    //分区指令  

  [   ]   sleep   command                   //停顿指令  

  [   ]   prompt   command               //提示指令  

  [   ]   built-in   command                //内部指令  

  [   ]   amd   command                    //初期指令   
基本的配置就这些，这里参考了：[http://sns.linuxpk.com/space-1717-do-blog-id-15328.html](http://sns.linuxpk.com/space-1717-do-blog-id-15328.html)


> 
在配置完vivi后，生成Makefile后，还不能正常的编译，这里需要做一些修改。下边是一个Makefile文件需要修改的地方：





```
LINUX_INCLUDE_DIR    = /usr/local/arm/2.95.3/include/

CROSS_COMPILE = /usr/local/arm/2.95.3/bin/arm-linux-

ARM_GCC_LIBS    = /usr/local/arm/2.95.3/lib/gcc-lib/arm-linux/2.95.3
```



需要注意的是，vivi-20030629.tar.bz2这般的vivi只支持编译器为2.95.3,这是在嵌入式开发当中经常遇到的问题，如果 Makefile没有问题，而编译又不能正常进行，首先需要想到的就是编译器的版本问题，这个版本的vivi使用3.4.1的编译器是不能正常编译的，应 为我们要把vivi放到arm平台上运行，所以需要把arm相关的库文件，这就需要在Makefile中制定这些库文件存储的地方。做完这些，基本上就可 以了。


   我们在看看vivi中关于flash的分区，在vivi中受用part show，可以看到有一下几个分区： vivi  param kernel  root  usr ,这些分区信息定义在下边的结构体当中：


```
vivi/arch/s3c2410/smdk.c
```



```
#ifdef CONFIG_S3C2410_NAND_BOOT

mtd_partition_t default_mtd_partitions[] = {

    {

        name:        "vivi",

        offset:        0,

        size:        0x00020000,

        flag:        0

    }, {

        name:        "param",

        offset:        0x00020000,

        size:        0x00010000,

        flag:        0

    }, {

        name:        "kernel",

        offset:        0x00030000,

        size:        0x000C0000,

        flag:        0

    }, {

        name:        "root",

        offset:        0x00100000,

        size:        0x00140000,

        flag:        MF_BONFS

    }
};
#endif
#ifdef CONFIG_S3C2410_AMD_BOOT

mtd_partition_t default_mtd_partitions[] = {

    {

        name:        "vivi",

        offset:        0,

        size:        0x00020000,

        flag:        0

    }, {

        name:        "param",

        offset:        0x00020000,

        size:        0x00010000,

        flag:        0

    }, {

        name:        "kernel",

        offset:        0x00030000,

        size:        0x000C0000,

        flag:        0

    }, {

        name:        "root",

        offset:        0x00100000,

        size:        0x00140000,

        flag:        MF_BONFS

    }
};
```



在上边的分区表可以看出，这些分区在flash中成线性排列的。这样，这里的这些数据是mizi公司推荐使用smdk开发板的用户的数据，当然可以 根据自己的需要来修改各个分区的大小了。这里的offset是相对于flash起始地址0而言，自己的相对地址，其实就是实际地址，而size就是这个分 区实际占据的大小了。 

vivi的使用在这里就告一段落了，更多的细节就需要看vivi的源码了。




