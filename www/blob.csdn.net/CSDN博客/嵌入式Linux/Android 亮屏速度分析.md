
# Android 亮屏速度分析 - 嵌入式Linux - CSDN博客

2019年01月11日 17:57:24[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：41


![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/Qof5hj3zMPcjDzgKoqsiad61ic39l4oQncdsBl2a5R871wAcMNiaMBZsQhmElmqZydtAR1xNvynEWo55gSPhibrnBw/640?wx_fmt=png)

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Qof5hj3zMPeiauStJMDaoxoicA9rgjdWtkKj2ZZA5bCEcib9Z8bkfqBBZoO2eItzHChicibqZlWc4dP9LHIJIUGGLicg/640?wx_fmt=jpeg)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/Qof5hj3zMPeiauStJMDaoxoicA9rgjdWtkuKydNT3aFnmxf1FpO3NglyxNtwmtSc9u4EdreSJL7c0icS65J9KUOibA/640?wx_fmt=png)
## 前面聊的
最近在调试项目的亮屏速度，我们希望在按下power键后到亮屏这个时间能达到500MS以内，在Rockchip 3399和3288上面的时间都不能达到要求，因此引发了一系列的调试之路。
## 计算按下power键到亮屏的时间
Android 唤醒时间统计
刚开始的时候，我只在android阶段统计时间，也能看到时间的差异，但是不是最准确的，我统计的时间日志如下
```python
01
```
```python
-18
```
```python
09
```
```python
:13
```
```python
:40.992
```
```python
683 772
```
```python
D
```
```python
SurfaceControl
```
```python
:
```
```python
Excessive
```
```python
delay
```
```python
in
```
```python
setPowerMode
```
```python
(): 743
```
```python
ms
```

```python
01
```
```python
-18
```
```python
09
```
```python
:13
```
```python
:45.304
```
```python
683 772
```
```python
D
```
```python
SurfaceControl
```
```python
:
```
```python
Excessive
```
```python
delay
```
```python
in
```
```python
setPowerMode
```
```python
(): 757
```
```python
ms
```

```python
01
```
```python
-18
```
```python
09
```
```python
:13
```
```python
:49.559
```
```python
683 772
```
```python
D
```
```python
SurfaceControl
```
```python
:
```
```python
Excessive
```
```python
delay
```
```python
in
```
```python
setPowerMode
```
```python
(): 725
```
```python
ms
```

```python
01
```
```python
-18
```
```python
09
```
```python
:18
```
```python
:27.461
```
```python
683 772
```
```python
D
```
```python
SurfaceControl
```
```python
:
```
```python
Excessive
```
```python
delay
```
```python
in
```
```python
setPowerMode
```
```python
(): 741
```
```python
ms
```

```python
01
```
```python
-18
```
```python
09
```
```python
:18
```
```python
:32.766
```
```python
683 772
```
```python
D
```
```python
SurfaceControl
```
```python
:
```
```python
Excessive
```
```python
delay
```
```python
in
```
```python
setPowerMode
```
```python
(): 743
```
```python
ms
```

```python
01
```
```python
-18
```
```python
09
```
```python
:18
```
```python
:35.861
```
```python
683 772
```
```python
D
```
```python
SurfaceControl
```
```python
:
```
```python
Excessive
```
```python
delay
```
```python
in
```
```python
setPowerMode
```
```python
(): 745
```
```python
ms
```

```python
01
```
```python
-18
```
```python
09
```
```python
:18
```
```python
:38.345
```
```python
683 772
```
```python
D
```
```python
SurfaceControl
```
```python
:
```
```python
Excessive
```
```python
delay
```
```python
in
```
```python
setPowerMode
```
```python
(): 733
```
```python
ms
```
Kernel从Power到亮屏的时间统计
后来同事中的精英古总在他的代码上加入了从按下Power键到亮屏的时间，直接通过printk打印，代码如下
```python
diff --git a/drivers/gpu/drm/panel/panel-simple.c b/drivers/gpu/drm/panel/panel-simple.c
```

```python
old mode
```
```python
100644
```

```python
new
```
```python
mode
```
```python
100755
```

