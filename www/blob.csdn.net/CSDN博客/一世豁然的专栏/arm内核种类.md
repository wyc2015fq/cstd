# arm内核种类 - 一世豁然的专栏 - CSDN博客





2015年04月13日 20:39:31[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1300








本博客转载于：[http://blog.chinaunix.net/uid-23670869-id-2391648.html](http://blog.chinaunix.net/uid-23670869-id-2391648.html)






|家族|架构|内核|特色|高速缓存 (I/D)/[MMU](http://baike.baidu.com/view/969924.htm)|常规 [MIPS](http://baike.baidu.com/view/120375.htm) 于 MHz|应用|
|----|----|----|----|----|----|----|
|ARM1|ARMv1|ARM1||无|||
|ARM2|ARMv2|ARM2|Architecture 2 加入了MUL（乘法）指令|无|4 MIPS @ 8MHz|Acorn Archimedes，Chessmachine|
|ARMv2a|ARM250|Integrated (完整的)MEMC (MMU)，图像与IO处理器。Architecture 2a 加入了SWP和SWPB（置换）指令。|无，MEMC1a|7 MIPS @ 12MHz|Acorn Archimedes| |
|ARM3|ARMv2a|ARM2a|首次在ARM架构上使用处理器高速缓存|均为4K|12 MIPS @ 25MHz|Acorn Archimedes|
|ARM6|ARMv3|ARM610|v3 架构首创支援寻址32位的内存（针对26位）|均为4K|28 MIPS @ 33MHz|Acorn Risc PC 600，Apple Newton|
|ARM7TDMI|ARMv4T|ARM7TDMI(-S)|三级流水线|无|15 MIPS @ 16.8 MHz|[Game Boy Advance](http://baike.baidu.com/view/853749.htm)，Nintendo DS，[iPod](http://baike.baidu.com/view/1165.htm)|
|||ARM710T||均为8KB, MMU|36 MIPS @ 40 MHz|Acorn Risc PC 700，Psion 5 series，Apple eMate 300|
|||ARM720T||均为8KB, MMU|60 MIPS @ 59.8 MHz|Zipit|
|||ARM740T||MPU|||
||ARMv5TEJ|ARM7EJ-S|Jazelle DBX|无|||
|ARM9TDMI|ARMv4T|ARM9TDMI|五级流水线|无|||
|||ARM920T||16KB/16KB, MMU|200 MIPS @ 180 MHz|[Armadillo](http://baike.baidu.com/view/1371072.htm)，GP32，[GP2X](http://baike.baidu.com/view/2886473.htm)（第一颗内核）, Tapwave Zodiac（[Motorola](http://baike.baidu.com/view/39480.htm) i. MX1）|
|||ARM922T||8KB/8KB, MMU|||
|||ARM940T||4KB/4KB, MPU||GP2X（第二颗内核）|
|ARM9E|ARMv5TE|ARM946E-S||可变动，tightly coupled memories, MPU||Nintendo DS，[Nokia](http://baike.baidu.com/view/10655.htm)[N-Gage](http://baike.baidu.com/view/753753.htm)Conexant 802.11 chips|
|||ARM966E-S||无高速缓存，TCMs||ST Micro STR91xF，包含Ethernet [2]|
|||ARM968E-S||无高速缓存，TCMs|||
||ARMv5TEJ|ARM926EJ-S|Jazelle DBX|可变动，TCMs, MMU|220 MIPS @ 200 MHz|移动电话：[Sony Ericsson](http://baike.baidu.com/view/82334.htm)（K, W系列）,Siemens 和[Benq](http://baike.baidu.com/view/4044.htm)（x65 系列和新版的）|
||ARMv5TE|ARM996HS|无振荡器处理器|无高速缓存，TCMs, MPU|||
|ARM10E|ARMv5TE|ARM1020E|(VFP)，六级流水线|32KB/32KB, MMU|||
|||ARM1022E|(VFP)|16KB/16KB, MMU|||
||ARMv5TEJ|ARM1026EJ-S|Jazelle DBX|可变动，MMU or MPU|||
|[XScale](http://baike.baidu.com/view/1447521.htm)|ARMv5TE|80200/IOP310/IOP315|I/O处理器||||
|||80219|||400/600MHz|Thecus N2100|
|||IOP321|||600 [BogoMips](http://baike.baidu.com/view/1086713.htm) @ 600 MHz|Iyonix|
|||IOP33x|||||
|||IOP34x|1-2核，RAID加速器|32K/32K L1, 512K L2, MMU|||
|||PXA210/PXA250|应用处理器，七级流水线|||Zaurus SL-5600|
|||PXA255||32KB/32KB, MMU|400 BogoMips @ 400 MHz|Gumstix，Palm Tungsten E2|
|||PXA26x|||可达 400 MHz|Palm Tungsten T3|
|||PXA27x|||800 MIPS @ 624 MHz|[HTC](http://baike.baidu.com/view/972605.htm) Universal, Zaurus SL-C1000,3000,3100,3200, Dell Axim x30, x50，和 x51 系列|
|||PXA800(E)F|||||
|||Monahans|||1000 MIPS @ 1.25 GHz||
|||PXA900||||Blackberry 8700, Blackberry Pearl (8100)|
|||IXC1100|Control Plane Processor||||
|||IXP2400/IXP2800|||||
|||IXP2850|||||
|||IXP2325/IXP2350|||||
|||IXP42x||||NSLU2|
|||IXP460/IXP465|||||
|ARM11|ARMv6|ARM1136J(F)-S|SIMD, Jazelle DBX, (VFP)，八级流水线|可变动，MMU|?? @ 532-665MHz (i.MX31 SoC)|[Nokia N93](http://baike.baidu.com/view/16317.htm)，[Zune](http://baike.baidu.com/view/575212.htm)，[Nokia N800](http://baike.baidu.com/view/1288725.htm)|
|ARMv6T2|ARM1156T2(F)-S|SIMD, Thumb-2, (VFP)，九级流水线|可变动，MPU||| |
|ARMv6KZ|ARM1176JZ(F)-S|SIMD, Jazelle DBX, (VFP)|可变动，MMU+TrustZone||| |
|ARMv6K|ARM11 MPCore|1-4核对称多处理器，SIMD, Jazelle DBX, (VFP)|可变动，MMU||| |
|Cortex|ARMv7-A|Cortex-A8|Application profile, VFP, NEON, Jazelle RCT, Thumb-2, 13-stage pipeline|可变动 (L1+L2), MMU+TrustZone|up to 2000（2.0 DMIPS/MHz 从600 MHz到超过1 GHz的速度）|Texas Instruments OMAP3|
||ARMv7-R|Cortex-R4(F)|Embedded profile, (FPU)|可变动高速缓存，MMU可选配|600 DMIPS|[Broadcom](http://baike.baidu.com/view/468426.htm) is a user|
||ARMv7-M|Cortex-M3|Microcontroller profile|无高速缓存，(MPU)|120 DMIPS @ 100MHz|Luminary Micro[3] 微控制器家族|




