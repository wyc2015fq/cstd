# u-boot nand flash read/write cmd - maopig的专栏 - CSDN博客
2012年03月05日 23:07:25[maopig](https://me.csdn.net/maopig)阅读数：1529
支援的命令函數說明
1. nand info/nand device
功能：顯示當前nand flash晶片資訊。
函數調用關係如下(按先後順序)：
static void nand_print(struct nand_chip *nand) ;
2. nand erase
功能：擦除指定塊上的數據。
函數調用關係如下(按先後順序)：
int nand_erase(struct nand_chip* nand, size_t ofs, size_t len, int clean);
3. nand bad
功能：顯示壞塊。
函數調用關係如下(按先後順序)：
static void nand_print_bad(struct nand_chip* nand);
int check_block (struct nand_chip *nand, unsigned long pos);
4. nand read
功能：讀取nand flash信息到SDRAM。
函數調用關係如下(按先後順序)：
int nand_rw (struct nand_chip* nand, int cmd,size_t start, size_t len, size_t * retlen, u_char * buf);
static int nand_read_ecc(struct nand_chip *nand, size_t start, size_t len,
size_t * retlen, u_char *buf, u_char *ecc_code);
static void NanD_ReadBuf (struct nand_chip *nand, u_char * data_buf, int cntr);
READ_NAND(adr);
5. nand write
功能：從SDRAM寫數據到nand flash中。
函數調用關係如下(按先後順序)：
int nand_rw (struct nand_chip* nand, int cmd,size_t start, size_t len, size_t * retlen, u_char * buf);
static int nand_write_ecc (struct nand_chip* nand, size_t to, size_t len,
size_t * retlen, const u_char * buf, u_char * ecc_code);
static int nand_write_page (struct nand_chip *nand, int page, int col, int last, u_char * ecc_code);
WRITE_NAND(d , adr);
4.2.32 nand info
列印nand flash資訊
CRANE2410 # nand info
Device 0: Samsung K9F1208U0B at 0x4e000000 (64 MB, 16 kB sector)
4.2.33 nand device <n>
顯示某個nand設備
CRANE2410 # nand device 0
Device 0: Samsung K9F1208U0B at 0x4e000000 (64 MB, 16 kB sector)
... is now current device
4.2.34 nand bad
CRANE2410 # nand bad
Device 0 bad blocks:
4.2.35 nand read
nand read InAddr FlAddr size
InAddr: 從nand flash中讀到記憶體的起始位址。
FlAddr: nand flash 的起始位址。
size: 從nand flash中讀取的數據的大小。
CRANE2410 # nand read 0x30008000 0 0x100000
NAND read: device 0 offset 0, size 1048576 ...
1048576 bytes read: OK
4.2.36 nand erease
nand erase FlAddr size
FlAddr: nand flash 的起始位址
size: 從nand flash中擦除資料塊的大小
CRANE2410 # nand erase 0x100000 0x20000
NAND erase: device 0 offset 1048576, size 131072 ... OK
4.2.37 nand write
nand write InAddr FlAddr size
InAddr: 寫到Nand Flash中的資料在記憶體的起始位址
FlAddr: Nand Flash的起始位址
size: 數據的大小
CRANE2410 # nand write 0x30f00000 0x100000 0x20000
NAND write: device 0 offset 1048576, size 131072 ...
131072 bytes written: OK
4.2.37 nboot
u-boot-1.1.4代碼對於nboot命令的幫助不正確，修改如下:
正確的順序為：
nboot InAddr dev FlAddr
InAddr: 需要裝載到的記憶體的位址。
FlAddr: 在nand flash上uImage存放的地址
dev: 設備號
需要提前設置環境變數，否則nboot不會調用bootm
CRANE2410 #setenv autostart yes
CRANE2410 # nboot 30008000 0 100000
Loading from device 0: <NULL> at 0x4e000000 (offset 0x100000)
Image Name: Linux-2.6.14.3
Created: 2006-07-06 7:31:52 UTC
Image Type: ARM Linux Kernel Image (uncompressed)
Data Size: 897428 Bytes = 876.4 kB
Load Address: 30008000
Entry Point: 30008040
Automatic boot of image at addr 0x30008000 ...
## Booting image at 30008000 ...
Starting kernel ...
4.3 命令簡寫說明
所以命令都可以簡寫，只要命令前面的一部分不會跟其他命令相同,就可以不用寫全整個命令.
save命令
CRANE2410 # sa
Saving Environment to Flash...
Un-Protected 1 sectors
Erasing Flash...Erasing sector 10 ... Erased 1 sectors
4.4 把文件寫入NandFlash
如果把一個傳到記憶體中的檔寫入到Nand Flash中, 如：新的uboot.bin, zImage(內核),
rootfs等, 如果做呢？我們可以用Nand Flash命令來完成. 但是Nand Flash寫時,必須先要把Nand
Flash的寫入區全部擦除後,才能寫. 下面以把記憶體0x30008000起長度為0x20000的內容寫到Nand
Flash中的0x100000為例.
CRANE2410 # nand erase 0x100000 0x20000
NAND erase: device 0 offset 1048576, size 131072 ... OK
CRANE2410 # nand write 0x30008000 0x100000 0x20000
NAND write: device 0 offset 1048576, size 131072 ...
131072 bytes written: OK
我的nand flash 32M，kernel 2.6.18, rootfs emb linux, cramfs.
nand flash分区如下：
static struct mtd_partition nand_partitions[] = {
  /* bootloader (UBL, U-Boot, BBT) in sectors: 0 - 14 */
  {
  .name = "bootloader",
  .offset = 0,
  .size = 32 * NAND_BLOCK_SIZE, //32x16 = 512k
  .mask_flags = MTD_WRITEABLE, /* force read-only */
  },
  /* bootloader params in the next sector 15 */
  {
  .name = "params",
  .offset = MTDPART_OFS_APPEND,
  .size = 96 * NAND_BLOCK_SIZE, //96x16 = 1536k = 1.5M
  .mask_flags = MTD_WRITEABLE, /* force read-only */
  },
  /* kernel in sectors: 16 */
  {
  .name = "kernel",
  .offset = MTDPART_OFS_APPEND,
  .size = SZ_2M, //2M
  .mask_flags = 0
  },
  /* rootfs */
  {
  .name = "filesystem1",
  .offset = MTDPART_OFS_APPEND,
  .size = SZ_16M + SZ_8M, //24M
  .mask_flags = 0
  },
  /*parameter*/
  {
  .name = "filesystem2",
  .offset = MTDPART_OFS_APPEND,
  .size = MTDPART_SIZ_FULL, //4M, /mnt/nand, save log,sysenv,
  .mask_flags = 0
  }
};
//系统启动后
# cat /proc/partitions   
major minor #blocks name   
  31 0 512 mtdblock0   
  31 1 1536 mtdblock1   
  31 2 2048 mtdblock2   
  31 3 24576 mtdblock3   
  31 4 4096 mtdblock4   
254 0 512 sbulla   
#
# cat /proc/mtd   
dev: size erasesize name   
mtd0: 00080000 00004000 "bootloader"   
mtd1: 00180000 00004000 "params"   
mtd2: 00200000 00004000 "kernel"   
mtd3: 01800000 00004000 "filesystem1"   
mtd4: 00400000 00004000 "filesystem2"   
#
我的问题是, 以下命令是厂家提供的，是可以执行成功的命令，我不理解其中到某些命令：
在U-boot烧写时，用以下命令：
tftpboot 0x80700000 uImage_ipnc_dm365 //可以理解
tftpboot 0x82000000 cramfsImage_ipnc_dm365.v1.2.4-debug-param //可以理解
nand erase 0x200000 0xDF0000 //不理解，本句的意思是把kernel分区和rootfs分区擦除，0xDF0000 = 13.9375M, 而内核和文件系统共2M+24M=26M，为什么不是#nand erase 0x200000 0x1A00000 (26M).
nand write 0x80700000 0x200000 0x200000 //可以理解，写内核到NAND FLASH, 从内存地址 0x80700000 读取大小为 0x200000(2M)的数据，写入nand flash，写入的起始地址为0x200000。
nand write 0x82000000 0x400000 0xBF0000 //不理解，写文件系统到NAND FLASH, 意思是从内存地址 0x82000000 读取大小为 0xBF0000(11.9375M)的数据，写入nand flash，写入的起始地址为0x400000。为什么只写0xBF0000(11.9375M)的数据，我的文件系统分区24M啊，实际的文件系统也 22M左右，why？
请各路高手看看。
，比如我在U-boot烧写文件系统时，我只想对文件系统filesystem1分区擦除然后升级文件系统，不擦内核和最后的分区（filesystem2），该怎么用nand erase命令，偏移多少，大小多少。如何精确的调整,我觉得这个值必须很精确，不然会影响到前一个分区和最后的分区的数据。
 启动NFS
/usr/sbin/exportfs -a
/sbin/service nfs restart
开启tftp服务
/sbin/chkconfig tftp on
启动NFS服务
/etc/init.d/portmap start
/etc/init.d/nfs start
停止NFS服务
/etc/init.d/nfs stop
/etc/init.d/portmap stop
重新启动NFS服务
/etc/init.d/nn/se 
--------------------------------------------------------------------------------------
/etc/init.d/ssh restart
mount -t vfat /dev/sda1 /mnt
fdisk －l
----------------------------------------------------------------------------------------
DVS6446环境变量设置
NFS booot
setenv bootargs mem=60M console=ttyS0,115200n8 root=/dev/nfs noinitrd rw ip=192.168.253.130:255.255.255.0:192.168.253.1 nfsroot=192.168.253.229:/opt/nfs video=dm64xxfb:output=pal
NAND boot
setenv bootargs mem=60M console=ttyS0,115200n8 init=/linuxrc root=/dev/mtdblock0 rw noinitrd rootfstype=yaffs ip=192.168.253.140:255.255.255.0:192.168.253.1 video=dm64xxfb:output=pal
硬盘boot
setenv bootargs mem=60M console=ttyS0,115200n8 root=/dev/hda1 noinitrd rw ip=192.168.253.183:255.255.255.0:192.168.253.1 video=dm64xxfb:output=pal
--------------------------------------------------------------------------------------------
DVS6467
setenv bootargs mem=80M console=ttyS0,115200n8 root=/dev/mtdblock3 rootfstype=jffs2 noinitrd rw ip=192.168.253.183:192.168.253.1:255.255.255.0 video=dm64xxfb:output=pal
vlc rtsp://192.168.253.130/TSStream-1 --clock-synchro=0
------------------------------------------------------------------------------------------------
IPC365板卡网络文件系统；
setenv bootargs console=ttyS0,115200n8 noinitrd rw ip=192.168.253.130 root=/dev/nfs nfsroot=192.168.253.229:/opt/nfs, nolock mem=80M video=davincifb dm365_imp.oper_mode=0 davinci_capture.device_type=4
IPC365 FLASH文件系统
setenv bootargs mem=80M console=ttyS0,115200n8 noinitrd rw ip=192.168.253.130:192.168.253.1:255.255.255.0 root=/dev/mtdblock3 rootfstype=yaffs video=davincifb:osd0=720x576x16,4050K dm365_imp.oper_mode=0 davinci_capture.device_type=4
IPC365板卡的环境变量设置
bootdelay=4
baudrate=115200
bootargs=console=ttyS0,115200n8 noinitrd rw ip=192.168.253.130 root=/dev/nfs nfsroot=192.168.253.229:/opt/nfs, nolock mem=80M video=davincifb dm365_imp.oper_mode=0 davinci_capture.device_type=4
stdin=serial
stdout=serial
stderr=serial
ver=U-Boot 1.3.4 (Dec 7 2009 - 11:07:06)
bootfile="uImage-dm365"
bootcmd=tftp 0x80700000 uImage-dm365;bootm
serverip=192.168.253.229
ipaddr=192.168.253.120
autostart=yes
ethaddr=11:22:23:24:15:19
setenv bootcmd 'tftp 0x80700000 uImage-dm365;bootm'
setenv bootcmd 'tftp 0x80700000 uImage-dm365;bootm'
setenv ethaddr 11:22:23:24:15:19
setenv autostart
------------------------------------------------------------------------------------------------------------------
DM365 开发板 环境变量
bootdelay=4
baudrate=115200
bootfile="uImage"
ipadd=192.168.1.3
server=192.168.1.2
filesize=1FC08C
fileaddr=80800000
autostart=yes
bootcmd=nboot 0x80800000 0 0x200000
serverip=192.168.15.110
ethaddr=00:11:11:11:11:11
setenv bootargs mem=80M console=ttyS0,115200n8 noinitrd rw ip=192.168.253.186:255.255.255.0:192.168.253.1 root=/dev/mtdblock3 rootfstype=yaffs video=davincifb:osd0=720x576x16,4050K dm365_imp.oper_mode=0 davinci_capture.device_type=4
ipaddr=192.168.15.3
stdin=serialip
stdout=serial
stderr=serial
ver=U-Boot 1.3.4 (Dec 7 2009 - 11:07:06)
Environment size: 544/262140 bytes
setenv bootargs mem=80M console=ttyS0,115200n8 root=/dev/nfs noinitrd rw ip=192.168.253.144:255.255.255.0:192.168.253.1 nfsroot=192.168.253.229:/opt/nfs video=davincifb:osd0=720x576x16,4050K dm365_imp.oper_mode=0 davinci_capture.device_type=4
---------------------------------------------------------------------------------------------------------------
SEED-DEC138板卡环境变量修改：
网络文件系统启动
setenv bootargs mem=128M console=tty0 root=/dev/nfs nfsroot=192.168.253.229:/opt/nfs rw ip=192.168.253.145:192.168.253.1:255.255.255.0
setenv bootcmd 'tftp 0xc80800000 uImage; bootm'
0x80700000
setenv bootcmd 'tftp 0xc0700000 uImage_dec138; bootm'
FLASH文件系统启动
SEED-OMAP3530
DNS设置
vi etc/resolv.conf
nameserver 202.113.196.115
nameserver 202.113.0.20 