```python
index
```
```python
17
```
```python
c3b94.
```
```python
.2
```
```python
b39662
```

```python
--- a/drivers/gpu/drm/panel/panel-simple.c
```

```python
+++ b/drivers/gpu/drm/panel/panel-simple.c
```

```python
@@
```
```python
-504
```
```python
,
```
```python
6
```
```python
+
```
```python
504
```
```python
,
```
```python
7
```
```python
@@
```
```python
static
```
```python
int panel_simple_enable(struct drm_panel *panel)
```

```python
}
```

```python
p->enabled =
```
```python
true
```
```python
;
```

```python
+       printk(
```
```python
"%s exit\n"
```
```python
, __func__);
```

```python
return
```
```python
0
```
```python
;
```

```python
}
```

```python
diff --git a/drivers/input/keyboard/rk_keys.c b/drivers/input/keyboard/rk_keys.c
```

```python
old mode
```
```python
100644
```

```python
new
```
```python
mode
```
```python
100755
```

```python
index fed5ced.
```
```python
.537
```
```python
b599
```

```python
--- a/drivers/input/keyboard/rk_keys.c
```

```python
+++ b/drivers/input/keyboard/rk_keys.c
```

```python
@@
```
```python
-134
```
```python
,
```
```python
6
```
```python
+
```
```python
134
```
```python
,
```
```python
10
```
```python
@@
```
```python
static
```
```python
void keys_timer(unsigned long _data)
```

```python
key_dbg(pdata,
```
```python
"%skey[%s]: report event[%d] state[%d]\n"
```
```python
,
```

```python
button->type == TYPE_ADC ?
```
```python
"adc"
```
```python
:
```
```python
"gpio"
```
```python
,
```

```python
button->desc, button->code, button->state);
```

```python
+
```
```python
if
```
```python
(strcmp(button->desc,
```
```python
"power"
```
```python
) ==
```
```python
0
```
```python
)
```

```python
+               printk(
```
```python
"%skey[%s]: report event[%d] state[%d]\n"
```
```python
,
```

```python
+                       button->type == TYPE_ADC ?
```
```python
"adc"
```
```python
:
```
```python
"gpio"
```
```python
,
```

```python
+                       button->desc, button->code, button->state);
```

```python
input_event(input, EV_KEY, button->code, button->state);
```

```python
input_sync(input);
```

```python
}
```

## 统计每个驱动的resume函数调用时间
上面的时间对我们调试非常有用，然后就需要细分到每个驱动的resume函数执行的时间，用的方法是我之前写过的，大概统计了下TP，LCD，sensor的resume时间，发现TP和LCD占用的时间非常多，然后跟同事一起看了下，同事把TP resume里面的代码用工作队列实现后速度明显有了提升。
然后有很长一段时间不知道干嘛，想打印其他每个驱动的resume时间，一直没找到方法，后面看到一个代码，非常有用。
kernel/drivers/base/power/main.c
```python
static
```
```python
void
```
```python
dpm_show_time
```
```python
(
```
```python
ktime_t
```
```python
starttime,
```
```python
pm_message_t
```
```python
state,
```
```python
char
```
```python
*info)
```

```python
{
```

```python
ktime_t
```
```python
calltime;
```

```python
u64 usecs64;
```

```python
int
```
```python
usecs;
```

```python
calltime = ktime_get();
```

```python
usecs64 = ktime_to_ns(ktime_sub(calltime, starttime));
```

```python
do_div(usecs64, NSEC_PER_USEC);
```

```python
usecs = usecs64;
```

```python
if
```
```python
(usecs ==
```
```python
0
```
```python
)
```

```python
usecs =
```
```python
1
```
```python
;
```

```python
pr_info(
```
```python
"PM: %s%s%s of devices complete after %ld.%03ld msecs\n"
```
```python
,
```

```python
info ?:
```
```python
""
```
```python
, info ?
```
```python
" "
```
```python
:
```
```python
""
```
```python
, pm_verb(state.event),
```

```python
usecs / USEC_PER_MSEC, usecs % USEC_PER_MSEC);
```

```python
}
```
这个函数用来打印resume的函数消耗的时间，但是如何去触发打印这个函数呢?
一定保证设备进入深度睡眠，串口也进入深度睡眠，没有任何打印后。
执行以下命令

