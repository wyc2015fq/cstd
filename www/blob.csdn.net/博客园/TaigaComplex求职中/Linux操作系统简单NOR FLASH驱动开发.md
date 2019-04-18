# Linux操作系统简单NOR FLASH驱动开发 - TaigaComplex求职中 - 博客园







# [Linux操作系统简单NOR FLASH驱动开发](https://www.cnblogs.com/TaigaCon/archive/2012/11/17/2775464.html)





首先，nor flash驱动作为块设备驱动，大部分的流程跟nand flash一样，只是最底层的硬件操作层跟协议层有区别，

代码如下

```
1 #include <linux/module.h>
 2 #include <linux/types.h>
 3 #include <linux/kernel.h>
 4 #include <linux/init.h>
 5 #include <linux/slab.h>
 6 #include <linux/device.h>
 7 #include <linux/platform_device.h>
 8 #include <linux/mtd/mtd.h>
 9 #include <linux/mtd/map.h>
10 #include <linux/mtd/partitions.h>
11 #include <linux/mtd/physmap.h>
12 #include <linux/mtd/concat.h>
13 #include <linux/io.h>
14 
15 static struct mtd_info *s3c_nor_mtd;
16 static struct map_info *s3c_nor_map;
17 
18 static struct mtd_partition s3c_nor_parts[] = {
19     [0] = {
20         .name    = "uboot_nor",
21         .size    = 0x00040000,
22         .offset    = 0,
23     },
24     [1] = {
25         .name    = "nor_root",
26         .offset = MTDPART_OFS_APPEND,
27         .size    = MTDPART_SIZ_FULL,
28     }
29 };
30 
31 static int s3c_nor_init(void)
32 {
33     /*1.分配一个map_info结构体*/
34     s3c_nor_map=kzalloc(sizeof(struct map_info),GFP_KERNEL);
35     
36     /*2.设置：物理地址（phys）大小（size）位宽（bankwidth），虚拟基地址（virt）*/
37     s3c_nor_map->name="s3c_nor";
38     s3c_nor_map->phys=0;
39     s3c_nor_map->size=0x1000000;
40     s3c_nor_map->bankwidth=2;
41     s3c_nor_map->virt=ioremap(s3c_nor_map->phys,s3c_nor_map->size);
42     
43     simple_map_init(s3c_nor_map);
44     
45     /*3.使用：调用NOR FLASH协议层提供的函数来识别，识别方式有cfi接口跟jedec接口*/
46     /*    cfi是通过读取nor flash的信息进行系统自动调整*/
47     /*    jedec是规定好nor flash的信息，如果从nor中读出来的信息跟规定的信息不符，则出错*/
48     s3c_nor_mtd=do_map_probe("cfi_probe",s3c_nor_map);
49     printk("use cfi_probe\n");
50     if(!s3c_nor_mtd)
51         {
52             printk("cfi_probe error\nuse jedec_probe\n");
53             s3c_nor_mtd=do_map_probe("jedec_probe",s3c_nor_map);
54         }
55         if(!s3c_nor_mtd)
56             {
57                 printk("use jedec_probe error\n");
58                 iounmap(s3c_nor_map->virt);
59                 kfree(s3c_nor_map);
60                 return -EIO;
61             }
62         
63     /*4.分区：add_mtd_partitions*/
64     add_mtd_partitions(s3c_nor_mtd,s3c_nor_parts,2);
65     return 0;
66 }
67 
68 static void s3c_nor_exit(void)
69 {
70     iounmap(s3c_nor_map->virt);
71     kfree(s3c_nor_map);
72 }
73 
74 module_init(s3c_nor_init);
75 module_exit(s3c_nor_exit);
76 
77 MODULE_LICENSE("GPL");
```



不过Linux的nor驱动一般使用总线设备驱动框架，在probe函数内部进行芯片识别（do_map_probe）跟分区（add_mtd_partitions）.



最后就是nor在Linux系统开发板上使用时的注意事项

这里以mini2440为例：

要识别nor flash，必须把开关打到nor flash上，因为友善之臂开发板上的nor flash默认装的是supervivi，因此要把uboot烧到nor flash上去，这里用的是oflash
然后用 nfs 30010000 192.168.1.100:/home/slswxom/secondfs/work/uImage 加载内核
bootm 30010000 运行
然后再加载norflash驱动模块就能运行



如果内核没被配置cfi那么nor flash芯片还是不能被识别的（友善之臂的就没有配置，不过可能可以通过jedec进行识别，我没测试。。）：
配置内核
以下内容必选:
Memory Technology Devices(MTD)-->
　　<*>Memory Technology Device (MTD) support
　　[*]MTD partitioning support
   　　<*>Direct char device access to MTD devices
   　　<*>Caching block device access to MTD devices
　　RAM/ROM/Flash chip drivers-->
   　　　　<*>Detect flash chips by Common Flash Interface (CFI) probe
   　　　　<*>Support for Inter/Sharp flash chips
   　　　　<*>Supportfor AMD/Fujitsu/Spansion flash chips












