
# Android Flash分区大小查看 - 嵌入式Linux - CSDN博客

2018年10月26日 15:32:30[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：460



# 正常的8G Flash有多少可以用的
这样算吧：
硬盘和U盘厂家的算法：8GB*1000=8000MB*1000=8000000KB*1000=8000000000B
[电脑系统](https://www.baidu.com/s?wd=%E7%94%B5%E8%84%91%E7%B3%BB%E7%BB%9F&tn=SE_PcZhidaonwhc_ngpagmjz&rsv_dl=gh_pc_zhidao)的算法：8000000000B/1024=7812500KB/1024=7629.4MB/1024=7.45GB
所以U盘在7.4G左右是正常的，是[电脑系统](https://www.baidu.com/s?wd=%E7%94%B5%E8%84%91%E7%B3%BB%E7%BB%9F&tn=SE_PcZhidaonwhc_ngpagmjz&rsv_dl=gh_pc_zhidao)算法和厂家算法的区别而已，所有硬盘和存储设备都是这样算的。
一种是1:1000的算法 一种是1:1024的算法

# 从日志上看分区大小
从日志上看 Flash 文件系统计算出来的大小是 7.28CB
```python
[    1.932780] inv_mpu_iio: inv_mpu: reset chip failed, err = -6
[    1.965906] mmc1: MAN_BKOPS_EN bit is not set
[    1.978246] mmc1: new HS400 Enhanced strobe MMC card at address 0001
[    1.979106] mmcblk1: mmc1:0001 8GME4R 7.28 GiB 
[    1.979611] mmcblk1boot0: mmc1:0001 8GME4R partition 1 4.00 MiB
[    1.983828] mmcblk1boot1: mmc1:0001 8GME4R partition 2 4.00 MiB
[    1.984383] mmcblk1rpmb: mmc1:0001 8GME4R partition 3 512 KiB
[    1.984837]      uboot: 0x000400000 -- 0x000800000 (4 MB)
[    1.984852]      trust: 0x000800000 -- 0x000c00000 (4 MB)
[    1.984865]       misc: 0x000c00000 -- 0x001000000 (4 MB)
[    1.984877]   resource: 0x001000000 -- 0x002000000 (16 MB)
[    1.984889]     kernel: 0x002000000 -- 0x003800000 (24 MB)
[    1.984902]       boot: 0x003800000 -- 0x005800000 (32 MB)
[    1.984914]   recovery: 0x005800000 -- 0x009800000 (64 MB)
[    1.984927]     backup: 0x009800000 -- 0x010800000 (112 MB)
[    1.984939]      cache: 0x010800000 -- 0x038800000 (640 MB)
[    1.985226]     system: 0x038800000 -- 0x098800000 (1536 MB)
[    1.985241]   metadata: 0x098800000 -- 0x099800000 (16 MB)
[    1.985253] verity_mode: 0x099800000 -- 0x099808000 (0 MB)
[    1.985266] baseparamer: 0x099808000 -- 0x099c08000 (4 MB)
[    1.985278]        frp: 0x099c08000 -- 0x099c88000 (0 MB)
[    1.985291]   userdata: 0x099c88000 -- 0x1d1c00000 (4991 MB)
[    1.985417]  mmcblk1: p1 p2 p3 p4 p5 p6 p7 p8 p9 p10 p11 p12 p13 p14 p15
[    2.035459] inv_mpu_iio: inv_mpu: reset chip failed, err = -6
[    2.138786] inv_mpu_iio: inv_mpu: reset chip failed, err = -6
[    2.242241] inv_mpu_iio: inv_mpu: reset chip failed, err = -6
[    2.345572] inv_mpu_iio: inv_mpu: reset chip failed, err = -6
[    2.449042] inv_mpu_iio: inv_mpu: reset chip failed, err = -6
[    2.552228] inv_mpu_iio: inv_mpu: reset chip failed, err = -6
```
# 代码设置Flash分区大小位置
```python
weiqifa@ubuntu:~/repoRk3399_yan4_dev/device/rockchip/rk3399/rk3399_mid$ cat parameter.txt 
FIRMWARE_VER: 0.8.0
MACHINE_MODEL: RK3399
MACHINE_ID: 007
MANUFACTURER: RK3399
MAGIC: 0x5041524B
ATAG: 0x00200800
MACHINE: 3399
CHECK_MASK: 0x80
PWR_HLD: 0,0,A,0,1
#KERNEL_IMG: 0x00280000
#FDT_NAME: rk-kernel.dtb
#RECOVER_KEY: 1,1,0,20,0
#in section; per section 512(0x200) bytes
CMDLINE: androidboot.baseband=N/A androidboot.selinux=enforcing androidboot.hardware=rk30board androidboot.console=ttyFIQ0 init=/init mtdparts=rk29xxnand:0x00002000@0x00002000(uboot),0x00002000@0x00004000(trust),0x00002000@0x00006000(misc),0x00008000@0x00008000(resource),0x0000C000@0x00010000(kernel),0x00010000@0x0001C000(boot),0x00020000@0x0002C000(recovery),0x00038000@0x0004C000(backup),0x00140000@0x00084000(cache),0x00300000@0x001C4000(system),0x00008000@0x004C4000(metadata),0x00000040@0x004CC000(verity_mode),0x00002000@0x004CC040(baseparamer),0x00000400@0x004CE040(frp),-@0x004CE440(userdata)
weiqifa@ubuntu:~/repoRk3399_yan4_dev/device/rockchip/rk3399/rk3399_mid$
```
## 解释
**0x00002000****@****0x00002000**(uboot)
@后面指的是起始地址，前面指的是分区的大小，0x00002000+0x00002000等于下一个分区的起始地址**0x00004000**