```python
echo N >
```
```python
/sys/m
```
```python
odule/printk/parameters/console_suspend
```

```python
//使控制台在suspend最后才关闭，这样可以打印出休眠过程完整信息
```

```python
echo
```
```python
1
```
```python
>
```
```python
/sys/
```
```python
power/pm_print_times
```

```python
//使能调试变量
```
打印的LOG类似下面的
```python
[ 37.031413]
```
```python
bcmsdh_sdmmc_resume
```
```python
Exit
```

```python
[ 37.082174]
```
```python
PM
```
```python
:
```
```python
resume
```
```python
of
```
```python
devices
```
```python
complete
```
```python
after
```
```python
78
```
```python
.589
```
```python
msecs
```

```python
[ 37.085277]
```
```python
[BT_RFKILL]
```
```python
: **
```
```python
disable
```
```python
irq
```

```python
[ 37.087645]
```
```python
Restarting
```
```python
tasks
```
```python
...
```

## 修改Lcd配置减小resume时间
古总在调试过程中展现了非常厉害的功底，第一步就是修改了LCD的参数，让亮屏时间加快。修改如下
```python
--- a/arch/arm/boot/dts/rk3288-pad.dts
```

```python
+++ b/arch/arm/boot/dts/rk3288-pad.dts
```

```python
@@ -169,10 +169,10 @@
```

```python
dsi,lanes = <4>;
```

```python
prepare-delay-ms = <20>;
```

```python
-               init-delay-ms = <20>;
```

```python
-               enable-delay-ms = <100>;
```

```python
-               disable-delay-ms = <20>;
```

```python
-               unprepare-delay-ms = <20>;
```

```python
+               //init-delay-ms = <20>;
```

```python
+               enable-delay-ms = <1>;
```

```python
+               disable-delay-ms = <1>;
```

```python
+               unprepare-delay-ms = <1>;
```

```python
panel-init-sequence = [
```

```python
15 32 02 8F A5
```

```python
15 01 02 83 00
```

## 修改DRM 超时时间减小唤醒时间
这是最关键的，DRM框架非常复杂，RK也是从开源的DRM移植过来使用，在DRM部分有个时间超时导致问题，最终跟RK拿到最新的patch让唤醒时间直接加速500MS.
我们在日志下发现问题，并给询问了RK，最终发现这部分代码没有更新到最新的部分。
> hi rk:

> 为什么亮屏的时候有时候会打印这句VOP等待超时？请问下这是什么意思。

> [ 1211.293492] rockchip-vop ff930000.vop: wait win close timeout

> [ 1211.293514] rockchip-vop ff930000.vop: [drm:vop_crtc_enable] Update mode to 1200*1920, close all win

> 有时候却不会打印。

> [ 1216.423283] rockchip-vop ff930000.vop: [drm:vop_crtc_enable] Update mode to 1200
> 1920, close all win[ 1223.899741] rockchip-vop ff930000.vop: [drm:vop_crtc_enable] Update mode to 1200
> 1920, close all win

> [ 1234.386252] rockchip-vop ff930000.vop: [drm:vop_crtc_enable] Update mode to 1200*1920, close all win
patch代码如下
```python
--- a/drivers/gpu/drm/rockchip/rockchip_drm_vop.c
```

```python
+++ b/drivers/gpu/drm/rockchip/rockchip_drm_vop.c
```

```python
@@
```
```python
-139
```
```python
,
```
```python
6
```
```python
+
```
```python
139
```
```python
,
```
```python
9
```
```python
@@
```

```python
#
```
```python
define
```
```python
to_vop_win(x) container_of(x, struct vop_win, base)
```

```python
#
```
```python
define
```
```python
to_vop_plane_state(x) container_of(x, struct vop_plane_state, base)
```

```python
+
```
```python
/*add by VENDOR_PATCH for seep up the drm vop driver at 2018/1/18 for RK Defect #191554, VENDOR_PATCH PAD100-193*/
```

```python
+
```
```python
#
```
```python
define
```
```python
VENDOR_PATCH
```

```python
+
```

```python
struct
```
```python
vop_zpos {
```

