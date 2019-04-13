
# rk3188开机失败（ump_file_open() 251） - 嵌入式Linux - CSDN博客

2016年08月24日 11:41:34[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：983


**刚开始配置rk3188板子的时候，**
**基本配置，RK3188 RK616 ACT8846 8G的flash,**
**开机的时候提示如下：**


```python
<6>[   18.729270] request_suspend_state: wakeup (0->0) at 18680347129 (1970-01-01 00:00:18.671397630 UTC)
<4>[   23.810524] UMP: ERR: /home/xxm/project/mali_git/driver/src/devicedrv/ump/linux/ump_kernel_linux.c
<4>[   23.810593]            ump_file_open() 251
<4>[   23.810634] Minor not zero in ump_file_open()
<4>[   23.810662] 
<4>[   23.811502] UMP: ERR: /home/xxm/project/mali_git/driver/src/devicedrv/ump/linux/ump_kernel_linux.c
<4>[   23.811564]            ump_file_open() 251
<4>[   23.811591] Minor not zero in ump_file_open()
<4>[   23.811619] 
<6>[   23.990544] request_suspend_state: wakeup (0->0) at 23941622629 (1970-01-01 00:00:23.932673130 UTC)
<4>[   29.040916] UMP: ERR: /home/xxm/project/mali_git/driver/src/devicedrv/ump/linux/ump_kernel_linux.c
<4>[   29.040968]            ump_file_open() 251
<4>[   29.041005] Minor not zero in ump_file_open()
<4>[   29.041025] 
<4>[   29.041583] UMP: ERR: /home/xxm/project/mali_git/driver/src/devicedrv/ump/linux/ump_kernel_linux.c
<4>[   29.041627]            ump_file_open() 251
<4>[   29.041647] Minor not zero in ump_file_open()
<4>[   29.041667] 
<6>[   29.201502] request_suspend_state: wakeup (0->0) at 29152579130 (1970-01-01 00:00:29.143630006 UTC)
```
**然后RK那边让我们修改了ACT8846的VCCio**


```python
{
name          = "act_ldo8",   //vcc28_cif
.min_uv          = 3300000,
.max_uv         = 3300000,
},
```
**硬件回复**
因为我把DCDC4 设置成了主电压，然后很多IO口和供电都需要3.0V以上，系统就将PMU DCDC4 的电压设置了3.3V



