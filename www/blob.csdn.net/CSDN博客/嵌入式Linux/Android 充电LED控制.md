
# Android 充电LED控制 - 嵌入式Linux - CSDN博客

2018年12月14日 10:36:36[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：127



## 1、前言
充电LED灯控制是Android设备的基本功能，主要用于提示设备的电池状态，充电状态，满电状态，低电状态，很多手机厂商还使用呼吸效果来显示，普通的低端设备会使用两个LED灯表示，红灯表示充电和低电，绿灯表示满电。
之前写的文章 《[Android充电系统介绍](http://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA5NTM3MjIxMw%3D%3D%26mid%3D2247484017%26idx%3D1%26sn%3D6f747ea0647eee66089f8e4955833550%26chksm%3D904114aba7369dbdf72185741ec996309e28dbf86b3fedd7bc9debe22bc3f1ee59018078c106%26token%3D1602048692%26lang%3Dzh_CN%23rd)》
## 2、kernel 层控制充电LED
代码位置主要在kernel/driver/power/power_supply_leds.c
`/* Battery specific LEDs triggers. */
static void power_supply_update_bat_leds(struct power_supply *psy)
{
    union power_supply_propval status;
    unsigned long delay_on = 0;
    unsigned long delay_off = 0;
    if (power_supply_get_property(psy, POWER_SUPPLY_PROP_STATUS, &status))
        return;
    dev_dbg(&psy->dev, "%s %d\n", __func__, status.intval);
    switch (status.intval) {
    case POWER_SUPPLY_STATUS_FULL:
        led_trigger_event(psy->charging_full_trig, LED_FULL);
        led_trigger_event(psy->charging_trig, LED_OFF);
        led_trigger_event(psy->full_trig, LED_FULL);
        led_trigger_event(psy->charging_blink_full_solid_trig,
            LED_FULL);
        break;
    case POWER_SUPPLY_STATUS_CHARGING:
        led_trigger_event(psy->charging_full_trig, LED_FULL);
        led_trigger_event(psy->charging_trig, LED_FULL);
        led_trigger_event(psy->full_trig, LED_OFF);
        led_trigger_blink(psy->charging_blink_full_solid_trig,
            &delay_on, &delay_off);
        break;
    default:
        led_trigger_event(psy->charging_full_trig, LED_OFF);
        led_trigger_event(psy->charging_trig, LED_OFF);
        led_trigger_event(psy->full_trig, LED_OFF);
        led_trigger_event(psy->charging_blink_full_solid_trig,
            LED_OFF);
        break;
    }
}`
如果想在kernel 层控制led灯，就需要在这里修改代码，会需要使用到trigger相关的东西，要理解什么是trigger，可以看下面的链接
[https://blog.csdn.net/sgmenghuo/article/details/38322141](http://link.zhihu.com/?target=https%3A//blog.csdn.net/sgmenghuo/article/details/38322141)
trigger主要是做led灯的一些效果的东西，默认有default-on 对应打开led的trigger，还有none对应关闭LED的trigger。
![](https://pic2.zhimg.com/80/v2-4c42cf20347f369602b82d7bc538a55d_hd.jpg)
被[ ]括起来的，就是当前trigger的值，然后回触发对应trigger的.activate函数。
Trigger的代码位置在 kernel/drivers/leds/trigger
## 3、framework控制led灯
为了统一控制led接口，Android有一个自己的框架控制led灯
Framework->Hal->kernel->hardware
设置对应的LED灯颜色主要在
Framework/base/core/res/res/values/config.xml
`<color name="config_defaultNotificationColor">#ffffffff</color>
    <!-- Default LED on time for notification LED in milliseconds. -->
    <integer name="config_defaultNotificationLedOn">500</integer>
    <!-- Default LED off time for notification LED in milliseconds. -->
    <integer name="config_defaultNotificationLedOff">2000</integer>
    <!-- Default value for led color when battery is low on charge -->
    <integer name="config_notificationsBatteryLowARGB">0xFFFF0000</integer>                                            
    <!-- Default value for led color when battery is medium charged -->
    <integer name="config_notificationsBatteryMediumARGB">0xFFFF0000</integer>
    <!-- Default value for led color when battery is fully charged -->
    <integer name="config_notificationsBatteryFullARGB">0xFF00FF00</integer>
    <!-- Default value for LED on time when the battery is low on charge in miliseconds -->
    <integer name="config_notificationsBatteryLedOn">125</integer>`
充电状态灯指示状态函数调用
frameworks\base\services\core\java\com\android\server\BatteryService.java
`/**
* Synchronize on BatteryService.
*/
public void updateLightsLocked() {
final int level = mBatteryProps.batteryLevel;
final int status = mBatteryProps.batteryStatus;
if (level < mLowBatteryWarningLevel) {
    if (status == BatteryManager.BATTERY_STATUS_CHARGING) {
        // Solid red when battery is charging
        mBatteryLight.setColor(mBatteryLowARGB);
    } else {
        // Flash red when battery is low and not charging
        mBatteryLight.setFlashing(mBatteryLowARGB, Light.LIGHT_FLASH_TIMED,
        mBatteryLedOn, mBatteryLedOff);
    }
} else if (status == BatteryManager.BATTERY_STATUS_CHARGING
|| status == BatteryManager.BATTERY_STATUS_FULL) {
    if (status == BatteryManager.BATTERY_STATUS_FULL || level >= 90) {
    // Solid green when full or charging and nearly full
        mBatteryLight.setColor(mBatteryFullARGB);
    } else {
        if (isHvdcpPresent()) {
            // Blinking orange if HVDCP charger
            mBatteryLight.setFlashing(mBatteryMediumARGB, Light.LIGHT_FLASH_TIMED,
            mBatteryLedOn, mBatteryLedOn);
        } else {
            // Solid orange when charging and halfway full
            mBatteryLight.setColor(mBatteryMediumARGB);
        }
    }
} else {
    // No lights if not charging and not low
    mBatteryLight.turnOff();
}
}
}`
LightsService 管理LED
frameworks\base\services\core\java\com\android\server\lights\LightsService.java
`private void setLightLocked(int color, int mode, int onMS, int offMS, int brightnessMode)；
{
//调用JNI函数设置状态
setLight_native(mNativePointer, mId, color, mode, onMS, offMS, brightnessMode);
}
static void setLight_native(JNIEnv* /* env */, jobject /* clazz */, jlong ptr,jint light, jint colorARGB, jint flashMode, jint onMS, jint offMS, jint brightnessMode)
{
//调用HAL层接口设置指示灯状态
devices->lights[light]->set_light(devices->lights[light], &state);
}`
获取HAL层接口函数
`static jlong init_native(JNIEnv* /* env */, jobject /* clazz */)
{
int err;
hw_module_t* module;
Devices* devices;
devices = (Devices*)malloc(sizeof(Devices));

err = hw_get_module(LIGHTS_HARDWARE_MODULE_ID, (hw_module_t const**)&module);
if (err == 0) {
    devices->lights[LIGHT_INDEX_BACKLIGHT] = get_device(module, LIGHT_ID_BACKLIGHT);
    devices->lights[LIGHT_INDEX_KEYBOARD] = get_device(module, LIGHT_ID_KEYBOARD);
    devices->lights[LIGHT_INDEX_BUTTONS] = get_device(module, LIGHT_ID_BUTTONS);
    devices->lights[LIGHT_INDEX_BATTERY] = get_device(module, LIGHT_ID_BATTERY);
    devices->lights[LIGHT_INDEX_NOTIFICATIONS] = get_device(module, LIGHT_ID_NOTIFICATIONS);
    devices->lights[LIGHT_INDEX_ATTENTION] = get_device(module, LIGHT_ID_ATTENTION);
    devices->lights[LIGHT_INDEX_BLUETOOTH] = get_device(module, LIGHT_ID_BLUETOOTH);
    devices->lights[LIGHT_INDEX_WIFI] = get_device(module, LIGHT_ID_WIFI);
} else {
    memset(devices, 0, sizeof(Devices));
}
return (jlong)devices;
}`
中间层的代码位置主要在
Hardware/rockchip/liblight/
一个字节对应一种颜色，一共有三个颜色，想最终控制颜色就需要修改上面说的xml文件，比如0x00ffffff,表示红蓝绿三个灯同时亮。具体代码如下
`int set_battery_light(struct light_device_t* dev, struct light_state_t const* state)
{                                     
    int err = 0;                      
    char red,blue,green;              
    unsigned int colorRGB = state->color & 0x00ffffff;
    red = (colorRGB >> 16) & 0xFF; 
    green = (colorRGB >> 8) & 0xFF;                                                                                    
    blue = colorRGB & 0xFF;           
    pthread_mutex_lock(&g_lock);   
    LOGI(">>>weiqifa set_battery_light 0x%x",state->color);
    err = write_int(RED_LED_PATH, red?1:0);
    err = write_int(BLUE_LED_PATH, blue?1:0);
    err = write_int(GREEN_LED_PATH, green?1:0);
    pthread_mutex_unlock(&g_lock); 
    return 0;                         
}`
然后控制的主要是led的设备节点，led的设备节点主要是驱动层生成，代码的位置如下：kernel/driver/leds/
> \#define RED_LED_PATH "sys/class/leds/red/brightness"

> \#define GREEN_LED_PATH "sys/class/leds/green/brightness"

> \#define BLUE_LED_PATH "sys/class/leds/blue/brightness"

Led驱动对应的dts代码如下：
`leds: gpio-leds {
        compatible = "gpio-leds";
        led-green {
        ┊   label = "green";
            gpios = <&gpio7 8 GPIO_ACTIVE_HIGH>;
            inux,default-trigger = "timer";
            default-state = "off";
        }; 
        led-blue {
        ┊   label = "blue";
            gpios = <&gpio7 14 GPIO_ACTIVE_HIGH>;
            inux,default-trigger = "timer";
            default-state = "off";
        }; 
        led-red {
        ┊   label = "red";
            gpios = <&gpio7 15 GPIO_ACTIVE_HIGH>;
            inux,default-trigger = "timer";                                                                            
            default-state = "off";
        };
    };`里面的linux,default-trigger 表示默认的trigger模式，如果设置为none就是表示关闭，现在我们的代码设置为timer表示用定时器的模式，实际上我们没有编译定时器模式，所以设置并没有生效。
![](https://pic3.zhimg.com/80/v2-0320c5c677bcb4c44a9ff7c3912739a6_hd.jpg)
欢迎关注微信公众号-**嵌入式Linux**
觉得不错，请帮忙**转发，点赞**，您的每一次支持，我都将铭记于心