```python
int
```
```python
win_id;
```

```python
int
```
```python
zpos;
```

```python
@@
```
```python
-868
```
```python
,
```
```python
9
```
```python
+
```
```python
871
```
```python
,
```
```python
15
```
```python
@@
```
```python
static
```
```python
void
```
```python
vop_disable_all_planes
```
```python
(
```
```python
struct
```
```python
vop *vop
```
```python
)
```

```python
vop_disable_allwin
```
```python
(
```
```python
vop
```
```python
)
```
```python
;
```

```python
vop_cfg_done(vop);
```

```python
+
```
```python
#ifdef VENDOR_PATCH
```

```python
ret = readx_poll_timeout_atomic(vop_is_allwin_disabled,
```

```python
vop, active, active,
```

```python
+
```
```python
0
```
```python
,
```
```python
100
```
```python
*
```
```python
1000
```
```python
);
```

```python
+
```
```python
#
```
```python
else
```

```python
+       ret = readx_poll_timeout_atomic(vop_is_allwin_disabled,
```

```python
+                               vop, active, active,
```

```python
0
```
```python
,
```
```python
500
```
```python
*
```
```python
1000
```
```python
);
```

```python
+
```
```python
#
```
```python
endif
```

```python
if
```
```python
(ret)
```

```python
dev_err(vop->dev,
```
```python
"wait win close timeout\n"
```
```python
);
```

```python
}
```

```python
@@
```
```python
-2215
```
```python
,
```
```python
20
```
```python
+
```
```python
2224
```
```python
,
```
```python
36
```
```python
@@
```
```python
static
```
```python
size_t
```
```python
vop_crtc_bandwidth
```
```python
(
```
```python
struct
```
```python
drm_crtc *crtc,
```

```python
u16 htotal = adjusted_mode->crtc_htotal;
```

```python
u16 vdisplay = adjusted_mode->crtc_vdisplay;
```

```python
int
```
```python
clock = adjusted_mode->crtc_clock;
```

```python
+#ifndef VENDOR_PATCH
```

```python
struct
```
```python
vop *vop = to_vop(crtc
```
```python
)
```
```python
;
```

```python
const
```
```python
struct
```
```python
vop_data *vop_data = vop->data;
```

```python
+
```
```python
#
```
```python
endif
```

```python
struct
```
```python
vop_plane_state *vop_plane_state;
```

```python
struct
```
```python
drm_plane_state *pstate;
```

```python
struct
```
```python
vop_bandwidth *pbandwidth;
```

```python
struct
```
```python
drm_plane *plane;
```

```python
u64 bandwidth;
```

```python
int
```
```python
i, cnt =
```
```python
0
```
```python
;
```

```python
+
```
```python
#ifdef VENDOR_PATCH
```

```python
+
```
```python
int
```
```python
plane_num =
```
```python
0
```
```python
;
```

```python
+
```
```python
#
```
```python
endif
```

```python
if
```
```python
(!htotal || !vdisplay)
```

```python
return
```
```python
0
```
```python
;
```

```python
+
```
```python
#ifndef VENDOR_PATCH
```

```python
pbandwidth = kmalloc_array(vop_data->win_size,
```
```python
sizeof
```
```python
(*pbandwidth),
```

```python
GFP_KERNEL);
```

```python
+
```
```python
#
```
```python
else
```

```python
+       for_each_plane_in_state(state, plane, pstate, i) {
```

```python
+
```
```python
if
```
```python
(pstate->crtc != crtc || !pstate->fb)
```

```python
+
```
```python
continue
```
```python
;
```

```python
+               plane_num++;
```

```python
+       }
```

```python
+       pbandwidth = kmalloc_array(plane_num,
```
```python
sizeof
```
```python
(*pbandwidth),
```

```python
+                                  GFP_KERNEL);
```

```python
+
```
```python
#
```
```python
endif
```

```python
+
```

```python
if
```
```python
(!pbandwidth)
```

```python
return
```
```python
-ENOMEM;
```

