# arm平台和相关的矩阵运算库 - xmdxcsj的专栏 - CSDN博客





2016年11月24日 21:07:17[xmucas](https://me.csdn.net/xmdxcsj)阅读数：3015








## 一、arm平台

ARM(Acorn RISC Machine,后改为Advanced RISC Machine)是一个32位精简指令集（RISC）处理器架构，其广泛地使用在许多嵌入式系统设计。类比于服务端的X86，其他的架构还有MIPS/PowerPC/DSP。差异参考[这篇博文](http://navigating.blogbus.com/logs/67535773.html)

### [arm处理器架构](https://www.arm.com/zh/products/processors/instruction-set-architectures/)

![arm架构演化](https://www.arm.com/zh/assets/images/roadmap/V5_to_V8_Architecture.jpg)

包括ARMv5/ARMv6/ARMv7-A/ARMv8-A等型号，对应的指令集和性能不同
### arm内核

ARM在最初的处理器型号都用数字命名，最后一个是ARM11系列，在应用ARMv7架构后，推出了Cortex这一系列，老式的则命名为Classic系列。 

Cortex系列包括： 

- “应用”配置: Cortex-A 系列 

- “嵌入式”配置: Cortex-R 系列 

- “微处理器”配置: ARM Cortex-M 系列.
不同arm内核型号对应的架构类型可以参考[ARM处理器内核](https://en.wikipedia.org/wiki/List_of_ARM_microarchitectures)

### arm bit width

有32-bit和64/32-bit两种，和arm处理器架构的对应关系可以参考[表格](https://en.wikipedia.org/wiki/ARM_architecture)

ARMv8-A以后才开始支持64bit，之前只有32bit。

### 指令集

比较关注几个指令集： 

1. SIMD 

ARMv6架构引入 

2. NEON 

也叫Advanced SIMD，使用SIMD指令集提供运算加速。 

在ARMv7-A引入，所有的Cortex-A8设备都支持，部分Cortex-9A设备支持。 
[ProjectNe10](https://github.com/projectNe10/Ne10)是对多种函数使用NEON和c加速的开源库。
### ABI

参考[官方文档](https://developer.android.com/ndk/guides/abis.html#sa)，不同的CPU支持不同的指令集，CPU和指令集组合具有自己的ABI(Application Binary Interface). 

ABI定义了application的机器码在运行时如何跟系统进行交互，包括 

- CPU指令集 

- 存储和加载的大小端 

- 可执行文件和库文件的格式 

- 代码和系统之间数据传输的规则 

- 可用函数符号列表
ABI的种类和支持的指令集参考ABI，参考[官方文档](https://developer.android.com/ndk/guides/abis.html#sa)

1. armeabi 

至少支持ARMv5TE指令集的CPU，支持软浮点运算（不支持硬件辅助的浮点计算） 

2. armeabi-v7a 

针对ARMv7-A架构的cpu，支持硬件浮点运算 

3. arm64-v8a 

针对ARMv8而且支持AArch64的cpu，包含NEON和VFPv4指令集。
## 二、开源矩阵库

## eigen

可以使用eigen计算矩阵运算，参考[博文](http://blog.csdn.net/xmdxcsj/article/details/52643891)

## openblas

android端使用openblas库，参考[官方文档](https://github.com/xianyi/OpenBLAS/wiki/How-to-build-OpenBLAS-for-Android)








