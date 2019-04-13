
# Linux加载vmlinux调试 - 嵌入式Linux - CSDN博客

2015年03月12日 16:01:04[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：4726



## 使用gdb加载内核符号表
## arm-eabi-gdb out/target/product/msm8625/obj/KERNEL_OBJ/vmlinux

## 在内核的.config里面要打开 DEBUG_INFO和DEBUG_VM
定位故障代码
(gdb) l * qrd7627a_add_io_devices+0x100
0xc07cd05c is in qrd7627a_add_io_devices (/home/yejialong/GH700C/kernel/arch/arm/mach-msm/msm8x25/goso-msm7627a-io.c:1851).
1846            } else if (machine_is_msm8625q_skud() || machine_is_msm8625q_evbd()) {
1847        \#ifndef CONFIG_CALA02
1848                    platform_device_register(&pmic_mpp_leds_pdev_skud);
1849        \#endif
1850                    /* enable the skud flash and torch by gpio leds driver */
1851                    platform_device_register(&gpio_flash_skud);
1852            } else if (machine_is_msm8625q_skue()) {
1853                     /* enable the skue flashlight by gpio leds driver */
1854                    platform_device_register(&gpio_flash_skue);
1855            }


## 下面是mtk平台加载的方法，但是不管哪个平台都是差不多的，道理都是一样的。
weiqifa@weiqifa-Inspiron-3847:~/weiqifa/tm100$ ./prebuilts/gcc/linux-x86/arm/arm-eabi-4.7/bin/arm-eabi-gdb ./out/target/product/tm100/obj/KERNEL_OBJ/vmlinux
GNU gdb (GDB) 7.3.1-gg2
Copyright (C) 2011 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "--host=x86_64-linux-gnu --target=arm-linux-android".
For bug reporting instructions, please see:
<http://source.android.com/source/report-bugs.html>...
Reading symbols from /home/weiqifa/weiqifa/tm100/out/target/product/tm100/obj/KERNEL_OBJ/vmlinux...done.
(gdb)

## rockchip rk3399调试vmlinux
./prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9/bin/aarch64-linux-android-addr2line -f -e kernel/vmlinux
weiqifa@dev:~/rk3399_7in1$ ./prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9/bin/aarch64-linux-android-addr2line -f -e kernel/vmlinuxffffff8008459f3c
rk_iommu_domain_free
/data/weiqifa/rk3399_7in1/kernel/drivers/iommu/rockchip-iommu.c:1005 (discriminator 2)
weiqifa@dev:~/rk3399_7in1$
## RK平台tombstone调试
./prebuilts/gcc/linux-x86/arm/arm-eabi-4.8/bin/arm-eabi-addr2line -e ./out/target/product/rk3399_mid/symbols/system/lib/libspeexresampler.so 0x00002d54
/proc/self/cwd/external/speex/libspeex/resample.c:376
arm-eabi-gdb  先用命令找到这个东西，然后在去找去找到vmlinux  还有就是我**arm-eabi-4.7/ 这个版本才可以用，这个是我试出来的。**