```python
@@
```
```python
-2421
```
```python
,
```
```python
7
```
```python
+
```
```python
2446
```
```python
,
```
```python
10
```
```python
@@
```
```python
static
```
```python
void
```
```python
vop_crtc_enable
```
```python
(
```
```python
struct
```
```python
drm_crtc *crtc
```
```python
)
```

```python
rockchip_set_system_status
```
```python
(
```
```python
sys_status
```
```python
)
```
```python
;
```

```python
mutex_lock(&vop->vop_lock);
```

```python
vop_initial(crtc);
```

```python
-
```

```python
+
```
```python
#ifdef VENDOR_PATCH
```

```python
+       vop_disable_allwin(vop);
```

```python
+       VOP_CTRL_SET(vop, standby,
```
```python
0
```
```python
);
```

```python
+
```
```python
#
```
```python
endif
```

```python
VOP_CTRL_SET(vop, dclk_pol,
```
```python
1
```
```python
);
```

```python
val = (adjusted_mode->flags & DRM_MODE_FLAG_NHSYNC) ?
```

```python
0
```
```python
: BIT(HSYNC_POSITIVE);
```

```python
@@
```
```python
-2549
```
```python
,
```
```python
8
```
```python
+
```
```python
2577
```
```python
,
```
```python
9
```
```python
@@
```
```python
static
```
```python
void
```
```python
vop_crtc_enable
```
```python
(
```
```python
struct
```
```python
drm_crtc *crtc
```
```python
)
```

```python
/*
```

```python
* enable vop, all the register would take effect when vop exit standby
```

```python
*/
```

```python
+#ifndef VENDOR_PATCH
```

```python
VOP_CTRL_SET
```
```python
(
```
```python
vop, standby,
```
```python
0
```
```python
)
```
```python
;
```

```python
-
```

```python
+
```
```python
#
```
```python
endif
```

```python
enable_irq(vop->irq);
```

```python
drm_crtc_vblank_on(crtc);
```

```python
mutex_unlock(&vop->vop_lock);
```


## 修改QOS相关代码
QOS为Quality Of Service（服务质量）的简称，对PM QoS而言，表示Linux kernel电源管理相关的服务质量。那到底什么是服务质量呢？
我们知道，Linux PM的主要功能，是节省功耗，但同时，会付出一定的性能代价，例如延迟（latency）增加、吞吐量（throughput）下降。可以把PM当作一种服务，把它对性能的影响，类比为服务的质量（QoS）。对性能的影响越大，QoS越低，反之越高。
我们可以这么认为，我们在某个时候需要增加代码的执行速度，就通过这个去控制CPU的运行策略，这样确保代码可以快速执行。
不过这个方法没有使用到，如果对某个resume时间不是十分满意，可以尝试这个方法。
## 休眠唤醒流程图
从网上拷贝了个休眠唤醒的流程图，如果以后有问题需要分析的话，可以跟进这个流程去排查。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Qof5hj3zMPcjDzgKoqsiad61ic39l4oQnccyS8qy7L8Ik1HBkg8Hib02yKKNaXl4PiaAYo5Cw1m6kQ5JrjrbsMEbgg/640?wx_fmt=jpeg)
## 参考
https://www.spinics.net/lists/linux-pm/msg24063.html
http://eeepage.info/pm-failed-suspend-wakeup_event/
https://blog.csdn.net/qq_695538007/article/details/41208135
https://blog.csdn.net/jacobywu/article/details/24735521

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/Qof5hj3zMPeiauStJMDaoxoicA9rgjdWtkuKydNT3aFnmxf1FpO3NglyxNtwmtSc9u4EdreSJL7c0icS65J9KUOibA/640?wx_fmt=png)
当你看到这里的时候，说明你已经阅读完上面的内容，不管怎样，感谢您有心或者无意的关注和支持，
觉得不错，请帮忙**转发****，****点好看**，您的每一次支持，我都将铭记于心
公众号接入了**AI**功能，随意回复任意消息哦（**比如笑话，天气**）
想获取学习资料，请点击状态栏**公众号福利**按钮


![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Qof5hj3zMPdjf4zGB2y66oSmGibcFIozabEFpsMLLROecoibJKwC177cvkgQcB7n7NLpNJLUSeGTKqUHAHQhJDxw/640?wx_fmt=jpeg)





