
# Bootloader传参数到Kernel - 嵌入式Linux - CSDN博客

2018年09月03日 17:22:26[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：181所属专栏：[微信公众号:嵌入式linux](https://blog.csdn.net/column/details/26885.html)




# 01 前言
这几天一直在考虑准备写点什么东西，本来想介绍下文件系统，不过文件系统是概念性的东西比较多，我自己也是看书摘抄的多，Bootloader传参数到Kernel，这个在我们开发中会经常遇到。
**bootloader有什么用？**
我突然想到，小时候去河里炸鱼，我们要自制鱼雷，把烟花里面的火药拆出来放到一个玻璃瓶里面，然后接上雷管露在外面，用泥封住瓶口，再用一些饭来吸引鱼，当我们看到鱼在吃饭的时候，放个土鱼雷上去，那个爽啊，水能飞起来老高。
**bootloader可以看成那个雷管引线，可以把那些火药燃爆起来。**
好了吹牛这么多了。

# 02 正文
![](https://img-blog.csdn.net/20180903172030362?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**一、bootloader基本概述**
Bootloader是嵌入式系统的引导加载程序，它是系统上电后运行的第一段程序，所以他的地址一般是0x0000000，其作用类似于PC机上的BIOS。Bootloader是依赖于硬件而实现的，特别是在嵌入式领域，为嵌入式系统建立一个通用的Bootloader是很困难的，但为了能达到启动Linux内核的目的，所有的Bootloader都必须具备以下功能：
1)初始化RAM
因为Linux内核一般都会在RAM中运行，所以在调用Linux内核之前Bootloader必须设置和初始化RAM，为调用Linux内核做好准备。初始化RAM的任务包括设置CPU的控制寄存器参数，以便能正常使用RAM以及检测RAM大小等。
2)初始化串口端口
在Linux的启动过程中有着非常重要的作用，它是Linux内核和用户交互的方式之一。Linux在启动过程中可以将信息通过串口输出，这样便可清楚的了解Linux的启动过程。虽然它并不是Bootloader必须要完成的工作，但是通过串口输出信息是调试Bootloader和Linux内核的强有力的工具，所以一般的Bootloader都会在执行过程中初始化一个串口作为调试端口。
3)检测处理器类型
Bootloader在调用Linux内核前必须检测系统的处理器类型，**并将其保存到某个常量中提供给Linux内核**。Linux内核在启动过程中会根据该处理器类型调用相应的初始化程序。
4)设置Linux启动参数
Bootloader在执行过程中必须设置和初始化Linux的内核启动参数。
5)调用Linux内核映像
Bootloader完成的最后一项工作便是调用Linux内核。如果Linux内核存放在Flash中，并且可直接在上面运行（这里的Flash指NorFlash），那么可直接跳转到内核中去执行。但由于在Flash中执行代码会有种种限制，而且速度也远不及RAM快，所以一般的嵌入式系统都是将Linux内核拷贝到RAM中，然后跳转到RAM中去执行。
**问：Linux内核代码是在RAM中执行还是在Flash中执行的？**
**答：嵌入式系统中广泛采用的非易失性存储器通常是 Flash，而 Flash 又分为 Nor Flash 和Nand Flash 两种。 它们之间的不同在于： Nor Flash 支持芯片内执行（XIP， eXecute In Place），这样代码可以在Flash上直接执行而不必拷贝到RAM中去执行。而Nand Flash并不支持XIP，所以要想执行 Nand Flash 上的代码，必须先将其拷贝到 RAM中去，然后跳到 RAM 中去执行。实际应用中的 bootloader根据所需功能的不同可以设计得很复杂，除完成基本的初始化系统和调用 Linux 内核等基本任务外，还可以执行很多用户输入的命令，比如设置 Linux 启动参数，给 Flash 分区等；也可以设计得很简单，只完成最基本的功能。**
**二、实现bootloader传参数到kernel**
问：为什么需要这个功能？
答：上面提到，bootloader需要传很多参数给kernel,对我们来说，我们在开发的时候，经常更换硬件芯片，我们需要在软件上实现兼容，就需要把芯片的信息从bootloader传到kernel。
**举个栗子：**
我们在实际做项目的时候，因为一个芯片采购难度较大的原因，更换了一个新的芯片，软件上需要做兼容。这时候我们就需要在bootloader获取当前是哪个芯片，告诉kernel，kernel再去配置哪个dts。

