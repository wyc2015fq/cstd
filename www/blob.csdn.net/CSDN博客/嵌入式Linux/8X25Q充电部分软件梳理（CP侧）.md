
# 8X25Q充电部分软件梳理（CP侧） - 嵌入式Linux - CSDN博客

2014年10月30日 20:07:51[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：1897个人分类：[高通qualcomm8X25Q																](https://blog.csdn.net/weiqifa0/article/category/2673957)



分享链接：http://note.youdao.com/share/?id=4f6665eee6bad5ea27eee47f74bcfa4b&type=note
8X25Q充电部分软件梳理（CP侧）
作者：韦启发
目录
1、过放电池的充电阶段介绍...2
2、Autonomous充电介绍...5
3、USB充电器检测...6
4、在AMSS中的Autonomous充电...7
5、修改充电时上报的电池电压电池温度的时间频率...10
6、不插入电池连接USB线手机循环开关机，类似小米一样...11






1、过放电池的充电阶段介绍

If the battery voltage (VBAT) is:
VBAT < 3.2 V→Dead battery
3.2 V < VBAT < 3.4 V→Weak battery
VBAT > 3.4 V→Good battery
All the thresholds above are configurable
涓流充电：
涓流充电在电池电压低于3.2V时就会使能。
快速充电：
当电池电压在VWEAK
 3.2V和VDD_MAX4.2V之间时就会启动快速充电。
首先电池先充电到3.4V达到开机状态后然后手机会开机。
在涓流充电阶段和快速充电阶段都有一个定时器来管理，这样避免如果有坏电池的时候会恋永远充电下去造成错误。
代码位置：\modem_proc\core\wiredconnectivity\hsusb\ driver\src\chg\
使能过放电池充电：一定要打开这两个宏FEATURE_OEMSBL_USB_ BATT_CHG和FEATURE_HS_USB_FAST_CHARGE_VIA_HID
过放电池充电：这个充电阶段存在是因为电池电压低于3.4V的时候，并且USB线提供的电池不足以让手机开机，就会启动过放电池充电。
过放电池充电截止点：当电池电压达到3.4V就会停止过放电池充电阶段，让手机开机。
软件配置注意：
1.如果你想使能电池过放充电，那么请打开宏：FEATURE_HS_USB_CHG_BOOT_DCP_
 CHARGING
2.为了支持无效的充电器（D+和D-悬空的充电器），你可以打开宏：
FEATURE_HS_USB_CHG_BOOT_ SUPPORT_INVALCHGR
3.电池电压门限值设置，
CHG_BOOT_UCHG_VBATT_THRESH_TRICKLE 3.2V
FEATURE_HS_USB_CHG_BOOT_ SUPPORT_INVALCHGR 3.4V
4.补尝值 这个值根据需要设置，我理解是一个区间范围值：
HSU_CHG_BOOT_VBATT_WEAK_FAST_CHG_BOOST_OFFSET
软件代码流程：

Oemsbl_usb_chg_init()函数作用介绍：
设置弱电池电压门限：BOOT_WEAK_BATTERY_THRESHOLD
设置手机电流消耗限制：BOOT_I_DEVICE_POWER_ CONSUMPTION
Hsu_chg_boot()函数作用介绍：
chg_boot_uchg_pm_setup()设置电压门限，弱电池电压的两个门限
chg_boot_pm_get_batt_level()来检测电池的状态。
如果电池是过放电池，就会启动过放电池充电，使用函数hsu_chg_boot_loop()。
Hsu_chg_boot_loop()函数作用介绍：
不断的检测USB连接状态和电池电压是否超过门限值
如果断开USB线或者su_chg_boot_poll_usb_hid()函数检测到墙充，就会跳出这个循环。
如果电池电压大于设置门限值3.5V（用函数hsu_chg_boot_poll_batt_lvl()获取&&看具体设置）就会跳出循环。




2、Autonomous充电介绍
从上面的图中可以看出来，启动充电的前2个阶段（涓流充电和恒流充电）是一样的，之后会做一次选择，选择恒压充电还是脉冲充电，现在我们的项目最后那个阶段是脉冲充电。
可里也可以从我们的代码上可以看出来：
chg_autocharging_pulse_topoff.cpp编译这个文件走的是脉冲充电分支。
chg_autocharging_cv_topoff.cpp编译这个文件走的是恒压充电分支。
很明显我们走的是脉冲充电，恒流阶段进入脉冲充电也是由我们决定了，这里有一个设置的门限值，我们后面会再讲。

3、USB充电器检测
系统会判断两个充电的流程，一个是usb wall charger和usb host charger,看下面的图片。墙充电后的中断是CHG_CMD__IRQ_TRIGGERED,另一个是CHG_CMD__SYSTEM_STATUS_CHANGED。
墙充检测（实际上这个流程我们代码中并没有做判断这个分支）：
USB HOST检测：
4、在AMSS中的Autonomous充电
下面讲的都是在中断CHG_CMD__SYSTEM_STATUS_CHANGED进行解析的事件，在chg_config.h头文件中集合了一些参数的设置。
CHG_BATTERY_V_MAX：看名字说的是电池电压的最大值，通过这个值来判断电池是否已经满电，在开始充电的时候用pm_chg_vmaxsel_set(CHG_BATTERY_V_MAX);函数去设置他的值（以最后一次调用这个函数为有效值）。
CHG_VBATDET_V：电池检测的电压值，这个电压值的设置会产生一个中断，用这个中断来再设置进入脉冲充电方式，pm_chg_vbatdet_set（），这个函数去设置他的值(以最后一次调用这个函数为最终有效值)，如果达到这个设置值之后，就会产生PM_VBAT_DET_IRQ_HDL中断，在这个中断里面再去startPulseCharging(pulseConfig);启动脉冲充电，在startPulseCharging(pulseConfig);函数里设置脉冲充电的停止电压值，
CHG_PULSE_V_MAX：脉冲充电的最大电压值，startPulseCharging(pulseConfig)里面会设置这个值（以最后一次调用这个函数为有效值）。
CHG_PULSE_VBATDET_V：停止脉冲充电电压值，startPulseCharging(pulseConfig)里面会设置这个值（以最后一次调用这个函数为有效值）。
CHG_CHARGER_I_MAX：墙充充电电流值.
CHG_UNKNOWN_I_MAX:未知充电方式的充电电流大小，一般设置为450MA，后面在软件上做了DPM（检测充电端的电压大小，动大的调整充电电流大小）检测，充电电流最大可以达到1000MA。
CHG_HOST_I_MAX：电脑充电的时候的充电电流大小，这个充电电流大小不要修改。
err |= pm_chg_vbatdet_set(CHG_VBATDET_V);这个设置的CHG_VBATDET_V电压值，当电池电压大于CHG_VBATDET_V电压时，会一直产生PM_VBAT_DET_IRQ_HDL中断，当电池电压小于CHG_VBATDET_V时，会产生PM_VBATT_DET_LOW_IRQ_HDL中断。当脉冲充电结束时，会产生的是PM_PULSE_CHG_DONE_IRQ_HDL中断。
PM_VBATT_DET_LOW_IRQ_HDL这个中断在我们的代码中是没有的，需要自己去先监听这个中断，在Chg_isr.cpp文件中加上的代码如下：
//owen.wei add start
pm_err_flag_type IrqEventGenerator::AddListenerForVbatdetLowIrq(ChargingEventListener *listener)
{
/* Listen to VBATDET IRQ */
addListener(listener, PM_VBATT_DET_LOW_IRQ_HDL);
pm_err_flag_type err = PM_ERR_FLAG__SUCCESS;
INTLOCK();
/* Disable the IRQ */
err = pm_clear_irq_multi_handle(PM_VBATT_DET_LOW_IRQ_HDL, isr);
/* Clear the IRQ. */
err = pm_clear_irq(PM_VBATT_DET_LOW_IRQ_HDL);
/* Enable the IRQ, pass the IRQ ID as data */
err = pm_set_irq_multi_handle(PM_VBATT_DET_LOW_IRQ_HDL,
isr,
(void*)PM_VBATT_DET_LOW_IRQ_HDL);
/* Find out if there was an PMIC API error */
if (err != PM_ERR_FLAG__SUCCESS)
{
MSG_ERROR("PMIC API ERROR(0x%x) DETECTED",err,0,0);
}
INTFREE();
return err;
}
void IrqEventGenerator::RemoveListenerForVbatdetLowIrq(ChargingEventListener *listener)
{
/* Remove listener */
removeListener(listener, PM_VBAT_DET_IRQ_HDL);
}
//owen.wei add end
解决问题：
我们的代码中有一个定时器函数ListenerForBatteryAlarm()，这个函数用来监听电池电压的变化，一般情况下我们电池只能在脉冲充电结束后才会是满电状态，才会达到100%，通过这个函数监听，可以让用户感觉充电时间会有缩短的假象。这个函数是国速那边增加的。
二次充电时修改的文件：






5、修改充电时上报的电池电压电池温度的时间频率
在vbatt_update_adc_battery_params函数里，有一个计数的变量pollimg_timer
在这个位置会有返回的结果，所以如果想让上报的频率增大一些，可以在这里把这段代码注解掉。
如果不修改这个位置的话，就会1分钟进行上报一次电池电压和电池温度，这样的话会比较慢。兴飞测试那边实现温度是45度了，但是我们的手机显示还只是43度，最好能够做成时实上报温度。
6、不插入电池连接USB线手机循环开关机，类似小米一样
\#define FEATURE_SUPPORT_ONLY_USB_POWER_ON  //Owen.wei modify
\#define FEATURE_HS_USB_BASIC //Owen.wei modify
打开这两个宏就可以了。

