
# Android 亮屏速度分析总结 - 嵌入式Linux - CSDN博客

2019年01月11日 11:30:46[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：201



## 前面聊的
最近在调试项目的亮屏速度，我们希望在按下power键后到亮屏这个时间能达到500MS以内，在Rockchip 3399和3288上面的时间都不能达到要求，因此引发了一系列的调试之路。
## 计算按下power键到亮屏的时间
Android 唤醒时间统计
刚开始的时候，我只在android阶段统计时间，也能看到时间的差异，但是不是最准确的，我统计的时间日志如下
`01-18 09:13:40.992 683 772 D SurfaceControl: Excessive delay in setPowerMode(): 743ms
01-18 09:13:45.304 683 772 D SurfaceControl: Excessive delay in setPowerMode(): 757ms
01-18 09:13:49.559 683 772 D SurfaceControl: Excessive delay in setPowerMode(): 725ms
01-18 09:18:27.461 683 772 D SurfaceControl: Excessive delay in setPowerMode(): 741ms
01-18 09:18:32.766 683 772 D SurfaceControl: Excessive delay in setPowerMode(): 743ms
01-18 09:18:35.861 683 772 D SurfaceControl: Excessive delay in setPowerMode(): 745ms
01-18 09:18:38.345 683 772 D SurfaceControl: Excessive delay in setPowerMode(): 733ms`Kernel从Power到亮屏的时间统计
后来同事中的精英古总在他的代码上加入了从按下Power键到亮屏的时间，直接通过printk打印，代码如下
`diff --git a/drivers/gpu/drm/panel/panel-simple.c b/drivers/gpu/drm/panel/panel-simple.c
old mode 100644
new mode 100755
index 17c3b94..2b39662
--- a/drivers/gpu/drm/panel/panel-simple.c
+++ b/drivers/gpu/drm/panel/panel-simple.c
@@ -504,6 +504,7 @@ static int panel_simple_enable(struct drm_panel *panel)
        }
        p->enabled = true;
+       printk("%s exit\n", __func__);
        return 0;
 }
diff --git a/drivers/input/keyboard/rk_keys.c b/drivers/input/keyboard/rk_keys.c
old mode 100644
new mode 100755
index fed5ced..537b599
--- a/drivers/input/keyboard/rk_keys.c
+++ b/drivers/input/keyboard/rk_keys.c
@@ -134,6 +134,10 @@ static void keys_timer(unsigned long _data)
                key_dbg(pdata, "%skey[%s]: report event[%d] state[%d]\n",
                        button->type == TYPE_ADC ? "adc" : "gpio",
                        button->desc, button->code, button->state);
+               if(strcmp(button->desc, "power") == 0)
+               printk("%skey[%s]: report event[%d] state[%d]\n",
+                       button->type == TYPE_ADC ? "adc" : "gpio",
+                       button->desc, button->code, button->state);
                input_event(input, EV_KEY, button->code, button->state);
                input_sync(input);
        }`
## 统计每个驱动的resume函数调用时间
上面的时间对我们调试非常有用，然后就需要细分到每个驱动的resume函数执行的时间，用的方法是我之前写过的，大概统计了下TP，LCD，sensor的resume时间，发现TP和LCD占用的时间非常多，然后跟同事一起看了下，同事把TP resume里面的代码用工作队列实现后速度明显有了提升。
然后有很长一段时间不知道干嘛，向打印其他每个驱动的resume时间，一直没找到方法，后面看到一个代码，非常有用。
kernel/drivers/base/power/main.c
`static void dpm_show_time(ktime_t starttime, pm_message_t state, char *info)                                                       
{                
    ktime_t calltime;
    u64 usecs64;
    int usecs;
    calltime = ktime_get();
    usecs64 = ktime_to_ns(ktime_sub(calltime, starttime));
    do_div(usecs64, NSEC_PER_USEC);
    usecs = usecs64;
    if (usecs == 0)
        usecs = 1; 
    pr_info("PM: %s%s%s of devices complete after %ld.%03ld msecs\n",
        info ?: "", info ? " " : "", pm_verb(state.event),
        usecs / USEC_PER_MSEC, usecs % USEC_PER_MSEC);
}`这个函数用来打印resume的函数消耗的时间，但是如何去触发打印这个函数呢?
一定保证设备进入深度睡眠，串口也进入深度睡眠，没有任何打印后。
执行以下命令
`echo N > /sys/module/printk/parameters/console_suspend
//使控制台在suspend最后才关闭，这样可以打印出休眠过程完整信息
echo 1 > /sys/power/pm_print_times
//使能调试变量`打印的LOG类似下面的
`[ 37.031413] bcmsdh_sdmmc_resume Exit
[ 37.082174] PM: resume of devices complete after 78.589 msecs
[ 37.085277] [BT_RFKILL]: ** disable irq
[ 37.087645] Restarting tasks ...`
## 修改Lcd配置减小resume时间
古总在调试过程中展现了非常厉害的功底，第一步就是修改了LCD的参数，让亮屏时间加快。修改如下
`--- a/arch/arm/boot/dts/rk3288-pad.dts
+++ b/arch/arm/boot/dts/rk3288-pad.dts
@@ -169,10 +169,10 @@
                dsi,lanes = <4>;
                prepare-delay-ms = <20>;
-               init-delay-ms = <20>;
-               enable-delay-ms = <100>;
-               disable-delay-ms = <20>;
-               unprepare-delay-ms = <20>;
+               //init-delay-ms = <20>;
+               enable-delay-ms = <1>;
+               disable-delay-ms = <1>;
+               unprepare-delay-ms = <1>;
                panel-init-sequence = [
                        15 32 02 8F A5
                        15 01 02 83 00`
## 修改DRM 超时时间减小唤醒时间
这是最关键的，DRM框架非常复杂，RK也是从开源的DRM移植过来使用，在DRM部分有个时间导致问题，最终跟RK拿到最新的patch让唤醒时间直接加速500MS.
我们在日志下发现问题，并给询问了RK，最终发现这部分代码没有更新到最新的部分。
> hi rk:

> 为什么亮屏的时候有时候会打印这句VOP等待超时？请问下这是什么意思。

> [ 1211.293492] rockchip-vop ff930000.vop: wait win close timeout

> [ 1211.293514] rockchip-vop ff930000.vop: [drm:vop_crtc_enable] Update mode to 1200*1920, close all win

> 有时候却不会打印。
[ 1216.423283] rockchip-vop ff930000.vop: [drm:vop_crtc_enable] Update mode to 1200*1920, close all win [ 1223.899741] rockchip-vop ff930000.vop: [drm:vop_crtc_enable] Update mode to 1200*1920, close all win
[ 1234.386252] rockchip-vop ff930000.vop: [drm:vop_crtc_enable] Update mode to 1200*1920, close all win
代码如下
`--- a/drivers/gpu/drm/rockchip/rockchip_drm_vop.c
+++ b/drivers/gpu/drm/rockchip/rockchip_drm_vop.c
@@ -139,6 +139,9 @@
 #define to_vop_win(x) container_of(x, struct vop_win, base)
 #define to_vop_plane_state(x) container_of(x, struct vop_plane_state, base)
+/*add by VENDOR_PATCH for seep up the drm vop driver at 2018/1/18 for RK Defect #191554, VENDOR_PATCH PAD100-193*/
+#define VENDOR_PATCH
+
 struct vop_zpos {
        int win_id;
        int zpos;
@@ -868,9 +871,15 @@ static void vop_disable_all_planes(struct vop *vop)
        vop_disable_allwin(vop);
        vop_cfg_done(vop);
+#ifdef VENDOR_PATCH
        ret = readx_poll_timeout_atomic(vop_is_allwin_disabled,
                                        vop, active, active,
+                                       0, 100 * 1000);
+#else
+       ret = readx_poll_timeout_atomic(vop_is_allwin_disabled,
+                               vop, active, active,
                                        0, 500 * 1000);
+#endif
        if (ret)
                dev_err(vop->dev, "wait win close timeout\n");
 }
@@ -2215,20 +2224,36 @@ static size_t vop_crtc_bandwidth(struct drm_crtc *crtc,
        u16 htotal = adjusted_mode->crtc_htotal;
        u16 vdisplay = adjusted_mode->crtc_vdisplay;
        int clock = adjusted_mode->crtc_clock;
+#ifndef VENDOR_PATCH
        struct vop *vop = to_vop(crtc);
        const struct vop_data *vop_data = vop->data;
+#endif
        struct vop_plane_state *vop_plane_state;
        struct drm_plane_state *pstate;
        struct vop_bandwidth *pbandwidth;
        struct drm_plane *plane;
        u64 bandwidth;
        int i, cnt = 0;
+#ifdef VENDOR_PATCH
+       int plane_num = 0;
+#endif
        if (!htotal || !vdisplay)
                return 0;
+#ifndef VENDOR_PATCH
        pbandwidth = kmalloc_array(vop_data->win_size, sizeof(*pbandwidth),
                                   GFP_KERNEL);
+#else
+       for_each_plane_in_state(state, plane, pstate, i) {
+               if (pstate->crtc != crtc || !pstate->fb)
+                       continue;
+               plane_num++;
+       }
+       pbandwidth = kmalloc_array(plane_num, sizeof(*pbandwidth),
+                                  GFP_KERNEL);
+#endif
+
        if (!pbandwidth)
                return -ENOMEM;
@@ -2421,7 +2446,10 @@ static void vop_crtc_enable(struct drm_crtc *crtc)
        rockchip_set_system_status(sys_status);
        mutex_lock(&vop->vop_lock);
        vop_initial(crtc);
-
+#ifdef VENDOR_PATCH
+       vop_disable_allwin(vop);
+       VOP_CTRL_SET(vop, standby, 0);
+#endif
        VOP_CTRL_SET(vop, dclk_pol, 1);
        val = (adjusted_mode->flags & DRM_MODE_FLAG_NHSYNC) ?
                   0 : BIT(HSYNC_POSITIVE);
@@ -2549,8 +2577,9 @@ static void vop_crtc_enable(struct drm_crtc *crtc)
        /*
         * enable vop, all the register would take effect when vop exit standby
         */
+#ifndef VENDOR_PATCH
        VOP_CTRL_SET(vop, standby, 0);
-
+#endif
        enable_irq(vop->irq);
        drm_crtc_vblank_on(crtc);
        mutex_unlock(&vop->vop_lock);`
## 休眠唤醒流程图
从网上拷贝了个休眠唤醒的流程图，如果以后有问题需要分析的话，可以跟进这个流程去排查。
![](https://img-blog.csdn.net/20141117140319765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfNjk1NTM4MDA3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](http://pji2tpxwy.bkt.clouddn.com/Fko92726fGh1Qy0Dz5PgCyyHCZtf)参考
https://www.spinics.net/lists/linux-pm/msg24063.html
http://eeepage.info/pm-failed-suspend-wakeup_event/
https://blog.csdn.net/qq_695538007/article/details/41208135
https://blog.csdn.net/jacobywu/article/details/24735521