**1、BootLoader部分修改**
我们用的是rockchip的方案，bootloader的位置在
common/cmd_bootrk.c
修改代码如下
`diff --git a/common/cmd_bootrk.c b/common/cmd_bootrk.c
index 708bd1f..fbc5cc2 100755
--- a/common/cmd_bootrk.c
+++ b/common/cmd_bootrk.c
@@ -34,6 +34,7 @@ extern u8 g_increment;
 extern uchar g_CPUBDCDCChipID;
 extern uchar g_GPUDCDCChipID;
+extern u8 g_hardwareVersion;
 #if defined(CONFIG_RK3036_TVE) || defined(CONFIG_RK1000_TVE) ||defined(CONFIG_GM7122_TVE)
 extern int g_tve_pos;
@@ -50,6 +51,7 @@ extern bool is_rk81x_fg_init(void);
 extern int rkimage_load_image(rk_boot_img_hdr *hdr,
                const disk_partition_t *boot_ptn, const disk_partition_t *kernel_ptn);
+
 __weak int board_modify_fdt(void) { return 0; }
 /* Section for Android bootimage format support
@@ -577,6 +579,11 @@ static void rk_commandline_setenv(const char *boot_name, rk_boot_img_hdr *hdr, b
                         "%s adc.incre=%d", command_line, g_increment);
 #endif
+#ifdef CONFIG_AUDIO_HW_REV
+        snprintf(command_line, sizeof(command_line),
+                         "%s androidboot.wzb210_audio_hw_rev=%d", command_line, g_hardwareVersion);
+#endif
+
        snprintf(command_line, sizeof(command_line),
                         "%s CPUBDCDCChipID=%d", command_line, g_CPUBDCDCChipID);
(END)`
**在内核里面如何解析呢？**

`diff --git a/init/main.c b/init/main.c
index 86f5ce9e..7195439 100644
--- a/init/main.c
+++ b/init/main.c
@@ -231,6 +231,40 @@ static int __init loglevel(char *str)
 early_param("loglevel", loglevel);
+
+/* add DCDC  cmdline parse */
+int gCPUBDCDCChipID;
+static int __init early_cpub_rev(char *p)
+{
+       int ChipID_rev;
+       get_option(&p, &ChipID_rev);
+       gCPUBDCDCChipID = ChipID_rev;
+
+       printk("CPUBChipID:%d\n", gCPUBDCDCChipID);
+       return 0;
+}
+
+early_param("CPUBDCDCChipID", early_cpub_rev);
+
+
+int gGPUDCDCChipID;
+static int __init early_gpu_rev(char *p)
+{
+       int ChipID_rev;
+       get_option(&p, &ChipID_rev);
+       gGPUDCDCChipID = ChipID_rev;
+
+       printk("GPUChipID:%d\n", gGPUDCDCChipID);
+       return 0;
+}
+
+early_param("GPUDCDCChipID", early_gpu_rev);
+
+
+/* DCDC parse end */
+
+
+`
我们主要调用了early_param这个函数
**early_parm 是在 setup_arch--> parse_command中处理的， 只有在bootloader设置传递该参数， 或者在内核CONFIG_DEFAULT_COMMANDLINE中声明了， 内核在处理参数的时候才会执行相对应的函数。**
如果我们在bootloader里面没有传这个东西，在kernel用这个函数取，那肯定是取不到的了。
喜欢可以关注微信公众号：嵌入式Linux





​

