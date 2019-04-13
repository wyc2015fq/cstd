
# linux驱动里获取时间差 - 嵌入式Linux - CSDN博客

2015年01月27日 11:10:44[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：1537



```python
--- a/android-lte/kernel/drivers/gosodrv/camera/ov8825_v4l2.c
+++ b/android-lte/kernel/drivers/gosodrv/camera/ov8825_v4l2.c
@@ -14,6 +14,16 @@
 #include <linux/module.h>
 #include "msm_sensor.h"
 #include "msm.h"
+#include <linux/timer.h>
+#include <linux/timex.h>
+#include <linux/rtc.h>
 #define SENSOR_NAME "ov8825"
 #define PLATFORM_DRIVER_NAME "msm_camera_ov8825"
 #define ov8825_obj ov8825_##obj
@@ -896,11 +906,20 @@ int32_t ov8825_sensor_i2c_probe(struct i2c_client *client,
 int32_t ov8825_sensor_power_up(struct msm_sensor_ctrl_t *s_ctrl)
 {
        int32_t rc = 0;
+       struct timex  txc;
+       struct rtc_time tm;
+
        struct msm_camera_sensor_info *info = s_ctrl->sensordata;
+       struct timeval tpstart,tpend;
+       unsigned int  timeuse,timevse;
+       do_gettimeofday(&tpstart); <span style="color:#ff0000;"><strong>// 开始时间</strong></span>
+
        CDBG("%s IN\r\n", __func__);
        CDBG("%s, sensor_pwd:%d, sensor_reset:%d\r\n",__func__, info->sensor_pwd, info->sensor_reset);
<span style="white-space:pre">	</span><strong><span style="color:#ff0000;">//开始当前时间</span></strong>
+       do_gettimeofday(&(txc.time));
+    rtc_time_to_tm(txc.time.tv_sec,&tm);
+    printk("UTC time :%d-%d-%d %d:%d:%d \n",tm.tm_year+1900,tm.tm_mon, tm.tm_mday,tm.tm_hour,tm.tm_min,tm.tm_sec);
        gpio_direction_output(info->sensor_pwd, 0);
        gpio_direction_output(info->sensor_reset, 0);
        usleep_range(5000, 6000);
@@ -919,6 +938,14 @@ int32_t ov8825_sensor_power_up(struct msm_sensor_ctrl_t *s_ctrl)
        msleep(20);
        gpio_direction_output(info->sensor_reset, 1);
        msleep(40);
+       do_gettimeofday(&tpend);  <strong><span style="color:#ff0000;"> // 结束时间</span></strong>
+       //timeuse=1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec;
+       timeuse = tpend.tv_usec-tpstart.tv_usec;
+       timevse = tpend.tv_sec-tpstart.tv_sec;
+       do_gettimeofday(&(txc.time));
+       printk("used time:vsec:%ds usec:%dus \n",timevse,timeuse);
```
```python
//<span style="color:#ff0000;"><strong>结束当前时间</strong></span>
+    rtc_time_to_tm(txc.time.tv_sec,&tm);
+    printk("UTC time :%d-%d-%d %d:%d:%d \n",tm.tm_year+1900,tm.tm_mon, tm.tm_mday,tm.tm_hour,tm.tm_min,tm.tm_sec);
        return rc;
 }
```


