# NAND　FLASH分区规划 - maopig的专栏 - CSDN博客
2012年12月16日 23:28:24[maopig](https://me.csdn.net/maopig)阅读数：7160
由于BOOTLOADRER、PARAMS以及内核、文件系统都在NAND FLASH上，因此分区就得进行统一规划。系统的NAND FLASH分区依赖于u-boot和Linux内核两方面的设置。
## U-Boot中的NAND分区
    文件：include/configs/开发板.h
    这是Phy3250的参数，Phy3250采用32MB的NAND FLASH，扇区大小为16KB：
189 /* Phy3250's NAND FLASH, 32MB, 16K size(Block, Sector) */
190 //#define CFG_ENV_SIZE             0x4000      /* 1 block, 16K */
191 //#define CFG_ENV_OFFSET        0x168000  /* Block 90       */
    如下是SmartARM3250的NAND FLASH，K9F2G08，256MB，扇区大小为128K:
193 /* SmartARM3250's NAND FLASH, K9F2G08, 256MB, 128K size(Block, Sector) */
194 #define CFG_ENV_SIZE           0x40000       /* 2 Block, 256K */
195 #define CFG_ENV_OFFSET      0x180000     /* Block 12         */ 
## Kernel中的NAND FLASH分区
### NAND FLASH规划
分     区                                   地址范围                        扇区范围                    大    小                用     途
 Bootloader             0x0000 0000 ~ 0x0018 0000     0 ~ 11          12个扇区，1.50M               S1L和U-Boot
 param                  0x0018 0000 ~ 0x001C 0000      12~13           2个扇区，  256K                 U-Boot参数
保留                    0x001C 0000 ~ 0x0020 0000         14~15
 Linux Kernel           0x0020 0000 ~ 0x0060 0000    16~47           32个扇区，4.00M                 Linux内核
 Rootfs                 0x0060 0000 ~ 0x1000 0000        48~2047         2000个扇区，250M           Linux文件系统   
|rootfs|204748|共2000扇区|
|----|----|----|
|Kernel|4716|共32扇区|
|保留|15 14|共2扇区|
|param|1312|共2扇区|
|Bootlloader|11 0|共12扇区|
分区实现
在linux-2.6.27.8/arch/arm/mach-lpc32xx/board-smartarm3250.c中的实现代码：
153 #define BLK_SIZE (0x20000) //128KB
154 static struct mtd_partition __initdata smartarm3250_nand_partition[] = {
155     {
156         .name   = "smartarm3250-boot",
157         .offset = 0,
158         .size   = (BLK_SIZE * 12)
159     },
160     {
161         .name   = "smartarm3250-ubt-prms",
162         .offset = (BLK_SIZE * 12),
163         .size   = (BLK_SIZE * 2)
164     },
165     {
166         .name   = "smartarm3250-kernel",
167         .offset = (BLK_SIZE * 16),
168         .size   = (BLK_SIZE * 32)
169     },
170     {
171         .name   = "smartarm3250-rootfs",
172         .offset = (BLK_SIZE * 48),
173         .size   = MTDPART_SIZ_FULL
174     },
175 };

